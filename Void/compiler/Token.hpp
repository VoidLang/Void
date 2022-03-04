#pragma once

#include "../Common.hpp"

namespace Void
{
    /**
     * Determines the types of the existing tokens.
     */
    enum class TokenType
    {
        String,    
        Character,
        Begin,
        End,
        Byte,
        Short,
        Double,
        Float,
        Long,
        Integer,
        Hexadecimal,
        Boolean,   
        Semicolon, 
        Expression, 
        Colon,
        Open,
        Close,
        Identifier,
        Operator,
        Type,
        Modifier,
        Start,
        Stop,
        Annotation,
        Comma,
        Info,
        Null,
        LineNumber,
        Finish,
        Unexpected
    };

    /**
     * Determines a holder of a set of characters separated 
     * by the lexer to be handled by the parser.
     */
    class Token
    {
    public:
        /**
         * The type of the token.
         */
        TokenType type;

        /**
         * The value of the token. (Might be empty)
         */
        STRING value;

        /**
         * Initialize token.
         */
        Token(TokenType type, STRING value);

        /**
         * Initialize default value given.
         */
        Token(TokenType type);

        /**
         * Determine whether the current token was not the last one.
         */
        bool hasNext();

        /**
         * Determine whether the token itself has the given type.
         */
        bool is(TokenType type);

        /**
         * Determine whether the token itself has the given type and value.
         */
        bool is(TokenType type, STRING value);

        /**
         * Determine whether the token itself has the given value.
         */
        bool val(STRING value);
    };

    /**
     * Make token type printable.
     */
    OUTPUT_STREAM& operator<<(OUTPUT_STREAM& stream, const TokenType& type);

    /**
     * Make token printable.
     */
    OUTPUT_STREAM& operator<<(OUTPUT_STREAM& stream, Token& token);
}