#pragma once

#include "../../Common.hpp"

namespace Void
{
	class Class;

	/**
	 * Determines an object in the virtual machine 
	 * that is able to execute bytecode in it.
	 */
	class Executable
	{
	protected:
		/**
		 * The bytecode to be executed.
		 */
		LIST bytecode;

		/**
		 * Sections are made to link up section names
		 * with their corresponding line numbers. 
		 */
		MAP<STRING, SIZE_T> sections;

		/**
		 * Linkers are made to link up variable names
		 * with their corresponding storage units.
		 */
		MAP<STRING, int> linkers;
		
	public:
		/**
		 * The class that holds the executable.
		 */
		Class* clazz;

		/**
		 * Determine if the given section exists.
		 */
		bool hasSection(STRING section);

		/**
		 * Determine if the given linker exists.
		 */
		bool hasLinker(STRING linker);

		/**
		 * Determine if the given linker value exists.
		 */
		bool hasLinkerValue(int value);

		/**
		 * Get the value of a linker.
		 */
		int getLinker(STRING linker);

		/**
		 * Get the value of a section.
		 */
		SIZE_T getSection(STRING section);
	};
}