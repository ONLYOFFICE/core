#pragma once
#ifndef PPTX_THEME_FILE_INCLUDE_H_
#define PPTX_THEME_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "Theme/ThemeElements.h"
#include "Logic/DefaultShapeDefinition.h"
#include "Theme/ExtraClrScheme.h"

#include "Logic/ShapeProperties.h"
#include "Logic/ClrMap.h"

#include "Presentation.h"
#include "DocxFormat/Media/Image.h"
#include "DocxFormat/Media/OleObject.h"
#include "DocxFormat/External/HyperLink.h"

namespace PPTX
{
	class Theme : public PPTX::WrapperFile, public PPTX::FileContainer
	{
	public:
		Theme()
		{
			isThemeOverride = false;
		}
		Theme(const OOX::CPath& filename, FileMap& map)
		{
			isThemeOverride = false;
			m_map = NULL;
			read(filename, map);
		}
		virtual ~Theme()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			isThemeOverride = false;
			//FileContainer::read(filename, map);
			m_map = NULL;

			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile2(filename.m_strFilename);
			
			CString strNodeName = XmlUtils::GetNameNoNS(oNode.GetName());
			if (_T("themeOverride") == strNodeName)
			{
				themeElements = oNode;
				themeElements.SetParentFilePointer(this);

				extraClrSchemeLst.clear();
				isThemeOverride = true;
				return;
			}

			oNode.ReadAttributeBase(_T("name"), name);

			themeElements = oNode.ReadNode(_T("a:themeElements"));
			themeElements.SetParentFilePointer(this);

			XmlUtils::CXmlNode oDefaults;
			if (oNode.GetNode(_T("a:objectDefaults"), oDefaults))
			{
				spDef = oDefaults.ReadNode(_T("a:spDef"));
				if (spDef.IsInit())
					spDef->SetParentFilePointer(this);

				lnDef = oDefaults.ReadNode(_T("a:lnDef"));
				if (lnDef.IsInit())
					lnDef->SetParentFilePointer(this);

				txDef = oDefaults.ReadNode(_T("a:txDef"));
				if (txDef.IsInit())
					txDef->SetParentFilePointer(this);
			}

			extraClrSchemeLst.clear();
			XmlUtils::CXmlNode oNodeList;
			if (oNode.GetNode(_T("a:extraClrSchemeLst"), oNodeList))
			{
				oNodeList.LoadArray(_T("a:extraClrScheme"), extraClrSchemeLst);
			}

			size_t count = extraClrSchemeLst.size();
			for (size_t i = 0; i < count; ++i)
				extraClrSchemeLst[i].SetParentFilePointer(this);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content)const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			oAttr.Write(_T("name"), name);

			XmlUtils::CNodeValue oValue;
			oValue.Write(themeElements);

			oValue.m_strValue += _T("<a:objectDefaults>");
			oValue.WriteNullable(spDef);
			oValue.WriteNullable(lnDef);
			oValue.WriteNullable(txDef);
			oValue.m_strValue += _T("</a:objectDefaults>");

			oValue.WriteArray(_T("a:extraClrSchemeLst"), extraClrSchemeLst);

			XmlUtils::SaveToFile(filename.m_strFilename, XmlUtils::CreateNode(_T("a:theme"), oAttr, oValue));
			
			content.registration(type().OverrideType(), directory, filename);
			m_written = true;
			m_WrittenFileName = filename.GetFilename();
			FileContainer::write(filename, directory, content);
		}

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::Themes);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, name);

			if (isThemeOverride)
				pWriter->WriteBool1(1, true);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord1(0, themeElements);
			pWriter->WriteRecord2(1, spDef);
			pWriter->WriteRecord2(2, lnDef);
			pWriter->WriteRecord2(3, txDef);
			pWriter->WriteRecordArray(4, 0, extraClrSchemeLst);

			pWriter->EndRecord();
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (isThemeOverride)
			{
				pWriter->StartNode(_T("a:themeOverride"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
				pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
				pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
				pWriter->EndAttributes();

				themeElements.clrScheme.toXmlWriter(pWriter);
				themeElements.fontScheme.toXmlWriter(pWriter);
				themeElements.fmtScheme.toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:themeOverride"));
				return;
			}

			pWriter->StartNode(_T("a:theme"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->EndAttributes();

			themeElements.toXmlWriter(pWriter);
			pWriter->StartNode(_T("a:objectDefaults"));
			pWriter->EndAttributes();
			pWriter->Write(spDef);
			pWriter->Write(lnDef);
			pWriter->Write(txDef);
			pWriter->EndNode(_T("a:objectDefaults"));

			pWriter->WriteArray(_T("a:extraClrSchemeLst"), extraClrSchemeLst);

			pWriter->EndNode(_T("a:theme"));
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			BYTE type = pReader->GetUChar();

			LONG _rec_start = pReader->GetPos();
			LONG _end_rec = _rec_start + pReader->GetULong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
			
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else if (1 == _at)
					isThemeOverride = pReader->GetBool();
				else
					break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						// themeElements
						themeElements.fromPPTY(pReader);
						break;
					}
					case 1:
					{
						spDef = new Logic::DefaultShapeDefinition();
						spDef->m_name = _T("spDef");
						spDef->fromPPTY(pReader);
						break;
					}
					case 2:
					{
						lnDef = new Logic::DefaultShapeDefinition();
						lnDef->m_name = _T("lnDef");
						lnDef->fromPPTY(pReader);
						break;
					}
					case 3:
					{
						txDef = new Logic::DefaultShapeDefinition();
						txDef->m_name = _T("txDef");
						txDef->fromPPTY(pReader);
						break;
					}
					case 4:
					{
						extraClrSchemeLst.clear();
						pReader->Skip(4); // len
						ULONG _len = pReader->GetULong();
						for (ULONG i = 0; i < _len; ++i)
						{
							pReader->Skip(1); // type
							extraClrSchemeLst.push_back(nsTheme::ExtraClrScheme());
							extraClrSchemeLst[i].fromPPTY(pReader);
						}
					}
				}
			}

			pReader->Seek(_end_rec);			
		}

	public:
		virtual const PPTX::FileType type() const
		{
			return PPTX::FileTypes::ThemePPTX;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		//////
		DWORD GetRGBAFromScheme(const CString& str)const
		{
			return themeElements.clrScheme.GetRGBAFromScheme(str);
		}
		DWORD GetARGBFromScheme(const CString& str)const
		{
			return themeElements.clrScheme.GetARGBFromScheme(str);
		}
		DWORD GetBGRAFromScheme(const CString& str)const
		{
			return themeElements.clrScheme.GetBGRAFromScheme(str);
		}
		DWORD GetABGRFromScheme(const CString& str)const
		{
			return themeElements.clrScheme.GetABGRFromScheme(str);
		}

		//////
		DWORD GetRGBAFromMap(const CString& str)const
		{
			return GetRGBAFromScheme(m_map->GetColorSchemeIndex(str));
		}
		DWORD GetARGBFromMap(const CString& str)const
		{
			return GetARGBFromScheme(m_map->GetColorSchemeIndex(str));
		}
		DWORD GetBGRAFromMap(const CString& str)const
		{
			return GetBGRAFromScheme(m_map->GetColorSchemeIndex(str));
		}
		DWORD GetABGRFromMap(const CString& str)const
		{
			return GetABGRFromScheme(m_map->GetColorSchemeIndex(str));
		}

		virtual void FillShapeProperties(Logic::ShapeProperties& props, const CString& type)const
		{
            if(presentation.IsInit())
			{
                props.FillFromTextListStyle(presentation->defaultTextStyle);
				props.SetTextType(0);
			}
			if(type == _T("table-cell"))
				props.FillMasterFontSize(1800);
			//if(spDef.is_init())
			//{
			//	props.FillFromTextListStyle(spDef->lstStyle, true);
			//	if(spDef->style.is_init())
			//		props.FillFontRef(spDef->style->fontRef.get());
			//}
			props.SetMajorLatin(themeElements.fontScheme.majorFont.latin);
			props.SetMinorLatin(themeElements.fontScheme.minorFont.latin);
		}
		virtual CString GetMediaFullPathNameFromRId(const PPTX::RId& rid)const
		{
			smart_ptr<PPTX::Image> p = image(rid);
			if (!p.is_init())
				return _T("");
			return p->filename().m_strFilename;
		}
		virtual CString GetFullHyperlinkNameFromRId(const PPTX::RId& rid)const
		{
			smart_ptr<PPTX::HyperLink> p = hyperlink(rid);
			if (!p.is_init())
				return _T("");
			return p->Uri().m_strFilename;
		}
		virtual CString GetOleFromRId(const PPTX::RId& rid)const
		{
			smart_ptr<PPTX::OleObject> p = oleObject(rid);
			if (!p.is_init())
				return _T("");
			return p->filename().m_strFilename;
		}
		void GetLineStyle(int number, Logic::Ln& lnStyle)const
		{
			themeElements.fmtScheme.GetLineStyle(number, lnStyle);
		}
		void GetFillStyle(int number, Logic::UniFill& fillStyle)const
		{
			themeElements.fmtScheme.GetFillStyle(number, fillStyle);
		}

	public:
		nullable_string							name;
		nsTheme::ThemeElements					themeElements;
		nullable<Logic::DefaultShapeDefinition> spDef;
		nullable<Logic::DefaultShapeDefinition> lnDef;
		nullable<Logic::DefaultShapeDefinition> txDef;
		
		std::vector<nsTheme::ExtraClrScheme>		extraClrSchemeLst;

		bool									isThemeOverride;

        smart_ptr<Presentation>					presentation;

	public:
		void SetColorMap(const Logic::ClrMap& map){m_map = &map;};
	
	private:
		Logic::ClrMap const* m_map;
	};
} // namespace PPTX

#endif // PPTX_THEME_FILE_INCLUDE_H_
