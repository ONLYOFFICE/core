/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_IMAGEFILESCACHE_H_
#define _BUILD_IMAGEFILESCACHE_H_

#include "Image.h"
#include "TemporaryCS.h"
#include <map>
#include "../common/File.h"

#ifndef GRAPHICS_DISABLE_METAFILE
#include "../raster/Metafile/MetaFile.h"
#endif

#if defined (GetTempPath)
#undef GetTempPath
#endif

#include "../graphics/pro/Image.h"

class CCacheImage : public NSImages::ICacheImage
{
private:
	Aggplus::CImage m_oImage;

public:
	CCacheImage(NSFonts::IApplicationFonts* pFonts) : NSImages::ICacheImage(), m_oImage()
	{
	}

	CCacheImage(NSFonts::IApplicationFonts* pFonts, const std::wstring& strFile) : NSImages::ICacheImage()
	{
		if (NULL == pFonts)
		{
			m_oImage.Create(strFile);
		}
		else
		{
#ifdef GRAPHICS_DISABLE_METAFILE
			m_oImage.Create(strFile);
#else
			MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
			bool bIsMetafile = pMetafile->LoadFromFile(strFile.c_str());
			if (!bIsMetafile)
			{
				m_oImage.Create(strFile);
			}
			else
			{
				std::wstring sTempFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"AscMetafile_");
				pMetafile->ConvertToRaster(sTempFile.c_str(), 4, 1000, -1);
				m_oImage.Create(sTempFile);

				NSFile::CFileBinary::Remove(sTempFile);
			}
			RELEASEINTERFACE(pMetafile);
#endif
		}
	}

	virtual ~CCacheImage()
	{
	}

	virtual Aggplus::CImage* GetImage()
	{
		return &m_oImage;
	}
};

class CImageFilesCache : public NSImages::IImageFilesCache
{
private:
	std::map<std::wstring, CCacheImage*> m_mapImages;
	LONG m_lMaxCount;

	NSFonts::IApplicationFonts* m_pApplicationFonts;

	NSCriticalSection::CRITICAL_SECTION m_oCS;

public:
	CImageFilesCache(NSFonts::IApplicationFonts* pFonts = NULL) : NSImages::IImageFilesCache()
	{
		m_pApplicationFonts = pFonts;
		m_lMaxCount = 10;

		m_oCS.InitializeCriticalSection();

		ADDREFINTERFACE(m_pApplicationFonts);
	}

	virtual ~CImageFilesCache()
	{
		Clear();
		m_oCS.DeleteCriticalSection();

		RELEASEINTERFACE(m_pApplicationFonts);
	}

	virtual void Clear()
	{
		CTemporaryCS oCS(&m_oCS);

		for (std::map<std::wstring,CCacheImage*>::iterator it = m_mapImages.begin(); it != m_mapImages.end(); ++it)
		{
			it->second->Release();
		}

		m_mapImages.clear();
	}

	virtual NSImages::ICacheImage* Lock(const std::wstring& strFile)
	{
		CTemporaryCS oCS(&m_oCS);

		std::map<std::wstring,CCacheImage*>::iterator it = m_mapImages.find(strFile);
		if (it != m_mapImages.end())
		{
			CCacheImage* pImage = it->second;
			pImage->AddRef();
			return pImage;
		}

		int nCount = (int)m_mapImages.size();
		if (nCount >= m_lMaxCount)
		{
			int nNeedDelete = nCount - m_lMaxCount;

			while (nNeedDelete > 0)
			{
				std::map<std::wstring,CCacheImage*>::iterator it2 = m_mapImages.begin();
				if (it2 != m_mapImages.end())
				{
					it2->second->Release();
					m_mapImages.erase(it2);
				}
				--nNeedDelete;
			}
		}

		CCacheImage* pImage = new CCacheImage(m_pApplicationFonts, strFile);

		if (pImage->GetImage()->GetLastStatus() != Aggplus::Ok)
			return pImage;

		m_mapImages[strFile] = pImage;
		pImage->AddRef();
		return pImage;
	}

	virtual bool UnLock(const std::wstring& strFile)
	{
		CTemporaryCS oCS(&m_oCS);

		std::map<std::wstring,CCacheImage*>::iterator it = m_mapImages.find(strFile);
		if (it != m_mapImages.end())
		{
			it->second->Release();
			m_mapImages.erase(it);
			return true;
		}

		return false;
	}
	
	virtual int Release()
	{
		m_oCS.Enter();
		--m_lRef;

		if (0 == m_lRef)
		{
			m_oCS.Leave();
			delete this;
			return 0;
		}

		m_oCS.Leave();
		return m_lRef;
	}

	virtual void SetApplicationFonts(NSFonts::IApplicationFonts* pApplicationFonts)
	{
		if (m_pApplicationFonts)
			m_pApplicationFonts->Release();
		m_pApplicationFonts = pApplicationFonts;
		if (m_pApplicationFonts)
			m_pApplicationFonts->AddRef();
	}
};

#endif // _BUILD_IMAGEFILESCACHE_H_
