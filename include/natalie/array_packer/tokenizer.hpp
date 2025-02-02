#pragma once

#include "tm/optional.hpp"
#include "tm/string.hpp"
#include "tm/vector.hpp"

namespace Natalie {

using namespace TM;

namespace ArrayPacker {
    enum class Endianness {
        Native,
        Little,
        Big,
    };

    struct Token {
        signed char directive { 0 };
        int count { -1 };
        bool star { false };
        bool native_size { false };
        Endianness endianness { Endianness::Native };
        Optional<String> error {};
    };

    class Tokenizer {
    public:
        Tokenizer(String directives)
            : m_directives { directives } { }

        Vector<Token> *tokenize() {
            auto tokens = new Vector<Token> {};
            for (Token token = next_token(); token.directive; token = next_token()) {
                tokens->push(token);
                if (token.error)
                    break;
            }
            return tokens;
        }

    private:
        Token next_token() {
            auto directive = current_char();
            if (!directive) return {};

            auto token = Token { directive };

            auto modifier = next_char();

            if (apply_endianness_modifier(token, modifier))
                modifier = next_char();

            if (modifier == '_' || modifier == '!') {
                apply_modifier_error_if_not_allowed(token, modifier);
                token.native_size = true;
                modifier = next_char();
            }

            if (apply_endianness_modifier(token, modifier))
                modifier = next_char();

            if (isdigit(modifier)) {
                token.count = (int)modifier - '0';
                modifier = next_char();
                while (isdigit(modifier)) {
                    token.count *= 10;
                    token.count += ((int)modifier - '0');
                    modifier = next_char();
                }
            }

            if (modifier == '*') {
                next_char();
                token.star = true;
            }

            // printf("token { directive='%c', count=%d, star=%d, native_size=%d, endianness=%d }\n", token.directive, token.count, (int)token.star, (int)token.native_size, (int)token.endianness);

            return token;
        }

        signed char next_char() {
            if (m_index >= m_directives.length())
                return 0;
            m_index++;
            return current_char();
        }

        signed char current_char() {
            signed char c = char_at_index(m_index);

            while (m_index < m_directives.size() && (isspace(c) || c == '\0'))
                c = char_at_index(++m_index);

            if (c == '#') {
                while (c && c != '\n')
                    c = char_at_index(++m_index);
                if (c == '\n')
                    c = char_at_index(++m_index);
            }

            return c;
        }

        signed char char_at_index(size_t index) {
            if (index >= m_directives.length())
                return 0;
            return m_directives[index];
        }

        void apply_modifier_error_if_not_allowed(Token &token, signed char modifier) {
            switch (token.directive) {
            case 'i':
            case 'I':
            case 'j':
            case 'J':
            case 'l':
            case 'L':
            case 'q':
            case 'Q':
            case 's':
            case 'S':
                return;
            default:
                token.error = String::format("'{}' allowed only after types sSiIlLqQjJ", modifier);
            }
        }

        bool apply_endianness_modifier(Token &token, signed char modifier) {
            switch (modifier) {
            case '>':
                apply_modifier_error_if_not_allowed(token, modifier);
                token.endianness = Endianness::Big;
                return true;
            case '<':
                apply_modifier_error_if_not_allowed(token, modifier);
                token.endianness = Endianness::Little;
                return true;
            default:
                return false;
            }
        }

        String m_directives;
        size_t m_index { 0 };
    };

}

}
