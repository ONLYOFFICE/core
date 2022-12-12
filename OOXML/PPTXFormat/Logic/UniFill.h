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
#ifndef PPTX_LOGIC_UNIFILL_INCLUDE_H_
#define PPTX_LOGIC_UNIFILL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Fills/BlipFill.h"
#include "Fills/NoFill.h"
#include "Fills/SolidFill.h"
#include "Fills/GradFill.h"
#include "Fills/PattFill.h"

namespace PPTX
{
	namespace Logic
	{
		class UniFill : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(UniFill)

			UniFill();
			virtual OOX::EElementType getType() const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			void GetFillFrom(XmlUtils::CXmlNode& element);
			virtual std::wstring toXML() const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual bool is_init() const;
			
			template<class T> AVSINLINE const bool	is() const	{ return Fill.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Fill.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Fill.as<T>(); }

			const UniFill& operator+=(const UniFill& fill);
			void Merge(UniFill& fill) const;

			smart_ptr<WrapperWritingElement>	Fill;
			enum Type {notInit, noFill, solidFill, gradFill, blipFill, pattFill, grpFill};
			Type								m_type;
			std::wstring						m_namespace;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIFILL_INCLUDE_H
