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
		Folder(const boost::filesystem::wpath& path, IPPTXEvent* Event);

	public:
		void read(const boost::filesystem::wpath& path, IPPTXEvent* Event);
		void write(const boost::filesystem::wpath& path);
		void createFromTemplate(const boost::filesystem::wpath& path);

	public:
		const bool isValid(const boost::filesystem::wpath& path) const;

	public:
		void extractPictures(const boost::filesystem::wpath& path);
		void extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path);
	private:
		long CountFiles(const boost::filesystem::wpath& path);
	};
} // namespace PPTX

#endif //PPTX_FOLDER_INCLUDE_H_