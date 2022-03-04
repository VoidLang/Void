#include "VMethod.hpp"
#include "../TokenHandler.hpp"
#include "../../util/Strings.hpp"
#include "../instruction/Body.hpp"

namespace Void
{
    /**
     * Initialize method.
     */
    VMethod::VMethod(VClass* clazz)
    {
        this->clazz = clazz;
    }

    /**
     * Perform method token parsing.
     */
    bool VMethod::parse(LIST_T<Token> tokens, SIZE_T& begin, SIZE_T& cursor)
    {
        // create token handler

        TokenHandler handler(tokens, begin, cursor);

        // handle method modifiers

        for (SIZE_T i = begin; i < cursor; i++)
        {
            Token token = handler.at(i);

            // ignore line numbers

            if (token.is(TokenType::LineNumber))
            {
                continue;
            }

            // make sure modifier is a method modifier

            if (!isMethodModifier(token.value))
            {
                println("Method modifier expected, but got " << token);
                return false;
            }

            // check for duplicate modifiers

            if (CONTAINS(modifiers, token.value))
            {
                println("Duplicate method modifier " << token);
                return false;
            }

            // register modifier

            modifiers.push_back(token.value);
        }

        // get the current token

        Token token = handler.get();

        // declare a list for generic type parsing

        LIST_T<Token> generics;

        // check if method has generic types

        if (token.is(TokenType::Operator, "<"))
        {
            // determine which tokens belongs to the generic types
            // '<' -> index++
            // '>' -> index--
            // initially =1 because one '<' is already handled above

            int index = 1;
            cursor++;

            // handle method generic types

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // check if token is an operator

                if (token.is(TokenType::Operator))
                {
                    // increment index

                    if (token.val("<"))
                    {
                        index++;
                    }

                    // decrement index

                    else if (token.val(">"))
                    {
                        // end of generic type declaration

                        if (--index == 0)
                        {
                            // move to the next token and exit loop

                            token = handler.next();
                            break;
                        }
                    }
                }

                // append generic type token

                generics.push_back(token);
            }
        }

        Token typeToken = token;
        STRING type = token.value;

        // check the return type token

        if (!token.is(TokenType::Type) && !token.is(TokenType::Identifier))
        {
            println("Type or identifier expected, but got " << token);
            return false;
        }

        // set the method return type

        RETURN_TYPE = getType(type);

        // get the next token

        token = handler.next();

        // declare a list for generic return type parsing

        LIST_T<Token> typeGenerics;

        // check if the return type has generic arguments

        if (token.is(TokenType::Operator, "<"))
        {
            // generics types can only be applied for non-primitives

            if (!typeToken.is(TokenType::Identifier))
            {
                println("Generic type arguments can by only applided for non-primitive types.");
                return false;
            }

            // determine which tokens belongs to the generic types
            // '<' -> index++
            // '>' -> index--
            // initially =1 because one '<' is already handled above

            int index = 1;

            // handle return type generic types

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // check if token is an operator

                if (token.is(TokenType::Operator))
                {
                    // increment index

                    if (token.val("<"))
                    {
                        index++;
                    }

                    // decrement index

                    else if (token.val(">"))
                    {
                        // end of generic type declaration

                        if (--index == 0)
                        {
                            // move to the next token and break loop

                            token = handler.next();
                            break;
                        }
                    }
                }

                // append generic type token

                typeGenerics.push_back(token);
            }
        }

        // make sure the token is for the method name if not a generic type token

        if (!token.is(TokenType::Identifier))
        {
            println("Method name or generic type arguments expected, but got " << token);
            return false;
        }

        // set the name of the method

        name = token.value;

        // get the next token

        token = handler.next();

        // next token should be a parameter list start

        if (!token.is(TokenType::Open))
        {
            println("Parameter list begin expected, but got " << token);
            return false;
        }

        // declare a list for the parameter token parsing
        
        LIST_T<Token> parameterTokens;

        // handle method parameters

        for (; cursor < tokens.size(); )
        {
            // get the next token

            token = handler.next();

            // handle parameter declaration end

            if (token.is(TokenType::Close))
            {
                // move to the next token and break loop

                token = handler.next();
                break;
            }

            parameterTokens.push_back(token);
        }

        // TODO handle method parameter tokens

        println("omg token " << token);

        // handle method modifiers which are declared after the parameter list

        if (token.is(TokenType::Modifier))
        {
            // move cursor backwards cuz otherwise the first modifier
            // that is declared after the parameter list is skipped

            cursor--;

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // check for method body begin

                if (token.is(TokenType::Begin) || token.is(TokenType::Operator, "=") || token.is(TokenType::Semicolon)
                    || (token.is(TokenType::Operator, "-") && handler.has(cursor) && handler.get().is(TokenType::Operator, ">")))
                {
                    // move to the next token and exit loop

                    break;
                }

                // make sure modifier is a method modifier

                if (!isMethodModifier(token.value))
                {
                    println("Method modifier expected, but got " << token);
                    return false;
                }

                // check for duplicate modifiers

                if (CONTAINS(modifiers, token.value))
                {
                    println("Duplicate method modifier " << token);
                    return false;
                }
                    
                // register modifier

                modifiers.push_back(token.value);
            }
        }

        bool isArrow = token.is(TokenType::Operator, "-") && handler.has(cursor) && handler.get().is(TokenType::Operator, ">");

        // declare a list for body parsing

        LIST_T<Token> bodyTokens;

        // handle normal body declaration

        if (token.is(TokenType::Begin))
        {
            int indexLessMore = 0;
            int indexBracket = 0;
            int indexSquareBracket = 0;
            int indexCurlyBracket = 1;

            // handle method body tokens

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // handle less than/more than chars

                if (token.is(TokenType::Operator))
                {
                    if (token.val("<"))
                        indexLessMore++;

                    else if (token.val(">"))
                        indexLessMore--;
                }

                // handle brackets

                else if (token.is(TokenType::Open))
                    indexBracket++;

                else if (token.is(TokenType::Close))
                    indexBracket--;

                // handle square brackets

                else if (token.is(TokenType::Start))
                    indexSquareBracket++;

                else if (token.is(TokenType::Stop))
                    indexSquareBracket--;

                // handle curly backets

                else if (token.is(TokenType::Begin))
                    indexCurlyBracket++;

                else if (token.is(TokenType::End))
                {
                    // end of method body declaration

                    if (--indexCurlyBracket == 0)
                    {
                        // move to the next token and break loop

                        break;
                    }
                }

                // append method body token

                bodyTokens.push_back(token);
            }
            
            println("normal decl");
        }

        // handle '=' or '->' body declaration
        // but here the body can only have one statement

        else if (token.is(TokenType::Operator, "=") || isArrow)
        {
            // skip the '>' token

            if (isArrow)
            {
                cursor++;
            }

            int indexLessMore = 0;
            int indexBracket = 0;
            int indexSquareBracket = 0;
            int indexCurlyBracket = 1;

            println("equal/arr decl");

            // handle method body tokens

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // handle less than/more than chars

                if (token.is(TokenType::Operator))
                {
                    if (token.val("<"))
                        indexLessMore++;

                    else if (token.val(">"))
                        indexLessMore--;
                }

                // handle brackets

                else if (token.is(TokenType::Open))
                    indexBracket++;

                else if (token.is(TokenType::Close))
                    indexBracket--;

                // handle square brackets

                else if (token.is(TokenType::Start))
                    indexSquareBracket++;

                else if (token.is(TokenType::Stop))
                    indexSquareBracket--;

                // handle curly backets

                else if (token.is(TokenType::Begin))
                    indexCurlyBracket++;

                else if (token.is(TokenType::End))
                    --indexCurlyBracket;

                // handle statement end

                else if (token.is(TokenType::Semicolon) && indexLessMore == 0 && indexBracket == 0 && indexSquareBracket == 0 && indexCurlyBracket == 0)
                {
                    bodyTokens.push_back(token);
                    break;
                }

                // append method body token

                bodyTokens.push_back(token);
            }
        }

        // method has no body, stop parsing

        else if (token.is(TokenType::Semicolon))
        {
            println("abstract decl");

            return true;
        }

        // handle invalid body declaration

        else
        {
            println("Invalid method body declaration. Begin, '->' or '=' expected, but got " << token);
            return false;
        }

        println("## handle method body :)");

        for (Token t : bodyTokens)
        {
            println("> " << t);
        }

        Body body(bodyTokens);

        // body.next();

        return true;
    }

    /**
     * Determine whether the given modifier is a method modifier.
     */
    bool VMethod::isMethodModifier(STRING modifier)
    {
        return modifier == "public"
            || modifier == "protected"
            || modifier == "private"
            || modifier == "static"
            || modifier == "final"
            || modifier == "native"
            || modifier == "unsafe";
    }

    /**
     * Convert types to virtual machine types.
     */
    STRING VMethod::getType(STRING type)
    {
        if (type == "void")
            return "V";
        else if (type == "long")
            return "L";
        else if (type == "float")
            return "F";
        else if (type == "double")
            return "D";
        else if (type == "int")
            return "I";
        else if (type == "short")
            return "S";
        else if (type == "byte")
            return "B";
        else if (type == "boolean")
            return "Z";
        return "L" + type;
    }

    /**
     * Build the method to bytecode.
     */
    void VMethod::build(LIST& bytecode)
    {
        // write method name

        bytecode.push_back("mdef " + name);
        
        // write method modifiers

        if (!modifiers.empty())
        {
            bytecode.push_back("mmod " + Strings::join(modifiers, " "));
        }

        // write method body

        bytecode.push_back("mbegin");
        bytecode.push_back("mend");
    }
}