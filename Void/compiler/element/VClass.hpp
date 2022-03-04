#pragma once

#include "../../Common.hpp"
#include "../Token.hpp"

namespace Void
{
	class VMethod;

	/**
	 * Represents a Void class to be built by the compiler.
	 */
	class VClass
	{
	private:
		/**
		 * The parent class.
		 */
		VClass* parent;

	public:
		/**
		 * The name of the class.
		 */
		STRING name;

		/**
		 * The modifiers of the class.
		 */
		LIST modifiers;

		/**
		 * The declared methods of the class.
		 */
		LIST_T<VMethod*> methods;

		/**
		 * Initialize class.
		 */
		VClass(VClass* parent);

		/**
		 * Initialize class.
		 */
		VClass();

		/**
		 * Perform class token parsing.
		 */
		bool parse(LIST_T<Token> tokens, SIZE_T& begin, SIZE_T& cursor);

		/**
		 * Determine whether the given modifier is a class modifier. 
		 */
		bool isClassModifier(STRING modifier);

		/**
		 * Build the class to bytecode.
		 */
		void build(LIST& bytecode);
	};
}