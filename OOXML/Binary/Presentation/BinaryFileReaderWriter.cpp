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
#include "BinaryFileReaderWriter.h"
#include "BinReaderWriterDefines.h"

#include "../../Base/Nullable.h"
#include "../../DocxFormat/WritingElement.h"

#include "../../DocxFormat/Media/OleObject.h"
#include "../../DocxFormat/Media/ActiveX.h"
#include "../../DocxFormat/Media/Video.h"
#include "../../DocxFormat/Media/Audio.h"
#include "../../DocxFormat/Media/VbaProject.h"
#include "../../DocxFormat/Media/JsaProject.h"

#include "../../../Common/Base64.h"

#include "./imagemanager.h"
#include "./XmlWriter.h"
#include "./FontPicker.h"
#include "../../Binary/Document/DocWrapper/DocxSerializer.h"

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

#include "../../PPTXFormat/FileContainer.h"
#include <iostream>

#ifndef DISABLE_FILE_DOWNLOADER
#include "../../../Common/Network/FileTransporter/include/FileTransporter.h"
#endif 

#define BYTE_SIZEOF		sizeof(BYTE)
#define UINT16_SIZEOF	sizeof(_UINT16)
#define UINT32_SIZEOF	sizeof(_UINT32)
#define DOUBLE_SIZEOF	sizeof(double)

#define CHAR_SIZEOF		sizeof(CHAR)
#define INT16_SIZEOF	sizeof(_INT16)
#define INT32_SIZEOF	sizeof(_INT32)
#define INT64_SIZEOF	sizeof(_INT64)

#define DOUBLE_MAIN		10000

#if defined(_WIN32) || defined (_WIN64)
	#include "../../Base/unicode_util.h"
#endif

namespace NSBinPptxRW
{
	template <typename T,unsigned S>
	inline unsigned arraysize(const T (&v)[S]) { return S; }

	CCommonWriter::CCommonWriter()
	{
		m_pNativePicker = NULL;
		m_pFontPicker = NULL;
		m_bDeleteFontPicker = true;
		m_pMediaManager = new NSShapeImageGen::CMediaManager();
	}
	CCommonWriter::~CCommonWriter()
	{
		m_pNativePicker = NULL;
		if(m_bDeleteFontPicker)
			RELEASEOBJECT(m_pFontPicker);
		RELEASEOBJECT(m_pMediaManager);
	}
	void CCommonWriter::CreateFontPicker(COfficeFontPicker* pPicker)
	{
		if(m_bDeleteFontPicker)
			RELEASEOBJECT(m_pFontPicker);
		m_pNativePicker = NULL;
		if (pPicker != NULL)
		{
			m_pFontPicker = pPicker;
			m_bDeleteFontPicker = false;
		}
		else
		{
			m_pFontPicker = new COfficeFontPicker();
			m_bDeleteFontPicker = true;
		}

		m_pNativePicker = m_pFontPicker->GetNativePicker();
	}
	void CCommonWriter::CheckFontPicker()
	{
		if (NULL == m_pFontPicker)
			CreateFontPicker(NULL);
	}

	CImageManager2::CImageManager2() : m_mapImages(), m_lIndexNextImage(0), m_lIndexCounter(0)
	{
        m_nDocumentType = XMLWRITER_DOC_TYPE_PPTX;
		m_pContentTypes = new OOX::CContentTypes();
	}
	CImageManager2::~CImageManager2()
	{
		delete m_pContentTypes;
	}
	void CImageManager2::Clear()
	{
		m_mapImages.clear();
		m_lIndexNextImage	= 0;
		m_lIndexCounter		= 0;
	}
	void CImageManager2::SetDstFolder(const std::wstring& strDst)
	{
		m_strDstFolder = strDst;
		m_strDstMedia = m_strDstFolder + FILE_SEPARATOR_STR + _T("media");
		m_strDstEmbed = m_strDstFolder + FILE_SEPARATOR_STR + _T("embeddings");

		NSDirectory::CreateDirectory(m_strDstMedia);
		NSDirectory::CreateDirectory(m_strDstEmbed);
	}
	std::wstring CImageManager2::GetDstFolder()
	{
		return m_strDstFolder;
	}
	void CImageManager2::SetDstMedia(const std::wstring& strDst)
	{
		m_strDstMedia = strDst;
	}
	std::wstring CImageManager2::GetDstMedia()
	{
		return m_strDstMedia;
	}
	void CImageManager2::SetDstCharts(const std::wstring& strDst)
	{
		m_strDstCharts = strDst;
	}
	std::wstring CImageManager2::GetDstCharts()
	{
		return m_strDstCharts;
	}
	void CImageManager2::SetDstEmbed(const std::wstring& strDst)
	{
		m_strDstEmbed = strDst;
	}
	std::wstring CImageManager2::GetDstEmbed()
	{
		return m_strDstEmbed;
	}
	std::wstring CImageManager2::GetDstDiagram()
	{
		return m_strDstDiagram;
	}
	void CImageManager2::SetDstDiagram(const std::wstring& strDst)
	{
		m_strDstDiagram = strDst;
	}	
	int CImageManager2::IsDisplayedImage(const std::wstring& strInput)
	{
		int nRes = 0;
		//шаблон display[N]image.ext
		std::wstring sFind1 = _T("display");
		int nIndex1 = (int)strInput.find(sFind1);
		if(-1 != nIndex1)
		{
			if(nIndex1 + sFind1.length() < strInput.length())
			{
                wchar_t cRes1 = strInput[nIndex1 + sFind1.length()];
				if('1' <= cRes1 && cRes1 <= '9')
				{
					wchar_t cRes2 = strInput[nIndex1 + sFind1.length() + 1];
	
					int nImageIndex = nIndex1 + (int)sFind1.length() + 1;
					if (std::wstring::npos != strInput.find(_T("image"), nImageIndex))
					{
						nRes = cRes1 - '0';
						if('0' <= cRes2 && cRes2 <= '9')
						{
							 nRes = nRes * 10 + (cRes2 - '0');
						}	
					}

				}

			}
		}
		return nRes;
	}
	_imageManager2Info CImageManager2::GenerateMedia(const std::wstring& strInput)
	{
		std::map<std::wstring, _imageManager2Info>::const_iterator pPair = m_mapImages.find(strInput);

		if (pPair != m_mapImages.end())
		{
			return pPair->second;
		}

		_imageManager2Info oImageManagerInfo = GenerateMediaExec(strInput);
			
		m_mapImages[strInput] = oImageManagerInfo;

		return oImageManagerInfo;
	}
	_imageManager2Info CImageManager2::GenerateImage(const std::wstring& strInput, NSCommon::smart_ptr<OOX::File> & additionalFile, const std::wstring& oleData, std::wstring strBase64Image)
	{
		if (IsNeedDownload(strInput))
			return DownloadImage(strInput);

		std::map<std::wstring, _imageManager2Info>::const_iterator pPair = m_mapImages.find ((strBase64Image.empty()) ? strInput : strBase64Image);

		if (pPair != m_mapImages.end())
		{
			smart_ptr<OOX::Media> mediaFile = additionalFile.smart_dynamic_cast<OOX::Media>();
			if (mediaFile.IsInit())
				mediaFile->set_filename(pPair->second.sFilepathAdditional, false);

			return pPair->second;
		}

		std::wstring strExts = _T(".jpg");
		//use GetFileName to avoid defining '.' in the directory as extension
		std::wstring strFileName = NSFile::GetFileName(strInput);
		int sizeExt = (int)strFileName.rfind(wchar_t('.'));
		if (-1 != sizeExt)
		{
			strExts = strFileName.substr(sizeExt);
			sizeExt = (int)strFileName.length() - sizeExt;
		}
		else sizeExt = 0;

		int	typeAdditional = 0;
		std::wstring strAdditional;
		std::wstring strImage = strInput;

		int nDisplayType = IsDisplayedImage(strInput);
		size_t nFileNameLength = strFileName.length();
		if (0 != nDisplayType && nFileNameLength > sizeExt)
		{
			OOX::CPath oPath = strInput;
			
			std::wstring strFolder		= oPath.GetDirectory();
			std::wstring strFileName	= oPath.GetFilename();

			strFileName.erase(strFileName.length() - sizeExt, sizeExt);

			if(0 != (nDisplayType & 1))
			{
				std::wstring strVector = strFolder + strFileName + _T(".wmf");
				if (OOX::CSystemUtility::IsFileExist(strVector))
				{
					strImage = strVector;
					strExts = _T(".wmf");
				}
			}
			if(0 != (nDisplayType & 2))
			{
				std::wstring strVector = strFolder + strFileName + L".emf";
				if (OOX::CSystemUtility::IsFileExist(strVector))
				{
					m_pContentTypes->AddDefault(L"emf");
					strImage = strVector;
					strExts = L".emf";
				}
			}
			if(0 != (nDisplayType & 4))
			{
				smart_ptr<OOX::OleObject> oleFile = additionalFile.smart_dynamic_cast<OOX::OleObject>();
				if (oleFile.IsInit())
				{
					if (OOX::CSystemUtility::IsFileExist(oleFile->filename()) == false)
					{
						typeAdditional = 1;
						
						std::wstring strOle = strFolder + strFileName + oleFile->filename().GetExtention();
						if (OOX::CSystemUtility::IsFileExist(strOle))
						{
							m_pContentTypes->AddDefault(oleFile->filename().GetExtention(false));
							strAdditional = strOle;
						}
						else
						{
							strOle = strFolder + strFileName + L".bin";
							if (OOX::CSystemUtility::IsFileExist(strOle))
								strAdditional = strOle;
						}
					}
				}
			}
			if(0 != (nDisplayType & 8))
			{
				smart_ptr<OOX::Media> mediaFile = additionalFile.smart_dynamic_cast<OOX::Media>();
				if (mediaFile.IsInit())
				{
					if (OOX::CSystemUtility::IsFileExist(mediaFile->filename()) == false)
					{
						typeAdditional = 2;

						if (!mediaFile->IsExternal())
						{
							std::wstring strMedia = strFolder + strFileName + mediaFile->filename().GetExtention();
							if (OOX::CSystemUtility::IsFileExist(strMedia))
							{
								m_pContentTypes->AddDefault(mediaFile->filename().GetExtention(false));
								strAdditional = strMedia;
							}
							else
							{
								strMedia = strFolder + strFileName;
								
								if (mediaFile.is<OOX::Audio>()) strMedia += L".wav";
								if (mediaFile.is<OOX::Video>()) strMedia += L".avi";
								
								if (OOX::CSystemUtility::IsFileExist(strMedia))
									strAdditional = strMedia;
							}
						}
					}
				}
			}
		}
		if (!strExts.empty())
		{
			m_pContentTypes->AddDefault(strExts.substr(1));
		}

		if (oleData.empty() == false)
		{
			//plugins data - generate ole
			typeAdditional = 1;
		}

		_imageManager2Info oImageManagerInfo = GenerateImageExec(strImage, strExts, strAdditional, typeAdditional, oleData);

		if (!oImageManagerInfo.sFilepathAdditional.empty()) 
		{
			smart_ptr<OOX::Media> mediaFile = additionalFile.smart_dynamic_cast<OOX::Media>();
			if (mediaFile.IsInit())
			{
				mediaFile->set_filename(oImageManagerInfo.sFilepathAdditional, false);
			}
		}
			
		if (strBase64Image.empty())
			m_mapImages[strInput] = oImageManagerInfo;
		else
			m_mapImages [strBase64Image] = oImageManagerInfo;
		return oImageManagerInfo;
	}
	bool CImageManager2::WriteOleData(const std::wstring& sFilePath, const std::wstring& sData)
	{
		bool bRes = false;
		//EncodingMode.unparsed https://github.com/tonyqus/npoi/blob/master/main/POIFS/FileSystem/Ole10Native.cs
		POLE::Storage oStorage(sFilePath.c_str());
		if(oStorage.open(true, true))
		{
			//CompObj Stream
			BYTE dataCompObj[] = {0x01,0x00,0xfe,0xff,0x03,0x0a,0x00,0x00,0xff,0xff,0xff,0xff,0x0c,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x0c,0x00,0x00,0x00,0x4f,0x4c,0x45,0x20,0x50,0x61,0x63,0x6b,0x61,0x67,0x65,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x50,0x61,0x63,0x6b,0x61,0x67,0x65,0x00,0xf4,0x39,0xb2,0x71,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
			POLE::Stream oStream1(&oStorage, L"\001CompObj", true, arraysize(dataCompObj));
			oStream1.write(dataCompObj, arraysize(dataCompObj));
			oStream1.flush();
			//ObjInfo Stream
			BYTE dataObjInfo[] = {0x00,0x00,0x03,0x00,0x0d,0x00};
			POLE::Stream oStream2(&oStorage, L"\003ObjInfo", true, arraysize(dataObjInfo));
			oStream2.write(dataObjInfo, arraysize(dataObjInfo));
			oStream2.flush();
			//Ole10Native Stream
			std::string sDataUtf8 = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sData.c_str(), (LONG)sData.size());
			BYTE head[] = {0x00,0x00,0x00,0x00};
			//LittleEndian
			
			unsigned char*	aData		= (unsigned char*)sDataUtf8.c_str();			
			_UINT32			nDataSize	= (_UINT32)sDataUtf8.size();

			memcpy(head, &nDataSize, sizeof(_UINT32));

			POLE::Stream oStream(&oStorage, L"\001Ole10Native", true, arraysize(head) + nDataSize);
			oStream.write(head, arraysize(head));
			oStream.write(aData, nDataSize);
			oStream.flush();

			oStorage.close();
			bRes = true;
		}
		return bRes;
	}
	_imageManager2Info CImageManager2::GenerateMediaExec(const std::wstring& strInput)
	{
		OOX::CPath			oPathOutput;
		_imageManager2Info	oImageManagerInfo;
		
		std::wstring strExts;
        std::wstring strMedia = L"media" + std::to_wstring(++m_lIndexNextImage);
		
		int pos = (int)strInput.rfind(L".");
		if (pos >= 0) 
		{
			strExts = strInput.substr(pos);
			m_pContentTypes->AddDefault(strExts.substr(1));
		}

		oPathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strMedia + strExts;

		if (oPathOutput.GetPath() != strInput && NSFile::CFileBinary::Exists(strInput))
		{
            NSFile::CFileBinary::Copy(strInput, oPathOutput.GetPath());
			oImageManagerInfo.sFilepathImage = oPathOutput.GetPath();
		}
		return oImageManagerInfo;
	}
	_imageManager2Info CImageManager2::GenerateImageExec(const std::wstring& strInput, const std::wstring& sExts, const std::wstring& strAdditionalImage, int nAdditionalType, const std::wstring& oleData)
	{
		OOX::CPath			oPathOutput;
		_imageManager2Info	oImageManagerInfo;
		
		std::wstring strExts	= sExts;
        std::wstring strImage	= L"image" + std::to_wstring(++m_lIndexNextImage);
		
		if ((_T(".jpg") == strExts) || (_T(".jpeg") == strExts) || (_T(".png") == strExts) || (_T(".emf") == strExts) || (_T(".wmf") == strExts))
		{
			oPathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strImage + strExts;

			if (oPathOutput.GetPath() != strInput && NSFile::CFileBinary::Exists(strInput))
			{
                NSFile::CFileBinary::Copy(strInput, oPathOutput.GetPath());
				oImageManagerInfo.sFilepathImage = oPathOutput.GetPath();
			}
		}
		else
		{
	// content types!!!
			strExts = _T(".png");
			oPathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strImage + strExts;
            SaveImageAsPng(strInput, oPathOutput.GetPath());
			oImageManagerInfo.sFilepathImage = oPathOutput.GetPath();
		}		
		
		if ((!strAdditionalImage.empty() || !oleData.empty() ) && (nAdditionalType == 1))
		{
			std::wstring strAdditionalExt  = L".bin";

			int pos = (int)strAdditionalImage.rfind(L".");
			if (pos >= 0) strAdditionalExt = strAdditionalImage.substr(pos);

			std::wstring strImageAdditional = L"oleObject" + std::to_wstring(++m_lIndexCounter) + strAdditionalExt;
			
			OOX::CPath pathOutput = m_strDstEmbed + FILE_SEPARATOR_STR + strImageAdditional;
			
			std::wstring strAdditionalImageOut = pathOutput.GetPath();
			
			if(!oleData.empty())
			{
				WriteOleData(strAdditionalImageOut, oleData);
				oImageManagerInfo.sFilepathAdditional = strAdditionalImageOut;
			}
			else if (NSFile::CFileBinary::Exists(strAdditionalImage))
			{
				NSFile::CFileBinary::Copy(strAdditionalImage, strAdditionalImageOut);
				oImageManagerInfo.sFilepathAdditional = strAdditionalImageOut;
			}

		}
		else if (!strAdditionalImage.empty() && nAdditionalType == 2)
		{			
			std::wstring strAdditionalExt;

			int pos = (int)strAdditionalImage.rfind(L".");
			if (pos >= 0) strAdditionalExt = strAdditionalImage.substr(pos);

			std::wstring strImageAdditional = L"media" + std::to_wstring(++m_lIndexCounter) + strAdditionalExt;
			
			OOX::CPath pathOutput = m_strDstMedia + FILE_SEPARATOR_STR + strImageAdditional;
			
			std::wstring strAdditionalImageOut = pathOutput.GetPath();

			if (NSFile::CFileBinary::Exists(strAdditionalImage))
			{
				NSFile::CFileBinary::Copy(strAdditionalImage, strAdditionalImageOut);
				oImageManagerInfo.sFilepathAdditional = strAdditionalImageOut;
			}
		}

		return oImageManagerInfo;
	}
	void CImageManager2::SaveImageAsPng(const std::wstring& strFileSrc, const std::wstring& strFileDst)
	{
		CBgraFrame oBgraFrame;
		if(oBgraFrame.OpenFile(strFileSrc))
			oBgraFrame.SaveFile(strFileDst, _CXIMAGE_FORMAT_PNG);
	}

	void CImageManager2::SaveImageAsJPG(const std::wstring& strFileSrc, const std::wstring& strFileDst)
	{
		CBgraFrame oBgraFrame;
		if(oBgraFrame.OpenFile(strFileSrc))
			oBgraFrame.SaveFile(strFileDst, _CXIMAGE_FORMAT_JPG);
	}

	bool CImageManager2::IsNeedDownload(const std::wstring& strFile)
	{
		size_t n1 = strFile.find(_T("www"));
		size_t n2 = strFile.find(_T("http"));
		size_t n3 = strFile.find(_T("ftp"));
		size_t n4 = strFile.find(_T("https://"));

        //если nI сранивать не с 0, то будут проблемы
        //потому что в инсталяции мы кладем файлы в /var/www...
        if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			return true;
		return false;
	}
	_imageManager2Info CImageManager2::DownloadImage(const std::wstring& strUrl)
	{
		std::map<std::wstring, _imageManager2Info>::const_iterator pPair = m_mapImages.find (strUrl);

		if (pPair != m_mapImages.end())
			return pPair->second;

		std::wstring strExts = _T(".jpg");
        int nIndexExt = (int)strUrl.rfind(wchar_t('.'));
		if (-1 != nIndexExt)
			strExts = strUrl.substr(nIndexExt);

		std::wstring strImage;
		
		int nDisplayType = IsDisplayedImage(strUrl);
		
		if(0 != nDisplayType)
		{
			std::wstring strInputMetafile = strUrl.substr(0, strUrl.length() - strExts.length());
			std::wstring sDownloadRes;

			if(0 != (nDisplayType & 1))
			{
				strImage = DownloadImageExec(strInputMetafile + _T(".wmf"));
				strExts = _T(".wmf");
			}
			else if(0 != (nDisplayType & 2))
			{
				strImage = DownloadImageExec(strInputMetafile + _T(".emf"));
				strExts = _T(".emf");
			}
			else
			{
				strImage = DownloadImageExec(strUrl);
			}
		}
		else
		{
			strImage = DownloadImageExec(strUrl);
		}

		if (!strExts.empty())
		{
			m_pContentTypes->AddDefault(strExts.substr(1));
		}

		_imageManager2Info oImageManagerInfo;
		if (!strImage.empty())
		{
			oImageManagerInfo = GenerateImageExec(strImage, strExts, L"", 0, L"");
			CDirectory::DeleteFile(strImage);
		}

		m_mapImages[strUrl] = oImageManagerInfo;
		return oImageManagerInfo;
	}
	std::wstring CImageManager2::DownloadImageExec(const std::wstring& strFile)
	{
#ifndef DISABLE_FILE_DOWNLOADER
        NSNetwork::NSFileTransport::CFileDownloader oDownloader(strFile, false);

		if ( oDownloader.DownloadSync() )
		{
			std::wstring file_name = oDownloader.GetFilePath();
			
			CImageFileFormatChecker checker;
			if (checker.isImageFile(file_name))
			{
				return file_name;
			}
		}
#endif
		return L"";
	}

	CBinaryFileWriter::CSeekTableEntry::CSeekTableEntry()
	{
		Type = 0;
		SeekPos = 0;
	}
	BYTE* CBinaryFileWriter::GetBuffer()
	{
		return m_pStreamData;
	}
	_UINT32 CBinaryFileWriter::GetPosition()
	{
		return m_lPosition;
	}
	void CBinaryFileWriter::SetPosition(const _UINT32& lPosition)
	{
		m_lPosition = lPosition;
		m_pStreamCur = m_pStreamData + m_lPosition;
	}
	void CBinaryFileWriter::Skip(const _UINT32& lSize)
	{
		CheckBufferSize(lSize);

		m_lPosition += lSize;
		m_pStreamCur = m_pStreamData + m_lPosition;
	}
	double CBinaryFileWriter::GetShapeHeight()
	{
		if (m_lCyCurShape == 0)
			return -1;
		return (double)m_lCyCurShape / 36000; //mm
	}
	double CBinaryFileWriter::GetShapeWidth()
	{
		if (m_lCyCurShape == 0)
			return -1;
		return (double)m_lCxCurShape / 36000;
	}
	double CBinaryFileWriter::GetShapeY()
	{
		return (double)m_lYCurShape / 36000;
	}
	double CBinaryFileWriter::GetShapeX()
	{
		return (double)m_lXCurShape / 36000; //mm
	}
	void CBinaryFileWriter::ClearCurShapePositionAndSizes()
	{
		m_lXCurShape	= 0;
		m_lYCurShape	= 0;
		
		m_lCxCurShape = 0;
		m_lCyCurShape = 0;
	}
	void CBinaryFileWriter::Clear()
	{
		m_lSize		= 0;
		m_lPosition = 0;

		m_pStreamData	= NULL;
		m_pStreamCur	= NULL;

		m_lStackPosition = 0;
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(_UINT32));

		m_lCxCurShape = 0;
		m_lCyCurShape = 0;

		m_lXCurShape = 0;
		m_lYCurShape = 0;
	}

	void CBinaryFileWriter::SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc)
	{
		m_pMainDocument = pMainDoc;
	}

	void CBinaryFileWriter::ClearNoAttack()
	{
		m_lPosition = 0;
		m_pStreamCur	= m_pStreamData;

		m_lStackPosition = 0;
		memset(m_arStack, 0, MAX_STACK_SIZE * sizeof(_UINT32));
	}

	void CBinaryFileWriter::CheckBufferSize(_UINT32 lPlus)
	{
		if (NULL != m_pStreamData)
		{
			size_t nNewSize = m_lPosition + lPlus;

			if (nNewSize >= m_lSize)
			{
				while (nNewSize >= m_lSize)
				{
					unsigned int lSize = m_lSize * 2;
					if (lSize < m_lSize)
					{
						m_lSize = nNewSize;
						break;
					}
					m_lSize = lSize;
				}

				BYTE* pNew = new BYTE[m_lSize];
				memcpy(pNew, m_pStreamData, m_lPosition);

				RELEASEARRAYOBJECTS(m_pStreamData);
				m_pStreamData = pNew;

				m_pStreamCur = m_pStreamData + m_lPosition;
			}
		}
		else
		{
			m_lSize = 1024 * 1024; // 1Mb
			m_pStreamData = new BYTE[m_lSize];

			m_lPosition = 0;
			m_pStreamCur = m_pStreamData;

			CheckBufferSize(lPlus);
		}
	}

	void CBinaryFileWriter::WriteBYTE(const BYTE& lValue)
	{			
		CheckBufferSize(BYTE_SIZEOF);

		*m_pStreamCur = lValue; 
		m_lPosition += BYTE_SIZEOF;
		m_pStreamCur += BYTE_SIZEOF;
	}
	void CBinaryFileWriter::WriteSBYTE(const signed char& lValue)
	{
		CheckBufferSize(BYTE_SIZEOF);

		if (lValue < 0)
			*m_pStreamCur = (lValue + 256);
		else
			*m_pStreamCur = lValue;
		m_lPosition += BYTE_SIZEOF;
		m_pStreamCur += BYTE_SIZEOF;
	}
	void CBinaryFileWriter::WriteBOOL(const bool& bValue)
	{			
		WriteBYTE((bValue == true) ? 1 : 0);
	}
	void CBinaryFileWriter::WriteUSHORT(const _UINT16& lValue)
	{
		CheckBufferSize(UINT16_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lValue, sizeof(_UINT16));
#else
        *((_UINT16*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT16_SIZEOF;
		m_pStreamCur += UINT16_SIZEOF;
	}
	void CBinaryFileWriter::WriteSHORT(const _INT16& lValue)
	{
		CheckBufferSize(INT16_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
		memcpy(m_pStreamCur, &lValue, sizeof(_INT16));
#else
		*((_INT16*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT16_SIZEOF;
		m_pStreamCur += INT16_SIZEOF;
	}
	void CBinaryFileWriter::WriteULONG(const _UINT32& lValue)
	{
		CheckBufferSize(UINT32_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lValue, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteLONG(const _INT32& lValue)
	{
		CheckBufferSize(INT32_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lValue, sizeof(_INT32));
#else
        *((_INT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT32_SIZEOF;
		m_pStreamCur += INT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteLONG64(const _INT64& lValue)
	{
		CheckBufferSize(INT64_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lValue, sizeof(_INT64));
#else
        *((_INT64*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT64_SIZEOF;
		m_pStreamCur += INT64_SIZEOF;
	}
	void CBinaryFileWriter::WriteINT(const _INT32& lValue)
	{
		CheckBufferSize(INT32_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lValue, sizeof(_INT32));
#else
        *((_INT32*)m_pStreamCur) = lValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += INT32_SIZEOF;
		m_pStreamCur += INT32_SIZEOF;
	}
	void CBinaryFileWriter::WriteDouble(const double& dValue)
	{
		_INT64 _val = (_INT64)(dValue * 100000);

		if (_val > 0x7fffffff)
		{
			WriteLONG(0x7fffffff);
		}
		else if ( _val < -0x7fffffff)
		{
			WriteLONG(-0x7fffffff);
		}		
		else
		{
			WriteLONG((int)_val);
		}
	}
	void CBinaryFileWriter::WriteDoubleReal(const double& dValue)
	{
		CheckBufferSize(DOUBLE_SIZEOF);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &dValue, sizeof(double));
#else
		*((double*)m_pStreamCur) = dValue; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += DOUBLE_SIZEOF;
		m_pStreamCur += DOUBLE_SIZEOF;
	}
	void CBinaryFileWriter::WriteBYTEArray(const BYTE* pBuffer, size_t len)
	{
		CheckBufferSize((_UINT32)len);
		memcpy(m_pStreamCur, pBuffer, len);
		m_lPosition += (_UINT32)len;
		m_pStreamCur += len;
	}
	void CBinaryFileWriter::WriteStringA(std::string& sBuffer)
	{
		_UINT32 lSize		= (_UINT32)sBuffer.length();
		_UINT32 lSizeMem	= lSize * sizeof(char);

		CheckBufferSize(UINT32_SIZEOF + lSizeMem);
#if defined(_IOS) || defined(__ANDROID__)
        memcpy(m_pStreamCur, &lSizeMem, sizeof(_UINT32));
#else
        *((_UINT32*)m_pStreamCur) = lSizeMem; // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;

		memcpy(m_pStreamCur, sBuffer.c_str(), lSizeMem);
		
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}
	void CBinaryFileWriter::WriteStringW(const std::wstring& sBuffer)
	{
		_WriteStringWithLength(sBuffer.c_str(), (_UINT32)sBuffer.length(), true);
	}
    void CBinaryFileWriter::WriteStringW2(const std::wstring& sBuffer)
	{
        _WriteStringWithLength(sBuffer.c_str(), (_UINT32)sBuffer.length(), false);
	}
    void CBinaryFileWriter::WriteStringW3(const std::wstring& sBuffer)
	{
		_WriteString(sBuffer.c_str(), (_UINT32)sBuffer.length());
	}
	void CBinaryFileWriter::WriteStringW4(const std::wstring& sBuffer)
	{
        _WriteString(sBuffer.c_str(), (_UINT32)sBuffer.length());
	}
	void CBinaryFileWriter::WriteStringUtf8(const std::wstring& sBuffer)
	{
		BYTE* pData = NULL;
		LONG lLen = 0;

		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sBuffer.c_str(), (LONG)sBuffer.length(), pData, lLen, false);

		WriteBYTEArray(pData, lLen);

		RELEASEARRAYOBJECTS(pData);
	}
	CBinaryFileWriter::CBinaryFileWriter()
	{
		m_pMainDocument		= NULL;
		m_pCommon			= new CCommonWriter();
		//m_pCommonRels		= new NSCommon::smart_ptr<PPTX::CCommonRels>();
		m_pCurrentContainer = new NSCommon::smart_ptr<OOX::IFileContainer>();
		m_pTheme			= new NSCommon::smart_ptr<PPTX::Theme>();
		m_pClrMap			= new NSCommon::smart_ptr<PPTX::Logic::ClrMap>();
		
		Clear();
	}
	CBinaryFileWriter::~CBinaryFileWriter()
	{
		RELEASEARRAYOBJECTS	(m_pStreamData);
		RELEASEOBJECT		(m_pCommon);
		//RELEASEOBJECT		(m_pCommonRels);
		RELEASEOBJECT		(m_pCurrentContainer);
		
		RELEASEOBJECT		(m_pTheme);
		RELEASEOBJECT		(m_pClrMap);
	}
	void CBinaryFileWriter::SetRels(NSCommon::smart_ptr<OOX::IFileContainer> container)
	{
		*m_pCurrentContainer = container;
	}
	void CBinaryFileWriter::SetRels(OOX::IFileContainer *container)
	{
		*m_pCurrentContainer = NSCommon::smart_ptr<OOX::IFileContainer>(container);
		m_pCurrentContainer->AddRef();
	}
	NSCommon::smart_ptr<OOX::IFileContainer> CBinaryFileWriter::GetRels()
	{
		return *m_pCurrentContainer;
	}
	void CBinaryFileWriter::StartRecord(_INT32 lType)
	{
		m_arStack[m_lStackPosition] = m_lPosition + 5; // sizeof(BYTE) + sizeof(_UINT32)
		m_lStackPosition++;
		WriteBYTE((BYTE)lType);
		WriteULONG(0);
	}
	void CBinaryFileWriter::EndRecord()
	{
		m_lStackPosition--;

        _UINT32 size_record = m_lPosition - m_arStack[m_lStackPosition];
        (*(_UINT32*)(m_pStreamData + m_arStack[m_lStackPosition] - 4)) = size_record ;
	}

	void CBinaryFileWriter::StartMainRecord(_INT32 lType)
	{
		CSeekTableEntry oEntry;
		oEntry.Type = lType;
		oEntry.SeekPos = m_lPosition;
		m_arMainTables.push_back(oEntry);
		//StartRecord(lType);
	}
	void CBinaryFileWriter::WriteRecord2(int type, OOX::WritingElement* pVal)
	{
		if (pVal)
		{
			StartRecord(type);
			pVal->toPPTY(this);
			EndRecord();
		}
	}
	void CBinaryFileWriter::WriteReserved(size_t lCount)
	{
		CheckBufferSize((_UINT32)lCount);
		memset(m_pStreamCur, 0, lCount);
		m_pStreamCur += lCount;
		m_lPosition += (_UINT32)lCount;
	}

	void CBinaryFileWriter::WriteMainPart(_UINT32 nStartPos)
	{
		BYTE* pData = m_pStreamData + nStartPos;
		size_t nCount = m_arMainTables.size();

		for (size_t i = 0; i < nCount; i++)
		{
			*pData = (BYTE)m_arMainTables[i].Type;
			++pData;
#if defined(_IOS) || defined(__ANDROID__)
            memcpy(pData, &m_arMainTables[i].SeekPos, sizeof(_INT32));
#else
            *((_INT32*)pData) = m_arMainTables[i].SeekPos; // EXC_ARM_DA_ALIGN on ios
#endif
			pData += 4;
		}
	}

	void CBinaryFileWriter::WriteString1(int type, const std::wstring& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

		std::wstring* s = const_cast<std::wstring*>(&val);
		_WriteStringWithLength(s->c_str(), (_UINT32)s->length(), false);
	}
	void CBinaryFileWriter::WriteString2(int type, const NSCommon::nullable_string& val)
	{
		if (val.is_init())
			WriteString1(type, *val);
	}
	void CBinaryFileWriter::WriteString(const std::wstring& val)
	{
		std::wstring* s = const_cast<std::wstring*>(&val);
        _WriteStringWithLength(s->c_str(), (_UINT32)s->length(), false);
	}

	void CBinaryFileWriter::WriteStringData(const WCHAR* pData, _UINT32 len)
	{
		_WriteStringWithLength(pData, len, false);
	}

	void CBinaryFileWriter::WriteString1Data(int type, const WCHAR* pData, _UINT32 len)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);

		_WriteStringWithLength(pData, len, false);
	}

	void CBinaryFileWriter::WriteBool1(int type, const bool& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteBYTE((val == true) ? 1 : 0);
	}
	void CBinaryFileWriter::WriteBool2(int type, const NSCommon::nullable_bool& val)
	{
		if (val.is_init())
			WriteBool1(type, *val);
	}
	void CBinaryFileWriter::WriteByte1(int type, const BYTE& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteBYTE(val);
	}
	void CBinaryFileWriter::WriteInt1(int type, const int& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteINT(val);
	}
	void CBinaryFileWriter::WriteInt2(int type, const NSCommon::nullable_int& val)
	{
		if (val.is_init())
			WriteInt1(type, *val);
	}
	void CBinaryFileWriter::WriteUInt1(int type, const unsigned int& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteULONG(val);
	}
	void CBinaryFileWriter::WriteUInt2(int type, const NSCommon::nullable_uint& val)
	{
		if (val.is_init())
			WriteUInt1(type, *val);
	}
	void CBinaryFileWriter::WriteDouble1(int type, const double& val)
	{
		_INT64 _val = (_INT64)(val * 100000);

		if (_val > 0x7fffffff)
		{
			WriteInt1(type, 0x7fffffff);
		}
		else if (_val < -0x7fffffff)
		{
			WriteInt1(type, -0x7fffffff);
		}
		else
		{
			WriteInt1(type, (int)_val);
		}
	}
	void CBinaryFileWriter::WriteDouble2(int type, const NSCommon::nullable_double& val)
	{
		if (val.is_init())
			WriteDouble1(type, *val);
	}
	void CBinaryFileWriter::WriteDoubleReal1(int type, const double& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		WriteDoubleReal(val);
	}
	void CBinaryFileWriter::WriteDoubleReal2(int type, const NSCommon::nullable_double& val)
	{
		if (val.is_init())
			WriteDoubleReal1(type, *val);
	}
	void CBinaryFileWriter::WriteSize_t1(int type, const size_t& val)
	{
		BYTE bType = (BYTE)type;
		WriteBYTE(bType);
		_UINT32 ival = (_UINT32)val;
		WriteULONG(ival);
	}
	void CBinaryFileWriter::WriteSize_t2(int type, const NSCommon::nullable_sizet& val)
	{
		if (val.is_init())
			WriteSize_t1(type, *val);
	}

	void CBinaryFileWriter::GetBase64File(const std::wstring& sFile, std::string& strDst64)
	{
		NSFile::CFileBinary oFile;
		bool res = oFile.OpenFile(sFile);

		if (false == res)
		{
			strDst64 = "";
			return;
		}

		DWORD dwLen = (DWORD)oFile.GetFileSize();
		BYTE* pBuffer = new BYTE[dwLen];

		oFile.SetPosition(0);
		oFile.ReadFile(pBuffer, dwLen);

		int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength((int)dwLen, Base64::B64_BASE64_FLAG_NOCRLF);
        BYTE *pbBase64Buffer = new BYTE[nBase64BufferLen + 1 + 64];
        pbBase64Buffer[nBase64BufferLen] = '\0';
        //if (true == Base64::Base64Encode(pBuffer, (int)dwLen, pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
        if (true == Base64_1::Base64Encode(pBuffer, (int)dwLen, pbBase64Buffer, &nBase64BufferLen))
		{
            //strDst64.SetString(pbBase64Buffer, nBase64BufferLen);
            strDst64 = (char*)pbBase64Buffer;
		}
		RELEASEARRAYOBJECTS(pbBase64Buffer);
		RELEASEARRAYOBJECTS(pBuffer);

		oFile.CloseFile();

		//DeleteFile(sFile);
	}

	void CBinaryFileWriter::WriteTheme64(_INT32 lIndex, const std::wstring& sFile)
	{
		GetBase64File(sFile, m_pCommon->m_oRels[lIndex].m_strImageBase64);
	}

	void CBinaryFileWriter::WriteLayoutTheme64(_INT32 lIndexTheme, _INT32 lIndexLayout, const std::wstring& sFile)
	{
		GetBase64File(sFile, m_pCommon->m_oRels[lIndexTheme].m_arLayoutImagesBase64[lIndexLayout]);
	}

	std::wstring CBinaryFileWriter::GetFolderForGenerateImages()
	{
		return m_strMainFolder + _T("\\extract_themes");
	}

	// embedded fonts
	void CBinaryFileWriter::WriteEmbeddedFonts()
	{
		if (NULL == m_pCommon->m_pNativePicker)
			return;

		if (!m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
			return;

		StartMainRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);

		// добавим мега шрифт
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(_T(".)abcdefghijklmnopqrstuvwxyz"));
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckFont(_T("Wingdings 3"), m_pCommon->m_pNativePicker->m_pFontManager);
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckFont(_T("Arial"), m_pCommon->m_pNativePicker->m_pFontManager);

		StartRecord(NSBinPptxRW::NSMainTables::FontsEmbedded);
		m_pCommon->m_pNativePicker->m_oEmbeddedFonts.WriteEmbeddedFonts(this);
		EndRecord();
	}


    bool CBinaryFileWriter::GetSafearray(BYTE **ppArray, size_t& szCount)
    {
        if (NULL == ppArray)
            return false;

        _UINT32 lBinarySize = this->GetPosition();
        if (0 == lBinarySize)
            return false;

        *ppArray = new BYTE [lBinarySize];
        szCount = lBinarySize;

        memcpy(*ppArray, this->GetBuffer(), lBinarySize);
        return true;
    }
	_INT32 CBinaryFileWriter::_WriteString(const WCHAR* sBuffer, _UINT32 lCount)
	{
		_INT32 lSizeMem = 0;
		if (sizeof(wchar_t) == 4)
		{
			_INT32 lSizeMemMax = 4 * lCount + 2;//2 - for null terminator
			CheckBufferSize(lSizeMemMax);
			NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes(sBuffer, lCount, m_pStreamCur, lSizeMem);
		}
		else
		{
			lSizeMem = 2 * lCount;
			CheckBufferSize(lSizeMem);
			memcpy(m_pStreamCur, sBuffer, lSizeMem);
		}
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
		return lSizeMem;
	}
	void CBinaryFileWriter::_WriteStringWithLength(const WCHAR* sBuffer, _UINT32 lCount, bool bByte)
	{
		if (sizeof(wchar_t) == 4)
		{
			_INT32 lSizeMemMax = 4 * lCount + 2;//2 - for null terminator
			CheckBufferSize(UINT32_SIZEOF + lSizeMemMax);
		}
		else
		{
			_INT32 lSizeMem = 2 * lCount;
			CheckBufferSize(UINT32_SIZEOF + lSizeMem);
		}
		//skip size
		m_lPosition += UINT32_SIZEOF;
		m_pStreamCur += UINT32_SIZEOF;
		//write string
		_INT32 lSizeMem = _WriteString(sBuffer, lCount);
		//back to size
		m_lPosition -= lSizeMem;
		m_pStreamCur -= lSizeMem;
		m_lPosition -= UINT32_SIZEOF;
		m_pStreamCur -= UINT32_SIZEOF;
		//write size
		if (bByte)
		{
			//byte
			WriteLONG(lSizeMem);
		}
		else
		{
			//length
			WriteLONG(lSizeMem / 2);
		}
		//skip string
		m_lPosition += lSizeMem;
		m_pStreamCur += lSizeMem;
	}

	CStreamBinaryWriter::CStreamBinaryWriter(size_t bufferSize)
	{
		m_lSize		= bufferSize;
		m_pStreamData	= new BYTE[bufferSize];

		m_lPosition = 0;
		m_pStreamCur = m_pStreamData;

		m_lPositionFlushed = 0;
	}
	void CStreamBinaryWriter::CheckBufferSize(_UINT32 lPlus)
	{
		if ((m_lPosition + lPlus) > m_lSize)
		{
			Flush();
			if ((m_lPosition + lPlus) > m_lSize)
			{
				CBinaryFileWriter::CheckBufferSize(lPlus);
			}
		}
	}
	_UINT32 CStreamBinaryWriter::GetPositionAbsolute()
	{
		return m_lPosition + m_lPositionFlushed;
	}
	void CStreamBinaryWriter::CloseFile()
	{
		Flush();
		CFileBinary::CloseFile();
	}
	void CStreamBinaryWriter::Flush()
	{
		if (m_lPosition > 0)
		{
			CFileBinary::WriteFile(m_pStreamData, m_lPosition);
		}
		m_lPositionFlushed += m_lPosition;
		m_lPosition = 0;
		m_pStreamCur = m_pStreamData;
	}
	void CStreamBinaryWriter::WriteReserved(size_t lCount)
	{
		CBinaryFileWriter::WriteReserved(lCount);
	}

	CXlsbBinaryWriter::CXlsbBinaryWriter(size_t bufferSize) : CStreamBinaryWriter(bufferSize)
	{
	}
	void CXlsbBinaryWriter::XlsbStartRecord(_INT16 lType, _INT32 nLen)
	{
		//Type
		if (lType < 0x80)
		{
			WriteBYTE(lType);
		}
		else
		{
			WriteBYTE((lType & 0x7F) | 0x80);
			WriteBYTE(lType >> 7);
		}
		//Len
		for (int i = 0; i < 4; ++i)
		{
			BYTE nPart = nLen & 0x7F;
			nLen = nLen >> 7;
			if(nLen == 0)
			{
				WriteBYTE(nPart);
				break;
			}
			else
			{
				WriteBYTE(nPart | 0x80);
			}
		}
	}
	void CXlsbBinaryWriter::XlsbEndRecord()
	{
	}


	CRelsGenerator::CRelsGenerator(CImageManager2* pManager) : m_lNextRelsID(1), m_mapImages()
	{
		m_pManager = pManager;
		m_pWriter = new NSStringUtils::CStringBuilder();
	}
	CRelsGenerator::~CRelsGenerator()
	{
		RELEASEOBJECT(m_pWriter);
	}
	void CRelsGenerator::Clear()
	{
		m_pWriter->ClearNoAttack();
		m_lNextRelsID = 1;
		m_mapImages.clear();
		m_mapLinks.clear();
	}

	void CRelsGenerator::StartRels()
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));
	}

	void CRelsGenerator::StartTheme()
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));
	}

	void CRelsGenerator::StartMaster(int nIndexTheme, const _slideMasterInfo& oInfo)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

		int nCountLayouts = (int)oInfo.m_arLayouts.size();
		for (int i = 0; i < nCountLayouts; ++i)
		{
            std::wstring str = L"<Relationship Id=\"rId" +
                    std::to_wstring(m_lNextRelsID++) +
                    L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout" +
                    std::to_wstring(oInfo.m_arLayouts[i] + 1) +
                    L".xml\"/>";
			m_pWriter->WriteString(str);
		}

        std::wstring s = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme" +
                std::to_wstring(nIndexTheme + 1) + L".xml\"/>";
		m_pWriter->WriteString(s);
	}
	void CRelsGenerator::StartThemeNotesMaster(int nIndexTheme)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

        std::wstring s = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"../theme/theme" +
                std::to_wstring(nIndexTheme + 1) + L".xml\"/>";
		m_pWriter->WriteString(s);
	}
	void CRelsGenerator::StartLayout(int nIndexTheme)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

        std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"../slideMasters/slideMaster" +
                std::to_wstring(nIndexTheme + 1) + L".xml\"/>";
		m_pWriter->WriteString(str);
	}
	void CRelsGenerator::StartSlide(int nIndexSlide, int nIndexLayout, int nIndexNotes)
	{
		m_pWriter->WriteString(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));
		m_pWriter->WriteString(_T("<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"));

        std::wstring str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout\" Target=\"../slideLayouts/slideLayout" +
                std::to_wstring(nIndexLayout + 1) + L".xml\"/>";
		m_pWriter->WriteString(str);

		if (nIndexNotes >= 0)
		{
			str = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
			        L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide\" Target=\"../notesSlides/notesSlide" +
			        std::to_wstring(nIndexNotes + 1) + L".xml\"/>";
			m_pWriter->WriteString(str);
		}
	}
	void CRelsGenerator::StartNotes(int nIndexSlide)
	{
		m_pWriter->WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
		m_pWriter->WriteString(L"<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">");

		std::wstring sNum = std::to_wstring(nIndexSlide + 1);
		std::wstring strNoteSlideRels = L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"../slides/slide" + sNum + L".xml\"/>";

		m_pWriter->WriteString(strNoteSlideRels);
		m_pWriter->WriteString(L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"../notesMasters/notesMaster1.xml\"/>");

		m_lNextRelsID = 3;
	}
	void CRelsGenerator::WriteMasters(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring( m_lNextRelsID++ ) + 
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster\" Target=\"slideMasters/slideMaster" + 
                std::to_wstring(i + 1) + L".xml\"/>";
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteThemes(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring( m_lNextRelsID++ ) + 
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme" +
				std::to_wstring(i + 1) + L".xml\"/>";
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteSlides(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring( m_lNextRelsID++ ) + 
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"slides/slide" + 
				std::to_wstring(i + 1) + L".xml\"/>";
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::WriteSlideComments(int nComment)
	{
		std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring( m_lNextRelsID++ ) + 
			L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments\" Target=\"../comments/comment" + 
			std::to_wstring(nComment) + L".xml\"/>";

		m_pWriter->WriteString(strRels);
	}
	void CRelsGenerator::WriteNotesMaster()
	{
		std::wstring strRels0 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster\" Target=\"notesMasters/notesMaster1.xml\"/>";
		m_pWriter->WriteString(strRels0);			
	}
	void CRelsGenerator::WritePresentationComments(int nComment)
	{
		std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring( m_lNextRelsID++ ) +
			L"\" Type=\"http://schemas.onlyoffice.com/comments\" Target=\"comments/comment" +
			std::to_wstring(nComment) + L".xml\"/>";

		m_pWriter->WriteString(strRels);
	}
	void CRelsGenerator::WriteCustoms(int nCount)
	{
		for (int i = 0; i < nCount; ++i)
		{
			std::wstring strRels = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/customXml\" Target=\"../customXml/item" +
				std::to_wstring(i + 1) + L".xml\"/>";
			m_pWriter->WriteString(strRels);
		}
	}
	void CRelsGenerator::EndPresentationRels(bool bIsCommentsAuthors, bool bIsVbaProject, bool bIsJsaProject)
	{
        std::wstring strRels1 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/presProps\" Target=\"presProps.xml\" />";
        std::wstring strRels2 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableStyles\" Target=\"tableStyles.xml\" />";
        std::wstring strRels3 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/viewProps\" Target=\"viewProps.xml\" />";

		m_pWriter->WriteString(strRels1);
		m_pWriter->WriteString(strRels2);
		m_pWriter->WriteString(strRels3);

		if (bIsCommentsAuthors)
		{
            std::wstring strRels4 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                    L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors\" Target=\"commentAuthors.xml\"/>";
			m_pWriter->WriteString(strRels4);
		}
		if (bIsVbaProject)
		{
            std::wstring strRels4 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
                    L"\" Type=\"http://schemas.microsoft.com/office/2006/relationships/vbaProject\" Target=\"vbaProject.bin\"/>";
			m_pWriter->WriteString(strRels4);
		}
		if (bIsJsaProject)
		{
			std::wstring strRels5 = L"<Relationship Id=\"rId" + std::to_wstring(m_lNextRelsID++) +
					L"\" Type=\"" + OOX::FileTypes::JsaProject.RelationType() + L"\" Target=\"" + OOX::FileTypes::JsaProject.DefaultFileName().GetPath() + L"\"/>";
			m_pWriter->WriteString(strRels5);
		}
	}
	void CRelsGenerator::CloseRels()
	{
		m_pWriter->WriteString(_T("</Relationships>"));
	}
	void CRelsGenerator::AddRels(const std::wstring& strRels)
	{
		m_pWriter->WriteString(strRels);			
	}
	void CRelsGenerator::SaveRels(const std::wstring& strFile)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFile(strFile);
		std::wstring strMem = m_pWriter->GetData();
		oFile.WriteStringUTF8(strMem);
		oFile.CloseFile();
	}
	_relsGeneratorInfo CRelsGenerator::WriteMedia(const std::wstring& strImage, int type)
	{
		_imageManager2Info oImageManagerInfo = m_pManager->GenerateMedia(strImage);
		
		std::wstring strImageRelsPath; 
		
		if (m_pManager->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	strImageRelsPath = L"media/";
		else														strImageRelsPath = L"../media/";

		_relsGeneratorInfo oRelsGeneratorInfo;
		
		if (!oImageManagerInfo.sFilepathImage.empty())
		{
			strImageRelsPath += OOX::CPath(oImageManagerInfo.sFilepathImage).GetFilename();

			std::map<std::wstring, _relsGeneratorInfo>::iterator pPair = m_mapImages.find(strImageRelsPath);

			if (m_mapImages.end() != pPair)
			{
				return pPair->second;				
			}
			
			oRelsGeneratorInfo.nImageRId		= m_lNextRelsID++;
			oRelsGeneratorInfo.sFilepathImage	= oImageManagerInfo.sFilepathImage;
			
			std::wstring strRid = L"rId" + std::to_wstring(oRelsGeneratorInfo.nImageRId);

			if (type == 0)
			{
				m_pWriter->WriteString( L"<Relationship Id=\"" + strRid + 
					L"\" Type=\"http://schemas.microsoft.com/office/2007/relationships/media\" Target=\"" + strImageRelsPath +
					L"\"/>");
			}
			else if (type == 1)
			{
				m_pWriter->WriteString( L"<Relationship Id=\"" + strRid + 
					L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/audio\" Target=\"" + strImageRelsPath +
					L"\"/>");
			}		
		}

		m_mapImages.insert(std::pair<std::wstring, _relsGeneratorInfo>(strImageRelsPath, oRelsGeneratorInfo));
		return oRelsGeneratorInfo;
	}

	_relsGeneratorInfo CRelsGenerator::WriteImage(const std::wstring& strImage, smart_ptr<OOX::File> & additionalFile, const std::wstring& oleData, std::wstring strBase64Image = _T(""))
	{
		_imageManager2Info oImageManagerInfo = m_pManager->GenerateImage(strImage, additionalFile, oleData, strBase64Image);
		
		std::wstring strImageRelsPath; 
		
		if (m_pManager->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	strImageRelsPath = L"media/";
		else														strImageRelsPath = L"../media/";

		_relsGeneratorInfo oRelsGeneratorInfo;
		
		if (!oImageManagerInfo.sFilepathImage.empty())
		{
			strImageRelsPath += OOX::CPath(oImageManagerInfo.sFilepathImage).GetFilename();

			std::map<std::wstring, _relsGeneratorInfo>::iterator pPair = m_mapImages.find(strImageRelsPath);

			if (m_mapImages.end() != pPair)
			{
				return pPair->second;				
			}
			
			oRelsGeneratorInfo.nImageRId		= m_lNextRelsID++;
			oRelsGeneratorInfo.sFilepathImage	= oImageManagerInfo.sFilepathImage;
			
			std::wstring strRid = L"rId" + std::to_wstring(oRelsGeneratorInfo.nImageRId);

			m_pWriter->WriteString( L"<Relationship Id=\"" + strRid + 
				L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"" + strImageRelsPath +
				L"\"/>");
		}

		if(additionalFile.is<OOX::OleObject>())
		{
			smart_ptr<OOX::OleObject> oleFile = additionalFile.smart_dynamic_cast<OOX::OleObject>();
			
			std::wstring strOleRelsPath;
			
			oRelsGeneratorInfo.nOleRId = m_lNextRelsID++;
			oRelsGeneratorInfo.sFilepathOle	= oleFile->filename().GetPath();

			if	(m_pManager->m_nDocumentType != XMLWRITER_DOC_TYPE_XLSX)
			{
				std::wstring strRid = L"rId" + std::to_wstring(oRelsGeneratorInfo.nOleRId);

				if (m_pManager->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	strOleRelsPath = L"embeddings/";		
				else														strOleRelsPath = L"../embeddings/";
				
				strOleRelsPath += oleFile->filename().GetFilename();

				if (oleFile->isMsPackage())
				{
					m_pWriter->WriteString( L"<Relationship Id=\"" + strRid
						+ L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/package\" Target=\"" +
						strOleRelsPath + L"\"/>");
				}else{
					m_pWriter->WriteString( L"<Relationship Id=\"" + strRid
						+ L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject\" Target=\"" +
						strOleRelsPath + L"\"/>");
				}
			}
		}
		else if(additionalFile.is<OOX::Media>())
		{
			smart_ptr<OOX::Media> mediaFile = additionalFile.smart_dynamic_cast<OOX::Media>();
			
			std::wstring strMediaRelsPath;
			
			oRelsGeneratorInfo.nMediaRId = m_lNextRelsID++;
			oRelsGeneratorInfo.sFilepathMedia	= mediaFile->filename().GetPath();

			if	(m_pManager->m_nDocumentType != XMLWRITER_DOC_TYPE_XLSX)
			{
				std::wstring strRid = L"rId" + std::to_wstring(oRelsGeneratorInfo.nMediaRId);

				if (mediaFile->IsExternal())
				{
					strMediaRelsPath = mediaFile->filename().GetFilename();
				}
				else
				{
					if (m_pManager->m_nDocumentType == XMLWRITER_DOC_TYPE_DOCX)	strMediaRelsPath = L"media/";		
					else														strMediaRelsPath = L"../media/";
					
					strMediaRelsPath += mediaFile->filename().GetFilename();				

					m_pWriter->WriteString( L"<Relationship Id=\"" + strRid
						+ L"\" Type=\"http://schemas.microsoft.com/office/2007/relationships/media\" Target=\"" +
						strMediaRelsPath + L"\"" + (mediaFile->IsExternal() ? L" TargetMode=\"External\"" : L"") + L"/>");
				}
			}
		}
		m_mapImages.insert(std::pair<std::wstring, _relsGeneratorInfo>(strImageRelsPath, oRelsGeneratorInfo));
		return oRelsGeneratorInfo;
	}

	unsigned int CRelsGenerator::WriteRels(const std::wstring& bsType, const std::wstring& bsTarget, const std::wstring& bsTargetMode)
	{
		std::wstring strRid = L"rId" + std::to_wstring(m_lNextRelsID++);

		std::wstring strType = _T("Type=\"") + bsType + _T("\" ");
		std::wstring strTarget = _T("Target=\"") + bsTarget + _T("\" ");
		std::wstring strTargetMode = bsTargetMode.empty() ? _T("") : (_T("TargetMode=\"") + bsTargetMode + _T("\""));

		std::wstring strRels = _T("<Relationship Id=\"") + strRid + _T("\" ") + strType + strTarget + strTargetMode + _T("/>");
		m_pWriter->WriteString(strRels);
		return m_lNextRelsID - 1;
	}

	unsigned int CRelsGenerator::WriteHyperlink(const std::wstring& strLink, const bool& bIsActionInit)
	{
		std::map<std::wstring, unsigned int>::iterator pPair = m_mapLinks.find(strLink);

		if (m_mapLinks.end() != pPair)
		{
			return pPair->second;				
		}

		m_mapLinks.insert(std::pair<std::wstring, unsigned int>(strLink, m_lNextRelsID));
		std::wstring strRid = L"rId" + std::to_wstring(m_lNextRelsID++);

		std::wstring sLink = XmlUtils::EncodeXmlString(strLink);

		bool bIsSlide = (0 == sLink.find(_T("slide")));
		if (!bIsActionInit)
			bIsSlide = false;

		std::wstring strRels;

		if (!bIsSlide)
		{
			strRels = L"<Relationship Id=\"" + strRid
				+ L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"" + sLink
				+ L"\" TargetMode=\"External\"/>";
		}
		else
		{
			strRels = L"<Relationship Id=\"" + strRid
				+ L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide\" Target=\"" + sLink 
				+ L"\"/>"; 
		}

		m_pWriter->WriteString(strRels);

		return m_lNextRelsID - 1;
	}		

	CBinaryFileReader::CBinaryFileReader()
	{
		m_pMainDocument		= NULL;
		m_lNextId			= 0;
		m_nDocumentType		= XMLWRITER_DOC_TYPE_PPTX;

		m_pRels				= new CRelsGenerator();
		m_nCurrentRelsStack = -1;
	}
	CBinaryFileReader::~CBinaryFileReader()
	{
		RELEASEOBJECT(m_pRels);

		size_t nCountStackRels = m_stackRels.size();
		for (size_t i = 0; i < nCountStackRels; ++i)
		{
			CRelsGenerator* pCur = m_stackRels[i];
			RELEASEOBJECT(pCur);
		}
		m_stackRels.clear();
	}

	void CBinaryFileReader::SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc)
	{
		m_pMainDocument = pMainDoc;
	}

	void CBinaryFileReader::Init(BYTE* pData, _INT32 lStart, _INT32 lSize)
	{
		m_pData = pData;
		m_lSize = lSize + lStart;
		m_lPos = lStart;
		m_pDataCur = m_pData + m_lPos;
	}
	_INT32 CBinaryFileReader::GenerateNextId()
	{
		++m_lNextId;
		return m_lNextId;
	}

	int CBinaryFileReader::Seek(LONG _pos)
	{
		if (_pos > m_lSize)
		{
			_pos = m_lSize;
		}
		if (_pos < 0 )
		{
			_pos = 0;
			throw;
		}
		m_lPos = _pos;
		m_pDataCur = m_pData + m_lPos;
		return 0;
	}
	int CBinaryFileReader::Skip(LONG _skip)
	{
		if (_skip < 0)
			return 1;
		return Seek(m_lPos + _skip);
	}
	bool CBinaryFileReader::Peek(LONG nSizeToRead = 0)
	{
		return !(m_lPos + nSizeToRead > m_lSize);
	}

	// 1 bytes
	bool CBinaryFileReader::GetUCharWithResult(BYTE *value_)
	{
		if (!value_ || m_lPos >= m_lSize)
		{
			return false;
		}

		*value_ = *m_pDataCur;
		++m_lPos;
		++m_pDataCur;
		return true;
	}
	BYTE CBinaryFileReader::GetUChar()
	{
		if (m_lPos >= m_lSize || m_lPos < 0)
		{
			throw;
		}

		BYTE res = *m_pDataCur;
		++m_lPos;
		++m_pDataCur;
		return res;
	}
	signed char CBinaryFileReader::GetChar()
	{
		if (m_lPos >= m_lSize || m_lPos <0)
		{
			throw;
		}

		BYTE res = *m_pDataCur;
		if (res > 127)
			res -= 256;
		++m_lPos;
		++m_pDataCur;
		return res;
	}

	// 1 bytes
	BYTE CBinaryFileReader::GetUChar_TypeNode()
	{
		if (m_lPos >= m_lSize)
			return NSBinPptxRW::g_nodeAttributeEnd;

		BYTE res = *m_pDataCur;
		++m_lPos;
		++m_pDataCur;
		return res;
	}

	bool CBinaryFileReader::GetBool()
	{
		return (GetUChar() != 0) ? true : false;
	}

	// 2 byte
	_UINT16 CBinaryFileReader::GetUShort()
	{
		if (m_lPos + 1 >= m_lSize)
		{
			throw;
		}
#if defined(_IOS) || defined(__ANDROID__)
        _UINT16 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_UINT16));
#else
        _UINT16 res = *((_UINT16*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPos += 2;
		m_pDataCur += 2;
		return res;		
	}
	_INT16 CBinaryFileReader::GetShort()
	{
		if (m_lPos + 1 >= m_lSize)
		{
			throw;
		}

#if defined(_IOS) || defined(__ANDROID__)
		_INT16 res = 0;
		memcpy(&res, m_pDataCur, sizeof(_INT16));
#else
		_INT16 res = *((_INT16*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
		m_lPos += 2;
		m_pDataCur += 2;
		return res;
	}

	// 4 byte
	_UINT32 CBinaryFileReader::GetULong()
	{
		if (m_lPos + 3 >= m_lSize)
		{
			throw;
		}

#if defined(_IOS) || defined(__ANDROID__)
        _UINT32 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_UINT32));
#else
        _UINT32 res = *((_UINT32*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
        m_lPos += 4;
		m_pDataCur += 4;
        return res;
	}
	_INT64 CBinaryFileReader::GetLong64()
	{
		if (m_lPos + 7 >= m_lSize)
		{
			throw;
		}

#if defined(_IOS) || defined(__ANDROID__)
        _INT64 res = 0;
        memcpy(&res, m_pDataCur, sizeof(_INT64));
#else
        _INT64 res = *((_INT64*)m_pDataCur);   // EXC_ARM_DA_ALIGN on ios
#endif
        m_lPos += 8;
		m_pDataCur += 8;
        return res;			
	}
	_INT32 CBinaryFileReader::GetRecordSize()
	{
		_INT32 sz = (_INT32)GetULong();
		if (m_lPos + sz > m_lSize)
		{
			//todooo - переделать
			throw;
		}
		return sz;
	}
	_INT32 CBinaryFileReader::GetLong()
	{
		return (_INT32)GetULong();			
	}
	double CBinaryFileReader::GetDouble()
	{
		return 1.0 * GetLong() / 100000.;
	}
// 8 byte
	double CBinaryFileReader::GetDoubleReal()
	{
        if (m_lPos + (int)DOUBLE_SIZEOF > m_lSize)
		{
			throw;
		}

#if defined(_IOS) || defined(__ANDROID__)
        double res = 0.0;
        memcpy(&res, m_pDataCur, sizeof(double));
#else
        double res = *((double*)m_pDataCur);    // EXC_ARM_DA_ALIGN on ios
#endif        
        m_lPos += DOUBLE_SIZEOF;
        m_pDataCur += DOUBLE_SIZEOF;
        return res;
	}

	//String
	std::wstring CBinaryFileReader::GetString(_INT32 len, bool bDeleteZero)
	{
        len *= 2;
		return GetString3(len, bDeleteZero);
	}
	std::string CBinaryFileReader::GetString1(_INT32 len)
	{
        if (len < 1 )
            return "";
        if (m_lPos + len > m_lSize)
		{
			throw;
		}

		std::string res((CHAR*)m_pDataCur, len);
		m_lPos += len;
		m_pDataCur += len;
		return res;
	}
	std::wstring CBinaryFileReader::GetString2(bool bDeleteZero)
    {
        _INT32 len = GetLong();
		return GetString(len, bDeleteZero);
	}
    std::wstring CBinaryFileReader::GetString3(_INT32 len, bool bDeleteZero)//len in byte for utf16
	{
        if (len < 1 )
			return L""; 
        
		if (m_lPos + len > m_lSize)
		{
			throw;
		}

        int lSize = len >> 1; //string in char

		if (bDeleteZero)
		{
			while (lSize > 0)
			{
				if (((unsigned short*)m_pDataCur)[lSize - 1] == 0)
					lSize--;
				else
					break;
			}
		}

		if (sizeof(wchar_t) == 4)
        {

            std::wstring val = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)m_pDataCur, lSize);
            std::wstring res(val.c_str(), val.length());

            m_lPos += len;
            m_pDataCur += len;

            return res;
        }
        else
        {
            std::wstring res((WCHAR*)m_pDataCur, lSize);

            m_lPos += len;
            m_pDataCur += len;

            return res;
        }
	}
	std::wstring CBinaryFileReader::GetString4(_INT32 len)//len in byte for utf16
	{
		if (len < 1)
			return _T("");
		if (m_lPos + len > m_lSize)
		{
			throw;
		}

		_UINT32 lSize = len >> 1; //string in char

        std::wstring res = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)m_pDataCur, lSize);
        m_lPos += len;
        m_pDataCur += len;

        return res;
	}

    bool CBinaryFileReader::GetArray(BYTE *pBuffer, _INT32 len)
	{
		if (0 == len)
            return false;
		if (m_lPos + len > m_lSize)
            return false;

        //*pBuffer = new BYTE [len];

        memcpy(pBuffer, m_pDataCur, len);

		m_lPos += len;
		m_pDataCur += len;

        return true;
	}
    /*LPSAFEARRAY CBinaryFileReader::GetArray(_INT32 len)
    {
        if (0 == len)
            return NULL;
        if (m_lPos + len > m_lSize)
            return NULL;

        SAFEARRAY* pArray = SafeArrayCreateVector(VT_UI1, (_UINT32)len);

        BYTE* pDataD = (BYTE*)pArray->pvData;
        memcpy(pDataD, m_pDataCur, len);

        m_lPos += len;
        m_pDataCur += len;

        return pArray;
    }
    */

	std::string CBinaryFileReader::GetString2A()
	{
		_INT32 len = GetULong();
		return GetString1(len);
	}

	void CBinaryFileReader::SkipRecord()
	{
		_INT32 _len = GetRecordSize();
		Skip(_len);
	}

	LONG CBinaryFileReader::GetPos()
    {
		return m_lPos;
    }

	LONG CBinaryFileReader::GetSize()
	{
		return m_lSize;
	}

	BYTE* CBinaryFileReader::GetData()
	{
		return m_pData;
	}
	BYTE* CBinaryFileReader::GetPointer(int nSize = 0)
	{
        if (nSize < 0) return 0;

		if (m_lPos + nSize > m_lSize)
		{
			throw;
		}

		BYTE* res = (BYTE*)m_pDataCur;
		m_lPos += nSize;
		m_pDataCur += nSize;
		return res;
	}
	_UINT16 CBinaryFileReader::XlsbReadRecordType()
	{
		_UINT16 nValue = GetUChar();
		if(0 != (nValue & 0x80))
                {
			BYTE nPart = GetUChar();
                        nValue = (nValue & 0x7F) | ((nPart & 0x7F) << 7);
		}
		return nValue;
	}
	void CBinaryFileReader::XlsbSkipRecord()
	{
		Skip(XlsbReadRecordLength());
	}
	_UINT32 CBinaryFileReader::XlsbReadRecordLength()
	{
                _UINT32 nValue = 0;
		for (int i = 0; i < 4; ++i)
		{
			BYTE nPart = GetUChar();
			nValue |= (nPart & 0x7F) << (7 * i);
			if(0 == (nPart & 0x80))
			{
				break;
			}
		}
		return nValue;
	}

	void CBinaryFileReader::SetDstContentRels()
	{
		++m_nCurrentRelsStack;

		//чистить текущий m_pRels хорошо при последовательной записи автофигур в word.
		//плохо в случае записи перезентаций, с момента перехода на единственный обьект m_pReader.
		//пример: презетации записали несколько Rels, записываем chart, вызывается SetDstContentRels и трутся Rels презентаций
		//if (0 == m_pReader->m_nCurrentRelsStack)
		//{
		//	m_pReader->m_pRels->Clear();
		//	m_pReader->m_pRels->StartRels();
		//}
		//else
		{
			m_stackRels.push_back(m_pRels);

			m_pRels = new NSBinPptxRW::CRelsGenerator(m_pRels->m_pManager);
			m_pRels->StartRels();
		}
	}
	void CBinaryFileReader::SaveDstContentRels(const std::wstring& bsRelsPath)
	{
		m_pRels->CloseRels();
		m_pRels->SaveRels(bsRelsPath);

		--m_nCurrentRelsStack;
		if (-1 > m_nCurrentRelsStack)
			m_nCurrentRelsStack = -1;

		//if (-1 != m_pReader->m_nCurrentRelsStack)
		{
			int nIndex = (int)m_stackRels.size() - 1;

			if (0 <= nIndex)
			{
				NSBinPptxRW::CRelsGenerator* pCur = m_pRels;
				m_pRels = m_stackRels[nIndex];
				m_stackRels.erase(m_stackRels.begin() + nIndex);
				RELEASEOBJECT(pCur);
			}
		}
	}
}
