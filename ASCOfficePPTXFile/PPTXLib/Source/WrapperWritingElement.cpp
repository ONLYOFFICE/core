#include "WrapperWritingElement.h"

namespace PPTX
{
	WrapperWritingElement::WrapperWritingElement()
		:parentElement(NULL),parentFile(NULL)
	{
	}

	WrapperWritingElement::~WrapperWritingElement()
	{
	}

	void WrapperWritingElement::SetParentPointer(const WrapperWritingElement& pParent)
	{
		parentElement = &pParent;

		//if(pParent != NULL)
			parentFile = parentElement->parentFile;
		//else
		//	parentFile = NULL;

		FillParentPointersForChilds();
	}

	void WrapperWritingElement::SetParentFilePointer(const WrapperFile& pFile)
	{
		parentFile = &pFile;
		FillParentPointersForChilds();
	};
} // namespace PPTX
