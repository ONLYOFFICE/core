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
#include "OnlineOfficeBinToPdf.h"

#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/graphics/MetafileToRenderer.h"

namespace NSOnlineOfficeBinToPdf
{
	class CMetafileToRenderterPDF : public IMetafileToRenderter
	{
	public:
		CMetafileToRenderterPDF(IRenderer* pRenderer) : IMetafileToRenderter(pRenderer)
		{
		}

	public:
		virtual void SetLinearGradiant(const double& x0, const double& y0, const double& x1, const double& y1)
		{
			((CPdfFile*)m_pRenderer)->SetLinearGradient(x0, y0, x1, y1);
		}

		virtual void SetRadialGradiant(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
		{
			((CPdfFile*)m_pRenderer)->SetRadialGradient(dX0, dY0, dR0, dX1, dY1, dR1);
		}
	};

	static bool ConvertBufferToPdf(CPdfFile* pPdf, BYTE* pBuffer, LONG lBufferLen, CConvertFromBinParams* pParams)
	{
		CMetafileToRenderterPDF oCorrector(pPdf);
		oCorrector.SetTempDirectory(pPdf->GetTempDirectory());
		if (pParams)
		{
			oCorrector.SetMediaDirectory(pParams->m_sMediaDirectory);
			oCorrector.SetInternalMediaDirectory(pParams->m_sInternalMediaDirectory);
			oCorrector.SetThemesDirectory(pParams->m_sThemesDirectory);

			if (pParams->m_bIsUsePicker)
				oCorrector.InitPicker(pPdf->GetFonts());
		}
		NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(pBuffer, lBufferLen, &oCorrector);

		return true;
	}
	bool ConvertBinToPdf(CPdfFile* pPdf, const std::wstring& wsSrcFile, const std::wstring& wsDstFile, bool bBinary, CConvertFromBinParams* pParams)
	{
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(wsSrcFile))
			return false;

		DWORD dwFileSize = oFile.GetFileSize();
		BYTE* pFileContent = new BYTE[dwFileSize];
		if (!pFileContent)
		{
			oFile.CloseFile();
			return false;
		}

		DWORD dwReaded;
		oFile.ReadFile(pFileContent, dwFileSize, dwReaded);
		oFile.CloseFile();

		bool bIsNeedDestroy = (NULL == pParams) ? true : false;
		if (bIsNeedDestroy)
			pParams = new CConvertFromBinParams();

		if (pParams->m_sMediaDirectory.empty())
			pParams->m_sMediaDirectory = NSFile::GetDirectoryName(wsSrcFile);

		if (bBinary)
		{
			ConvertBufferToPdf(pPdf, pFileContent, dwFileSize, pParams);
		}
		else
		{
			int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
			BYTE* pBuffer = new BYTE[nBufferLen];
			if (!pBuffer)
			{
				RELEASEARRAYOBJECTS(pFileContent);

				if (bIsNeedDestroy)
					RELEASEOBJECT(pParams);
				return false;
			}

			if (NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen))
			{
				ConvertBufferToPdf(pPdf, pBuffer, nBufferLen, pParams);
			}
			else
			{
				RELEASEARRAYOBJECTS(pBuffer);
				RELEASEARRAYOBJECTS(pFileContent);

				if (bIsNeedDestroy)
					RELEASEOBJECT(pParams);
				return false;
			}

			RELEASEARRAYOBJECTS(pBuffer);
		}
		RELEASEARRAYOBJECTS(pFileContent);

		if (bIsNeedDestroy)
			RELEASEOBJECT(pParams);

		if (!wsDstFile.empty())
		{
			if (0 != pPdf->SaveToFile(wsDstFile))
				return false;
		}

		return true;
	}

	enum class AddCommandType
	{
		EditPage   = 0,
		AddPage    = 1,
		RemovePage = 2,
		WidgetInfo = 3,
		MovePage   = 4,
		MergePages = 5,
		Undefined  = 255
	};

	bool AddBinToPdf(CPdfFile* pPdf, BYTE* pBuffer, unsigned int nBufferLen, CConvertFromBinParams* pParams)
	{
		CMetafileToRenderterPDF oCorrector(pPdf);
		oCorrector.SetTempDirectory(pPdf->GetTempDirectory());
		if (pParams)
		{
			oCorrector.SetMediaDirectory(pParams->m_sMediaDirectory);
			oCorrector.SetInternalMediaDirectory(pParams->m_sInternalMediaDirectory);
			oCorrector.SetThemesDirectory(pParams->m_sThemesDirectory);

			if (pParams->m_bIsUsePicker)
				oCorrector.InitPicker(pPdf->GetFonts());
		}

		NSOnlineOfficeBinToPdf::CBufferReader oReader(pBuffer, (int)nBufferLen);

		while (oReader.Check())
		{
			int nLen = oReader.ReadInt();
			AddCommandType CommandType = (AddCommandType)oReader.ReadByte();
			int nPageNum = 0;
			if (CommandType != AddCommandType::WidgetInfo && CommandType != AddCommandType::MergePages)
				nPageNum = oReader.ReadInt();

			if (nPageNum < 0)
				return false;

			switch (CommandType)
			{
			case AddCommandType::EditPage:
			{
				if (!pPdf->EditPage(nPageNum))
				{
					oReader.Skip(nLen - 9);
					break;
				}

				NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(oReader.GetCurrentBuffer(), (LONG)(nLen - 9) , &oCorrector);
				oReader.Skip(nLen - 9);
				break;
			}
			case AddCommandType::AddPage:
			{
				if (!pPdf->AddPage(nPageNum))
				{
					oReader.Skip(nLen - 9);
					break;
				}

				NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(oReader.GetCurrentBuffer(), (LONG)(nLen - 9) , &oCorrector);
				oReader.Skip(nLen - 9);
				break;
			}
			case AddCommandType::MovePage:
			{
				int nPos = oReader.ReadInt();
				pPdf->MovePage(nPageNum, nPos);
				break;
			}
			case AddCommandType::RemovePage:
			{
				pPdf->DeletePage(nPageNum);
				break;
			}
			case AddCommandType::MergePages:
			{
				std::wstring wsPath = NSFile::CFileBinary::CreateTempFileWithUniqueName(pPdf->GetTempDirectory(), L"PDF");
				int nLength = oReader.ReadInt();
				BYTE* pFile = oReader.GetCurrentBuffer();
				oReader.Skip(nLength);
				if (!wsPath.empty())
				{
					NSFile::CFileBinary oFile;
					if (oFile.CreateFileW(wsPath))
						oFile.WriteFile(pFile, nLength);
					oFile.CloseFile();
				}

				int nMaxID = oReader.ReadInt();
				std::wstring wsPrefix = oReader.ReadString();
				pPdf->MergePages(wsPath, nMaxID, wsPrefix);
				break;
			}
			case AddCommandType::WidgetInfo:
			{
				NSOnlineOfficeBinToPdf::ConvertBufferToRenderer(oReader.GetCurrentBuffer(), (LONG)(nLen - 5) , &oCorrector);
				oReader.Skip(nLen - 5);
				break;
			}
			default:
				return false;
			}
		}

		return true;
	}
}
