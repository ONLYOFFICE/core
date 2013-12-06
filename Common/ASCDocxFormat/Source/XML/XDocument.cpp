
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


	XDocument::XDocument(const char* source, const bool space)
	{
		boost::filesystem::wpath path(Encoding::utf82unicode(std::string(source)));
		TxtFile text(CString(path.string().c_str()));
		if(text.isUnicode())
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			boost::shared_ptr<Private::XSingleSource> xsource(new Private::XFileSource(path));
			Load(xsource, space);
		}
	}


	XDocument::XDocument(const wchar_t* source, const bool space)
	{
		boost::filesystem::wpath path(source);
		TxtFile text(CString(path.string().c_str()));
		if(text.isUnicode())
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(path, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			boost::shared_ptr<Private::XSingleSource> xsource(new Private::XFileSource(path));
			Load(xsource, space);
		}
	}


	XDocument::XDocument(const std::string& source, const bool space)
	{
		boost::shared_ptr<Private::XSingleSource> xsource(new Private::XStringSource(source));
		Load(xsource, space);
	}


	XDocument::XDocument(const std::wstring& source, const bool space)
	{
		boost::shared_ptr<Private::XWideSource> xsource(new Private::XWStringSource(source));
		Load(xsource, space);
	}


	XDocument::XDocument(const boost::filesystem::path& source, const bool space)
	{
		boost::filesystem::wpath path(Encoding::utf82unicode(source.string()));
		TxtFile text(CString(path.string().c_str()));
		if(text.isUnicode())
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			boost::shared_ptr<Private::XSingleSource> xsource(new Private::XFileSource(path));
			Load(xsource, space);
		}
	}


	XDocument::XDocument(const boost::filesystem::wpath& source, const bool space)
	{
		TxtFile text(CString(source.string().c_str()));
		if(text.isUnicode())
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estLittleEndian));
			Load(xsource, space);
		}
		else if((text.isBigEndian()) || (text.isUnicodeWithOutBOM()))
		{
			boost::shared_ptr<Private::XWideSource> xsource(new Private::XWFileSource(source, Private::XWideSource::estBigEndian));
			Load(xsource, space);
		}
		else
		{
			boost::shared_ptr<Private::XSingleSource> xsource(new Private::XFileSource(source));
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


	void XDocument::Load(boost::shared_ptr<Private::XSingleSource> source, const bool space)
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


	void XDocument::Load(boost::shared_ptr<Private::XWideSource> source, const bool space)
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


	void XDocument::Save(const boost::filesystem::path& path)
	{
		std::ofstream file(path.string().c_str());
		if (!file.bad())
		{
			file << ToString();
			file.close();
		}
	}


	void XDocument::Save(const boost::filesystem::wpath& path)
	{
		std::ofstream file(path.string().c_str());
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