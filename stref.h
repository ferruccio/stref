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

#ifndef STREF_H
#define STREF_H

#include <cctype>
#include <cwctype>

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

namespace tools {

    // a local exception class
    class bad_stref_op : public std::exception
    {
    public:
        bad_stref_op(const char* msg) : std::exception(), msg(msg) {}
        const char* what() { return msg; }

    private:
        const char* msg;
    };

    // char traits
    class char_traits
    {
    public:
        typedef char char_type;
        static char to_lower_case(char ch) { return std::tolower(ch); }
        static bool is_whitespace(char ch) { return !!std::isspace(ch); }
    };

    // wchar_t traits
    class wchar_traits
    {
    public:
        typedef wchar_t char_type;
        static wchar_t to_lower_case(wchar_t ch) { return std::towlower(ch); }
        static bool is_whitespace(wchar_t ch) { return !!std::iswspace(ch); }
    };

    // is_any_of(chT) functor
    template <typename chT>
    class is_any_of
    {
    public:
        is_any_of(const chT* str) : str(str) {}

        bool operator() (chT ch) {
            for (const chT *s = str; *s; ++s)
                if (*s == ch)
                    return true;
            return false;
        }

    private:
        const chT* str;
    };

    // basic string reference
    template <typename TT>
	class basic_stref
	{
	private:
        typedef basic_stref<TT> bstref;
        typedef typename TT::char_type chT;

	public:
		basic_stref<TT>(const bstref& sr) : ref(sr.ref), len(sr.len) {}
		basic_stref<TT>(const chT* str) : ref(str), len(0) { while (str[len] != 0) ++len; }
		basic_stref<TT>(const chT* str, size_t len) : ref(str), len(len) {}
		basic_stref<TT>(const std::basic_string<chT>& str) : ref(str.data()), len(str.length()) {}

		bstref& operator= (const bstref& sr) {
			ref = sr.ref;
			len = sr.len;
			return *this;
		}

		chT operator[] (int index) const { return _at(index); }

        chT at(int index) {
            if (index < 0 || index >= length())
                throw bad_stref_op("at(): invalid index");
            return _at[index];
        }

        chT front() const {
            if (length() == 0)
                throw bad_stref_op("front(): length == 0");
            return _front();
        }

        chT back() const {
            if (length() == 0)
                throw bad_stref_op("back(): length == 0");
            return _back();
        }

        //
        //  relational operators
        //

        int compare(const bstref& rhs) const {
			size_t len = std::min(length(), rhs.length());
			for (size_t i = 0; i < len; ++i)
                if (_at(i) != rhs._at(i))
                    return _at(i) - rhs._at(i);
			return length() == rhs.length() ? 0 : length() - rhs.length();
		}

		bool operator== (const bstref& rhs) const {
			if (len != rhs.len) return false;	// different lengths, cannot be equal
			if (ref == rhs.ref) return true;	// same object (+same length), must be equal
            return compare(rhs) == 0;
		}

		bool operator!= (const bstref& rhs) const {
			if (len != rhs.len) return true;	// different lengths, must be unequal
			if (ref == rhs.ref) return false;	// same object (+same length), cannot be unequal
            return compare(rhs) != 0;
		}

        bool operator< (const bstref& rhs) const {
            if (ref == rhs.ref && len < rhs.len) return true;   // same string, shorter
            return compare(rhs) < 0;
        }

        bool operator<= (const bstref& rhs) const {
            if (ref == rhs.ref && len <= rhs.len) return true;  // same string, equal length or shorter
            return compare(rhs) <= 0;
        }

        bool operator> (const bstref& rhs) const {
            if (ref == rhs.ref && rhs.len > len) return true;   // same string, longer
            return compare(rhs) > 0;
        }

        bool operator>= (const bstref& rhs) const {
            if (ref == rhs.ref && rhs.len >= len) return true;  // same string, equal length or longer
            return compare(rhs) >= 0;
        }

        //
        // case-insensitive relational operators
        //

		int icompare(const bstref& rhs) const {
			size_t len = std::min(length(), rhs.length());
			for (size_t i = 0; i < len; ++i) {
				chT c1 = to_lower_case(_at(i)), c2 = to_lower_case(rhs._at(i));
				if (c1 != c2) return c1 - c2;			
			}
			return length() == rhs.length() ? 0 : length() - rhs.length();
		}

		bool iequals(const bstref& rhs) const {
			if (len != rhs.len) return false;	// different lengths, cannot be equal
			if (ref == rhs.ref) return true;	// same object (+same length), must be equal
            return icompare(rhs) == 0;
		}

        bool inot_equals(const bstref& rhs) const {
			if (len != rhs.len) return true;	// different lengths, must be unequal
			if (ref == rhs.ref) return false;	// same object (+same length), cannot be unequal
            return icompare(rhs) != 0;
		}

        bool iless_than(const bstref& rhs) {
            if (ref == rhs.ref && len < rhs.len) return true;   // same string, shorter
            return icompare(rhs) < 0;
        }

        bool iless_than_eq(const bstref& rhs) {
            if (ref == rhs.ref && len <= rhs.len) return true;  // same string, equal length or shorter
            return icompare(rhs) <= 0;
        }

        bool igreater_than(const bstref& rhs) {
            if (ref == rhs.ref && rhs.len > len) return true;   // same string, longer
            return icompare(rhs) > 0;
        }

        bool igreater_than_eq(const bstref& rhs) {
            if (ref == rhs.ref && rhs.len >= len) return true;  // same string, equal length or longer
            return icompare(rhs) >= 0;
        }

        //
        // predicates
        //

        bool starts_with(const bstref& rhs) const {
			if (rhs.length() == 0) return true;			// everything starts with an empty string
			if (length() < rhs.length()) return false;	// "starts with" string is longer than tested string
			return left(rhs.length()) == rhs;
		}

		bool istarts_with(const bstref& rhs) const {
			if (rhs.length() == 0) return true;			// everything starts with an empty string
			if (length() < rhs.length()) return false;	// "starts with" string is longer than tested string
			return left(rhs.length()).icompare(rhs) == 0;
		}

		bool ends_with(const bstref& rhs) const {
			if (rhs.length() == 0) return true;			// everything ends with an empty string
			if (length() < rhs.length()) return false;	// "ends with" string is longer than tested string
			return right(rhs.length()) == rhs;
		}

        bool iends_with(const bstref& rhs) const {
			if (rhs.length() == 0) return true;			// everything ends with an empty string
			if (length() < rhs.length()) return false;	// "ends with" string is longer than tested string
			return right(rhs.length()).icompare(rhs) == 0;
		}

        bool has(const chT ch) const {
            for (size_t i = 0; i < len; ++i)
                if (_at(i) == ch)
                    return true;
            return false;
        }

		bool has_any_of(bstref charset) const {
            const chT* end = charset.data() + charset.length();
			for (size_t i = 0; i < len; ++i)
                if (std::find(charset.data(), end, _at(i)) != end)
					return true;
			return false;
		}

        //
        // string algorithms
        //

		bstref substr(size_t offset, size_t len) const {
            return offset >= length()
                ? bstref(ref, 0)
                : bstref(ref + offset, std::min(len, this->len - offset));
		}

		bstref substr(size_t offset) const {
            return offset >= len
                ? bstref(ref, 0)
                : bstref(ref + offset, len - offset);
		}

        bstref left(size_t len) const {
            return substr(0, len);
        }

        bstref middle(size_t from, size_t to) const {
            if (to >= len) to = len - 1;
            return from > to
                ? bstref(ref, 0)
                : bstref(ref + from, to - from + 1);
        }

        bstref right(size_t len) const {
            len = std::min(len, length());
            return substr(length() - len, len);
        }

        bstref trim_left() const {
            bstref s(*this);
            while (s.length() > 0 && is_whitespace(s._front()))
                ++s.ref, --s.len;
            return s;
        }

        bstref trim_right() const {
            bstref s(*this);
            while (s.length() > 0 && is_whitespace(s._back()))
                --s.len;
            return s;
        }

        bstref trim() const { return trim_left().trim_right(); }

        //
        // find the first instance of one or more characters
        //

        enum { npos = -1 };

        int find(std::function< bool(chT) > match, int start = 0) const {
            for (size_t i = start; i < len; ++i)
                if (match(_at(i)))
                    return i;
            return npos;
        }

        int find(chT ch, int start = 0) const {
            return find(chmatch(ch), start);
        }

        //
        // split a string using one or more characters as separators
        //

        void split(std::function< bool(chT) > match, std::function< void(const bstref& sr) > body) const {
            int start = 0, pos = find(match, start);
            while (pos != npos) {
                body(middle(start, pos - 1));
                pos = find(match, start = pos + 1);
            }
            body(substr(start));
        }

        void split(chT ch, std::function< void(const bstref& sr) > body) const {
            split(chmatch(ch), body);
        }

        //
        // iterate over each character
        //

        void each(std::function< void(chT ch) > body) const {
            for (size_t i = 0; i < len; ++i)
                body(ref[i]);
        }

        void each_reverse(std::function< void(chT ch) > body) const {
            for (size_t i = len; i > 0; --i)
                body(ref[i - 1]);
        }

        // implicit cast to std::basic_string<chT>
		operator std::basic_string<chT>() const { return std::basic_string<chT>(data(), length()); }

        // readonly access to internal members
		const chT* data() const { return ref; }
		size_t length() const { return len; }

	private:
        // unchecked access to individual characters
        chT _at(int index) const { return ref[index]; }
        chT _front() const { return ref[0]; }
        chT _back() const { return ref[length() - 1]; }

        // traits methods
        chT to_lower_case(chT ch) const { return TT::to_lower_case(ch); }
        chT is_whitespace(chT ch) const { return TT::is_whitespace(ch); }

        struct chmatch {
            chmatch(chT ch) : ch(ch) {}
            bool operator() (chT c) { return c == ch; }
            chT ch;
        };

		const chT*	ref;	// original string data
		size_t		len;	// string length
	};

    // typedefs for string and wide-string references
	typedef basic_stref<char_traits> stref;
	typedef basic_stref<wchar_traits> wstref;

    // write narrow stref to narrow stream
	inline std::ostream& operator<<(std::ostream& os, const stref& s) {
		for (size_t i = 0; i < s.length(); ++i)
			os << s[i];
		return os;
	}

    // write wide stref to wide stream
	inline std::wostream& operator<<(std::wostream& os, const wstref& s) {
		for (size_t i = 0; i < s.length(); ++i)
			os << s[i];
		return os;
	}

}

#endif
