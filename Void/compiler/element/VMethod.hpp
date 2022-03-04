#pragma once

#include "../../Common.hpp"
#include "VClass.hpp"

namespace Void
{
	class VClass;

	/**
	 * Represents a Void method to be built by the compiler.
	 */
	class VMethod
	{
	private:
		/**
		 * The class that holds the method itself.
		 */
		VClass* clazz;

	public:
		/**
		 * The name of the method.
		 */
		STRING name;

		/**
		 * The return type of the method.
		 */
		STRING RETURN_TYPE;

		/**
		 * The modifiers of the method.
		 */
		LIST modifiers;

		/**
		 * Initialize method.
		 */
		VMethod(VClass* clazz);
	
		/**
		 * Perform method token parsing.
		 */
		bool parse(LIST_T<Token> tokens, SIZE_T& begin, SIZE_T& cursor);

		/**
		 * Determine whether the given modifier is a method modifier.
		 */
		bool isMethodModifier(STRING modifier);

		/**
		 * Convert types to virtual machine types.
		 */
		STRING getType(STRING type);

		/**
		 * Build the method to bytecode.
		 */
		void build(LIST& bytecode);
	};
}