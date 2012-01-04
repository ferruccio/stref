#include <iostream>
#include <string>
#include <vector>

#include "stref.h"

using namespace std;
using namespace tools;

void process_string(stref sr) {
    sr.split(is_any_of<char>(",;"), [](stref token) {
        cout << '[' << token.trim() << ']' << endl;
    });
}

int main(int, char**) {
    cout << "\n--- test 1 ---" << endl;
    process_string("a,  sample; delimited , string   ");

    cout << "\n--- test 2 ---" << endl;
	string ds(" another;delimited;string "); // string copy, std::string constructor
    process_string(ds);

    cout << "\n--- test 3 ---" << endl;
    vector<string> sv;
    string str(" yet:another:delimited: string   ");
    stref a = str;
    string b = a;  // string copy
    cout << '[' << b << ']' << endl;
    a.split(':', [&](stref sr) { sv.push_back(sr.trim().left(3)); }); // string copy, std::string constructor
    for (auto s = begin(sv); s != end(sv); ++s)
        cout << '[' << *s << ']' << endl;
		
	return 0;
}