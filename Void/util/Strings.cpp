#include "Strings.hpp"

#include <utility>

namespace Void
{
	/**
	 * Split up string into parts between each "separator".
	 */
	size_t Strings::split(STRING& string, LIST& result, char separator)
	{
		size_t pos = string.find(separator);
		size_t initialPos = 0;

		result.clear();

		// decompose statement
		while (pos != std::string::npos)
		{
			result.push_back(string.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = string.find(separator, initialPos);
		}

		// add the last one
		result.push_back(string.substr(initialPos, std::min(pos, string.size()) - initialPos + 1));

		return result.size();
	}

	/**
	 * Join all list elements in a list using a separator.
	 */
	STRING Strings::join(LIST& list, STRING separator)
	{
		std::string value = "";
		for (size_t i = 0; i < list.size(); i++)
		{
			value += list[i];
			if (i < list.size() - 1)
			{
				value += separator;
			}
		}
		return value;
	}

	/**
     * Update all the certain parts of the string to a new value.
	 */
	bool Strings::replace(STRING& string, STRING from, STRING to)
	{
		size_t startPos = string.find(from);
		if (startPos == STRING::npos)
		{
			return false;
		}
		string.replace(startPos, from.length(), to);
		return true;
	}

	/**
	 * Determine if the given string starts with the given prefix.
	 */
	bool Strings::startsWith(STRING string, STRING prefix)
	{
		return string.rfind(prefix, 0) == 0;
	}
}