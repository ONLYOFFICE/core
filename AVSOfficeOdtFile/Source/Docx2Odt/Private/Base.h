#pragma once
#ifndef DOCX_2_ODT_PRIVATE_BASE_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_BASE_INCLUDE_H_

#include "Docx.h"
#include "IFileContainer.h"
#include "Odt.h"


namespace Docx2Odt
{
	namespace Private
	{
		class Base
		{
		public:
			Base(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		protected:
			Odt::Folder&				m_odtFolder;
			const OOX::IFileContainer&	m_docxFileContainer;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_BASE_INCLUDE_H_