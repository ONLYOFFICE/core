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
#include "Writer/OOXWriter.h"
#include "RtfWriter.h"
#include "Utils.h"

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
