#pragma once
#ifndef DOCX_2_ODT_PRIVATE_FRAME_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_FRAME_INCLUDE_H_

#include "Base.h"


namespace Docx2Odt
{
	namespace Private
	{
		class Frame : public Base
		{
		public:
			Frame(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::Frame drawing2frame(const OOX::Logic::Drawing& ooxDrawing) const;

		private:
			const Odt::Content::Image drawing2image(const OOX::Logic::Drawing& ooxDrawing) const;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_FRAME_INCLUDE_H_