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
#pragma once
#include "RtfProperty.h"
#include "Utils.h"

class RtfPicture : public IDocumentElement
{
public: 
	typedef enum {dt_none, dt_png, dt_jpg, dt_emf, dt_wmf, dt_apm, dt_macpict, dt_svg}DataType;

	DataType eDataType;

	int m_nWidth;
	int m_nWidthGoal;
	int m_nHeight;
	int m_nHeightGoal;

	double m_dScaleX;
	double m_dScaleY;

	int m_bScaled;

	int m_nCropL;
	int m_nCropT;
	int m_nCropR;
	int m_nCropB;

	RtfBorder m_oBorderTop;
	RtfBorder m_oBorderLeft;
	RtfBorder m_oBorderBottom;
	RtfBorder m_oBorderRight;

	bool			m_bIsCopy;		// true - need to delete m_sPicFilename, false - do not delete
    std::wstring	m_sPicFilename; //always contains the image name whose type is supported by rtf

    std::vector<std::wstring> m_aTempFiles;
	
	std::wstring dump_shape_properties;

	RtfPicture();
	~RtfPicture();

	int GetType();
	bool IsValid();

	void SetDefaultRtf();
	void SetDefaultOOX();
	void SetDefault();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    std::wstring GenerateWMF(RenderParameter oRenderParameter);
	
    //static bool LoadPicture( IUnknown** piImage, std::wstring sPath );
    //static bool SavePicture( IUnknown* piImage, std::wstring sPath, long nFormat );
	
	static DataType GetPictureType( std::wstring sFilename );
};
typedef boost::shared_ptr<RtfPicture> RtfPicturePtr;
