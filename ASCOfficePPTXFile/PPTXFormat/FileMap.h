#pragma once
#ifndef PPTX_FILEMAP_INCLUDE_H_
#define PPTX_FILEMAP_INCLUDE_H_

#include "DocxFormat/File.h"
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
		CAtlMap<CString, smart_ptr<PPTX::File>> m_map;

	public:

		AVSINLINE CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* find(const OOX::CPath& path)
		{
			return m_map.Lookup(path.m_strFilename);
		}

		AVSINLINE void add(const OOX::CPath& key, const smart_ptr<PPTX::File>& value)
		{
			m_map.SetAt(key.m_strFilename, value);
		}

		AVSINLINE bool empty() const {return m_map.IsEmpty();}
		AVSINLINE size_t size() const {return m_map.GetCount();}
	};
} // namespace PPTX

#endif // PPTX_FILEMAP_INCLUDE_H_