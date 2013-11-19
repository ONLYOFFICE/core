#pragma once
#ifndef PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_
#define PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_

#include "DocxFormat/WritingElement.h"
#include "WrapperFile.h"

#include "../Editor/BinWriters.h"

#define PPTX_LOGIC_BASE(Class)										\
	Class()	{}														\
	virtual ~Class() {}												\
	explicit Class(XmlUtils::CXmlNode& node)	{ fromXML(node); }	\
	const Class& operator =(XmlUtils::CXmlNode& node)				\
	{																\
		fromXML(node);												\
		return *this;												\
	}																\
	Class(const Class& oSrc) { *this = oSrc; }						\

namespace PPTX
{
	class WrapperWritingElement : public OOX::WritingElement
	{
	public:
		WrapperWritingElement() : parentElement(NULL), parentFile(NULL)
		{
		}
		virtual ~WrapperWritingElement()
		{
		}
	protected:
		WrapperWritingElement const* parentElement;
		WrapperFile const* parentFile;
	protected:
		virtual void FillParentPointersForChilds()=0;
	public:
		virtual void SetParentPointer(const WrapperWritingElement* pParent)
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

		virtual void fromXMLString(CString strXml)
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
		template<class T> const T& parentFileAs()const
		{
			T* pResult = dynamic_cast<T*>(const_cast<PPTX::WrapperFile*>(parentFile));
			return *pResult;
		}

		// PPTY format serialise
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->SkipRecord();
		}
        virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
		}
	};
} // namespace PPTX

#endif // PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_