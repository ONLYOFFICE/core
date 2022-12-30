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

#include "../WrapperWritingElement.h"
#include "../Theme.h"

#include "SpPr.h"
#include "ShapeStyle.h"

#include <boost/smart_ptr/shared_array.hpp>

namespace PPTX
{
	namespace Logic
	{
        void CalculateFill(BYTE lDocType, PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle, smart_ptr<PPTX::Theme>& oTheme,
				smart_ptr<PPTX::Logic::ClrMap>& oClrMap, std::wstring& strAttr, std::wstring& strNode, bool bOle = false, bool bSignature = false);

        void CalculateLine(BYTE lDocType, PPTX::Logic::SpPr& oSpPr, nullable<ShapeStyle>& pShapeStyle,
				smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap, std::wstring& strAttr, std::wstring& strNode, bool bOle = false);

		class SpTreeElem : public WrapperWritingElement
		{
		public:
			SpTreeElem();
			virtual ~SpTreeElem();

			virtual OOX::EElementType getType () const;

			explicit SpTreeElem(XmlUtils::CXmlNode& node);
			const SpTreeElem& operator =(XmlUtils::CXmlNode& node);

			explicit SpTreeElem(XmlUtils::CXmlLiteReader& oReader);
			const SpTreeElem& operator =(XmlUtils::CXmlLiteReader& oReader);

			SpTreeElem& operator=(const SpTreeElem& oSrc);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributesRequires(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
            virtual bool is_init() const {return (m_elem.IsInit());}

			template<class T> AVSINLINE const bool	is() const	{ return m_elem.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_elem.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_elem.as<T>(); }

			void InitElem(WrapperWritingElement* pElem);

			virtual void fromPPTY	(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY		(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			void toXmlWriterVML	(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::Theme>& oTheme, smart_ptr<PPTX::Logic::ClrMap>& oClrMap) const;
			virtual void toXmlWriter	(NSBinPptxRW::CXmlWriter* pWriter) const;

			std::wstring GetUriElem();			
			smart_ptr<WrapperWritingElement> GetElem();

			virtual void SetParentPointer(const WrapperWritingElement* pParent);
			std::wstring GetSlicerRequires();

			nullable_string m_binaryData;
			std::wstring m_sRequires;//from mc:Choice

		private:
			smart_ptr<WrapperWritingElement> m_elem;

		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX
