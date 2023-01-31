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

	bool			m_bIsCopy;		// true - надо удалять m_sPicFilename, false - не надо удалять
    std::wstring	m_sPicFilename; //всегда содержит имя картинки, тип которой поддерживает rtf

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
