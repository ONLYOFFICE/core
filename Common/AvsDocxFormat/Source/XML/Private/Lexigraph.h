#pragma once
#ifndef XML_PRIVATE_LEXIGRAPH_INCLUDE_H_
#define XML_PRIVATE_LEXIGRAPH_INCLUDE_H_

#include <string>


namespace XML
{
	namespace Private
	{
		class Lexigraph
		{
		public:
			static const std::string fromSource(const std::string& input);
			static const std::string toSource(const std::string& input);

			static const std::wstring fromSource(const std::wstring& input);
			static const std::wstring toSource(const std::wstring& input);
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_LEXIGRAPH_INCLUDE_H_