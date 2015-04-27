#ifndef _METAFILE_COMMON_H
#define _METAFILE_COMMON_H

#include <map>
#include <vector>
#include "MetaFileObjects.h"
#include "MetaFile.h"

namespace MetaFile
{
	template <typename CMetaFileDC> class CMetaFilePlayer
	{
	public:

		CMetaFilePlayer(CMetaFileBase* pFile);
		~CMetaFilePlayer();
		void Clear();
		CMetaFileDC* SaveDC();
		CMetaFileDC* RestoreDC();
		CMetaFileDC* GetDC();
		void RegisterObject(unsigned int ulIndex, CMetaFileObjectBase* pObject);
		void SelectObject(unsigned int ulIndex);
		void DeleteObject(unsigned int ulIndex);
		void SelectPalette(unsigned int ulIndex);

	private:

		virtual void InitStockObjects();

	private:

		typedef std::map<unsigned int, CMetaFileObjectBase*> CMetaFileObjectMap;

		CMetaFileDC*              m_pDC;
		std::vector<CMetaFileDC*> m_vDCStack;
		CMetaFileBase*            m_pFile;
		CMetaFileObjectMap        m_mObjects;
	};
}

#endif //_METAFILE_COMMON_H