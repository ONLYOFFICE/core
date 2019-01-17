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
#ifndef PPTX_LOGIC_BLIP_INCLUDE_H_
#define PPTX_LOGIC_BLIP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/BlipCompression.h"
#include "./../UniEffect.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class Blip : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Blip)

			Blip(std::wstring ns = L"a")
			{
				m_namespace = ns;
				mediaExternal = false;
			}	
			Blip& operator=(const Blip& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Effects = oSrc.Effects;

				cstate	= oSrc.cstate;
				embed	= oSrc.embed;
				link	= oSrc.link;

				m_namespace		= oSrc.m_namespace;
				
				oleRid			= oSrc.oleRid;
				oleFilepathBin	= oSrc.oleFilepathBin;

				mediaRid		= oSrc.mediaRid;
				mediaFilepath	= oSrc.mediaFilepath;
				mediaExternal	= oSrc.mediaExternal;

				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_blip;
			}			
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:embed"), embed)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("r:link"), link )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("cstate"), cstate )
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			
			virtual std::wstring GetFullPicName(OOX::IFileContainer* pRels = NULL)const;
			virtual std::wstring GetFullOleName(const OOX::RId& pRId, OOX::IFileContainer* pRels = NULL)const;

			std::vector<UniEffect> Effects;

			nullable_limit<Limit::BlipCompression> cstate;
			nullable<OOX::RId>	embed;
			nullable<OOX::RId>	link;
			std::wstring		m_namespace;
	//internal
			std::wstring		mediaRid;
			std::wstring		mediaFilepath;
			bool				mediaExternal;

			std::wstring		oleRid;
			std::wstring		oleFilepathBin;
			std::wstring		oleFilepathImage;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIP_INCLUDE_H_
