#include "Token.hpp"

namespace Void
{
    /**
     * Initialize token.
     */
    Token::Token(TokenType type, STRING value)
    {
        this->type = type;
        this->value = value;
    }

    /**
     * Initialize token without default value given.
     */
    Token::Token(TokenType type) : Token(type, "")
    {
    }

    /**
     * Determine if the current token was not the last one.
     */
    bool Token::hasNext()
    {
        return type != TokenType::Finish 
            && type != TokenType::Unexpected;
    }

    /**
     * Determine whether the token itself has the given type.
     */
    bool Token::is(TokenType type)
    {
        return this->type == type;
    }

    /**
     * Determine whether the token itself has the given type and value.
     */
    bool Token::is(TokenType type, STRING value)
    {
        return this->type == type 
            && this->value == value;
    }

    /**
     * Determine whether the token itself has the given value.
     */
    bool Token::val(STRING value)
    {
        return this->value == value;
    }

    /**
     * Make token type printable.
     */
    OUTPUT_STREAM& operator<<(OUTPUT_STREAM& stream, const TokenType& type)
    {
        static const char* const names[]
        {
            "String",
            "Character",
            "Begin",
            "End",
            "Byte",
            "Short",
            "Double",
            "Float",
            "Long",
            "Integer",
            "Hexadecimal",
            "Boolean",
            "Semicolon",
            "Expression",
            "Colon",
            "Open",
            "Close",
            "Identifier",
            "Operator",
            "Type",
            "Modifier",
            "Start",
            "Stop",
            "Annotation",
            "Comma",
            "Info",
            "Null",
            "LineNumber",
            "Finish",
            "Unexpected"
        };
        return stream << names[static_cast<int>(type)];
    }

    /**
     * Make token printable.
     */
    OUTPUT_STREAM& operator<<(OUTPUT_STREAM& stream, Token& token)
    {
        return stream << token.type << (token.value.length() > 0 ? (" |" + token.value + "|") : "");
    }
}