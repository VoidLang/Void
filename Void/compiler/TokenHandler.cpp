#include "TokenHandler.hpp"

namespace Void
{
    /**
     * Initialize token handler.
     */
    TokenHandler::TokenHandler(LIST_T<Token>& tokens, SIZE_T& begin, SIZE_T& cursor) : tokens(tokens), begin(begin), cursor(cursor)
    {
    }

    /**
     * Get the token at the given index.
     */
    Token TokenHandler::at(SIZE_T& index)
    {
        // get the token at the given index

        Token token = tokens[index];

        // check if token is a line number

        if (token.is(TokenType::LineNumber))
        {
            return at(++index);
        }

        return token;
    }

    /**
     * Get the next token.
     */
    Token TokenHandler::next()
    {
        // get the next token

        Token token = tokens[cursor++];

        // check if token is a line number

        if (token.is(TokenType::LineNumber))
        {
            return next();
        }

        return token;
    }
    
    /**
     * Get the current token.
     */
    Token TokenHandler::get()
    {
        // get the next token

        Token token = tokens[cursor];

        // check if token is a line number

        if (token.is(TokenType::LineNumber))
        {
            return next();
        }

        return token;
    }

    /**
     * Determine if the given index is in bounds of the tokens.
     */
    bool TokenHandler::has(SIZE_T index)
    {
        return index < tokens.size();
    }
}

