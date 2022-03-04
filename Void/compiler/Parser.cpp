#include "Parser.hpp"

namespace Void
{
    /**
     * Initialize parser.
     */
    Parser::Parser(LIST_T<Token> tokens, VProgram& program) : program(program) 
    {
        this->tokens = tokens;
    }

    /**
     * Perform token parsing.
     */
    bool Parser::parse()
    {
        // create loop for the parsing

        for (SIZE_T cursor = 0, begin = 0; cursor < tokens.size(); cursor++)
        {
            // get the token at the current cursor

            Token token = tokens[cursor];

            // handle declarations

            if (token.is(TokenType::Expression))
            {
                // handle class declaration

                if (token.val("class"))
                {
                    // create class

                    VClass* clazz = new VClass();

                    // parse class

                    if (!clazz->parse(tokens, begin, cursor))
                    {
                        println("Unable to parse class");
                        return false;
                    }

                    // define class

                    if (!program.defineClass(clazz))
                    {
                        println("ClassRedefineException: Class '" << clazz->name << " already exists.");
                        return false;
                    }

                    // break;
                }

                // TODO handle interfaces, annotations, enums, etc

                // handle invalid declaration

                else
                {
                    println("Declaration token expected, but got ASD " << token);
                    return false;
                }
            }

            // handle invalid tokens

            else if (!token.is(TokenType::Modifier) && !token.is(TokenType::LineNumber))
            {
                // handle content end

                if (token.is(TokenType::Finish))
                {
                    return true;
                }

                // handle invalid token
                
                println("Declaration expected, but got DSA " << token);
                return false;
            }
        }

        // parsing successful

        return true;
    }
}