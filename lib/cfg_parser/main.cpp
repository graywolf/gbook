#include <iostream>
#include <string>

#include "cfg_parser.h"

using namespace std;

int main(int argc, char *argv[]) {
/*
	ConfigParser cfg;
	cfg.preserveEmptyLines(1);
	cfg.readFile("test.cfg");
*/
	const char* test;
	const char* test2;
	const char* test3;
	ConfigParser cfg("test.cfg");
	
	
	cfg.debug();
	cfg.writeFile("test2.cfg");
	
	int i = cfg.get("hier", -1);
	cout << "global hier=" << i << endl;
	i = cfg.get("hier", 0, "section");
	cout << "section hier=" << i << endl;
	
	string value = cfg.get<string>("testvar", "..not found..");
	cout << "value: " << value << endl;
	value.clear();

	string value2;
	cfg.getVar("testvar", value2);
	cout << "value: " << value << " value2: " << value2 << endl;

	test = cfg.getCString("testvar", "bla");
	test2 = cfg.getCString("testvar", "");
	test3 = cfg.getCString("testvar", "");
	printf("test: %s test2: %s test3: %s\n", test, test2, test3);
	
	//cfg.delVar("testvar");
	cfg.getCString("keyword");
	cfg.getCString("bal");
	cfg.getCString("bla");
	printf("test: %s test2: %s test3: %s\n", test, test2, test3);
	
	ConfigParser t;
	t.set<int>("value 1	b", 25);
	t.set("time", 300);
	t.set("angle", 2.5334);
	t.set("text", "welcome\nall your bayes");
	char* text = "hlablablablabl";
	t.set("text2", text);
	t.writeFile("new.cfg");
	
	ConfigParser b("new.cfg");
	b.debug();

    system("PAUSE");
    return 0;
}
