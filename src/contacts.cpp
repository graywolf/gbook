#include "contacts.h"
#include "user.h"

using namespace std;
using namespace gbook;
using namespace tinyxml2;

void gbook::contacts::get_all() {
    curl c;
    prepare_curl(c);
    c.url("https://www.google.com/m8/feeds/contacts/default/full");
    c.method(method::GET);
    c.execute();

    XMLDocument d;
    d.Parse(c.received_body().c_str());
    for (XMLElement * entry = d.FirstChildElement("feed")->FirstChildElement("entry"); entry != NULL; entry = entry->NextSiblingElement("entry")) {
        string id = entry->FirstChildElement("id")->GetText();
        user u;
        if (entry->FirstChildElement("gd:name")) {
            u.name = entry->FirstChildElement("gd:name")->FirstChildElement("gd:fullName")->GetText();
        }
    }
}
