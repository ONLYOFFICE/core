#pragma once
#ifndef PPTX_PRESPROPS_FILE_INCLUDE_H_
#define PPTX_PRESPROPS_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "Logic/UniColor.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	class PresProps : public WrapperFile
	{
	public:
		PresProps()
		{
		}
		PresProps(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~PresProps()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			ClrMru.clear();
			XmlUtils::CXmlNode oNodeClr;
			if (oNode.GetNode(_T("p:clrMru"), oNodeClr))
				oNodeClr.LoadArray(_T("*"), ClrMru);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			oAttr.Write(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			oAttr.Write(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(_T("p:clrMru"), ClrMru);

			XmlUtils::SaveToFile(filename.m_strFilename, XmlUtils::CreateNode(_T("p:presentationPr"), oAttr, oValue));
		
			content.registration(type().OverrideType(), directory, filename);
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:presentationPr"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			
			pWriter->EndAttributes();

			pWriter->EndNode(_T("p:presentationPr"));
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::PresProps;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	public:
		std::vector<Logic::UniColor>	ClrMru;
		//prnPr (Printing Properties)
		//showPr (Presentation-wide Show Properties)
	};
} // namespace PPTX

#endif // PPTX_PRESPROPS_FILE_INCLUDE_H_