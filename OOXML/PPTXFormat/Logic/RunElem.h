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
#ifndef PPTX_LOGIC_RUNELEM_INCLUDE_H_
#define PPTX_LOGIC_RUNELEM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Runs/RunBase.h"
#include "Runs/Run.h"
#include "Runs/Fld.h"
#include "Runs/Br.h"
#include "Runs/MathParaWrapper.h"

namespace PPTX
{
	namespace Logic
	{
		class RunElem : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(RunElem)

			RunElem();

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			void InitRun(RunBase* pRun);

			virtual bool is_init() const;
			virtual std::wstring GetText() const;

			smart_ptr<RunBase> GetElem();

			template<class T> AVSINLINE const bool	is() const	{ return Elem.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Elem.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Elem.as<T>(); }

		private:
			smart_ptr<RunBase> Elem;

		protected:
			virtual void FillParentPointersForChilds();

		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent);
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNELEM_INCLUDE_H
