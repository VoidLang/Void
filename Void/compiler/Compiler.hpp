#pragma once

#include "../Common.hpp"
#include "Project.hpp"
#include "VProgram.hpp"

namespace Void
{
    /**
     * Represents the Void compiler that builds a void executable from source files.
     */
    class Compiler
    {
    private:
        /**
         * Input project source folder.
         */
        STRING inputDir;

        /**
         * Output executable file path.
         */
        STRING outputFile;

    public:
        /**
         * The project xml file.
         */
        STRING projectFile;

        /**
         * The actual source folder.
         */
        STRING sourceFolder;

        /**
         * Initialize compiler.
         */
        Compiler(STRING inputDir, STRING outputFile);

        /**
         * Validate input data.
         */
        bool validate();

        /**
         * Read sources from source folder.
         */
        void readSources(VProgram& program);

        /**
         * Handle void source file.
         */
        void handleSourceFile(VProgram& program, PATH path);

        /**
         * Handle void resource file.
         */
        void handleResourceFile(VProgram& program, PATH path);

        /**
         * Perform a syntax error and stop compiling.
         */
        void syntaxError(STRING path, STRING content, SIZE_T lineNumber, SIZE_T lineIndex);
    };
}