/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "BinaryReaderV.h"

#include "../../../Common/Base64.h"
#include "../../../Common/ATLDefine.h"
#include "../../../Common/OfficeFileErrorDescription.h"

#include "../../../DesktopEditor/common/Path.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"

#include "../../PPTXFormat/Theme.h"
#include "../../PPTXFormat/Logic/HeadingVariant.h"
#include "../../PPTXFormat/Logic/Shape.h"

#include "../../DocxFormat/Media/VbaProject.h"
#include "../../DocxFormat/Media/JsaProject.h"
#include "../../DocxFormat/App.h"
#include "../../DocxFormat/Core.h"
#include "../../DocxFormat/CustomXml.h"

#include "../../VsdxFormat/Vsdx.h"
#include "../../VsdxFormat/VisioDocument.h"

namespace BinVsdxRW 
{
	SaveParams::SaveParams(bool bMacro) : bMacroEnabled(bMacro)
	{
	}
	BinaryFileReader::BinaryFileReader()
	{
	}
	int BinaryFileReader::ReadFile(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, bool &bMacro)
	{
		bool bResultOk = false;
	
		NSFile::CFileBinary oFile;
	
		if (false == oFile.OpenFile(sSrcFileName)) return AVS_FILEUTILS_ERROR_CONVERT;

		NSBinPptxRW::CBinaryFileReader& oBufferedStream = *pOfficeDrawingConverter->m_pReader;

		DWORD nBase64DataSize = 0;
		BYTE* pBase64Data = new BYTE[oFile.GetFileSize()];
		oFile.ReadFile(pBase64Data, oFile.GetFileSize(), nBase64DataSize);
		oFile.CloseFile();

		//проверяем формат
		bool bValidFormat = false;
		std::wstring sSignature(g_sFormatSignature);
		size_t nSigLength = sSignature.length();
		if (nBase64DataSize > nSigLength)
		{
			std::string sCurSig((char*)pBase64Data, nSigLength);
			std::wstring wsCurSig(sCurSig.begin(), sCurSig.end());

			if (sSignature == wsCurSig)
			{
				bValidFormat = true;
			}
		}
		if (bValidFormat)
		{
			//Читаем из файла версию и длину base64
			int nIndex = (int)nSigLength;
			int nType = 0;
			std::string version = "";
			std::string dst_len = "";
		
			while (nIndex < nBase64DataSize)
			{
				nIndex++;
				BYTE _c = pBase64Data[nIndex];
				if (_c == ';')
				{

					if (0 == nType)
					{
						nType = 1;
						continue;
					}
					else
					{
						nIndex++;
						break;
					}
				}
				if (0 == nType)
					version += _c;
				else
					dst_len += _c;
			}
			int nVersion = g_nFormatVersion;
			if (!version.empty())
			{
				version = version.substr(1);
				g_nCurFormatVersion = nVersion = std::stoi(version.c_str());
			}
			bool bIsNoBase64 = nVersion == g_nFormatVersionNoBase64;

			int nDataSize = 0;
			BYTE* pData = NULL;
			if (!bIsNoBase64)
			{
				nDataSize = atoi(dst_len.c_str());
				pData = new BYTE[nDataSize];
				if (Base64::Base64Decode((const char*)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
				{
					oBufferedStream.Init(pData, 0, nDataSize);
				}
				else
				{
					RELEASEARRAYOBJECTS(pData);
				}
			}
			else
			{
				nDataSize = nBase64DataSize;
				pData = pBase64Data;
				oBufferedStream.Init(pData, 0, nDataSize);
				oBufferedStream.Seek(nIndex);
			}

			if (NULL != pData)
			{
				bResultOk = true;

				OOX::Draw::CVsdx oVsdx;
				SaveParams oSaveParams(bMacro);

				try
				{
					ReadContent(oVsdx, oBufferedStream, OOX::CPath(sSrcFileName).GetDirectory(), sDstPath, oSaveParams);
				}
				catch (...)
				{
					bResultOk = false;
				}
				OOX::CContentTypes oContentTypes;
				oVsdx.Write(sDstPath, oContentTypes);

				bMacro = oSaveParams.bMacroEnabled;
			}
			if (!bIsNoBase64)
			{
				RELEASEARRAYOBJECTS(pData);
			}
		}
		RELEASEARRAYOBJECTS(pBase64Data);

		if (bResultOk)	return 0;
		else			return AVS_FILEUTILS_ERROR_CONVERT;
	}
	int BinaryFileReader::ReadContent(OOX::Draw::CVsdx& oVsdx, NSBinPptxRW::CBinaryFileReader& oBufferedStream, const std::wstring& sFileInDir, const std::wstring& sOutDir, SaveParams& oSaveParams)
	{
		long res = oBufferedStream.Peek(1) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
		
		if (c_oSerConstants::ReadOk != res)
			return res;
	
		OOX::CPath pathMedia = sOutDir + FILE_SEPARATOR_STR + L"visio" + FILE_SEPARATOR_STR + L"media";
		OOX::CPath pathEmbeddings = sOutDir + FILE_SEPARATOR_STR + L"visio" + FILE_SEPARATOR_STR + L"embeddings";

		oBufferedStream.m_nDocumentType = XMLWRITER_DOC_TYPE_VSDX;
		oBufferedStream.m_strFolder = sFileInDir;
		oBufferedStream.m_pRels->m_pManager->m_nDocumentType = XMLWRITER_DOC_TYPE_VSDX;
		oBufferedStream.m_pRels->m_pManager->SetDstMedia(pathMedia.GetPath());
		oBufferedStream.m_pRels->m_pManager->SetDstEmbed(pathEmbeddings.GetPath());

		std::vector<BYTE> aTypes;
		std::vector<long> aOffBits;	

		BYTE mtLen = oBufferedStream.GetUChar();

		for (int i = 0; i < mtLen; ++i)
		{
			//mtItem
			res = oBufferedStream.Peek(5) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if (c_oSerConstants::ReadOk != res)
				return res;

			BYTE mtiType = 0;
			if (false == oBufferedStream.GetUCharWithResult(&mtiType))
				break;

			long mtiOffBits = oBufferedStream.GetLong();

			aTypes.push_back(mtiType);
			aOffBits.push_back(mtiOffBits);
		}
		//sOutDir
		for (size_t i = 0, length = aTypes.size(); i < length; ++i)
		{
			BYTE mtiType = aTypes[i];
			long mtiOffBits = aOffBits[i];

			oBufferedStream.Seek(mtiOffBits);
			switch(mtiType)
			{
				case c_oSerTableTypes::App:
				{
					OOX::CApp* pApp = new OOX::CApp(NULL);

					smart_ptr<OOX::File> oCurFile(pApp);					
					oVsdx.m_pApp = oCurFile.smart_dynamic_cast<OOX::CApp>();
					
					pApp->fromPPTY(&oBufferedStream);
					pApp->SetRequiredDefaults();

					oVsdx.Add(oCurFile);

				}break;			
				case c_oSerTableTypes::Core:
				{
					OOX::CCore* pCore = new OOX::CCore(NULL);

					smart_ptr<OOX::File> oCurFile(pCore);
					oVsdx.m_pCore = oCurFile.smart_dynamic_cast<OOX::CCore>();					
					
					pCore->fromPPTY(&oBufferedStream);
					pCore->SetRequiredDefaults();

					oVsdx.Add(oCurFile);
				}break;			
				case c_oSerTableTypes::CustomProperties:
				{
					PPTX::CustomProperties* oCustomProperties = new PPTX::CustomProperties(NULL);
					oCustomProperties->fromPPTY(&oBufferedStream);
					smart_ptr<OOX::File> oCurFile(oCustomProperties);
					oVsdx.Add(oCurFile);
				}break;			
				case c_oSerTableTypes::Document:
				{
					OOX::Draw::CDocumentFile* pDocument = new OOX::Draw::CDocumentFile(&oVsdx);

					smart_ptr<OOX::File> oCurFile(pDocument);
					oVsdx.m_pDocument = oCurFile.smart_dynamic_cast<OOX::Draw::CDocumentFile>();

					pDocument->fromPPTY(&oBufferedStream);
					oVsdx.Add(oCurFile);					
				}break;

			}	
			if (c_oSerConstants::ReadOk != res)
				return res;
		}

		return res;
	}


}

