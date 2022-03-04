#pragma once

#include "../Common.hpp"
#include "Token.hpp"

namespace Void
{
	/**
	 * 
	 */
	class NodeParser
	{
	private:
		LIST_T<Token>& tokens;

		SIZE_T& begin;

		SIZE_T& cursor;

	public:
		NodeParser(LIST_T<Token>& tokens, SIZE_T& begin, SIZE_T& cursor);
	};
}