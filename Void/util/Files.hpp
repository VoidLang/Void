#pragma once

#include "../Common.hpp"

namespace Void
{
	namespace Files
	{
		const char SEPARATOR = 
#ifdef _WIN32
								'\\';
#else
								'/';
#endif _WIN32


		/**
		 * Recursively iterate over a directory.
		 */
		void walk(STRING directory, LIST_T<PATH>& result);

		/**
		 * Read all lines from the input stream.
		 */
		LIST readLines(FILE_INPUT_STREAM& stream);

		/**
		 * Read the content of a file.
		 */
		STRING readAll(FILE_INPUT_STREAM& stream);
	}
}