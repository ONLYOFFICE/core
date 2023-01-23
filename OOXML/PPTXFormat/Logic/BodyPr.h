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

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextAnchor.h"
#include "./../Limit/HorzOverflow.h"
#include "./../Limit/TextVerticalType.h"
#include "./../Limit/VertOverflow.h"
#include "./../Limit/TextWrap.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "PrstTxWarp.h"
#include "TextFit.h"
#include "ExtP.h"

namespace PPTX
{
	namespace Logic
	{
		class BodyPr : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(BodyPr)
			
			BodyPr(std::wstring ns = L"a");

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void Merge(nullable<BodyPr>& bodyPr) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable<PrstTxWarp>		prstTxWarp;
			TextFit						Fit;
			nullable<Scene3d>			scene3d;
			nullable_int				flatTx;
			nullable<Sp3d>				sp3d;

			nullable_limit<Limit::TextAnchor>		anchor;
			nullable_bool							anchorCtr;
			nullable_int							bIns;
			nullable_bool							compatLnSpc;
			nullable_bool							forceAA;
			nullable_bool							fromWordArt;
			nullable_limit<Limit::HorzOverflow>		horzOverflow;
			nullable_int							lIns;
			nullable_int							numCol;
			nullable_int							rIns;
			nullable_int							rot;
			nullable_bool							rtlCol;
			nullable_int							spcCol;
			nullable_bool							spcFirstLastPara;
			nullable_int							tIns;
			nullable_bool							upright;
			nullable_limit<Limit::TextVerticalType> vert;
			nullable_limit<Limit::VertOverflow>		vertOverflow;
			nullable_limit<Limit::TextWrap>			wrap;

			mutable std::wstring					m_namespace;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();
		};

		class LinkedTxbx : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(LinkedTxbx)

			LinkedTxbx(std::wstring ns = L"a");

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			nullable_uint id;
			nullable_uint seq;

			std::vector<Ext> extLst;

			mutable std::wstring m_namespace;

		protected:
			virtual void FillParentPointersForChilds();
			AVSINLINE void Normalize();
		};
	} // namespace Logic
} // namespace PPTX

