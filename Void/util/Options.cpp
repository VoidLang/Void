#include "Options.hpp"

namespace Void
{
	/**
	 * Parse the arguments.
	 */
	Options::Options(int argc, char** argv)
	{
		// these variables are used for the parsing

		std::string string = "";
		std::string theKey = "";

		bool lastWasKey = false;
		bool argsStarted = false;
		bool argsRequested = false;

		// parse arguments
		// skip first argument, because that is the file path

		for (int i = 1; i < argc; i++)
		{
			std::string arg = argv[i];

			// handle program arguments
			// anything after -run <file>

			if (argsStarted)
			{
				arguments.push_back(arg);
				continue;
			}

			// each key should start with '-'

			bool key = arg[0] == '-';

			// handle key

			if (key)
			{
				// key follows a key -> the key has no value

				if (lastWasKey)
				{
					data[theKey] = "";
					theKey = "";
				}

				theKey = arg.substr(1);

				// handle the last argument

				if (i == argc - 1)
				{
					data[theKey] = "";
					theKey = "";
				}

				// handle the -run key

				else if (theKey == "run")
				{
					argsRequested = true;
				}
			}

			// handle value

			else
			{
				// handle -run request

				data[theKey] = arg;
				theKey = "";

				if (argsRequested)
				{
					argsStarted = true;
				}
			}

			// update last key

			lastWasKey = key;
		}
	}

	/**
	 * Determine if the key is present in the data.
	 */
	bool Options::has(std::string key)
	{
		return data.count(key);
	}

	/**
	 * Get the value of the key in the data.
	 */
	std::string Options::get(std::string key)
	{
		return data[key];
	}
}