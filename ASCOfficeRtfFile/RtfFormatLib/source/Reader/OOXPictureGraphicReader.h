/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "OOXReaderBasic.h"

#include "../RtfPicture.h"

#include "../../../../DesktopEditor/raster/BgraFrame.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Drawing/Drawing.h"

#define PICTURE_BUFFER_SIZE 1024

class OOXPictureGraphicReader
{
private:
	OOX::Drawing::CGraphic *m_ooxGraphic;

public: 
	OOXPictureGraphicReader(OOX::Drawing::CGraphic *ooxGraphic)
	{
		m_ooxGraphic = ooxGraphic;
	}
	bool Parse( ReaderParameter oParam , RtfPicture& oOutput)
	{
		if (m_ooxGraphic == NULL) return false;
		
		switch(m_ooxGraphic->m_eGraphicType)
		{
			case OOX::Drawing::graphictypePicture:
				//собственно тока этот объект пока и есть (
				break;
			case OOX::Drawing::graphictypeLockedCanvas:
			case OOX::Drawing::graphictypeChart:
			case OOX::Drawing::graphictypeDiagram:
			case OOX::Drawing::graphictypeShape:
			case OOX::Drawing::graphictypeGroupShape:
				///todooo воткнуть конвертацию pptx->ppt->vml !!!
				break;
		}

		for (long i=0; i < m_ooxGraphic->m_arrItems.size(); i++)
		{
			if (m_ooxGraphic->m_arrItems[i] == NULL) continue;

			if (m_ooxGraphic->m_arrItems[i]->getType() == OOX::et_pic_pic)
			{
				OOX::Drawing::CPicture *picture = dynamic_cast<OOX::Drawing::CPicture *>(m_ooxGraphic->m_arrItems[i]);
				if ( (picture) && (picture->m_oBlipFill.m_oBlip.IsInit()))
				{
					CString sImageId = picture->m_oBlipFill.m_oBlip->m_oEmbed.GetValue();

					if (oParam.oReader->m_currentContainer)
					{
						smart_ptr<OOX::File> oFile = oParam.oReader->m_currentContainer->Find(sImageId);
						
						if ( oFile.IsInit() && (OOX::FileTypes::Image == oFile->type()))
						{
							OOX::Image* pImage = (OOX::Image*)oFile.operator->();

							CString sImagePath = pImage->filename().GetPath();
							WriteDataToPicture( sImagePath, oOutput, _T("") );
						}
					}
				}
			}
		}
		return true;

	}

	static bool WriteDataToPicture( CString sPath, RtfPicture& oOutput, CString sTempPath )
	{
		OOX::CPath ooxPath = sPath;	//для target 

		if (sTempPath.GetLength() > 0)
			ooxPath = sTempPath;

		oOutput.m_dScaleX = 100;
		oOutput.m_dScaleY = 100;

		//Выставляем тип картинки
		oOutput.eDataType = RtfPicture::GetPictureType( sPath );
		//ecли тип не поддерживается rtf конвертируем в png
		if( RtfPicture::dt_none == oOutput.eDataType )
		{
			//в туже папку что и исходная картинка
			CBgraFrame image;
			if (image.OpenFile(string2std_string(sPath)) == FALSE ) return false;
			
			//правильно выставляем размеры
			oOutput.m_nWidthGoal = image.get_Width(); 
			oOutput.m_nHeightGoal = image.get_Height();
		
			CString sTargetFile = FileSystem::Directory::CreateTempFileWithUniqueName(ooxPath.GetDirectory(), _T("img"));
			
			if (image.SaveFile(string2std_string(sTargetFile), 4 /*_CXIMAGE_FORMAT_PNG*/) )
			{
				oOutput.eDataType = RtfPicture::dt_png;
				//Запоминаем имя
				oOutput.m_sPicFilename = sTargetFile;
				oOutput.m_bIsCopy = true;//выставляем флаг чтобы потом удалить файл
			}
		}
		else
		{
			if( RtfPicture::dt_apm ==  oOutput.eDataType )
			{
				//убираем заголовок apm (22 byte)
				CFile file_inp; //mpa
				CFile file_out;//wmf

				CString sTargetFile = FileSystem::Directory::CreateTempFileWithUniqueName(ooxPath.GetDirectory(), _T("img"));

				int res = file_inp.OpenFile(sPath);
				if (res != S_OK) return false;

                res = file_out.CreateFile(sTargetFile);
				if (res != S_OK) return false;

				DWORD dwBytesRead = 0;
				DWORD dwBytesWrite = 0;
				BYTE pBuffer[1024];
				DWORD nHeaderLen = 22;

				dwBytesRead = file_inp.GetPosition();
				file_inp.ReadFile(pBuffer, 1024);
				dwBytesRead = file_inp.GetPosition() - dwBytesRead;
					
				while( 0 != dwBytesRead )
				{
					if( nHeaderLen > 0 )
					{
						if(dwBytesRead > nHeaderLen )
						{
							file_out.WriteFile(pBuffer + nHeaderLen, dwBytesRead - nHeaderLen);
							nHeaderLen = 0;
						}
						else
							nHeaderLen -= dwBytesRead;
					}
					else
						file_out.WriteFile( pBuffer, dwBytesRead);

					dwBytesRead = file_inp.GetPosition();
					file_inp.ReadFile(pBuffer, 1024);
					dwBytesRead = file_inp.GetPosition() - dwBytesRead;
				}
				file_inp.CloseFile();
				file_out.CloseFile();

				oOutput.eDataType = RtfPicture::dt_wmf;
				//Запоминаем имя
				oOutput.m_sPicFilename = sTargetFile;
				oOutput.m_bIsCopy = true;
			}
			else
			{
				typedef enum {dt_none, dt_png, dt_jpg, dt_emf, dt_wmf, dt_apm}DataType;
				int cxFormats [6] = {0, 4, 3, 10, 10, 10};
				CBgraFrame image;
				
				if (image.OpenFile(string2std_string(sPath), cxFormats[oOutput.eDataType]) == FALSE ) return false;
				//правильно выставляем размеры
				oOutput.m_nWidthGoal = image.get_Width(); 
				oOutput.m_nHeightGoal = image.get_Height();

				//Запоминаем только имя
				oOutput.m_sPicFilename = sPath;
				oOutput.m_bIsCopy = false; //не удалять 
			}
		}

		return true;
	}
};
