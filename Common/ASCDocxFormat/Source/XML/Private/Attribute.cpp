
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Attribute.h"
#include "Lexigraph.h"
#include "Encoding.h"


namespace XML
{
	namespace Private
	{

		Attribute::Attribute(const XML::XName& xname, const std::string& value)
			: XName(xname),
				Value(value)
		{
		}


		Attribute::Attribute()
		{
		}


		const bool Attribute::operator ==(const Attribute& rhs) const
		{
			//return ((XName == rhs.XName) || (XName->Equal(*rhs.XName)));// && Value == rhs.Value;
			return (XName->Equal(*rhs.XName));
		}


		const bool Attribute::operator <(const Attribute& rhs) const
		{
			return ((*XName->Name) < (*rhs.XName->Name));
		}


		const bool Attribute::exist() const
		{
			return true;
		}


		const std::string Attribute::ToString() const
		{
			return XName->ToString() + "=\"" + Private::Lexigraph::toSource(Value->ToString()) + "\"";
		}


		const std::wstring Attribute::ToWString() const
		{
			return XName->ToWString() + L"=\"" + Encoding::utf82unicode(Private::Lexigraph::toSource(Value->ToString())) + L"\"";
		}


		void Attribute::SaveToStringList(std::list<std::string>& strList)const
		{
			XName->SaveToStringList(strList);
			strList.push_back("=\"");
			strList.push_back(Private::Lexigraph::toSource(Value->ToString()));
			strList.push_back("\"");
		}


		void Attribute::SaveToWStringList(std::list<std::wstring>& strList)const
		{
			XName->SaveToWStringList(strList);
			strList.push_back(L"=\"");
			strList.push_back(Encoding::utf82unicode(Private::Lexigraph::toSource(Value->ToString())));
			strList.push_back(L"\"");
		}


		const XString Attribute::value() const
		{
			return *Value;
		}

	} // namespace Private
} // namespace XML