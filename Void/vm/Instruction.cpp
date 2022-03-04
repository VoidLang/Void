#include "Instruction.hpp"

namespace Void
{
	bool operator==(STRING string, Instruction instruction)
	{
		static const char* const NAMES[]
		{
			"cdef",
			"cmod",
			"cext",
			"cimpl",
			"cbegin",
			"cend",

			"mdef",
			"mmod",
			"mreturn",
			"mparam",
			"mbegin",
			"mend",

			"fdef",
			"fmod",
			"ftype",
			"fbegin",
			"fend",

			"#link"
		};

		static const char* const NAMES_UNMAPPED[]
		{
			"cdef",
			"cmod",
			"cext",
			"cimpl",
			"cbegin",
			"cend",

			"mdef",
			"mmod",
			"mreturn",
			"mparam",
			"mbegin",
			"mend",

			"fdef",
			"fmod",
			"ftype",
			"fbegin",
			"fend",

			"#link"
		};

		return string == NAMES[static_cast<int>(instruction)]
			|| string == NAMES_UNMAPPED[static_cast<int>(instruction)];
	}
}