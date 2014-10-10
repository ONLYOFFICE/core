#pragma once
#ifndef PPTX_FILEMAP_INCLUDE_H_
#define PPTX_FILEMAP_INCLUDE_H_

#include "DocxFormat/File.h"

#include <map>

namespace PPTX
{
	class FileMap
	{
	public:
		FileMap()
		{
		}

		~FileMap()
		{
		}
	public:

        std::map<CString, smart_ptr<PPTX::File>> m_map;

	public:

        AVSINLINE std::map<CString, smart_ptr<PPTX::File>>::const_iterator find(const OOX::CPath& path)
		{
            return m_map.find(path.m_strFilename);
		}

		AVSINLINE void add(const OOX::CPath& key, const smart_ptr<PPTX::File>& value)
		{
            m_map[key.m_strFilename] = value;
		}

        AVSINLINE bool empty() const {return m_map.empty();}
        AVSINLINE size_t size() const {return m_map.size();}
	};
} // namespace PPTX

#endif // PPTX_FILEMAP_INCLUDE_H_
