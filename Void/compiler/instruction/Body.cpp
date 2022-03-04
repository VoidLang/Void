#include "Body.hpp"

namespace Void
{
    /**
     * Initialize body.
     */
    Body::Body(LIST_T<Token> tokens)
    {
        this->tokens = tokens;
    }

    /**
     * Parse the body.
     */
    bool Body::parse()
    {
        for (Instruction instruction = next() ;; instruction = next())
        {
            
        }
    }

    /**
     * Parse the next instruction.
     */
    Instruction Body::next()
    {
        // handle local variable declaration

        if (peek().is(TokenType::Type))
        {
            return declareLocalPrimitive();
        }

        return {};
    }

    /**
     * Declare a primitive-type local variable.
     */
    Instruction Body::declareLocalPrimitive()
    {
        // get the local type token

        Token typeToken = get();

        // get the variable name token 

        Token nameToken = get();

        // make sure primitive type follows an identifier

        if (!nameToken.is(TokenType::Identifier))
        {
            println("Invalid primitive local variable declaration. Identifier expected, but got " << nameToken);
            return {};
        }

        println("local declare " << typeToken << " " << nameToken);

        // TODO i just added this cuz errors
        return {};
    }

    /**
     * Get the current token.
     */
    Token Body::peek()
    {
        Token token = at(cursor);
        
        // check for line number tokens

        while (token.is(TokenType::LineNumber))
        {
            // set the line number

            lineNumber = stoi(token.value);

            // get the next token

            token = at(++cursor);
        }
        
        return token;
    }

    /**
     * Get the current token and icrement the cursor.
     */
    Token Body::get()
    {
        Token token = at(cursor++);

        // check for line number tokens

        while (token.is(TokenType::LineNumber))
        {
            // set the line number

            lineNumber = stoi(token.value);

            // get the next token

            token = at(cursor++);
        }

        return token;
    }

    /**
     * Get the token at the given index.
     */
    Token Body::at(SIZE_T index)
    {
        if (has(index))
        {
            return tokens[index];
        }
        return Token(TokenType::Finish);
    }

    /**
     * Determine whether the given index is in bounds of the data length.
     */
    bool Body::has(SIZE_T index)
    {
        return index >= 0 && index < tokens.size();
    }
}