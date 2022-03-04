#pragma once

#include "../Common.hpp"

#undef INTERFACE

namespace Void
{
	/**
	 * List of modifiers.
	 */
	enum class Modifier
	{
		PUBLIC       = 0x00000001,
		PRIVATE      = 0x00000002,
		PROTECTED    = 0x00000004,
		STATIC       = 0x00000008,
		FINAL        = 0x00000010,
		SYNCHRONIZED = 0x00000020,
		VOLATILE     = 0x00000040,
		TRANSIENT    = 0x00000080,
		NATIVE       = 0x00000100,
		UNSAFE       = 0x00000200,
		ABSTRACT     = 0x00000400,
		INTERFACE    = 0x00000800,
		ANNOTATION   = 0x00001000,
		ENUM         = 0x00002000,
		STRUCT       = 0x00004000,
		WEAK         = 0x00008000,
		STRONG       = 0x00010000,
		DEFAULT      = 0x00020000
	};

	/**
	 * List of modifier names. 
	 */
	static const char* MODIFIER_KEYS[18] =
	{
		"public", "private", "protected", "static", "final",  "synchronized",
		"volatile", "transient", "native", "unsafe", "abstract", "interface",
		"annotation",  "enum",   "struct",   "weak",   "strong",   "default"
	};

	/**
	 * List of modifier values.
	 */
	static const int MODIFIER_VALUES[18] =
	{
		// haha those magic values
		// wait... ohhh
		0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020,
		0x00000040, 0x00000080, 0x00000100, 0x00000200, 0x00000400, 0x00000800,
		0x00001000, 0x00002000, 0x00004000, 0x00008000, 0x00010000, 0x00020000
	};

	/**
	 * Get the value of a modifier.
	 */
	int getModifier(STRING modifier);

	/**
	 * Get the values of the modifiers.
	 */
	int getModifiers(LIST modifiers);

	/**
	 * Get the names of the modifiers.
	 */
	LIST getModifiers(int modifier);

	/**
	 * Determines if the modifier name matches it's value.
     */
	bool operator==(STRING string, Modifier modifier);
}