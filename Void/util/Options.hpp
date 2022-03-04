#pragma once

#include "../Common.hpp"

namespace Void
{
	/**
	 * Command line options parser.
	 */
	class Options
	{
	private:
		/**
		 * Program arguments data.
		 */
		std::map<std::string, std::string> data;

	public:
		/**
		 * Program start arguments. 
		 */
		LIST arguments;

		/**
		 * Parse the arguments.
		 */
		Options(int argc, char** argv);

		/**
		 * Determine if the key is present in the data.
		 */
		bool has(std::string key);

		/**
		 * Get the value of the key in the data.
		 */
		std::string get(std::string key);
	};
}