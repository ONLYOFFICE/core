#pragma once
#ifndef XML_XDOCUMENT_INCLUDE_H_
#define XML_XDOCUMNET_INCLUDE_H_

#include "property.h"
#include "XElement.h"
#include "Private/XDeclaration.h"
#include <boost/filesystem.hpp>
//#include "Private/XSource.h"
#include "Private/XSingleSource.h"
#include "Private/XWideSource.h"
#include <boost/shared_ptr.hpp>


namespace XML
{
	class XDocument
	{
	public:
		explicit XDocument(const XElement& element);
		explicit XDocument(const char* source, const bool space = false);
		explicit XDocument(const wchar_t* source, const bool space = false);
		explicit XDocument(const std::string& source, const bool space = false);
		explicit XDocument(const std::wstring& source, const bool space = false);
		explicit XDocument(const boost::filesystem::path& source, const bool space = false);
		explicit XDocument(const boost::filesystem::wpath& source, const bool space = false);

	public:
		XDocument const* const	operator->() const	{return this;}
		XDocument*							operator->()				{return this;}

	public:
		const std::string ToString();
		const std::wstring ToWString();
	
	public:
		void Save(const boost::filesystem::path& path);
		void Save(const boost::filesystem::wpath& path);
		void Save(std::string& source);
		void Save(std::wstring& source);

	public:
		property<Private::XDeclaration> Declaration;
		XElement	Root;

	private:
		void Load(boost::shared_ptr<Private::XSingleSource> source, const bool space = false);
		void Load(boost::shared_ptr<Private::XWideSource> source, const bool space = false);
	};
} // namespace XML

#endif // XML_XDOCUMENT_INCLUDE_H_