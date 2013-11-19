#pragma once
#include "RtfPicture.h"
#include "XmlUtils.h"
#include "OOXReaderBasic.h"

#define PICTURE_BUFFER_SIZE 1024

class OOXPictureGraphicReader
{
public: OOXPictureGraphicReader()
		{
		}
public: bool Parse( ReaderParameter oParam , RtfPicture& oOutput)
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE == oXmlReader.OpenFromXmlNode(oParam.oNode) )
				if( TRUE == oXmlReader.ReadNode( _T("a:graphicData") ) )
					if( TRUE == oXmlReader.ReadNode( _T("pic:pic") ) )
					{
						oXmlReader.ReadNodeList( _T("*") );
						for(int i=0;i< oXmlReader.GetLengthList();i++)
						{
							CString sNodeName = oXmlReader.ReadNodeName(i);
							if( _T("pic:blipFill") == sNodeName )
							{
								XML::IXMLDOMNodePtr oNode;
								oXmlReader.GetNode(i, oNode);
								XmlUtils::CXmlReader oXmlSubReader;
								if( TRUE == oXmlSubReader.OpenFromXmlNode( oNode ) )
									if( TRUE == oXmlSubReader.ReadNode( _T("a:blip") ) )
									{
										CString sImageId = oXmlSubReader.ReadNodeAttribute( _T("r:embed"), _T("") );
										if( _T("") != sImageId )
										{
											CString sRelativePath = oParam.oRels->GetByID( sImageId ); //todo rels могут быть в других местах
											CString sImagePath = oParam.oReader->m_sDocumentPath + _T("\\")+ sRelativePath;

											WriteDataToPicture( sImagePath, oOutput, oParam.oReader->m_sDocumentPath );
										}
									}
								//oXmlSubReader.OpenFromXmlNode( oNode );
								//oXmlSubReader.ReadNode( "a:srcRect" );
								//int nCropPerLeft = Strings::ToInteger( oXmlSubReader.ReadNodeAttribute("l", "-1") );
								//int nCropPerRight = Strings::ToInteger( oXmlSubReader.ReadNodeAttribute("r", "-1") );
								//int nCropPerTop = Strings::ToInteger( oXmlSubReader.ReadNodeAttribute("t", "-1") );
								//int nCropPerBottom = Strings::ToInteger( oXmlSubReader.ReadNodeAttribute("b", "-1") );
								//if( -1 != nCropPerLeft && -1 != oOutput.m_oPicturePropertyShppict.m_nWidth)
								//	oOutput.m_oPicturePropertyShppict.m_nCropL = oOutput.m_oPicturePropertyShppict.m_nWidth * nCropPerLeft / 100000;//тысячные доли процента
								//if( -1 != nCropPerRight && -1 != oOutput.m_oPicturePropertyShppict.m_nWidth)
								//	oOutput.m_oPicturePropertyShppict.m_nCropL = oOutput.m_oPicturePropertyShppict.m_nWidth * nCropPerRight / 100000;//тысячные доли
								//if( -1 != nCropPerTop && -1 != oOutput.m_oPicturePropertyShppict.m_nWidth)
								//	oOutput.m_oPicturePropertyShppict.m_nCropL = oOutput.m_oPicturePropertyShppict.m_nWidth * nCropPerTop / 100000;//тысячные доли
								//if( -1 != nCropPerBottom && -1 != oOutput.m_oPicturePropertyShppict.m_nWidth)
								//	oOutput.m_oPicturePropertyShppict.m_nCropL = oOutput.m_oPicturePropertyShppict.m_nWidth * nCropPerBottom / 100000;//тысячные доли
							}
							else if( _T("pic:spPr") == sNodeName )
							{
								//todo
							}
						}
						return true;
					}
					return false;
		}
public: static bool WriteDataToPicture( CString sPath, RtfPicture& oOutput, CString sTempPath )
		{
			//важно только узнать размеры картинки	
			ImageStudio::IImageTransformsPtr piTransform;
			HRESULT hRes = piTransform.CreateInstance( ImageStudio::CLSID_ImageTransforms );
			if( FAILED( hRes ) || NULL == piTransform )
				return false;
			CString sTransformation = _T("<ImageFile-LoadImage sourcepath='") + sPath + _T("'/>");
			//sTransformation.Append( "<ImageFile-SaveAsPng destinationpath='"+sTempFile+"' format='888' />" );
			BSTR bstrLoad = sTransformation.AllocSysString();
			VARIANT vImage;
			if( piTransform->SetXml(bstrLoad) )
				if(piTransform->Transform())
					piTransform->GetResult( 0, &vImage );
			SysFreeString( bstrLoad );
			if( NULL == vImage.punkVal )
				return false;
			//выставляем свойства из медиадаты
			MediaCore::IAVSUncompressedVideoFrame* piFrame = NULL;
			hRes = vImage.punkVal->QueryInterface( __uuidof(MediaCore::IAVSUncompressedVideoFrame), (void**)&piFrame );
			RELEASEINTERFACE( vImage.punkVal );
			if( FAILED( hRes ) || NULL == piFrame )
				return false;

			//Выставляем тип картинки
			oOutput.eDataType = RtfPicture::GetPictureType( sPath );
			//ecли тип не поддерживается rtf конвертируем в png
			if( RtfPicture::dt_none == oOutput.eDataType )
			{
				//в туже папку что и исходная картинка
				TCHAR tExt[256];
				TCHAR tFilename[256];
				TCHAR tFolder[256];
				TCHAR tDrive[256];
				_tsplitpath( sPath, tDrive, tFolder, tFilename, tExt );
				CString sExt = CString(tExt);
				CString sFilename = CString(tFilename);
				CString sFolder = CString(tFolder);
				CString sDrive = CString(tDrive);

				CString sTargetFolder = sDrive + sFolder;
				CString sTargetFile = Utils::CreateTempFile( sTargetFolder );
				Utils::RemoveDirOrFile( sTargetFile );//удаляем фалй нам нужно было только имя

				CString sTransformation = _T("<ImageFile-SaveAsPng format=\"8888\" destinationpath='") + sTargetFile + _T("'/>");
				BSTR bstrLoad = sTransformation.AllocSysString();
				VARIANT vImage;
				if( piTransform->SetXml(bstrLoad) )
					if( piTransform->Transform() )
						piTransform->GetResult( 0, &vImage );
				SysFreeString( bstrLoad );

				//если файл записался сохраняем его настройки
				if( ::GetFileAttributes( sTargetFile ) != INVALID_FILE_ATTRIBUTES )
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
					
					CString sTargetFile = Utils::CreateTempFile( sTempPath );
					HANDLE hApmFile = ::CreateFile(sPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
					HANDLE hWmfFile = ::CreateFile(sTargetFile, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
					if( INVALID_HANDLE_VALUE != hApmFile && INVALID_HANDLE_VALUE != hWmfFile )
					{
						DWORD dwBytesRead = 0;
						DWORD dwBytesWrite = 0;
						BYTE pBuffer[1024];
						DWORD nHeaderLen = 22;
						::ReadFile( hApmFile, pBuffer, 1024, &dwBytesRead, NULL);
						while( 0 != dwBytesRead )
						{
							if( nHeaderLen > 0 )
							{
								if( dwBytesRead > nHeaderLen )
								{
									::WriteFile( hWmfFile, pBuffer + nHeaderLen, dwBytesRead - nHeaderLen, &dwBytesWrite, NULL);
									nHeaderLen = 0;
								}
								else
									nHeaderLen -= dwBytesRead;
							}
							else
								::WriteFile( hWmfFile, pBuffer, dwBytesRead, &dwBytesWrite, NULL);

							::ReadFile( hApmFile, pBuffer, 1024, &dwBytesRead, NULL);
						}
						oOutput.eDataType = RtfPicture::dt_wmf;
						//Запоминаем имя
						oOutput.m_sPicFilename = sTargetFile;
						oOutput.m_bIsCopy = true;
					}
					RELEASEHANDLE( hApmFile );
					RELEASEHANDLE( hWmfFile );
				}
				else
				{
					//Запоминаем только имя
					oOutput.m_sPicFilename = sPath;
				}
			}
			//правильно выставляем размеры
			oOutput.m_nWidthGoal = RtfUtility::px2Twip( piFrame->Width );
			oOutput.m_nHeightGoal = RtfUtility::px2Twip( piFrame->Height );
			oOutput.m_dScaleX = 100;
			oOutput.m_dScaleY = 100;
			RELEASEINTERFACE( piFrame );
			return true;
		}
};