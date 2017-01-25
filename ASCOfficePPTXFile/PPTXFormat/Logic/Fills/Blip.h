/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
			PPTX_LOGIC_BASE(Blip)

			Blip& operator=(const Blip& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Effects = oSrc.Effects;

				cstate	= oSrc.cstate;
				embed	= oSrc.embed;
				link	= oSrc.link;

				m_namespace	= oSrc.m_namespace;
				oleRid = oSrc.oleRid;
				oleFilepathBin = oSrc.oleFilepathBin;
				oleFilepathImg = oSrc.oleFilepathImg;
				oleRidImg = oSrc.oleRidImg;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring strName = (_T("") == m_namespace) ? _T("blip") : (m_namespace + _T(":blip"));
				pWriter->StartNode(strName);

				pWriter->StartAttributes();
				if (embed.IsInit())
					pWriter->WriteAttribute(_T("r:embed"), embed->ToString());
				if (link.IsInit())
					pWriter->WriteAttribute(_T("r:link"), link->ToString());
				pWriter->WriteAttribute(_T("cstate"), cstate);
				pWriter->EndAttributes();

				// TODO:
				size_t nCount = Effects.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					Effects[i].toXmlWriter(pWriter);
				}

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, cstate);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				/*
				old version
				if (embed.is_init())
					embed->toPPTY(0, pWriter);
				if (link.is_init())
					link->toPPTY(1, pWriter);
				*/

				// new version
				if (embed.is_init())
					pWriter->WriteString1(10, embed->get());
				if (link.is_init())
					pWriter->WriteString1(11, link->get());

				pWriter->StartRecord(2);
				ULONG len = (ULONG)Effects.size();
				pWriter->WriteULONG(len);

				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(3, Effects[i]);
				}

				pWriter->EndRecord();

				double dX = pWriter->GetShapeX(); //mm
				double dY = pWriter->GetShapeY();

				double dW = pWriter->GetShapeWidth(); //mm
				double dH = pWriter->GetShapeHeight();

				FileContainer* pRels = NULL;
				
				if (pWriter->m_pCommonRels->is_init())
					pRels = pWriter->m_pCommonRels->operator ->();

				std::wstring olePath;
				if(!oleFilepathBin.empty())
				{
					olePath = oleFilepathBin;
				}
				else if(!oleRid.empty())
				{
					olePath= this->GetFullOleName(OOX::RId(oleRid), pRels);
				}

				NSShapeImageGen::CImageInfo oId = pWriter->m_pCommon->m_pImageManager->WriteImage(this->GetFullPicName(pRels), olePath, dX, dY, dW, dH);
				std::wstring s = oId.GetPath2();

				pWriter->StartRecord(3);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, s);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);				

				pWriter->EndRecord();
			}
		public:
			virtual std::wstring GetFullPicName(FileContainer* pRels = NULL)const;
			virtual std::wstring GetFullOleName(const OOX::RId& pRId, FileContainer* pRels = NULL)const;

		public:
			std::vector<UniEffect> Effects;

			nullable_limit<Limit::BlipCompression> cstate;
			nullable<OOX::RId> embed;
			nullable<OOX::RId> link;
		//private:
		public:
			std::wstring m_namespace;
		//internal
			std::wstring oleRid;
			std::wstring oleFilepathBin;
			std::wstring oleFilepathImg;
			std::wstring oleRidImg;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIP_INCLUDE_H_
