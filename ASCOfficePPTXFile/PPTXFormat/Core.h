#pragma once
#ifndef PPTX_CORE_FILE_INCLUDE_H_
#define PPTX_CORE_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "DocxFormat/WritingElement.h"
//#include "DocxFormat/DateTime.h"
#include "Limit/ContentStatus.h"
#include "DocxFormat/FileTypes.h"

using namespace NSBinPptxRW;

namespace PPTX
{
	class Core : public WrapperFile
	{
	public:
		Core()
		{
		}
		Core(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~Core()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			oNode.ReadNodeValueBase(_T("dc:title"), title);
			oNode.ReadNodeValueBase(_T("dc:creator"), creator);
			oNode.ReadNodeValueBase(_T("cp:lastModifiedBy"), lastModifiedBy);
			oNode.ReadNodeValueBase(_T("cp:revision"), revision);
			oNode.ReadNodeValueBase(_T("dcterms:modified"), modified);
			oNode.ReadNodeValueBase(_T("dcterms:created"), created);

	//		created = OOX::DateTime::Parse(document.Root.element("created").text().ToString());
	//			modified = OOX::DateTime::Parse(document.Root.element("modified").text().ToString());
	/*
			category = document.Root.element("category").text();
			contentStatus = document.Root.element("contentStatus").text();
			description = document.Root.element("description").text();
	//identifier - ???
	//			keywords = document.Root.element("keywords").text();
			language = document.Root.element("language").text();
	//			lastPrinted = OOX::DateTime::Parse(document.Root.element("lastPrinted").text().ToString());
			subject = document.Root.element("subject").text();
			version = document.Root.element("version").text();
	*/
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::ContentTypes::File& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:dc"), OOX::g_Namespaces.dc.m_strLink);
			//oAttr.Write(_T("xmlns:cp"), OOX::g_Namespaces.cp.m_strLink);
			oAttr.Write(_T("xmlns:dcterms"), OOX::g_Namespaces.dcterms.m_strLink);
			oAttr.Write(_T("xmlns:xsi"), OOX::g_Namespaces.xsi.m_strLink);

			XmlUtils::CNodeValue oValue;
			oValue.Write2(_T("dc:title"), title);
			oValue.Write2(_T("dc:creator"), creator);
			oValue.Write2(_T("cp:lastModifiedBy"), lastModifiedBy);
			oValue.Write2(_T("cp:revision"), revision);

			if (created.IsInit())
			{
				oValue.m_strValue += _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">");
				oValue.m_strValue += *created;
				oValue.m_strValue += _T("</dcterms:created>");
			}		
			
			oValue.m_strValue += _T("<dcterms:modified xsi:type=\"dcterms:W3CDTF\">");
			if (modified.IsInit())
				oValue.m_strValue += *modified;
			oValue.m_strValue += _T("</dcterms:modified>");

			XmlUtils::SaveToFile(filename.m_strFilename, XmlUtils::CreateNode(_T("cp:coreProperties"), oAttr, oValue));

	/*
			if(category.is_init())
				root.Add(XML::XElement("category", XML::XText(category)));
			if(contentStatus.is_init())
				root.Add(XML::XElement("contentStatus", XML::XText(contentStatus)));
			if(description.is_init())
				root.Add(XML::XElement(ns.dc + "description", XML::XText(description)));
			//identifier - ??? dc
				//XML::XElement(ns.cp + "keywords", XML::XText(keywords)) +
			if(language.is_init())
				root.Add(XML::XElement(ns.dc + "language", XML::XText(language)));
	//			if(lastModifiedBy.is_init())
	//				root.Add(XML::XElement(ns.cp + "lastModifiedBy", XML::XText(lastModifiedBy)));
				//XML::XElement("lastPrinted",
				//	XML::XAttribute(,)+
				//	XML::XText(lastPrinted)
				//) + //??? namespace ???
	//			if(revision.is_init())
	//				root.Add(XML::XElement(ns.cp + "revision", XML::XText(revision)));
			if(subject.is_init())
				root.Add(XML::XElement(ns.dc + "subject", XML::XText(subject)));
	//			if(title.is_init())
	//				root.Add(XML::XElement(ns.dc + "title", XML::XText(title)));
			if(version.is_init())
				root.Add(XML::XElement("version", XML::XText(version)));
	*/
			content.registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
		}

	public:
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::Core;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSMainTables::Core);

			pWriter->WriteBYTE(g_nodeAttributeStart);

			pWriter->WriteString2(0, title);
			pWriter->WriteString2(1, creator);
			pWriter->WriteString2(2, lastModifiedBy);
			pWriter->WriteString2(3, revision);
			pWriter->WriteString2(4, created);
			pWriter->WriteString2(5, modified);

			pWriter->WriteBYTE(g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("cp:coreProperties"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns:cp"), OOX::g_Namespaces.cp.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dc"), OOX::g_Namespaces.dc.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dcterms"), OOX::g_Namespaces.dcterms.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:xsi"), OOX::g_Namespaces.xsi.m_strLink);

			pWriter->EndAttributes();

			pWriter->WriteNodeValue(_T("dc:title"), title);
			pWriter->WriteNodeValue(_T("dc:creator"), creator);
			pWriter->WriteNodeValue(_T("cp:lastModifiedBy"), lastModifiedBy);
			pWriter->WriteNodeValue(_T("cp:revision"), revision);

			pWriter->WriteNodeValue(_T("dcterms:created xsi:type=\"dcterms:W3CDTF\""), created);
			pWriter->WriteNodeValue(_T("dcterms:modified xsi:type=\"dcterms:W3CDTF\""), modified);
			
			pWriter->EndNode(_T("cp:coreProperties"));
		}

	public:
		nullable_string		title;
		nullable_string		creator;
		nullable_string		lastModifiedBy;
		nullable_string		revision;
//		nullable_property<OOX::DateTime> created;
		nullable_string		created;
//		nullable_property<OOX::DateTime> modified;
		nullable_string		modified;
/*
		nullable_property<std::string> category;
		nullable_property<std::string, Limit::ContentStatus> contentStatus;
		nullable_property<std::string> description;
//identifier - ???
//			nullable_property<std::string> keywords;
		nullable_property<std::string> language;
		nullable_property<OOX::DateTime> lastPrinted;
		nullable_property<std::string> subject;
		nullable_property<std::string> version;
*/
	};
} // namespace PPTX

#endif // PPTX_CORE_FILE_INCLUDE_H_