#pragma once
#ifndef PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_
#define PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_

#include "DocxFormat/WritingElement.h"
#include "WrapperFile.h"

namespace PPTX
{
	class WrapperWritingElement : public OOX::WritingElement
	{
	public:
		WrapperWritingElement();
		virtual ~WrapperWritingElement();
	protected:
		WrapperWritingElement const* parentElement;
		WrapperFile const* parentFile;
	protected:
		virtual void FillParentPointersForChilds()=0;
	public:
		virtual void SetParentPointer(const WrapperWritingElement& pParent);
		virtual void SetParentFilePointer(const WrapperFile& pFile);
		virtual WrapperWritingElement const* const	GetParentPointer()const		{return parentElement;};
		virtual WrapperFile const* const			GetParentFilePointer()const {return parentFile;};


		template<class T> const bool parentIs()const
		{
			return (parentElement == NULL)?false:typeid(*parentElement) == typeid(T);
		};
		//template<class T> T& parentAs()
		//{
		//	return static_cast<T&>(*parentElement);
		//};
		template<class T> const T& parentAs()const
		{
			return static_cast<const T&>(*parentElement);
		};


		template<class T> const bool parentFileIs()const
		{
			return (parentFile == NULL)?false:typeid(*parentFile) == typeid(T);
		};
		//template<class T> T& parentFileAs()
		//{
		//	return static_cast<T&>(*parentFile);
		//};
		template<class T> const T& parentFileAs()const
		{
			return static_cast<const T&>(*parentFile);
		};
	};
} // namespace PPTX

#endif // PPTX_WRAPPER_WRITING_ELEMENT_INCLUDE_H_