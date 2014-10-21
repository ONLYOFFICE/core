#pragma once
#ifndef PPTX_SLIDES_NOTESMASTER_INCLUDE_H_
#define PPTX_SLIDES_NOTESMASTER_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"
#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/Hf.h"
#include "Logic/TextListStyle.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	class NotesMaster : public WrapperFile, public FileContainer
	{
	public:
		NotesMaster()
		{
		}
		NotesMaster(const OOX::CPath& filename, FileMap& map)
		{
			read(filename, map);
		}
		virtual ~NotesMaster()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			//FileContainer::read(filename, map);
		
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);

			cSld = oNode.ReadNode(_T("p:cSld"));
			cSld.SetParentFilePointer(this);

			clrMap = oNode.ReadNode(_T("p:clrMap"));
			clrMap.SetParentFilePointer(this);

			hf = oNode.ReadNode(_T("p:hf"));
			if (hf.IsInit())
				hf->SetParentFilePointer(this);

			notesStyle = oNode.ReadNode(_T("p:notesStyle"));
			if (notesStyle.is_init())
				notesStyle->SetParentFilePointer(this);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			oAttr.Write(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);

			XmlUtils::CNodeValue oValue;
			oValue.Write(cSld);
			oValue.Write(clrMap);
			oValue.WriteNullable(hf);
			oValue.WriteNullable(notesStyle);

			XmlUtils::SaveToFile(filename.m_strFilename, XmlUtils::CreateNode(_T("p:notesMaster"), oAttr, oValue));
			
			content.registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
			FileContainer::write(filename, directory, content);
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::NotesMaster;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		void ApplyRels()
		{
			Theme = (FileContainer::get(PPTX::FileTypes::ThemePPTX)).smart_dynamic_cast<PPTX::Theme>();

			if (Theme.IsInit())
				Theme->SetColorMap(clrMap);

			tableStyles_ = (Theme->presentation->get(PPTX::FileTypes::TableStyles)).smart_dynamic_cast<PPTX::TableStyles>();
		}

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::NotesMasters);

			pWriter->WriteRecord1(0, cSld);
			pWriter->WriteRecord1(1, clrMap);
			pWriter->WriteRecord2(2, hf);
			pWriter->WriteRecord2(3, notesStyle);

			pWriter->EndRecord();
		}

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:notesMaster"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->EndAttributes();

			cSld.toXmlWriter(pWriter);
			clrMap.toXmlWriter(pWriter);
			pWriter->Write(hf);
			pWriter->Write(notesStyle);

			pWriter->EndNode(_T("p:notesMaster"));
		}

		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(1); // type
			LONG end = pReader->GetPos() + pReader->GetLong() + 4;

			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						cSld.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						clrMap.fromPPTY(pReader);
						break;
					}
					case 2:
					{
						hf = new Logic::HF();
						hf->fromPPTY(pReader);
						break;
					}
					case 3:
					{
						notesStyle = new Logic::TextListStyle();
						notesStyle->fromPPTY(pReader);
						break;
					}
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(end);
		}

	public:
		smart_ptr<Theme>			Theme;
		smart_ptr<TableStyles>		tableStyles_;

		Logic::CSld						cSld;
		Logic::ClrMap					clrMap;
		nullable<Logic::HF>				hf;
		nullable<Logic::TextListStyle>	notesStyle;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_NOTESMASTER_INCLUDE_H_