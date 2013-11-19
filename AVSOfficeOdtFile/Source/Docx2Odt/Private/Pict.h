#pragma once
#ifndef DOCX_2_ODT_PRIVATE_PICT_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_PICT_INCLUDE_H_

#include "Base.h"


namespace Docx2Odt
{
	namespace Private
	{
		class Pict : public Base
		{
		public:
			Pict(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::Frame pict2frame(const OOX::Logic::Pict& ooxPict) const;

		private:
			const Odt::Content::Image pict2image(const OOX::Logic::Pict& ooxPict) const;
			const Odt::Content::Image pict2oleView(const OOX::Logic::Pict& ooxPict) const;
			const Odt::Content::ObjectOle ole2ole(const OOX::Logic::Pict& ooxPict) const;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_PICT_INCLUDE_H_