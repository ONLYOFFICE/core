#pragma once
#ifndef PPTX_IFILE_CONTAINER_INCLUDE_H_
#define PPTX_IFILE_CONTAINER_INCLUDE_H_

#include "DocxFormat/IFileContainer.h"
#include "FileMap.h"
#include "PPTXEvent.h"

namespace PPTX
{
	class FileContainer : public OOX::IFileContainer
	{
	public:
		FileContainer()
		{
			m_lPercent = 0;
			m_bCancelled = false;

		}
	protected:
		void read(const boost::filesystem::wpath& filename);
		void read(const OOX::Rels::File& rels, const boost::filesystem::wpath& path);

		void read(const boost::filesystem::wpath& filename, FileMap& map, IPPTXEvent* Event);
		void read(const OOX::Rels::File& rels, const boost::filesystem::wpath& path, FileMap& map, IPPTXEvent* Event);
		void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content) const;
		void write(OOX::Rels::File& rels, const boost::filesystem::wpath& current, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content) const;

		void WrittenSetFalse();

		long m_lPercent;
		bool m_bCancelled;
	};
} // namespace PPTX

#endif // PPTX_IFILE_CONTAINER_INCLUDE_H_