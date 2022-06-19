#include "Compiler.hpp"
#include "../util/Files.hpp"
#include "../util/Strings.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

namespace Void
{
    /**
     * Initialize compiler.
     */
    Compiler::Compiler(STRING inputDir, STRING outputFile)
    {
        this->inputDir = inputDir;
        this->outputFile = outputFile;
    }

    /**
     * Validate input data.
     */
    bool Compiler::validate()
    {
        // check if input path is a directory

        if (!IS_DIRECTORY(inputDir))
        {
            println("Input path is not a directory: " << inputDir);
            return false;
        }

        projectFile = inputDir + Files::SEPARATOR + "project.xml";

        // check if project.xml exists inside the given path

        if (!EXISTS(projectFile))
        {
            println("Project folder missing file project.xml");
            return false;
        }

        sourceFolder = inputDir + Files::SEPARATOR + "src";

        // check if src folder exists
        
        if (!EXISTS(sourceFolder))
        {
            println("Source folder is missing");
            return false;
        }

        return true;
    }

    /**
     * Read sources from source folder.
     */
    void Compiler::readSources(VProgram& program)
    {
        // get the files from the source folder recursively

        LIST_T<PATH> files;
        Files::walk(sourceFolder, files);

        // handle files

        for (PATH path : files)
        {
            if (path.extension() == ".void")
            {
                handleSourceFile(program, path);
            }
            else
            {
                handleResourceFile(program, path);
            }
        }
    }

    /**
     * Handle void source file.
     */
    void Compiler::handleSourceFile(VProgram& program, PATH path)
    {
        println("Handling source file " << path);

        FILE_INPUT_STREAM stream;
        stream.open(path);

        // check state

        if (stream.fail())
        {
            error("Unable to read file: " << path);
        }

        // read file content

        STRING content = Files::readAll(stream);
        
        // create lexer

        Lexer lexer(content);
        LIST_T<Token> tokens;

        // lex file content

        for (Token token = lexer.next() ;; token = lexer.next())
        {
            println(INDENT(12) << token);

            tokens.push_back(token);

            // handle processing end

            if (!token.hasNext())
            {
                // handle unexpected token

                if (token.is(TokenType::Unexpected))
                    syntaxError(path.generic_string(), content, lexer.lineNumber, lexer.lineIndex);
                
                break;
            }
        }

        // create parser

        Parser parser(tokens, program);

        // parse tokens

        if (!parser.parse())
        {
            error("Unable to parse tokens");
        }

        stream.close();
    }

    /**
     * Handle void resource file.
     */
    void Compiler::handleResourceFile(VProgram& program, PATH path)
    {
        println("Handling resource file " << path);
    }

    /**
     * Perform a syntax error and stop compiling.
     */
    void Compiler::syntaxError(STRING path, STRING content, SIZE_T lineNumber, SIZE_T lineIndex)
    {
        // remove path prefix

        SIZE_T index = path.find("src/");
        path = path.substr(index + 4, path.size());

        // notify error

        println("Unexpected char in file " << path << " in line " << lineNumber << " at char " << lineIndex);

        // separate content to lines

        LIST lines;
        Strings::split(content, lines, '\n');

        // print the line the error occurred at

        println(lines[lineNumber - 1]);

        // print out the marker for the illegal character

        if (lineIndex > 0)
        {
            for (SIZE_T i = 0; i < lineIndex - 1; i++)
            {
                print(" ");
            }
        }

        error("^");
    }
}