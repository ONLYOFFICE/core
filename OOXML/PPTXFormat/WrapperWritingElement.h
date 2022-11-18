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

#include "../DocxFormat/WritingElement.h"
#include "WrapperFile.h"

#include "../Binary/Presentation/BinWriters.h"

#define PPTX_LOGIC_BASE(Class)										\
	Class()	{}														\
	virtual ~Class() {}												\
	explicit Class(XmlUtils::CXmlNode& node)	{ fromXML(node); }	\
    explicit Class(const XmlUtils::CXmlNode& node)	{ fromXML(const_cast<XmlUtils::CXmlNode&> (node)); }	\
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


#define PPTX_LOGIC_BASE2(Class)										\
	Class()	{}														\
	virtual ~Class() {}												\
	Class(const Class& oSrc) { *this = oSrc; }						\

namespace PPTX
{
	class WrapperWritingElement : public OOX::WritingElement
	{
	public:
		WrapperWritingElement(OOX::Document *pMain = NULL) :  OOX::WritingElement(pMain), parentElement(NULL), parentFile(NULL)
		{
		}
		virtual ~WrapperWritingElement()
		{
		}
	protected:
		WrapperWritingElement const* parentElement;
		WrapperFile const* parentFile;

		virtual void FillParentPointersForChilds(){}
	public:
        virtual void
        SetParentPointer(const WrapperWritingElement* pParent)
		{
			parentElement	= pParent;
			parentFile		= parentElement->parentFile;

			FillParentPointersForChilds();
		}
		virtual void SetParentFilePointer(const WrapperFile* pFile)
		{
			parentFile = pFile;
			FillParentPointersForChilds();
		}
		virtual WrapperWritingElement const* const	GetParentPointer()const		{return parentElement;}
		virtual WrapperFile const* const			GetParentFilePointer()const {return parentFile;}

		virtual void fromXMLString(std::wstring strXml)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlString(strXml);
			fromXML(oNode);
		}
		virtual OOX::EElementType getType() const
		{
			return OOX::et_Unknown;
		}
		template<class T> const bool parentIs()const
		{
			if (NULL == parentElement)
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperWritingElement*>(parentElement));
			return (NULL != pResult);
		}
		template<class T> const T& parentAs()const
		{
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperWritingElement*>(parentElement));
			return *pResult;
		}
		template<class T> const bool parentFileIs()const
		{
			if (NULL == parentFile)
				return false;
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperFile*>(parentFile));
			return (NULL != pResult);
		}
		template<class T> T& parentFileAs()const
		{
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperFile*>(parentFile));
			return *pResult;
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
		}
		virtual std::wstring toXML() const
		{
			NSBinPptxRW::CXmlWriter oWriter;
			toXmlWriter(&oWriter);

			return oWriter.GetXmlString();
		}
	};
} // namespace PPTX
