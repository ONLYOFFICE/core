#pragma once
#ifndef PPTX_FOLDER_INCLUDE_H_
#define PPTX_FOLDER_INCLUDE_H_

#include "FileContainer.h"
#include "DocxFormat/RId.h"

namespace PPTX
{
	class Folder : public PPTX::FileContainer
	{
	public:
		Folder();
		Folder(const OOX::CPath& path, IPPTXEvent* Event);

	public:
		void read(const OOX::CPath& path, IPPTXEvent* Event);
		void write(const OOX::CPath& path);
		void createFromTemplate(const OOX::CPath& path);

	public:
		const bool isValid(const OOX::CPath& path) const;

	public:
		void extractPictures(const OOX::CPath& path);
		void extractPictures(const OOX::CPath& source, const OOX::CPath& path);
	private:
		long CountFiles(const OOX::CPath& path);
	};
} // namespace PPTX

#endif //PPTX_FOLDER_INCLUDE_H_