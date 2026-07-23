/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _METAFILE_COMMON_METAFILE_H
#define _METAFILE_COMMON_METAFILE_H

#include "CClip.h"
#include "MetaFileTypes.h"
#include "MetaFileUtils.h"
#include "MetaFileObjects.h"

#include "../../../graphics/pro/Fonts.h"

namespace MetaFile
{
	class IMetaFileBase
	{
	public:
		IMetaFileBase()
		    : m_pOutput(NULL), m_pParent(NULL), m_pBufferData(NULL), m_bIsExternalBuffer(false), m_bError(false)
		{
			m_oStream.SetStream(NULL, 0);
		}
		virtual ~IMetaFileBase()
		{
			this->Close();
		}
		
		virtual void           PlayMetaFile() = 0;
		virtual void           ClearFile() {/*Cannot be pure virtual because it is called in destructor*/}
		virtual const TRectL&  GetDCBounds() const = 0;
		virtual const CClip*   GetClip() const = 0;
		virtual double         GetPixelHeight() const = 0;
		virtual double         GetPixelWidth() const = 0;
		virtual int            GetTextColor() const = 0;
		virtual const IFont*   GetFont() const = 0;
		virtual const IBrush*  GetBrush() const = 0;
		virtual const IPen*    GetPen() const = 0;
		virtual unsigned int   GetTextAlign() const = 0;
		virtual unsigned int   GetTextBgMode() const = 0;
		virtual int            GetTextBgColor() const = 0;
		virtual unsigned int   GetFillMode() const = 0;
		virtual TPointD        GetCurPos() const = 0;
		virtual const TXForm&  GetInverseTransform() const = 0;
		virtual const TXForm&  GetTransform(int iGraphicsMode = GM_ADVANCED) = 0;
		virtual unsigned int   GetMiterLimit() const = 0;
		virtual unsigned int   GetRop2Mode() const = 0;
		virtual int            GetCharSpace() const = 0;
		virtual bool           IsWindowFlippedY() const = 0;
		virtual bool           IsWindowFlippedX() const = 0;
		virtual unsigned int   GetMapMode() const = 0;
		virtual USHORT         GetDpi() const = 0;
		virtual const IRegion* GetRegion() const = 0;
		virtual unsigned int   GetArcDirection() const = 0;
		virtual const CPath*   GetPath() const = 0;

		virtual TXForm         CalculateCurrentTransform() const
		{
			return TXForm{};
		}

		virtual const TRectL& GetOriginalDCBounds() const
		{
			if (NULL != m_pParent)
				return m_pParent->GetOriginalDCBounds();

			return GetDCBounds();
		}

		bool ReadFromBuffer(BYTE* pBuffer, unsigned int unSize, const bool& bIsExternal = true)
		{
			if (NULL == pBuffer || 0 == unSize)
				return false;

			this->ClearFile();

			m_pBufferData = pBuffer;
			m_bIsExternalBuffer = bIsExternal;
			m_oStream.SetStream(pBuffer, unSize);

			return true;
		}
		bool OpenFromFile(const wchar_t* wsFilePath)
		{
			this->Close();

			NSFile::CFileBinary oFile;
			if (!oFile.OpenFile(wsFilePath))
				return false;

			int lFileSize = oFile.GetFileSize();

			m_bIsExternalBuffer = false;
			m_pBufferData = new BYTE[lFileSize];
			if (!m_pBufferData)
				return false;

			DWORD lReadedSize;
			oFile.ReadFile(m_pBufferData, lFileSize, lReadedSize);

			m_oStream.SetStream(m_pBufferData, lFileSize);
			oFile.CloseFile();

			return true;
		}
		void Close()
		{
			if (!m_bIsExternalBuffer)
				RELEASEARRAYOBJECTS(m_pBufferData);

			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;

			this->ClearFile();
		}
		void Scan()
		{
			IOutputDevice* pOutput = m_pOutput;
			m_pOutput = NULL;
			PlayMetaFile();
			m_pOutput = pOutput;

			this->ClearFile();
		}
		NSFonts::IFontManager* GetFontManager() const
		{
			return m_pFontManager;
		}
		void  SetFontManager(NSFonts::IFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
		}
		void SetOutputDevice(IOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void SetError()
		{
			m_bError = true;
		}
		bool CheckError() const
		{
			return m_bError;
		}

		void SetParent(const IMetaFileBase* pParent)
		{
			m_pParent = pParent;
		}
	protected:

		CDataStream    m_oStream;
		IOutputDevice* m_pOutput;

		const IMetaFileBase* m_pParent;
	private:
		NSFonts::IFontManager*  m_pFontManager;

		BYTE*          m_pBufferData;
		bool           m_bIsExternalBuffer;
		bool           m_bError;
	};
}

#endif //_METAFILE_COMMON_METAFILE_H
