#pragma once
#ifndef DOCX_2_ODT_DEFAULT_STYLE_INCLUDE_H_
#define DOCX_2_ODT_DEFAULT_STYLE_INCLUDE_H_

#include "Base.h"


namespace Docx2Odt
{
	namespace Private
	{
		class DefaultStyle : public Base
		{
		public:
			DefaultStyle(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_DEFAULT_STYLE_INCLUDE_H_