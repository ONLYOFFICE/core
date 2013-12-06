
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XDocument.h"
#include "Private/XSpace.h"
#include "Exception/not_implement.h"
#include "Private/XFileSource.h"
#include "Private/XWFileSource.h"
#include "Private/XStringSource.h"
#include "Private/XWStringSource.h"
#include <list>
#include "Utility.h"

namespace XML
{

	XDocument::XDocument(const XElement& element)
		: Root(element)
	{
	}

	XDocument::XDocument(const wchar_t* source, const bool space)
	{
		OOX::CPath path(source);
		TxtFile text(path);
		if(text.isUnicode())
		{
			NSCommon::smart_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			NSCommon::smart_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			NSCommon::smart_ptr<Private::XSingleSource> xsource(new Private::XFileSource(path));
			Load(xsource, space);
		}
	}


	XDocument::XDocument(const std::string& source, const bool space)
	{
		NSCommon::smart_ptr<Private::XSingleSource> xsource(new Private::XStringSource(source));
		Load(xsource, space);
	}


	XDocument::XDocument(const std::wstring& source, const bool space)
	{
		NSCommon::smart_ptr<Private::XWideSource> xsource(new Private::XWStringSource(source));
		Load(xsource, space);
	}

	XDocument::XDocument(const OOX::CPath& source, const bool space)
	{
		TxtFile text(source);
		if(text.isUnicode())
		{
			NSCommon::smart_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			NSCommon::smart_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			NSCommon::smart_ptr<Private::XSingleSource> xsource(new Private::XFileSource(source));
			Load(xsource, space);
		}
	}


	const std::string XDocument::ToString()
	{
		Root->Namespaces.merge(Root->usedNamespace());

		std::list<std::string> strList;
		strList.push_back(Declaration->ToString());
		Root->SaveToStringList(strList);

		std::string result = "";
		size_t length = 0;
		for(std::list<std::string>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
		{
			length += iter->length();
		}
		result.reserve(length);
		for(std::list<std::string>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
		{
			result += *iter;
		}

		return result;
		//return Declaration->ToString() + Root->ToString();
	}


	const std::wstring XDocument::ToWString()
	{
		Root->Namespaces.merge(Root->usedNamespace());

		std::list<std::wstring> strList;
		strList.push_back(Declaration->ToWString());
		Root->SaveToWStringList(strList);

		std::wstring result = L"";
		size_t length = 0;
		for(std::list<std::wstring>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
		{
			length += iter->length();
		}
		result.reserve(length);
		for(std::list<std::wstring>::const_iterator iter = strList.begin(); iter != strList.end(); iter++)
		{
			result += *iter;
		}

		return result;
		//return Declaration->ToWString() + Root->ToWString();
	}


	void XDocument::Load(NSCommon::smart_ptr<Private::XSingleSource> source, const bool space)
	{
		source->findAndSkip('<');
		if (source->get() == '?')
		{
			Declaration->fromSource(source);
			source->findAndSkip('<');
		}
		source->skipSpace();
		Root->fromSource(source, Root->Namespaces, space);
	}


	void XDocument::Load(NSCommon::smart_ptr<Private::XWideSource> source, const bool space)
	{
		source->findAndSkip(L'<');
		if (source->get() == L'?')
		{
			Declaration->fromSource(source);
			source->findAndSkip(L'<');
		}
		source->skipSpace();
		Root->fromSource(source, Root->Namespaces, space);
	}

	void XDocument::Save(const OOX::CPath& path)
	{
		std::ofstream file(path.GetPath());
		if (!file.bad())
		{
			file << ToString();
			file.close();
		}
	}


	void XDocument::Save(std::string& source)
	{
		source.clear();
		source = ToString();
	}


	void XDocument::Save(std::wstring& source)
	{
		source.clear();
		source = ToWString();
	}

} // namespace XML