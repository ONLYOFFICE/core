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
#ifndef _METAFILE_COMMON_METAFILE_H
#define _METAFILE_COMMON_METAFILE_H

#include "MetaFileTypes.h"
#include "MetaFileUtils.h"
#include "MetaFileObjects.h"
#include "MetaFileClip.h"

#include "../../../graphics/pro/Fonts.h"

namespace MetaFile
{
	class IMetaFileBase
	{
	public:
		IMetaFileBase()
		{
			m_pBufferData = NULL;
			m_bIsExternalBuffer = false;
			m_bError      = false;
			m_pOutput     = NULL;
			m_oStream.SetStream(NULL, 0);
		}
		virtual ~IMetaFileBase()
		{
			this->Close();
		}
		
		virtual void         PlayMetaFile() = 0;
		virtual void         ClearFile() {/*Нельзя делать чисто виртуальной, потому что вызывается в деструкторе*/}
		virtual TRect*       GetDCBounds() = 0;
		virtual double       GetPixelHeight() = 0;
		virtual double       GetPixelWidth() = 0;
		virtual int          GetTextColor() = 0;
		virtual IFont*       GetFont() = 0;
		virtual IBrush*      GetBrush() = 0;
		virtual IPen*        GetPen() = 0;
		virtual unsigned int GetTextAlign() = 0;
		virtual unsigned int GetTextBgMode() = 0;
		virtual int          GetTextBgColor() = 0;
		virtual unsigned int GetFillMode() = 0;
		virtual TPointD      GetCurPos() = 0;
		virtual TXForm*      GetInverseTransform() = 0;
		virtual TXForm*      GetTransform(int iGraphicsMode = GM_ADVANCED) = 0;
		virtual unsigned int GetMiterLimit() = 0;
		virtual unsigned int GetRop2Mode() = 0;
		virtual IClip*       GetClip() = 0;
		virtual int          GetCharSpace() = 0;
		virtual bool         IsWindowFlippedY() = 0;
		virtual bool         IsWindowFlippedX() = 0;
		virtual unsigned int GetMapMode() = 0;
		virtual double       GetDpi() = 0;
		virtual IRegion*     GetRegion() = 0;
		virtual unsigned int GetArcDirection() = 0;

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
			oFile.OpenFile(wsFilePath);
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
		void          Close()
		{
			if (!m_bIsExternalBuffer)
				RELEASEARRAYOBJECTS(m_pBufferData);

			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;

			this->ClearFile();
		}
		void          Scan()
		{
			IOutputDevice* pOutput = m_pOutput;
			m_pOutput = NULL;
			PlayMetaFile();
			m_pOutput = pOutput;

			this->ClearFile();
		}
		NSFonts::IFontManager* GetFontManager()
		{
			return m_pFontManager;
		}
		void          SetFontManager(NSFonts::IFontManager* pFontManager)
		{
			m_pFontManager = pFontManager;
		}
		void          SetOutputDevice(IOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void          SetError()
		{
			m_bError = true;
		}
		bool          CheckError()
		{
			return m_bError;
		}

	protected:

		CDataStream    m_oStream;
		IOutputDevice* m_pOutput;

	private:
		NSFonts::IFontManager*  m_pFontManager;

		BYTE*          m_pBufferData;
		bool           m_bIsExternalBuffer;
		bool           m_bError;
	};
}

#endif //_METAFILE_COMMON_METAFILE_H
