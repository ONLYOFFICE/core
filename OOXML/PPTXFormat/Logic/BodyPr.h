/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
			WritingElement_AdditionMethods(BodyPr)
			
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
			WritingElement_AdditionMethods(LinkedTxbx)

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

