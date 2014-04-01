#include "contacts.h"
#include "user.h"

#include "user_writer.h"
#include <boost/concept_check.hpp>

using namespace std;
using namespace gbook;
using namespace tinyxml2;

vector<user> gbook::contacts::get_all() {
    vector<user> users;
    string url = "https://www.google.com/m8/feeds/contacts/default/full?group=";
    url.append(my_contacts_group_id);
    do {
        curl c;
        prepare_curl(c);
        c.url(url);
        c.method(method::GET);
        c.execute();

        XMLDocument d;
        d.Parse(c.received_body().c_str());
        XMLElement * feed = d.FirstChildElement("feed");
        if (!feed) {
            throw runtime_error(string("No feed element: ").append(c.received_body()));
        }
        for (XMLElement * entry = feed->FirstChildElement("entry"); entry != NULL; entry = entry->NextSiblingElement("entry")) {
            user u;
            map_entry_to_user(entry, u);
            users.push_back(u);
        }

        url.clear();
        for (XMLElement * link = d.FirstChildElement("feed")->FirstChildElement("link"); link != NULL; link = link->NextSiblingElement("link")) {
            // get next page if it exists
            if (link->Attribute("rel", "next")) {
                url = link->Attribute("href");
            }
        }
    } while (!url.empty());
    return users;
}

void contacts::add(user u) {
    XMLDocument d;
    XMLElement * root = d.NewElement("x");
    d.InsertEndChild(root);
    map_user_to_entry(u, root, d);
    XMLPrinter printer(NULL, true);
    d.Print(&printer);

    string requestBody(printer.CStr());
    curl c;
    prepare_curl(c);
    c.url("https://www.google.com/m8/feeds/contacts/default/full");
    c.method(method::SIMPLE_POST);
    c.set_body(requestBody);
    c.header("Content-Type", "application/atom+xml");
    c.execute();
    if (c.return_code() != 201) {
        throw runtime_error(string("Cannot create contact: ").append(c.received_body()));
    }

    XMLDocument response;
    response.Parse(c.received_body().c_str());
    if (response.Error()) {
        throw invalid_argument(string("Cannot parse xml document: ").append(c.received_body()));
    }
    user returned_user;
    map_entry_to_user(response.FirstChildElement("entry"), returned_user);
    u.set_id("google", returned_user.get_id("google"));
}

void contacts::update(user u) {
    XMLDocument d;
    XMLElement * root = d.NewElement("x");
    d.InsertEndChild(root);
    map_user_to_entry(u, root, d);
    XMLPrinter printer(NULL, true);
    d.Print(&printer);

    string requestBody(printer.CStr());
    curl c;
    prepare_curl(c);
    c.url(string("https://www.google.com/m8/feeds/contacts/default/full/").append(u.get_id("google")));
    c.method(method::PUT);
    c.set_body(requestBody);
    c.header("Content-Type", "application/atom+xml");
    c.header("If-Match", "*");
    c.execute();
    if (c.return_code() != 200) {
        throw runtime_error(string("Cannot update contact: ").append(c.received_body()));
    }
}

void contacts::remove(string id) {
    curl c;
    prepare_curl(c);
    c.url(string("https://www.google.com/m8/feeds/contacts/default/full/").append(id));
    c.method(method::DELETE);
    c.header("If-Match", "*");
    c.execute();
    if (c.return_code() != 200) {
        throw runtime_error(string("Cannot delete contact: ").append(c.received_body()));
    }
}


string safe_assign_string(const char * str) {
    if (str == NULL) {
        return string();
    } else {
        return move(string(str));
    }
}

void contacts::map_entry_to_user(XMLElement* entry, gbook::user& u) {
    if (entry->FirstChildElement("gd:name")) {
        u.name = safe_assign_string(entry->FirstChildElement("gd:name")->FirstChildElement("gd:fullName")->GetText());
    }
    for (XMLElement * email = entry->FirstChildElement("gd:email"); email != NULL; email = email->NextSiblingElement("gd:email")) {
        const char * address = email->Attribute("address");
        if (address != NULL) {
            u.emails.push_back(address);
        }
    }
    XMLElement * spa = entry->FirstChildElement("gd:structuredPostalAddress");
    if (spa) {
        if (spa->FirstChildElement("gd:street")) {
            u.address = safe_assign_string(spa->FirstChildElement("gd:street")->GetText());
        }
        if (spa->FirstChildElement("gd:postcode")) {
            u.zip = safe_assign_string(spa->FirstChildElement("gd:postcode")->GetText());
        }
        if (spa->FirstChildElement("gd:city")) {
            u.city = safe_assign_string(spa->FirstChildElement("gd:city")->GetText());
        }
        if (spa->FirstChildElement("gd:region")) {
            u.state = safe_assign_string(spa->FirstChildElement("gd:region")->GetText());
        }
        if (spa->FirstChildElement("gd:country")) {
            u.country = safe_assign_string(spa->FirstChildElement("gd:country")->GetText());
        }
    }
    for (XMLElement * phone = entry->FirstChildElement("gd:phoneNumber"); phone != NULL; phone = phone->NextSiblingElement("gd:phoneNumber")) {
        if (phone->Attribute("rel", "http://schemas.google.com/g/2005#home")) {
            u.phone = safe_assign_string(phone->GetText());
        } else if (phone->Attribute("rel", "http://schemas.google.com/g/2005#work")) {
            u.workphone = safe_assign_string(phone->GetText());
        } else if (phone->Attribute("rel", "http://schemas.google.com/g/2005#fax")) {
            u.fax = safe_assign_string(phone->GetText());
        } else if (phone->Attribute("rel", "http://schemas.google.com/g/2005#mobile")) {
            u.mobile = safe_assign_string(phone->GetText());
        }
    }
    if (entry->FirstChildElement("gContact:nickname")) {
        u.nick = safe_assign_string(entry->FirstChildElement("gContact:nickname")->GetText());
    }
    if (entry->FirstChildElement("gContact:website")) {
        u.url = safe_assign_string(entry->FirstChildElement("gContact:website")->Attribute("href"));
    }
    if (entry->FirstChildElement("content")) {
        u.notes = safe_assign_string(entry->FirstChildElement("content")->GetText());
    }
    if (entry->FirstChildElement("gContact:birthday")) {
        u.anniversary = safe_assign_string(entry->FirstChildElement("gContact:birthday")->Attribute("when"));
    }
    for (XMLElement * field = entry->FirstChildElement("gContact:userDefinedField"); field != NULL; field = field->NextSiblingElement("gContact:userDefinedField")) {
        if (field->Attribute("key", "groups")) {
            string group_string = safe_assign_string(field->Attribute("value"));
            if (!group_string.empty()) {
                u.groups.clear();
                while (group_string.find(",") != string::npos) {
                    u.groups.push_back(group_string.substr(0, group_string.find(",")));
                    group_string = group_string.substr(group_string.find(",")+1);
                }
                u.groups.push_back(group_string);
            }
        } else if (field->Attribute("key", "c1")) {
            u.custom1 = safe_assign_string(field->Attribute("value"));
        } else if (field->Attribute("key", "c2")) {
            u.custom2 = safe_assign_string(field->Attribute("value"));
        } else if (field->Attribute("key", "c3")) {
            u.custom3 = safe_assign_string(field->Attribute("value"));
        } else if (field->Attribute("key", "c4")) {
            u.custom4 = safe_assign_string(field->Attribute("value"));
        } else if (field->Attribute("key", "c5")) {
            u.custom5 = safe_assign_string(field->Attribute("value"));
        }
    }
    string id = entry->FirstChildElement("id")->GetText();
    u.set_id("google", id.substr(id.find_last_of("/") +1));
}

inline XMLElement * add_xml_element(XMLDocument & d, XMLElement * parent, string name, string value = "", map<string, string> attributes = map<string, string>(), XMLElement * insert_after = nullptr) {
    XMLElement * e = d.NewElement(name.c_str());
    for (auto pair : attributes) {
        e->SetAttribute(pair.first.c_str(), pair.second.c_str());
    }
    e->SetText(value.c_str());
    if (!insert_after) {
        parent->InsertEndChild(e);
    } else {
        parent->InsertAfterChild(insert_after, e);
    }
    return e;
}

inline XMLElement * add_xml_element(XMLDocument & d, XMLElement * parent, string name, map<string, string> attributes, XMLElement * insert_after = nullptr) {
    XMLElement * e = d.NewElement(name.c_str());
    for (auto pair : attributes) {
        e->SetAttribute(pair.first.c_str(), pair.second.c_str());
    }
    if (!insert_after) {
        parent->InsertEndChild(e);
    } else {
        parent->InsertAfterChild(insert_after, e);
    }
    return e;
}

void contacts::map_user_to_entry(user& u, XMLElement* entry, XMLDocument & d) {
    entry->SetName("atom:entry");
    entry->SetAttribute("xmlns:atom", "http://www.w3.org/2005/Atom");
    entry->SetAttribute("xmlns:gd", "http://schemas.google.com/g/2005");
    entry->SetAttribute("xmlns:gContact", "http://schemas.google.com/contact/2008");
    add_xml_element(d, entry, "atom:category", { {"scheme", "http://schemas.google.com/g/2005#kind"}, {"term", "http://schemas.google.com/contact/2008#contact"} });
    add_xml_element(d, entry, "gContact:groupMembershipInfo", { {"href", my_contacts_group_id} });

    if (!u.name.empty()) {
        add_xml_element(d, add_xml_element(d, entry, "gd:name"), "gd:fullName", u.name);
    }

    for (string email : u.emails) {
        add_xml_element(d, entry, "gd:email", { {"address", email}, { "rel", "http://schemas.google.com/g/2005#other" } });
    }

    if (u.has_address()) {
        XMLElement * address_root = add_xml_element(d, entry, "gd:structuredPostalAddress", { { "rel", "http://schemas.google.com/g/2005#other" } });
        if (!u.address.empty()) {
            add_xml_element(d, address_root, "gd:street", u.address);
        }
        if (!u.city.empty()) {
            add_xml_element(d, address_root, "gd:city", u.city);
        }
        if (!u.state.empty()) {
            add_xml_element(d, address_root, "gd:region", u.state);
        }
        if (!u.zip.empty()) {
            add_xml_element(d, address_root, "gd:postcode", u.zip);
        }
        if (!u.country.empty()) {
            add_xml_element(d, address_root, "gd:country", u.country);
        }
    }

    if (!u.phone.empty()) {
        add_xml_element(d, entry, "gd:phoneNumber", u.phone, { { "rel", "http://schemas.google.com/g/2005#home" } } );
    }
    if (!u.workphone.empty()) {
        add_xml_element(d, entry, "gd:phoneNumber", u.workphone, { { "rel", "http://schemas.google.com/g/2005#work" } } );
    }
    if (!u.fax.empty()) {
        add_xml_element(d, entry, "gd:phoneNumber", u.fax, { { "rel", "http://schemas.google.com/g/2005#fax" } } );
    }
    if (!u.mobile.empty()) {
        add_xml_element(d, entry, "gd:phoneNumber", u.mobile, { { "rel", "http://schemas.google.com/g/2005#mobile" } } );
    }

    if (!u.nick.empty()) {
        add_xml_element(d, entry, "gContact:nickname", u.nick);
    }

    if (!u.url.empty()) {
        add_xml_element(d, entry, "gContact:website", { { "href", u.url }, { "rel", "home-page" } });
    }

    if (!u.notes.empty()) {
        add_xml_element(d, entry, "atom:content", u.notes, { { "type", "text" } });
    }

    if (!u.anniversary.empty()) {
        add_xml_element(d, entry, "gContact:birthday", { { "when", u.anniversary } });
    }

    string group_string;
    for (auto group : u.groups) {
        if (!group_string.empty()) {
            group_string.append(",");
        }
        group_string.append(sanitize(group));
    }
    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "groups" }, { "value", group_string } } );

    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "c1" }, { "value", sanitize(u.custom1) } } );
    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "c2" }, { "value", sanitize(u.custom2) } } );
    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "c3" }, { "value", sanitize(u.custom3) } } );
    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "c4" }, { "value", sanitize(u.custom4) } } );
    add_xml_element(d, entry, "gContact:userDefinedField", { { "key", "c5" }, { "value", sanitize(u.custom5) } } );
}
