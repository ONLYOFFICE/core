
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Text.h"
#include "Lexigraph.h"
#include "Encoding.h"


namespace XML
{
	namespace Private
	{

		Text::Text()
		{
		}


		Text::Text(const std::string& value)
			: Value(value)
		{
		}


		Text::~Text()
		{
		}


		const bool Text::isElement() const
		{
			return false;
		}


		const bool Text::isText() const
		{
			return true;
		}


		const bool Text::isComment() const
		{
			return false;
		}


		const std::string Text::ToString() const
		{
			return Private::Lexigraph::toSource(Value.ToString());
		}


		const std::wstring Text::ToWString() const
		{
			return Private::Lexigraph::toSource(Value.ToWString());
		}


		void Text::SaveToStringList(std::list<std::string>& strList)const
		{
			strList.push_back(Private::Lexigraph::toSource(Value.ToString()));
		}


		void Text::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			strList.push_back(Private::Lexigraph::toSource(Value.ToWString()));
		}

	} // namespace Private
} // namespace XML