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
#ifndef PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_
#define PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_

#include "../../WrapperWritingElement.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class WavAudioFile : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(WavAudioFile)
			
			WavAudioFile(const std::wstring & name = L"wavAudioFile")
			{
				m_name = name;
			}

			WavAudioFile& operator=(const WavAudioFile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name  = oSrc.name;
				embed = oSrc.embed;
				m_name = oSrc.m_name;
				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_snd; //todooo расширить ...
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name	= XmlUtils::GetNameNoNS(oReader.GetName());
				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:embed"), embed )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("name"), name )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= XmlUtils::GetNameNoNS(node.GetName());

				embed	= node.GetAttribute(_T("r:embed"));
				XmlMacroReadAttributeBase(node, L"name", name);
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:embed"), embed.ToString());
				oAttr.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("a:") + m_name, oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:") + m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("r:embed"), embed.ToString());
                pWriter->WriteAttribute2(_T("name"), name);
				pWriter->EndAttributes();
				
				pWriter->EndNode(_T("a:") + m_name);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_string			name;
			OOX::RId				embed;

			std::wstring			m_name; //node name
		protected:
			virtual void FillParentPointersForChilds(){};
			
			std::wstring GetPathFromId(OOX::IFileContainer* pRels, const std::wstring & rId) const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_WAVAUDIOFILE_INCLUDE_H_
