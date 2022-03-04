#include "Program.hpp"
#include "../util/Files.hpp"
#include "../util/Strings.hpp"
#include "../util/Lists.hpp"

namespace Void
{
	/**
	 * Initialize program.
	 */
	Program::Program(STRING path, LIST arguments)
	{
		this->path = path;
		this->arguments = arguments;
	}

	/**
	 * Build the bytecode.
	 */
	LIST Program::build()
	{
		// declare the result bytecode 

		LIST bytecode;

		// handle directory

		if (IS_DIRECTORY(path))
		{
			// get all the files in the directory

			LIST_T<PATH> files;
			Files::walk(path, files);

			// handle files

			for (PATH path : files)
			{
				// handle executable file

				if (path.extension() == ".v")
				{
					buildFile(path.generic_string(), bytecode);
				}

				else
				{
					// TODO handle resource
				}
			}
		}

		// handle executable

		else
		{
			buildFile(path, bytecode);
		}

		LIST result;

		// apply definitions

		for (STRING line : bytecode)
		{
			for (const auto& [key, value] : definitions)
			{
				Strings::replace(line, key, value);
			}

			result.push_back(line);
		}

		return result;
	}

	/**
	 * Validate executable path.
	 */
	bool Program::validate()
	{
		if (!EXISTS(path))
			return false;

		if (IS_DIRECTORY(path))
			return true;

		PATH path = this->path;
		return path.extension() == ".v";
	}

	/**
	 * Determine if program has definition.
	 */
	bool Program::hasDefinition(STRING definition)
	{
		for (const auto& [key, value] : definitions)
		{
			if (key == definition)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * Get definition value.
	 */
	STRING Program::getDefinition(STRING definition)
	{
		for (const auto& [key, value] : definitions)
		{
			if (key == definition)
			{
				return value;
			}
		}
		return "<unknown>";
	}

	/**
	 * Set definition value.
	 */
	void Program::setDefinition(STRING definition, STRING value)
	{
		definitions[definition] = value;
	}

	/**
     * Read file, build bytecode.
	 */
	void Program::buildFile(STRING path, LIST& bytecode)
	{
		FILE_INPUT_STREAM stream;
		stream.open(path);

		// check state

		if (stream.fail())
		{
			error("Unable to read file: " << path);
		}

		// read lines

		LIST lines = Files::readLines(stream);

		// handle lines

		for (STRING line : lines)
		{
			int begin = 0;

			// calculate statement start index

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] != ' ' && line[i] != '\t')
				{
					break;
				}
				begin++;
			}

			// ignore any spaces or tabs before the statement

			if (begin > 0)
			{
				line = line.substr(begin, line.length());
			}

			// ignore empty lines and comment lines

			if (line.length() == 0 || line[0] == ';')
			{
				continue;
			}

			// split instruction

			LIST args;
			Strings::split(line, args, ' ');
		
			// handle definitions

			if (args[0] == "#define")
			{
				// get definition key

				STRING definition = args[1];

				// warn definition duplication

				if (hasDefinition(definition))
				{
					println("[Warning] Duplicate definition '" << definition << "' in file " << path);
				}

				// set definition

				LIST subArgs = Lists::subList(args, 2);
				setDefinition(definition, Strings::join(subArgs, " "));
			}

			// handle main path

			else if (args[0] == "#main")
			{
				description.setProgramMain(args[1]);
			}

			// perform instruction manipulation

			if (args[0] != "spush")
			{
				// replace definitions

				// for (const auto& [key, value] : definitions)
				// {
				// 	Strings::replace(line, "$" + key, value);
				// }

				// remove comments

				for (size_t i = 0; i < line.size(); i++)
				{
					if (line[i] == ';')
					{
						line = line.substr(0, i);
						break;
					}
				}
			}

			// append modified bytecode

			bytecode.push_back(line);
		}

		// close input stream

		stream.close();
	}
}