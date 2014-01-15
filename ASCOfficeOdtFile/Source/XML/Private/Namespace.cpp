
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Namespace.h"
#include "Lexigraph.h"
#include "Encoding.h"


namespace XML
{
	namespace Private
	{
		//static const std::string strEmpty = "";

		Namespace::Namespace()
			//: Name(strEmpty)
		{
		}


		Namespace::Namespace(const std::string& prefix, const std::string& name)
			:	Prefix(prefix),
				Name(name)
		{
		}


		const bool Namespace::operator ==(const Namespace& rhs) const
		{
			return Prefix == rhs.Prefix;
		}


		const bool Namespace::operator <(const Namespace& rhs) const
		{
			return Prefix < rhs.Prefix;
		}


		const bool Namespace::exist() const
		{
			return true;
		}


		const std::string Namespace::ToString() const
		{
			if (Prefix != "xmlns")
				return "xmlns:" + (/***/Prefix)/*.ToString()*/ + "=\"" + Private::Lexigraph::toSource(/***/Name/*.ToString()*/) + "\"";
			return "xmlns=\"" + Private::Lexigraph::toSource(/***/Name/*.ToString()*/) + "\"";
		}


		const std::wstring Namespace::ToWString() const
		{
			if (Prefix != "xmlns")
				return L"xmlns:" + Encoding::utf82unicode(/***/Prefix/*.ToString()*/) + L"=\""
				+ Encoding::utf82unicode(Private::Lexigraph::toSource(/***/Name/*.ToString()*/)) + L"\"";
			return L"xmlns=\"" + Encoding::utf82unicode(Private::Lexigraph::toSource(/***/Name/*.ToString()*/)) + L"\"";
		}


		void Namespace::SaveToStringList(std::list<std::string>& strList)const
		{
			if (Prefix != "xmlns")
			{
				strList.push_back("xmlns:");
				strList.push_back(Prefix);
				strList.push_back("=\"");
				strList.push_back(Private::Lexigraph::toSource(Name));
				strList.push_back("\"");
				return;
			}
			strList.push_back("xmlns=\"");
			strList.push_back(Private::Lexigraph::toSource(Name));
			strList.push_back("\"");
		}


		void Namespace::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			if (Prefix != "xmlns")
			{
				strList.push_back(L"xmlns:");
				strList.push_back(Encoding::utf82unicode(Prefix));
				strList.push_back(L"=\"");
				strList.push_back(Encoding::utf82unicode(Private::Lexigraph::toSource(Name)));
				strList.push_back(L"\"");
				return;
			}
			strList.push_back(L"xmlns=\"");
			strList.push_back(Encoding::utf82unicode(Private::Lexigraph::toSource(Name)));
			strList.push_back(L"\"");
		}


		const std::string Namespace::GetPrefix()const
		{
			return Prefix;
		}


		const std::string Namespace::GetName()const
		{
			return Name;
		}

	} // namespace Private
} // namespace XML