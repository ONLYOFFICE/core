#pragma once
#ifndef PPTX_FILEMAP_INCLUDE_H_
#define PPTX_FILEMAP_INCLUDE_H_

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include "DocxFormat\File.h"
#include <vector>
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
	private:
		typedef std::pair<boost::filesystem::wpath, boost::shared_ptr<OOX::File> > FileMapPair;
		std::vector<FileMapPair> m_map;
	public:
		typedef std::vector<FileMapPair>::iterator iterator;
		typedef std::vector<FileMapPair>::const_iterator const_iterator;
	public:
		iterator begin()	{return m_map.begin();}
		iterator end()		{return m_map.end();}
		const_iterator begin()	const {return m_map.begin();}
		const_iterator end()		const {return m_map.end();}

		iterator find(const boost::filesystem::wpath& key)
		{
			for(iterator i = m_map.begin(); i != m_map.end(); i++)
			{
				if(key == i->first)
					return i;
			}
			return m_map.end();
		}

		const_iterator find(const boost::filesystem::wpath& key) const
		{
			for(const_iterator i = m_map.begin(); i != m_map.end(); i++)
			{
				if(key == i->first)
					return i;
			}
			return m_map.end();
		}

		const size_t size() const {return m_map.size();}

		void add(const boost::filesystem::wpath& key, const boost::shared_ptr<OOX::File>& value)
		{
			if(find(key) == end())
				m_map.push_back(std::make_pair(key, value));
		}

		const bool empty() const {return m_map.empty();}
	};
} // namespace PPTX

#endif // PPTX_FILEMAP_INCLUDE_H_