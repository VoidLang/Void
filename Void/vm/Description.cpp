#include "Description.hpp"

namespace Void
{
	/**
	 * Determines if the program main is set.
	 */
	bool Description::hasProgramMain()
	{
		return programMain.has_value();
	}

	/**
	 * Get the program's main path.
	 */
	STRING Description::getProgramMain()
	{
		return *programMain;
	}

	/**
	 * Sets the value of the program main.
	 */
	void Description::setProgramMain(STRING programMain)
	{
		this->programMain = programMain;
	}
}