#pragma once

#include "../Common.hpp"
#include "Token.hpp"
#include "VProgram.hpp"

namespace Void
{
    /**
     * Represents a per-file token parser that 
     * converts raw tokens to void nodes.
     */
    class Parser
    {
    private:
        /**
         * List of tokens to be parsed.
         */
        LIST_T<Token> tokens;

        /**
         * The program reference that holds classes and resources.
         */
        VProgram& program;


    public:
        /**
         * Initialize parser.
         */
        Parser(LIST_T<Token> tokens, VProgram& program);

        /**
         * Perform token parsing.
         */
        bool parse();
    };
}