#pragma once
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include "../XML/XmlSimple.h"
#include "../SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "ContentTypes.h"

namespace OOX
{
	class File
	{
	public:
		File(){
			m_bDoNotAddRels = false;
		}
		virtual ~File(){}

	public:
		virtual void read(const CPath& filename)                                                        = 0;
		virtual void write(const CPath& filename, const CPath& directory, CContentTypes& content) const = 0;

	public:
		virtual const OOX::FileType type() const                                                        = 0;
		virtual const CPath DefaultDirectory() const                                                    = 0;
		virtual const CPath DefaultFileName() const                                                     = 0;
		CString m_sFilename;
		bool m_bDoNotAddRels;
	};
	class FileGlobalEnumerated : public File
	{
	private:
		int m_nGlobalNumber;
	public:
		FileGlobalEnumerated()
		{
			m_nGlobalNumber = 0;
		}
		int GetGlobalNumber() const
		{
			return m_nGlobalNumber;
		}
		void SetGlobalNumber(int nValue)
		{
			m_nGlobalNumber = nValue;
		}
	};
} // namespace OOX

#endif // OOX_FILE_INCLUDE_H_