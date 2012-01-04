/*
    Copyright (C) 2012, Ferruccio Barletta (ferruccio.barletta@gmail.com)

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include "stref.h"
#include <vector>
#define BOOST_TEST_MODULE    stref_tests
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace tools;

BOOST_AUTO_TEST_CASE(stref_ctor) {
    const char* t = "abcd";
    stref sr1(t);
    BOOST_CHECK_EQUAL(sr1.data(), t);
    BOOST_CHECK_EQUAL(sr1.length(), strlen(t));

    stref sr2(sr1);
    BOOST_CHECK_EQUAL(sr2.data(), sr1.data());
    BOOST_CHECK_EQUAL(sr2.length(), sr1.length());

    std::string s("hello");
    stref sr3(s);
    BOOST_CHECK_EQUAL(sr3.data(), s.data());
    BOOST_CHECK_EQUAL(sr3.length(), s.length());

    std::wstring ws(L"HELLO");
    wstref sr4(ws);
    BOOST_CHECK_EQUAL(sr4.data(), ws.data());
    BOOST_CHECK_EQUAL(sr4.length(), ws.length());
    BOOST_CHECK(sr4.iequals(ws));

    wstref sr5(ws.data(), ws.length());
    BOOST_CHECK_EQUAL(sr5.data(), ws.data());
    BOOST_CHECK_EQUAL(sr5.length(), ws.length());
}

BOOST_AUTO_TEST_CASE(stref_assignment) {
    stref sr1(""), sr2("xyzzy");
    sr1 = sr2;
    BOOST_CHECK_EQUAL(sr1.data(), sr2.data());
    BOOST_CHECK_EQUAL(sr1.length(), sr2.length());
}

BOOST_AUTO_TEST_CASE(stref_equality) {
    stref sr1("hello");
    stref sr2(sr1);
    BOOST_CHECK(sr1 == sr2);

    stref sr3("hello");
    stref sr4("hello world");
    BOOST_CHECK(!(sr3 == sr4));
}

BOOST_AUTO_TEST_CASE(stref_inequality) {
    stref sr1("hello");
    stref sr2(sr1);
    BOOST_CHECK(!(sr1 != sr2));

    stref sr3("hello");
    stref sr4("hello world");
    BOOST_CHECK(sr3 != sr4);
}

BOOST_AUTO_TEST_CASE(stref_substr) {
    stref sr("sub-string test");

    BOOST_CHECK_EQUAL(sr.substr(0, sr.length()), sr);
    BOOST_CHECK_EQUAL(sr.substr(4, 6), stref("string"));
    BOOST_CHECK_EQUAL(sr.substr(0, 3), stref("sub"));
    BOOST_CHECK_EQUAL(sr.substr(11, 4), stref("test"));
    BOOST_CHECK_EQUAL(sr.substr(11, 3), stref("tes"));
    BOOST_CHECK_EQUAL(sr.substr(11, 5), stref("test"));
    BOOST_CHECK_EQUAL(sr.substr(20, 0), stref(""));
    BOOST_CHECK_EQUAL(sr.substr(20, 10), stref(""));

    BOOST_CHECK_EQUAL(sr.substr(0), sr);
    BOOST_CHECK_EQUAL(sr.substr(11), stref("test"));
}

BOOST_AUTO_TEST_CASE(stref_iequals) {
    BOOST_CHECK(stref("aaa").iequals(stref("aaa")));
    BOOST_CHECK(stref("Aaa").iequals(stref("aaa")));
    BOOST_CHECK(!stref("aaa").iequals(stref("aaaa")));
    BOOST_CHECK(!stref("aaa").iequals(stref("aba")));
}

BOOST_AUTO_TEST_CASE(stref_iless_than) {
    BOOST_CHECK(stref("aaa").iless_than(stref("aab")));
    BOOST_CHECK(stref("aaa").iless_than(stref("aaaa")));
    BOOST_CHECK(stref("aaa").iless_than(stref("Aba")));

    BOOST_CHECK(!stref("aaa").iless_than(stref("aaa")));
    BOOST_CHECK(!stref("aab").iless_than(stref("aaa")));
    BOOST_CHECK(!stref("aaaa").iless_than(stref("aaa")));
    BOOST_CHECK(!stref("Aba").iless_than(stref("aaa")));
}

BOOST_AUTO_TEST_CASE(stref_icompare) {
    BOOST_CHECK(stref("aaa").icompare(stref("aaa")) == 0);
    BOOST_CHECK(stref("aaa").icompare(stref("aaaa")) < 0);
    BOOST_CHECK(stref("aaaa").icompare(stref("aaa")) > 0);
    BOOST_CHECK(stref("aAa").icompare(stref("aaa")) == 0);
    BOOST_CHECK(stref("aaa").icompare(stref("aAAa")) < 0);
    BOOST_CHECK(stref("aaaa").icompare(stref("aAa")) > 0);
    BOOST_CHECK(stref("aaa").icompare(stref("AAB")) < 0);
}

BOOST_AUTO_TEST_CASE(stref_has_any_of) {
    BOOST_CHECK(stref("abcd").has_any_of("-cf"));
    BOOST_CHECK(!stref("abcd").has_any_of("!wxzy"));
}

BOOST_AUTO_TEST_CASE(stref_istarts_with) {
    BOOST_CHECK(stref("").istarts_with(stref("")));
    BOOST_CHECK(stref("hello").istarts_with(stref("")));
    BOOST_CHECK(stref("hello").istarts_with(stref("h")));
    BOOST_CHECK(stref("hello").istarts_with(stref("he")));
    BOOST_CHECK(stref("hello").istarts_with(stref("hel")));
    BOOST_CHECK(stref("hello").istarts_with(stref("hell")));
    BOOST_CHECK(stref("hello").istarts_with(stref("hello")));
    BOOST_CHECK(stref("hello").istarts_with(stref("H")));
    BOOST_CHECK(stref("hello").istarts_with(stref("HE")));
    BOOST_CHECK(stref("hello").istarts_with(stref("HEL")));
    BOOST_CHECK(stref("hello").istarts_with(stref("HELL")));
    BOOST_CHECK(stref("hello").istarts_with(stref("HELLO")));

    BOOST_CHECK(stref("hello, world").istarts_with(stref("hello")));

    BOOST_CHECK(!stref("").istarts_with(stref("hello")));
    BOOST_CHECK(!stref("hello").istarts_with(stref("fred")));
    BOOST_CHECK(!stref("hello").istarts_with(stref("helo")));
    BOOST_CHECK(!stref("hello").istarts_with(stref("hElo")));
    BOOST_CHECK(!stref("hello").istarts_with(stref("hello, world")));
}

BOOST_AUTO_TEST_CASE(stref_iends_with) {
    BOOST_CHECK(stref("").iends_with(stref("")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("O")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("LO")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("LLO")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("ELLO")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("HELLO")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("o")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("Lo")));
    BOOST_CHECK(stref("HELLO").iends_with(stref("LlO")));

    BOOST_CHECK(stref("hello, world").iends_with(stref("world")));

    BOOST_CHECK(!stref("").iends_with(stref("hello")));
    BOOST_CHECK(!stref("hello").iends_with(stref("fred")));
    BOOST_CHECK(!stref("hello").iends_with(stref("othello")));
    BOOST_CHECK(!stref("hello").iends_with(stref("LOL")));
}

BOOST_AUTO_TEST_CASE(stref_lt) {
    BOOST_CHECK(stref("") < "hello");
    BOOST_CHECK(!(stref("") < ""));
    BOOST_CHECK(stref("abc") < "abcdef");
    BOOST_CHECK(!(stref("abc") < "abc"));
    BOOST_CHECK(stref("abc") < "abd");
}

BOOST_AUTO_TEST_CASE(stref_le) {
    BOOST_CHECK(stref("") <= "hello");
    BOOST_CHECK(stref("") <= "");
    BOOST_CHECK(stref("abc") <= "abcdef");
    BOOST_CHECK(stref("abc") <= "abc");
    BOOST_CHECK(stref("abc") <= "abd");
    BOOST_CHECK(!(stref("abd") <= "abc"));
}

BOOST_AUTO_TEST_CASE(stref_gt) {
    BOOST_CHECK(stref("hello") > "");
    BOOST_CHECK(!(stref("") > ""));
    BOOST_CHECK(stref("abcdef") > "abc");
    BOOST_CHECK(!(stref("abc") > "abc"));
    BOOST_CHECK(stref("abd") > "abc");
}

BOOST_AUTO_TEST_CASE(stref_ge) {
    BOOST_CHECK(stref("hello") >= "");
    BOOST_CHECK(stref("") >= "");
    BOOST_CHECK(stref("abcdef") >= "abc");
    BOOST_CHECK(stref("abc") >= "abc");
    BOOST_CHECK(stref("abd") >= "abc");
    BOOST_CHECK(!(stref("abc") >= "abd"));
}

BOOST_AUTO_TEST_CASE(stref_trim) {
    BOOST_CHECK_EQUAL(stref("  xxx").trim_left(), "xxx");
    BOOST_CHECK_EQUAL(stref("xxx  ").trim_right(), "xxx");
    BOOST_CHECK_EQUAL(stref("   xxx   ").trim(), "xxx");
}

BOOST_AUTO_TEST_CASE(wide_trim) {
    BOOST_CHECK(wstref(L"  xxx").trim_left() == L"xxx");
    BOOST_CHECK(wstref(L"xxx  ").trim_right() == L"xxx");
    BOOST_CHECK(wstref(L"   xxx   ").trim() == L"xxx");
}

BOOST_AUTO_TEST_CASE(stref_find) {
    BOOST_CHECK_EQUAL(stref("abcde").find('c'), 2);
    BOOST_CHECK_EQUAL(stref("abcde").find('x'), stref::npos);
}

BOOST_AUTO_TEST_CASE(stref_find_any_of) {
    BOOST_CHECK_EQUAL(stref("abcde").find(is_any_of<char>("cd")), 2);
    BOOST_CHECK_EQUAL(stref("abcde").find(is_any_of<char>("xyz")), stref::npos);
}

BOOST_AUTO_TEST_CASE(stref_split) {
    vector<stref> srv;
    stref("a,comma,separated,list").split(',', [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 4);
    BOOST_CHECK_EQUAL(srv[0], "a");
    BOOST_CHECK_EQUAL(srv[1], "comma");
    BOOST_CHECK_EQUAL(srv[2], "separated");
    BOOST_CHECK_EQUAL(srv[3], "list");

    srv.clear();
    stref("one item").split(',', [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 1);
    BOOST_CHECK_EQUAL(srv[0], "one item");

    srv.clear();
    stref("").split(',', [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 1);
    BOOST_CHECK_EQUAL(srv[0], "");
}

BOOST_AUTO_TEST_CASE(stref_split_any_of) {
    vector<stref> srv;
    stref("a,punctuation;separated.list").split(is_any_of<char>(",:;."), [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 4);
    BOOST_CHECK_EQUAL(srv[0], "a");
    BOOST_CHECK_EQUAL(srv[1], "punctuation");
    BOOST_CHECK_EQUAL(srv[2], "separated");
    BOOST_CHECK_EQUAL(srv[3], "list");

    srv.clear();
    stref("one item").split(is_any_of<char>(",:;."), [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 1);
    BOOST_CHECK_EQUAL(srv[0], "one item");

    srv.clear();
    stref("").split(is_any_of<char>(",:;."), [&](stref sr) { srv.push_back(sr); });
    BOOST_CHECK_EQUAL(srv.size(), 1);
    BOOST_CHECK_EQUAL(srv[0], "");
}

BOOST_AUTO_TEST_CASE(stref_each) {
    string s;
    stref("test!").each([&](char ch) { s += ch; });
    BOOST_CHECK_EQUAL(s, "test!");
}

BOOST_AUTO_TEST_CASE(stref_each_reverse) {
    string s;
    stref("test!").each_reverse([&](char ch) { s += ch; });
    BOOST_CHECK_EQUAL(s, "!tset");
}

