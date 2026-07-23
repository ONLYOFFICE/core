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
#ifndef _OFFICE_DRAWING_FILE_H
#define _OFFICE_DRAWING_FILE_H

#include "./Fonts.h"
class IRenderer;

enum OfficeDrawingFileType
{
    odftPDF = 0,
    odftXPS = 1,
    odftDJVU = 2,
    odftOFD = 3,
    odftUndefined = 255
};

struct COfficeDrawingPageParams
{
	bool m_bNeedDrawAnnotation;

	COfficeDrawingPageParams() : m_bNeedDrawAnnotation(true){}

	void SetDrawAnnotation(bool bDraw)
	{
		m_bNeedDrawAnnotation = bDraw;
	}
};

class IOfficeDrawingFilePainter
{
public:
  enum class State
  {
	Paused,
	Stopped,
	Runned
  };

  enum class Status
  {
	Ok,
	Cancel
  };

  virtual State GetState() = 0;
  virtual void OnPaint(const Status&) = 0;
};

class GRAPHICS_DECL IOfficeDrawingFile
{
public:
    virtual ~IOfficeDrawingFile() {}

    // Open
    virtual bool LoadFromFile(const std::wstring& file, const std::wstring& options = L"",
								const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL) = 0;
    virtual bool LoadFromMemory(unsigned char* data, unsigned long length, const std::wstring& options = L"",
								const wchar_t* owner_password = NULL, const wchar_t* user_password = NULL) = 0;

    // Close
    virtual void Close() = 0;

    // Get IApplicationFonts for wrappers
    virtual NSFonts::IApplicationFonts* GetFonts() = 0;

    // Type
    virtual OfficeDrawingFileType GetType() = 0;

    // Temp directory
    virtual std::wstring GetTempDirectory() = 0;
    virtual void SetTempDirectory(const std::wstring& directory) = 0;

    // Pages info/draw
    virtual int GetPagesCount() = 0;
    virtual void GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) = 0;
    virtual void DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams = NULL) = 0;

    // Common methods/wrappers on GetPageInfo + DrawPageOnRenderer
    virtual unsigned char* ConvertToPixels(int nPageIndex, int nRasterW, int nRasterH,
                                           bool bIsFlip = false,
                                           NSFonts::IFontManager* pFonts = NULL,
                                           int nBackgroundColor = 0xFFFFFF,
                                           bool bIsDarkMode = false,
                                           int nBackgroundOpacity = 0xFF);
    virtual void ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW = -1, const int nRasterH = -1,
                                 bool bIsFlip = false,
                                 NSFonts::IFontManager* pFonts = NULL,
                                 int nBackgroundColor = 0xFFFFFF,
                                 bool bIsDarkMode = false,
                                 int nBackgroundOpacity = 0xFF);

    // Common methods for viewer
    virtual std::wstring GetInfo() = 0;
    virtual unsigned char* GetStructure() = 0;
    virtual unsigned char* GetLinks(int nPageIndex) = 0;

	// DrawingFile control
	virtual void SetPainter(IOfficeDrawingFilePainter* pPainter);
};

#endif // _OFFICE_DRAWING_FILE_H
