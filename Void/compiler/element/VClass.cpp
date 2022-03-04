#include "VClass.hpp"
#include "../TokenHandler.hpp"
#include "VMethod.hpp"
#include "../../util/Strings.hpp"

namespace Void
{
    /**
     * Initialize class.
     */
    VClass::VClass(VClass* parent)
    {
        this->parent = parent;
    }

    /**
     * Initialize class.
     */
    VClass::VClass() : VClass(nullptr)
    {
    }

    /**
     * Perform class token parsing.
     */
    bool VClass::parse(LIST_T<Token> tokens, SIZE_T& begin, SIZE_T& cursor)
    {
        // create token handler

        TokenHandler handler(tokens, begin, cursor);

        // handle class modifiers

        for (SIZE_T i = begin; i < cursor; i++)
        {
            Token token = tokens[i];

            // ignore line numbers

            if (token.is(TokenType::LineNumber))
            {
                continue;
            }

            // make sure modifier is a class modifier

            if (!isClassModifier(token.value))
            {
                println("Class modifier expected, but got " << token);
                return false;
            }

            // register modifier

            modifiers.push_back(token.value);
        }

        // move cursor once becase right now it is pointing to the "class" token

        cursor++;
    
        // get the next token

        Token token = handler.next();

        // handle invalid class name

        if (!token.is(TokenType::Identifier))
        {
            println("Identifier expected, but got " << token);
            return false;
        }

        // set class name

        this->name = token.value;

        // get the next token

        token = handler.next();

        // declare a list for generic type parsing

        LIST_T<Token> generics;

        // check if class has generic types

        if (token.is(TokenType::Operator, "<"))
        {
            // determine which tokens belongs to the generic types
            // '<' -> index++
            // '>' -> index--
            // initially =1 because one '<' is already handled above 

            int index = 1;

            // handle class generic types
            
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
                            // move to the next token

                            token = handler.next();
                            break;
                        }
                    }
                }

                // append generic type token

                generics.push_back(token);
            }
        }

        // declare a list for superclass and interface parsing

        LIST_T<Token> superAndInterfaces;

        // check if class has a superclass or interfaces

        if (token.is(TokenType::Colon))
        {
            // handle class superclass and interfaces

            for (; cursor < tokens.size(); )
            {
                // get the next token

                token = handler.next();

                // class body declaration begun, exit loop

                if (token.is(TokenType::Begin))
                {
                    break;
                }

                // append token

                superAndInterfaces.push_back(token);
            }
        }

        // handle invalid class body declaration

        if (!token.is(TokenType::Begin))
        {
            println("Class body declaration expected, but got " << token);
            return false;
        }

        // declare a list for body parsing

        LIST_T<Token> body;

        // determine which tokens belongs to the class body
        // '{' -> index++
        // '}' -> index--
        // initially =1 because one '{' is already handled above 

        int index = 1;

        // handle class body

        for (; cursor < tokens.size(); )
        {
            // get the next token

            token = handler.next();

            // increment index

            if (token.is(TokenType::Begin))
            {
                index++;
            }

            // decrement index

            else if (token.is(TokenType::End))
            {
                // end of class body declaration

                if (--index == 0)
                {
                    break;
                }
            }

            // append token

            body.push_back(token);
        }

        // determine which tokens belongs to the generic types
        // '<' -> index++
        // '>' -> index--

        index = 0;

        // handle class body

        bool parsed = true;

        for (SIZE_T cursor = 0, begin = 0, start = 0; cursor < body.size(); cursor++)
        {
            // get the token at the current cursor

            Token token = body[cursor];

            // handle class declarations

            if (token.is(TokenType::Expression))
            {
                // handle class declaration

                if (token.val("class"))
                {
                    println("class declaration 2x");
                    break;
                }

                // TODO handle interfaces, annotations, enums, etc

                // handle invalid declaration

                else
                {
                    println("Declaration token expected, but got " << token);
                    parsed = false;
                    break;
                }
            }

            // handle generic method declaration

            else if (token.is(TokenType::Operator, "<"))
            {
                println("handling generic method creation");

                // create method

                VMethod* method = new VMethod(this);

                if (!method->parse(body, begin, cursor))
                {
                    println("Unable to parse method");
                    return false;
                }

                methods.push_back(method);

                break; // true
            }

            // handle method or field declaration

            else if (token.is(TokenType::Type) || token.is(TokenType::Identifier))
            {
                // handle generics given for the type

                if (cursor + 2 >= body.size() || (!body[cursor + 1].is(TokenType::Identifier) && !body[cursor + 1].is(TokenType::Operator, "<")))
                {
                    println("Name or generic type expected, but got " << token);
                    parsed = false;
                    break;
                }

                // get the index the method/field name starts at

                SIZE_T end = cursor + 1;

                // method/field type has generics

                if (body[cursor + 1].is(TokenType::Operator, "<"))
                {
                    // handle invalid generic types

                    if (token.is(TokenType::Type))
                    {
                        println("Type token can not follow generic arguments");
                        parsed = false;
                        break;
                    }
                        
                    // skip generic tokens

                    int gIndex = 0;

                    for (; end < body.size(); end++)
                    {
                        // get the token at the end index

                        token = body[end];

                        // token is an operator

                        if (token.is(TokenType::Operator))
                        {
                            // increment index

                            if (token.val("<"))
                            {
                                gIndex++;
                            }

                            // decrement index

                            else if (token.val(">"))
                            {
                                // generic type declaration ended

                                if (--gIndex == 0)
                                {
                                    // move end index to the identifier position

                                    end++;
                                    break;
                                }
                            }
                        }
                    }
                }

                // get the method/field name 

                token = body[end];

                // check token type

                if (!token.is(TokenType::Identifier))
                {
                    println("Identifier expected, but got " << token);
                    parsed = false;
                    break;
                }

                println("naming " << token);

                // check body has no more elements but required

                if (end + 1 >= body.size())
                {
                    println("Open, semicolon or = operator expected");
                    parsed = false;
                    break;
                }
                    
                // get the next token

                token = body[end + 1];

                // handle method declaration

                if (token.is(TokenType::Open))
                {
                    println("handling normal method declaration");

                    // create method

                    VMethod* method = new VMethod(this);

                    if (!method->parse(body, begin, cursor))
                    {
                        println("Unable to parse method");
                        return false;
                    }

                    methods.push_back(method);
                    
                    break; // true
                }

                // handle field declaration

                else if (token.is(TokenType::Semicolon) || token.is(TokenType::Operator))
                {
                    println("handling field declaration");
                    break; // true
                }

                // handle invalid declaration

                else
                {
                    println("Open, semicolon or operator token expected, but got " << token);
                    parsed = false;
                    break;
                }

                println("declaration");
                break; // true
            }

            // handle invalid tokens

            else if (!token.is(TokenType::Modifier) && !token.is(TokenType::LineNumber))
            {
                println("Declaration expected, but got " << token);
                parsed = false;
                break;
            }
        }

        begin = cursor--;

        return parsed;
    }

    /**
     * Determine whether the given modifier is a class modifier.
     */
    bool VClass::isClassModifier(STRING modifier)
    {
        return modifier == "public"
            || modifier == "protected"
            || modifier == "private"
            || modifier == "static"
            || modifier == "final";
    }

    /**
     * Build the class to bytecode.
     */
    void VClass::build(LIST& bytecode)
    {
        // write the class name

        bytecode.push_back("cdef " + name);
    
        // write the class modifiers

        if (!modifiers.empty())
        {
            bytecode.push_back("cmod " + Strings::join(modifiers, " "));
        }

        // TODO write class generics, interfaces

        // write class body

        bytecode.push_back("cbegin");

        for (VMethod* method : methods)
        {
            method->build(bytecode);
        }

        bytecode.push_back("cend");
    }
}