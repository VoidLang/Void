#pragma once

#include "../Common.hpp"
#include "element/VClass.hpp"
#include "element/VResource.hpp"

namespace Void
{
	/**
	 * Represents the Void program that holds all the elements created by the parser.
	 */
	class VProgram
	{
	private:
		/**
		 * List of the classes to be compiled.
		 */
		LIST_T<VClass*> classes;

		/**
		 * List of resources to be compiled.
		 */
		LIST_T<VResource*> resources;

	public:
		/**
		 * Define a class.
		 */
		bool defineClass(VClass* clazz);

		/**
		 * Get a class with the given name.
		 */
		VClass* getClass(STRING name);

		/**
		 * Determine whether a class with the given name exists.
		 */
		bool hasClass(STRING name);

		/**
		 * Build the program to bytecode.
		 */
		LIST build();
	};
}