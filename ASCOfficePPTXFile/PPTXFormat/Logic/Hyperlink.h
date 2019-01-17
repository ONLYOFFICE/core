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
#ifndef PPTX_LOGIC_HYPERLINK_INCLUDE_H_
#define PPTX_LOGIC_HYPERLINK_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/RId.h"
#include "Media/WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{

		class Hyperlink : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Hyperlink)

			Hyperlink(const std::wstring & name = L"hlinkClick")
			{
				m_name = name;
			}

			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_hyperlink;
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = XmlUtils::GetNameNoNS(oReader.GetName());
				
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (sName == L"snd")
					{
						snd	= oReader;
						break;
					}

				}
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start	( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"), id )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("invalidUrl"), invalidUrl )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("action"), action )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("tgtFrame"), tgtFrame )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("tooltip"), tooltip )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("history"), history)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("highlightClick"), highlightClick )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("endSnd"), endSnd )
				WritingElement_ReadAttributes_End	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = XmlUtils::GetNameNoNS(node.GetName());

                std::wstring sSndNodeName = _T("snd");
                snd	= node.ReadNodeNoNS(sSndNodeName);

				XmlMacroReadAttributeBase(node, L"r:id", id);
				XmlMacroReadAttributeBase(node, L"invalidUrl", invalidUrl);
				XmlMacroReadAttributeBase(node, L"action", action);
				XmlMacroReadAttributeBase(node, L"tgtFrame", tgtFrame);
				XmlMacroReadAttributeBase(node, L"tooltip", tooltip);
				XmlMacroReadAttributeBase(node, L"history", history);
				XmlMacroReadAttributeBase(node, L"highlightClick", highlightClick);
				XmlMacroReadAttributeBase(node, L"endSnd", endSnd);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:" + m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute	(L"r:id",		id);
                pWriter->WriteAttribute2(L"invalidUrl", invalidUrl);
                pWriter->WriteAttribute2(L"action",		action);
                pWriter->WriteAttribute2(L"tgtFrame",	tgtFrame);
				
				if (tooltip.IsInit()) 
					pWriter->WriteAttribute(L"tooltip", XmlUtils::EncodeXmlString(*tooltip));
				
				pWriter->WriteAttribute(L"history",			history);
				pWriter->WriteAttribute(L"highlightClick",	highlightClick);
				pWriter->WriteAttribute(L"endSnd",			endSnd);
				pWriter->EndAttributes();

				pWriter->Write(snd);
				
				pWriter->EndNode(L"a:" + m_name);
			}
			
			nullable<WavAudioFile>	snd;

			nullable_string			id;				//<OOX::RId> id;//  <xsd:attribute ref="r:id" use="optional"/>
			nullable_string			invalidUrl;		//default=""
			nullable_string			action;			//default=""
			nullable_string			tgtFrame;		//default=""
			nullable_string			tooltip;		//default=""
			nullable_bool			history;		//default="true"
			nullable_bool			highlightClick;	//default="false"
			nullable_bool			endSnd;			//default="false"

			std::wstring			m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(snd.IsInit())
					snd->SetParentPointer(this);
			}

			virtual std::wstring GetPathFromId(OOX::IFileContainer* pRels, const std::wstring &rId)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_HYPERLINK_INCLUDE_H_
