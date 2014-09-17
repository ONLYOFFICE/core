#pragma once
#ifndef OOX_IMAGE_INCLUDE_H_
#define OOX_IMAGE_INCLUDE_H_

#include "Media.h"

namespace PPTX
{
	class Image : public Media
	{
	public:
		Image()
		{
		}
		Image(const OOX::CPath& filename)
		{
			read(filename);
		}
		virtual ~Image()
		{
		}

	public:
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
			CString newFilename = filename.GetFilename();
			OOX::CPath newFilePath = filename.GetDirectory();

			newFilename.Replace((TCHAR)' ', (TCHAR)'_');
			if (OOX::CSystemUtility::IsFileExist(m_filename) && !OOX::CSystemUtility::IsFileExist(newFilePath/newFilename))	
			{
				//if (m_filename.GetExtention(true) == _T(".svm"))
				//{
				//	ConvertSvmToImage(m_filename, replace_extension(newFilePath/newFilename, L"png"));
				//}
				//else if (m_filename.extension() != L"")
				//{
				//	boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
				//}
				//else
				//{
				//	newFilename += L".wmf";
				//	boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
				//}
			}
			//if (m_filename.extension() == L".svm")
			//	content.Default->add(replace_extension(newFilePath/newFilename, L"png"));
			//else
			//	content.Default->add(newFilePath/newFilename);
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::Image;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return m_filename.GetFilename();
		}
	};
} // namespace PPTX

#endif // OOX_IMAGE_INCLUDE_H_