#pragma once

#include "../Common.hpp"
#include "element/Class.hpp"
#include "../util/Options.hpp"

namespace Void
{
	class Class;
	class Stack;

	/**
	 * The factory manages any kind of class handling.
	 */
	class Factory
	{
	public:
		/**
		 * List of all the classes existing during runtime.
		 */
		LIST_T<Class*> classes;

		/**
		 * Program start options.
		 */
		Options& options;

		/**
		 * Initialize factory.
		 */
		Factory(Options& options);

		/**
		 * Build the factory.
		 * Make classes using bytecode.
		 */
		void build(LIST bytecode);

		/**
		 * Initialize static class fields.
		 * Call static constructors.
		 */
		void initialize(Stack* heap);

		/**
		 * Determine if a class with the given name exists.
		 */
		bool hasClass(STRING name);

		/**
		 * Get a class with the given name.
		 */
		Class* getClass(STRING name);

		/**
		 * Define a class in the factory.
		 */
		void defineClass(Class* clazz);

		/**
		 * Load a dynamic-link library and implement native methods for the given class.
		 */
		void loadLibrary(STRING name, Class* clazz);
	};
}