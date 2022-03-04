#include "Common.hpp"
#include "util/Exceptions.hpp"
#include "util/Options.hpp"
#include "util/Strings.hpp"
#include "util/Lists.hpp"
#include "util/Files.hpp"
#include "vm/Program.hpp"
#include "vm/VirtualMachine.hpp"
#include "vm/runtime/Stack.hpp"
#include "compiler/Compiler.hpp"

using namespace Void;

/**
 * Hey there,
 *  Welcome to the Void.
 *  I am Ábel (aka AdvancedAntiSkid) the founder of Void.
 *  If you have any questions feel free to join my discord.
 *	  https://bluenight.net
 * 
 * What is Void?
 *  Void is a dynamic, stack-based c++ virtual machine and compiler that I started making 
 *	for educational purposes. In my free time I always aim towards learning as much as possible, 
 *	and once I thought how cool it would be to have my own programming language. Well here it is.
 * Why Void?
 *  Void's vision is to provide an easy, understandable programming language for both 
 *	educational and production purposes.
 * How stable is Void?
 *  According to my testings Void should be pretty stable, but keep in mind that I consider myself
 *  as a c++ beginner, therefore I'm positive most parts of Void could be written in so much easier ways.
 *  If you have encountered any bug smake sure to report them on my discord.
 * Can I contribute?
 *  Sure. Just make sure to write clean code and comment out everything.
 * 
 * For further information, please visit https://github.com/VoidLang/Void
 * 
 * Written at: 08/10/2021 07:39 (dd-MM-yyyy HH-mm)
 * Last updated: 04/03/2022 13:14 (dd-MM-yyyy HH-mm)
 */

const STRING VERSION = "1.0.1-BETA";

/**
 * Print help message to the console.
 */
void sendHelp()
{
    println("Void v" << VERSION);
    println("Usage: void [options] [args...]" << '\n');
    println("Arguments following the -run <file> are passed as the arguments to the main method." << '\n');
    println("Where options include:" << '\n');
    println("	-run <executable file>		Execute a compiled vertex program.");
    println("	-compile <project folder>	Compile vertex source files.");
    println("	-header <source file>		Create a c++ header for the given source file.");
    println("");
}

/**
 * Start the program.
 * This must be a separated method because of the __try __except thing.
 */
void start(int argc, char** argv)
{
    // send help message if there are not enough arguments

    if (argc < 2)
    {
        sendHelp();
        return;
    }

    // handle parameters

    Options options(argc, argv);

    // launch compiled program

    if (options.has("run"))
    {
        // get the executable's path

        STRING path = options.get("run");

        // get program start arguments

        LIST arguments = options.arguments;
        STRING strArgs = Strings::join(arguments, ", ");

        println("Running " << path << " using args [" << strArgs << "]" << '\n');

        // create program

        Program program(path, arguments);

        // validate path

        if (!program.validate()) 
        {
            println("Given path must be a folder or a Void executable.");
            return;
        }

        // build program, make bytecode

        LIST bytecode = program.build();

        // create class factory

        Factory factory(options);

        // create virtual machine

        VirtualMachine vm(factory, program, options);
        vm.load(bytecode);

        // debug virtual machine

        if (options.has("XVMDebug"))
        {
            vm.debug();
        }
        
        // include void builtins

        include(&factory);

        Description description = program.description;

        // check if the main path is specified

        if (!description.hasProgramMain())
        {
            println("Unspecified main path.");
            return;
        }

        // get program main

        STRING programMain = description.getProgramMain();
            
        // check if main path is empty

        if (programMain.empty())
        {
            println("Invalid main path '" << programMain << "'. Ignoring it.");
            return;
        }

        // get the main class

        Class* mainClass = factory.getClass(programMain);

        // check if main class is missing

        if (mainClass == nullptr)
        {
            println("Missing main class: '" << programMain << "'.");
            return;
        }

        // create the required parameters for the main method

        LIST parameters;
        parameters.push_back("[Lvoid.lang.String");

        // get the main method

        Method* mainMethod = mainClass->getMethod("main", parameters);
        
        // check if main method is missing

        if (mainMethod == nullptr)
        {
            println("Missing main method: '" << programMain << ".main(" << Strings::join(parameters, " ") << ")V'.");
            return;
        }

        // check if main method is static

        if (!mainMethod->hasModifier(Modifier::STATIC))
        {
            println("Non-static main method: '" << programMain << ".main(" << Strings::join(parameters, " ") << ")V'.");
            return;
        }

        // create the heap (main stack)

        Stack* heap = new Stack(nullptr, "Heap");

        // call static constructors and initialize static fields

        factory.initialize(heap);

        // get the string class

        Class* clazz = factory.getClass("void.lang.String");

        // create start arguments array
    
        Array<Instance*>* startArgs = new Array<Instance*>(&factory, clazz, heap, "L" + clazz->name, (int) arguments.size());
            
        // fill up array

        for (int i = 0; i < arguments.size(); i++)
        {
            startArgs->set(i, new String(&factory, heap, arguments[i]));
        }

        // push the final start arguments onto the stack

        heap->instanceStack.push(startArgs);

        // call program entry point

        mainMethod->invoke(&factory, heap, nullptr, nullptr);
    }

    // compile program sources

    else if (options.has("compile"))
    {
        // check for correct command usage

        if (!options.has("out"))
        {
            println("Usage: void -compile <project folder> -out <output file path>");
            return;
        }

        STRING inputDir = options.get("compile");
        STRING outputFile = options.get("out");

        println("compiling " << inputDir << " to " << outputFile);

        // create compiler

        Compiler compiler(inputDir, outputFile);

        // validate compiler

        if (!compiler.validate())
        {
            return;
        }

        // create project settings

        Project project(compiler.projectFile);

        if (!project.load())
        {
            println("Unable to load project file");
            return;
        }

        // read source files

        VProgram program;
        compiler.readSources(program);

        // build the program

        LIST bytecode = program.build();

        // write the result to file

        FILE_OUTPUT_STREAM stream;
        stream.open(outputFile);

        for (STRING line : bytecode)
        {
            stream << line << '\n';
        }

        stream.close();
    }

    // create header file for class

    else if (options.has("header"))
    {
        STRING path = options.get("header");

        if (!EXISTS(path) || IS_DIRECTORY(path))
        {
            println("Given path must be a Void executable.");
            return;
        }

        // TODO check if file is .v

        // create program

        Program program(path, LIST());

        // build program, make bytecode

        LIST bytecode = program.build();

        // create class factory

        Factory factory(options);

        // create virtual machine

        VirtualMachine vm(factory, program, options);
        vm.load(bytecode);

        // debug virtual machine

        if (options.has("XVMDebug"))
        {
            vm.debug();
        }

        // set header file path

        PATH filePath = path;
        STRING headerPath = filePath.parent_path().generic_string() + Files::SEPARATOR + filePath.stem().generic_string() + ".hpp";

        // write header data
        
        FILE_OUTPUT_STREAM stream;
        stream.open(headerPath);

        stream << "/**" << '\n';
        stream << " * DO NOT EDIT THIS FILE - it is machine generated" << '\n';
        stream << " */ " << "\n\n";

        stream << "#include \"vni.h\"" << "\n\n";
        
        for (Class* clazz : factory.classes)
        {
            // get the native methods of the class

            LIST_T<Method*> methods = clazz->nativeMethods();

            // do not process class if it does not have any native methods

            if (methods.empty())
            {
                continue;
            }

            stream << "// Methods for class " << clazz->name << "\n\n";

            STRING path = clazz->nativePath();

            stream << "#ifndef " << path << '\n';
            stream << "#define " << path << '\n';
            stream << "#ifdef __cplusplus" << '\n';
            stream << "extern \"C\" {" << '\n';
            stream << "#endif" << "\n\n";

            for (Method* method : methods)
            {
                stream << '\t' << "/**" << '\n';
                stream << '\t' << " * Class:      " << clazz->name << '\n';
                stream << '\t' << " * Method:     " << method->name << '\n';
                stream << '\t' << " * Parameters: " << '(' << Strings::join(method->parameters, ", ") << ')' << '\n';
                stream << '\t' << " */" << '\n';

                stream << '\t' << "VNIEXPORT " << method->nativeReturnType() << " VNICALL " << method->nativePath() << '\n';
                // stream << '\t' << "(VNIEnv* env, VNIParams* params);" << "\n\n";
                stream << '\t' << "(VNIEnv* env, ";

                if (!method->hasModifier(Modifier::STATIC))
                {
                    stream << "VObject* self, ";
                }

                stream << "VNIParams* params);" << "\n\n";
            }

            stream << "#ifdef __cplusplus" << '\n';
            stream << "}" << '\n';
            stream << "#endif" << '\n';
            stream << "#endif " << path << "\n\n";
        }

        stream.close();
    }

    // invalid arguments -> send help message

    else
    {
        sendHelp();
        return;
    }

    println("\nProgram executed successfully.");
}

/**
 * Program entry point.
 */
int main(int argc, char** argv)
{
    // What the...?
    // Well, there should probably be a better way of doing this, 
    // but gotta catch those windows-level errors...

    __try
    {
        start(argc, argv);
    }
    __except (Exceptions::handle(GetExceptionCode(), GetExceptionInformation()))
    {
        // no need to do anything in here because the exception is handled in Exceptions::handle
    }
    return 0;
}
