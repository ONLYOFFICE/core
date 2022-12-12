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
#include "RtfPicture.h"
#include "../OOXml/Writer/OOXWriter.h"
#include "RtfWriter.h"
#include "Utils.h"

RtfPicture::RtfPicture()
{
	m_bIsCopy = false;
	SetDefault();
}
RtfPicture::~RtfPicture()
{
	SetDefault();
	for (size_t i = 0; i < m_aTempFiles.size(); i++ )
		Utils::RemoveDirOrFile( m_aTempFiles[i] );
}

int RtfPicture::GetType()
{
	return TYPE_RTF_PICTURE;
}
bool RtfPicture::IsValid()
{
	return !m_sPicFilename.empty() && dt_none != eDataType;
}
void RtfPicture::SetDefaultRtf()
{
	SetDefault();
}
void RtfPicture::SetDefaultOOX()
{
	SetDefault();
}
void RtfPicture::SetDefault()
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

std::wstring RtfPicture::GenerateWMF(RenderParameter oRenderParameter) //копия растра в векторе
{
    std::wstring sResult;
	sResult += L"{\\pict";

	RENDER_RTF_INT( (int)m_dScaleX, sResult, L"picscalex" )
	RENDER_RTF_INT( (int)m_dScaleY, sResult, L"picscaley" )

	RENDER_RTF_INT( m_nCropL,		sResult, L"piccropl" )
	RENDER_RTF_INT( m_nCropT,		sResult, L"piccropt" )
	RENDER_RTF_INT( m_nCropR,		sResult, L"piccropr" )
	RENDER_RTF_INT( m_nCropB,		sResult, L"piccropb" )

	RENDER_RTF_INT( m_nWidth,		sResult, L"picw" )
	RENDER_RTF_INT( m_nHeight,		sResult, L"pich" )
	RENDER_RTF_INT( m_nWidthGoal,	sResult, L"picwgoal" )
	RENDER_RTF_INT( m_nHeightGoal,	sResult, L"pichgoal" )

	sResult += L"\\wmetafile8";

	sResult += L" ";

	//сохраняем в темповую директорию и загружаем файл как текст
	RtfWriter * poWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter );
    //IUnknown* piTempPict = NULL;
	
	//todooo ??? переписать
	//if( true == LoadPicture( &piTempPict, m_sPicFilename ) )
	//{
    //	std::wstring sTempFile = Utils::CreateTempFile( poWriter->m_sTempFolder );
	//	if( true == SavePicture( piTempPict, sTempFile, IMAGEFORMAT_WMF ) )
	//	{
	//		m_aTempFiles.push_back( sTempFile );
	//		sResult += RtfInternalEncoder::Encode( sTempFile );
	//	}
	//}
    //RELEASEINTERFACE( piTempPict );

	sResult += L"}";
	return sResult;
}
std::wstring RtfPicture::RenderToRtf(RenderParameter oRenderParameter)
{
	if( !IsValid() )return L"";

    std::wstring sResult = L"{\\pict";

	if (!dump_shape_properties.empty())
		sResult += L"{\\*\\picprop" + dump_shape_properties + L"}";

	RENDER_RTF_INT( (int)m_dScaleX, sResult, L"picscalex" )
	RENDER_RTF_INT( (int)m_dScaleY, sResult, L"picscaley" )
	RENDER_RTF_INT( m_nCropL,		sResult, L"piccropl" )
	RENDER_RTF_INT( m_nCropT,		sResult, L"piccropt" )
	RENDER_RTF_INT( m_nCropR,		sResult, L"piccropr" )
	RENDER_RTF_INT( m_nCropB,		sResult, L"piccropb" )

	RENDER_RTF_INT( m_nWidth,		sResult, L"picw" )
	RENDER_RTF_INT( m_nHeight,		sResult, L"pich" )
	RENDER_RTF_INT( m_nWidthGoal,	sResult, L"picwgoal" )
	RENDER_RTF_INT( m_nHeightGoal,	sResult, L"pichgoal" )

	switch( eDataType )
	{
		case dt_emf: sResult += L"\\emfblip";		break;
		case dt_wmf: sResult += L"\\wmetafile8";	break;
		case dt_png: sResult += L"\\pngblip";		break;
		case dt_jpg: sResult += L"\\jpegblip";		break;
		case dt_svg: sResult += L"\\svgblip";		break;
		//case dt_bmp: sResult += L"wbitmap8";		break;
	}
	sResult += L" ";

    sResult += RtfUtility::RtfInternalEncoder::Encode( m_sPicFilename );

	sResult += L"}";
	return sResult;
}
std::wstring RtfPicture::RenderToOOX(RenderParameter oRenderParameter)
{
	if( false == IsValid() )	return L"";
	
	OOXWriter		* poOOXWriter	= static_cast<OOXWriter*>		(oRenderParameter.poWriter);
	OOXRelsWriter	* poRelsWriter	= static_cast<OOXRelsWriter*>	(oRenderParameter.poRels);
	RtfDocument		* poRtfDocument	= static_cast<RtfDocument*>		(oRenderParameter.poDocument);
	
    std::wstring sExtension;
    std::wstring sMime;
	switch( eDataType )
	{
		case dt_png:	sExtension = L"png"; sMime = L"image/png";		break;
		case dt_jpg:	sExtension = L"jpg"; sMime = L"image/jpg";		break;
		case dt_wmf:	sExtension = L"wmf"; sMime = L"image/x-wmf";	break;
		case dt_emf:	sExtension = L"emf"; sMime = L"image/x-emf";	break;
		case dt_macpict:sExtension = L"pct"; sMime = L"image/x-pict";	break;
		case dt_svg:	sExtension = L"svg"; sMime = L"image/svg+xml";	break;
	}
	if (poOOXWriter->m_sTargetFolder.empty()) return L"rIdtemp"; //test from fields

    std::wstring sFilenameRels = L"Image" + std::to_wstring(poRtfDocument->m_oIdGenerator.Generate_ImageIndex()) + L".";
	sFilenameRels += sExtension;
	
    std::wstring sFilenameFull = poOOXWriter->m_sTargetFolder + FILE_SEPARATOR_STR + L"word";	
    NSDirectory::CreateDirectory( sFilenameFull );

	sFilenameFull += std::wstring(FILE_SEPARATOR_STR) + L"media";	
    NSDirectory::CreateDirectory( sFilenameFull );

	sFilenameFull += FILE_SEPARATOR_STR + sFilenameRels;
	sFilenameRels = L"media/" + sFilenameRels;

	if( m_sPicFilename != sFilenameFull )
		Utils::CopyDirOrFile( m_sPicFilename, sFilenameFull );
	else
		m_bIsCopy = false;

    if( true == m_bIsCopy && !m_sPicFilename.empty() )
	{
		Utils::RemoveDirOrFile( m_sPicFilename );
	}
	poOOXWriter->m_oContentTypes.AddExtension( sMime, sExtension);

    std::wstring srId = poRelsWriter->AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image", sFilenameRels);

	return srId;
}

RtfPicture::DataType RtfPicture::GetPictureType( std::wstring sFilename )
{
	BYTE	pBuffer[ 100 ];
	DWORD	dwBytesRead = 0;

	NSFile::CFileBinary file;
	if (file.OpenFile(sFilename) == false) return dt_none;

	file.ReadFile(pBuffer, 100);
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

	if ('<' == pBuffer[0] &&
		's' == pBuffer[1] &&
		'v' == pBuffer[2] &&
		'g' == pBuffer[3])
	{
		return dt_svg;;
	}

	if ('<' == pBuffer[0] &&
		'?' == pBuffer[1] &&
		'x' == pBuffer[2] &&
		'm' == pBuffer[3] &&
		'l' == pBuffer[4])
	{
		std::string test((char*)pBuffer, dwBytesRead);
		if (std::string::npos != test.find("<svg"))
		{
			return dt_svg;;
		}
	}
	return dt_none;
}
