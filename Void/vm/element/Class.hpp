#pragma once

#include "../../Common.hpp"
#include "Method.hpp"
#include "Field.hpp"
#include "../Factory.hpp"

namespace Void
{
	// forward declare Factory because of recursive usage
	class Factory;
	class Method;
	class Field;

	/**
	 * Represents a Void class in the virtual machine.
	 * Manages all the methods and static fields.
	 * Also supports recursive class declaration.
	 */
	class Class
	{
	private:
		/**
		 * The virtual machine's factory.
		 */
		Factory* factory;

	public:
		/**
		 * The list of the methods of the class.
		 */
		LIST_T<Method*> methods;

		/**
		 * The list of the fields of the class.
		 */
		LIST_T<Field*> fields;

		/**
		 * The name of the class (without packages).
		 */
		STRING name;
		
		/**
		 * The superclass of the class.
		 */
		STRING superclass;

		/**
		 * The modifiers of the class.
		 */
		int modifiers;

		/**
		 * List of the interfaces of the class.
		 */
		LIST interfaces;

		/**
		 * Initialize class.
		 */
		Class(STRING name, STRING superclass, LIST modifiers, LIST interfaces, Factory* factory);

		/**
		 * Initialize static class fields.
		 * Call static constructor if exists.
		 */
		void initialize(Stack* heap);

		/**
		 * Build class content.
		 */
		void build(LIST bytecode);

		/**
		 * Get a method with the given name and arguments.
		 */
		Method* getMethod(STRING name, LIST parameters);

		/**
		 * Get a method with the given name and arguments recursively.
		 */
		Method* findMethod(STRING name, LIST parameters);

		/**
		 * Determine if a method exists with the given name and arguments.
		 */
		bool hasMethod(STRING name, LIST parameters);

		/**
		 * Define a method in the class.
		 */
		void defineMethod(Method* method);

		/**
		 * Get a field with the given name.
		 */
		Field* getField(STRING name);

		/**
		 * Defermine if a field exists with the given name.
		 */
		bool hasField(STRING name);

		/**
		 * Define a field in the class.
		 */
		void defineField(Field* field);

		/**
		 * Debug class and it's content.
		 */
		void debug();

		/**
		 * Parse the native path of the class.
		 */
		STRING nativePath();

		/**
		 * Get the list of the native methods declared.
		 */
		LIST_T<Method*> nativeMethods();
	};
}