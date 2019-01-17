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
	typedef enum {dt_none, dt_png, dt_jpg, dt_emf, dt_wmf, dt_apm, dt_macpict}DataType;

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

	RtfPicture()
	{
		m_bIsCopy = false;
		SetDefault();
	}
	~RtfPicture()
	{
		SetDefault();
		for (size_t i = 0; i < m_aTempFiles.size(); i++ ) 
			Utils::RemoveDirOrFile( m_aTempFiles[i] );
	}
	int GetType()
	{
		return TYPE_RTF_PICTURE;
	}
	bool IsValid()
	{
        return !m_sPicFilename.empty() && dt_none != eDataType;
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
	{
		eDataType = dt_none;
		DEFAULT_PROPERTY( m_nWidth )
		DEFAULT_PROPERTY( m_nWidthGoal )
		DEFAULT_PROPERTY( m_nHeight )
		DEFAULT_PROPERTY( m_nHeightGoal )

		DEFAULT_PROPERTY_DEF( m_dScaleX, 100 )
		DEFAULT_PROPERTY_DEF( m_dScaleY, 100 )
		DEFAULT_PROPERTY( m_bScaled )

		DEFAULT_PROPERTY( m_nCropL )
		DEFAULT_PROPERTY( m_nCropT )
		DEFAULT_PROPERTY( m_nCropR )
		DEFAULT_PROPERTY( m_nCropB )
		
		m_sPicFilename = L"";
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    std::wstring GenerateWMF(RenderParameter oRenderParameter);
	
    //static bool LoadPicture( IUnknown** piImage, std::wstring sPath );
    //static bool SavePicture( IUnknown* piImage, std::wstring sPath, long nFormat );
	
    static DataType GetPictureType( std::wstring sFilename )
	{
		BYTE	pBuffer[ 16 ];
		DWORD	dwBytesRead = 0;

        CFile file;
		if (file.OpenFile(sFilename) != S_OK) return dt_none;

		file.ReadFile(pBuffer, 16);
		dwBytesRead = (DWORD)file.GetPosition();
		file.CloseFile();

		//jpeg	
		// Hex: FF D8 FF
		if ( (3 <= dwBytesRead) && (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
			return dt_jpg;

		//png 
		//Hex: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
		//ASCII: .PNG........IHDR
		if ( (16 <= dwBytesRead) && (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
			&& (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
			&& (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
			&& (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
			return dt_png;
		//wmf (aldus placeable header (apm))
		//Hex: D7 CD C6 9A 00 00
		if ( 6 <= dwBytesRead )
		{
			if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) )
				return dt_apm;
		}
		//wmf
		//or for Windows 3.x
		//Hex: 01 00 09 00 00 03
		if ( 6 <= dwBytesRead )
		{
			if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) || 
				((0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x09 == pBuffer[2]) && (0x00 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x03 == pBuffer[5]) ))
				return dt_wmf;
		}
		//emf
		//Hex: 01 00 00 00
		if ( (4 <= dwBytesRead) && (0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x00 == pBuffer[3]) )
			return dt_emf;

		return dt_none;
	}
};
typedef boost::shared_ptr<RtfPicture> RtfPicturePtr;
