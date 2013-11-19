
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Comment.h"
#include "Lexigraph.h"
#include "Encoding.h"


namespace XML
{
	namespace Private
	{

		Comment::Comment()
		{
		}


		Comment::Comment(const std::string& value)
			: Value(value)
		{
		}


		Comment::~Comment()
		{
		}


		const bool Comment::isElement() const
		{
			return false;
		}


		const bool Comment::isText() const
		{
			return false;
		}


		const bool Comment::isComment() const
		{
			return true;
		}


		const std::string Comment::ToString() const
		{
			return "<!--" + Private::Lexigraph::toSource(Value.ToString()) + "-->";
		}


		const std::wstring Comment::ToWString() const
		{
			return L"<!--" + Private::Lexigraph::toSource(Value.ToWString()) + L"-->";
		}


		void Comment::SaveToStringList(std::list<std::string>& strList)const
		{
			strList.push_back("<!--");
			strList.push_back(Private::Lexigraph::toSource(Value.ToString()));
			strList.push_back("-->");
		}


		void Comment::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			strList.push_back(L"<!--");
			strList.push_back(Private::Lexigraph::toSource(Value.ToWString()));
			strList.push_back(L"-->");
		}

	} // namespace Private
} // namespace XML