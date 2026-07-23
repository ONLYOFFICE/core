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

#include "../DocxFormat/WritingElement.h"
#include "WrapperFile.h"

#include "../Binary/Presentation/BinWriters.h"

#define PPTX_LOGIC_BASE_NC(Class)									\
	virtual ~Class() {}												\
	const Class& operator =(XmlUtils::CXmlNode& node)				\
	{																\
		fromXML(node);												\
		return *this;												\
	}																\
    const Class& operator =(const XmlUtils::CXmlNode& node)			\
    {																\
        fromXML(const_cast<XmlUtils::CXmlNode&> (node));			\
        return *this;												\
    }																\
	Class(const Class& oSrc) { *this = oSrc; }						\

#define PPTX_LOGIC_BASE(Class)										\
	Class()	{}														\
	PPTX_LOGIC_BASE_NC(Class)

#define PPTX_LOGIC_BASE2(Class)										\
	Class()	{}														\
	virtual ~Class() {}												\
	Class(const Class& oSrc) { *this = oSrc; }						\

namespace PPTX
{
	#define PTR_WITH_XML(Class, XmlContent) \
		Class* ptr = new Class();\
		*ptr = XmlContent;\
		return ptr;\

	template<typename T> inline T* CreatePtrXmlContent(XmlUtils::CXmlNode& oNode)
	{
		PTR_WITH_XML(T, oNode)
	}
	template<typename T> inline T* CreatePtrXmlContent(XmlUtils::CXmlLiteReader& oReader)
	{
		PTR_WITH_XML(T, oReader)
	}	

	class WrapperWritingElement : public OOX::WritingElement
	{
	public:
		WrapperWritingElement(OOX::Document *pMain = NULL);
		virtual ~WrapperWritingElement();

	protected:
		WrapperWritingElement const* parentElement;
		WrapperFile const* parentFile;

		virtual void FillParentPointersForChilds();

	public:
		virtual void SetParentPointer(const WrapperWritingElement* pParent);
		virtual void SetParentFilePointer(const WrapperFile* pFile);

		virtual WrapperWritingElement const* const	GetParentPointer() const;
		virtual WrapperFile const* const			GetParentFilePointer() const;

		virtual void fromXMLString(std::wstring strXml);
		virtual OOX::EElementType getType() const;

		template<class T> const bool parentIs() const
		{
			if (NULL == parentElement)
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperWritingElement*>(parentElement));
			return (NULL != pResult);
		}
		template<class T> const T& parentAs() const
		{
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperWritingElement*>(parentElement));
			return *pResult;
		}
		template<class T> const bool parentFileIs() const
		{
			if (NULL == parentFile)
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperFile*>(parentFile));
			return (NULL != pResult);
		}
		template<class T> T& parentFileAs() const
		{
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperFile*>(parentFile));
			return *pResult;
		}

		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
		virtual std::wstring toXML() const;
	};
} // namespace PPTX
