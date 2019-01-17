/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#ifndef PPTX_PRESPROPS_FILE_INCLUDE_H_
#define PPTX_PRESPROPS_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"
#include "FileTypes.h"

#include "Logic/UniColor.h"
#include "ShowPr/ShowPr.h"

namespace PPTX
{
	class PresProps : public WrapperFile, public PPTX::FileContainer
	{
	public:
		PresProps(OOX::Document* pMain) : WrapperFile(pMain), PPTX::FileContainer(pMain)
		{
		}
		PresProps(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain), PPTX::FileContainer(pMain)
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
			oNode.FromXmlFile(filename.m_strFilename);

			ClrMru.clear();
			XmlUtils::CXmlNode oNodeClr;
			if (oNode.GetNode(_T("p:clrMru"), oNodeClr))
                XmlMacroLoadArray(oNodeClr, _T("*"), ClrMru, Logic::UniColor);

			showPr = oNode.ReadNode(_T("p:showPr"));
			if(showPr.is_init())
				showPr->SetParentFilePointer(this);
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
			FileContainer::write(filename, directory, content);
		}
		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::PresProps);

			pWriter->WriteRecordArray(0, 0, ClrMru);
			pWriter->WriteRecord2(1, showPr);

			pWriter->EndRecord();
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:presentationPr"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
			
			pWriter->EndAttributes();

			pWriter->WriteArray(_T("p:clrMru"), ClrMru);
			pWriter->Write(showPr);

			pWriter->EndNode(_T("p:presentationPr"));
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
						pReader->Skip(4); // len
						ULONG _c = pReader->GetULong();

						for (ULONG i = 0; i < _c; ++i)
						{
							pReader->Skip(1); // type (0)
							LONG pos = pReader->GetPos();
							LONG nElemLength = pReader->GetLong(); // len
							if(nElemLength > 0)
							{
								pReader->Seek(pos);
								Logic::UniColor elm;
								ClrMru.push_back(elm);
								ClrMru.back().fromPPTY(pReader);

								if (!ClrMru.back().is_init())
								{
									ClrMru.pop_back();
								}
							}
						}
						break;
					}
					case 1:
					{
						showPr = new nsShowPr::ShowPr();
						showPr->fromPPTY(pReader);
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
		virtual const OOX::FileType type() const
		{
			return OOX::Presentation::FileTypes::PresProps;
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
		nullable<nsShowPr::ShowPr>	showPr;
		//prnPr (Printing Properties)
	};
} // namespace PPTX

#endif // PPTX_PRESPROPS_FILE_INCLUDE_H_
