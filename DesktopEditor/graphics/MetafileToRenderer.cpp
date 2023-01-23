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

#include "./MetafileToRendererCheck.h"
#include "agg_math.h"
#include "../fontengine/FontManager.h"
#include "../raster/BgraFrame.h"
#include "../common/StringExt.h"
#include "./FormField.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include "../common/StringExt.h"
#endif

// этот класс нужно переписать. должно работать как и в js
// а не просто на каждом символе переключаться, если нужно
class CMetafileFontPicker
{
private:
	NSFonts::IApplicationFonts* m_pApplication;
	NSFonts::IFontManager* m_pManager;
	IRenderer* m_pRenderer;

public:
	CMetafileFontPicker(std::wstring sFolder)
	{
		m_pApplication = NSFonts::NSApplication::Create();
		m_pApplication->InitializeFromFolder(sFolder);
		m_pManager = m_pApplication->GenerateFontManager();

		NSFonts::IFontsCache* pCache = NSFonts::NSFontCache::Create();
		pCache->SetStreams(m_pApplication->GetStreams());
		m_pManager->SetOwnerCache(pCache);

		m_pRenderer = NULL;
	}
	CMetafileFontPicker(NSFonts::IApplicationFonts* pFonts)
	{
		m_pApplication = pFonts;
		m_pApplication->AddRef();
		m_pManager = m_pApplication->GenerateFontManager();

		NSFonts::IFontsCache* pCache = NSFonts::NSFontCache::Create();
		pCache->SetStreams(m_pApplication->GetStreams());
		m_pManager->SetOwnerCache(pCache);

		m_pRenderer = NULL;
	}
	~CMetafileFontPicker()
	{
		m_pManager->Release();
		m_pApplication->Release();
	}

	void SetFont(const std::wstring& sName, const bool& bBold, const bool& bItalic)
	{
		int nStyle = 0;
		if (bBold) nStyle |= 0x01;
		if (bItalic) nStyle |= 0x02;
		SetFont(sName, nStyle);
	}
	void SetFont(const std::wstring& sName, const int& nStyle)
	{
		m_pManager->LoadFontByName(sName, 10, nStyle, 72, 72);
	}

	void FillText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
	{
		int bGID = 0;
		m_pRenderer->get_FontStringGID(&bGID);

		CGlyphString oString(bsText, 0, 0);
		if (bGID || oString.GetLength() != 1)
		{
			m_pRenderer->CommandDrawText(bsText, x, y, w, h);
			return;
		}

		std::wstring sName;
		LONG nStyle = 0;
		m_pRenderer->get_FontName(&sName);
		m_pRenderer->get_FontStyle(&nStyle);
		SetFont(sName, (int)nStyle);

		NSFonts::IFontFile* pFont = m_pManager->GetFile();
		if (!pFont)
		{
			m_pRenderer->CommandDrawText(bsText, x, y, w, h);
			return;
		}

		int code = oString.GetAt(0)->lUnicode;
		if (pFont->GetGIDByUnicode(code))
		{
			m_pRenderer->CommandDrawText(bsText, x, y, w, h);
			return;
		}

		CFontFile* pFileNew = ((CFontManager*)m_pManager)->GetFontFileBySymbol((CFontFile*)m_pManager->GetFile(), code);
		if (!pFileNew)
		{
			m_pRenderer->CommandDrawText(bsText, x, y, w, h);
			return;
		}

		m_pRenderer->put_FontName(pFileNew->m_sName);
		int nNewStyle = 0;
		if (pFileNew->IsBold()) nNewStyle |= 0x01;
		if (pFileNew->IsItalic()) nNewStyle |= 0x02;
		m_pRenderer->put_FontStyle(nNewStyle);
		m_pRenderer->CommandDrawText(bsText, x, y, w, h);
		m_pRenderer->put_FontName(sName);
		m_pRenderer->put_FontStyle(nStyle);

		RELEASEINTERFACE(pFileNew);
	}

	void SetRenderer(IRenderer* pRenderer)
	{
		m_pRenderer = pRenderer;
	}
};

IMetafileToRenderter::IMetafileToRenderter(IRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
	m_pPicker = NULL;
}
IMetafileToRenderter::~IMetafileToRenderter()
{
	for (std::vector<std::wstring>::iterator i = m_arTempFiles.begin(); i != m_arTempFiles.end(); i++)
	{
		std::wstring sPath = *i;
		if (NSFile::CFileBinary::Exists(sPath))
			NSFile::CFileBinary::Remove(sPath);
	}

	if (m_pPicker)
	{
		CMetafileFontPicker* pPicker = (CMetafileFontPicker*)m_pPicker;
		RELEASEOBJECT(pPicker);
		m_pPicker = NULL;
	}
}

void IMetafileToRenderter::SetTempDirectory(const std::wstring& sDir) { m_sTempDir = sDir; }
std::wstring IMetafileToRenderter::GetTempDirectory() { return m_sTempDir; }
void IMetafileToRenderter::SetMediaDirectory(const std::wstring& sDir) { m_sMediaDir = sDir; }
std::wstring IMetafileToRenderter::GetMediaDirectory() { return m_sMediaDir; }
void IMetafileToRenderter::SetInternalMediaDirectory(const std::wstring& sDir) { m_sInternalMediaDir = sDir; }
std::wstring IMetafileToRenderter::GetInternalMediaDirectory() { return m_sInternalMediaDir; }
void IMetafileToRenderter::SetThemesDirectory(const std::wstring& sDir) { m_sThemesDir = sDir; }
std::wstring IMetafileToRenderter::GetThemesDirectory() { return m_sThemesDir; }

std::wstring IMetafileToRenderter::GetImagePath(const std::wstring& sPath)
{
	std::wstring sImagePath = L"";
	if (0 == sPath.find(L"data:"))
	{
		try
		{
			std::wstring::size_type posZ = sPath.find(',');
			if (std::wstring::npos == posZ)
				throw;

			bool bIsOnlyOfficeHatch = false;
			if (std::wstring::npos != sPath.substr(0, posZ).find(L"onlyoffice_hatch"))
				bIsOnlyOfficeHatch = true;

			int nBase64Size = (int)(sPath.length() - posZ - 1);
			const wchar_t* pBase64Data = sPath.c_str() + posZ + 1;

			char* pBase64Buffer = new char[nBase64Size];
			for (int i = 0; i < nBase64Size; ++i)
			{
				pBase64Buffer[i] = (char)pBase64Data[i];
			}

			int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(nBase64Size);
			BYTE* pImageBuffer = new BYTE[nBufferLen + 64];

			if (NSBase64::Base64Decode(pBase64Buffer, nBase64Size, pImageBuffer, &nBufferLen))
			{
				if (m_sTempDir.empty())
					m_sTempDir = NSFile::CFileBinary::GetTempPath();

				std::wstring sTempFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(m_sTempDir, L"Image_");

				if (!bIsOnlyOfficeHatch)
				{
					NSFile::CFileBinary oFile;
					if (oFile.CreateFileW(sTempFile))
					{
						oFile.WriteFile(pImageBuffer, nBufferLen);
						oFile.CloseFile();
						sImagePath = sTempFile;
					}
				}
				else
				{
					int nSize = (int)sqrt(nBufferLen >> 2);
					CBgraFrame oFrame;
					oFrame.put_Data(pImageBuffer);
					oFrame.put_Width(nSize);
					oFrame.put_Height(nSize);
					oFrame.put_Stride(4 * nSize);
					oFrame.put_IsRGBA(true);
					oFrame.SaveFile(sTempFile, 4); // PNG
					oFrame.put_Data(NULL);
					sImagePath = sTempFile;
				}

				m_arTempFiles.push_back(sTempFile);
			}

			RELEASEARRAYOBJECTS(pImageBuffer);
			RELEASEARRAYOBJECTS(pBase64Buffer);

			if (sImagePath.empty())
				throw;
		}
		catch (...)
		{
		}
	}
	else
	{
		if (0 != sPath.find(L"http:") &&
				0 != sPath.find(L"https:") &&
				0 != sPath.find(L"ftp:") &&
				0 != sPath.find(L"file:"))
		{
			if (0 == sPath.find(L"theme") && !m_sThemesDir.empty())
			{
				sImagePath = m_sThemesDir + L"/" + sPath;
			}
			else
			{
				if (!m_sMediaDir.empty())
				{
					std::wstring::size_type posMedia = sPath.find(L"media/");
					if (std::wstring::npos != posMedia)
					{
						if (0 == posMedia || NSFile::CFileBinary::Exists(m_sMediaDir + L"/" + sPath))
						{
							sImagePath = m_sMediaDir + L"/" + sPath;
						}
						else if (!m_sInternalMediaDir.empty() && (3 <= posMedia && posMedia <= 5) && NSFile::CFileBinary::Exists(m_sInternalMediaDir + L"/" + sPath))
						{
							// word/media/.. ppt/media/.. xl/media/..
							sImagePath = m_sInternalMediaDir + L"/" + sPath;
						}
						else
						{
							sImagePath = m_sMediaDir + L"/media/" + sPath;
						}
					}
					else
					{
						sImagePath = m_sMediaDir + L"/media/" + sPath;
					}
				}

				std::wstring wsSvgExt(L".svg");
				if (0 == sImagePath.compare(sImagePath.length() - wsSvgExt.length(), std::wstring::npos, wsSvgExt))
				{
					std::wstring wsTestPath = sImagePath.substr(0, sImagePath.length() - wsSvgExt.length());
					if (NSFile::CFileBinary::Exists(wsTestPath + L".emf"))
						sImagePath = wsTestPath + L".emf";
					else if (NSFile::CFileBinary::Exists(wsTestPath + L".wmf"))
						sImagePath = wsTestPath + L".wmf";
				}
			}

			NSStringExt::Replace(sImagePath, L"\\", L"/");
		}

		if (0 == sImagePath.find(L"file:///"))
		{
			// TODO: под linux код неправильный
			NSStringExt::Replace(sImagePath, L"file:///", L"");
			NSStringExt::Replace(sImagePath, L"\\", L"/");
		}
	}

	return sImagePath.empty() ? sPath : sImagePath;
}
void IMetafileToRenderter::SetLinearGradiant(const double& x0, const double& y0, const double& x1, const double& y1)
{
	double dAngle = 0;
	if (fabs(x1 - x0) >= FLT_EPSILON || fabs(y1 - y0) >= FLT_EPSILON)
		dAngle = 180 * atan2(y1 - y0, x1 - x0) / agg::pi;

	m_pRenderer->put_BrushType(/*c_BrushTypePathGradient1*/2006);
	m_pRenderer->put_BrushLinearAngle(dAngle);
}

void IMetafileToRenderter::SetRadialGradiant(const double& dX0, const double& dY0, const double& dR0, const double& dX1, const double& dY1, const double& dR1)
{
	// TODO:
	m_pRenderer->put_BrushType(/*c_BrushTypePathGradient2*/2007);
}

void IMetafileToRenderter::InitPicker(const std::wstring& sFontsFolder)
{
	CMetafileFontPicker* pPicker = new CMetafileFontPicker(sFontsFolder);
	m_pPicker = (void*)pPicker;
}
void IMetafileToRenderter::InitPicker(NSFonts::IApplicationFonts* pFonts)
{
	CMetafileFontPicker* pPicker = new CMetafileFontPicker(pFonts);
	m_pPicker = (void*)pPicker;
}

namespace NSOnlineOfficeBinToPdf
{
	inline BYTE ReadByte(BYTE*& pData, int& nOffset)
	{
		BYTE ret = *(pData);
		pData++;
		nOffset++;
		return ret;
	}
	inline bool ReadBool(BYTE*& pData, int& nOffset)
	{
		return ReadByte(pData, nOffset);
	}
	inline INT32 ReadInt(BYTE*& pData, int& nOffset)
	{
#ifdef _ARM_ALIGN_
		INT32 ret = 0;
		memcpy(&ret, pData, sizeof(INT32));
		pData   += 4;
		nOffset += 4;
		return ret;
#else
		INT32 ret = *((INT32*)pData);
		pData   += 4;
		nOffset += 4;
		return ret;
#endif
	}
	inline double ReadDouble(BYTE*& pData, int& nOffset)
	{
		return ReadInt(pData, nOffset) / 100000.0;
	}
	inline void SkipInt(BYTE*& pData, int& nOffset, int nCount = 1)
	{
		pData   += (nCount << 2);
		nOffset += (nCount << 2);
	}
	inline void SkipDouble(BYTE*& pData, int& nOffset, int nCount = 1)
	{
		SkipInt(pData, nOffset, nCount);
	}

	inline USHORT ReadUSHORT(BYTE*& pData, int& nOffset)
	{
#ifdef _ARM_ALIGN_
		USHORT ret = 0;
		memcpy(&ret, pData, sizeof(USHORT));
		pData   += 2;
		nOffset += 2;
		return ret;
#else
		USHORT ret = *((USHORT*)pData);
		pData   += 2;
		nOffset += 2;
		return ret;
#endif
	}
	inline void SkipUSHORT(BYTE*& pData, int& nOffset)
	{
		pData   += 2;
		nOffset += 2;
	}

	inline std::wstring ReadString16(BYTE*& pData, int& nOffset, int nLen)
	{
		std::wstring wsTempString;
#ifdef _ARM_ALIGN_

		if (sizeof(wchar_t) == 4)
		{
#if !defined(_WIN32) && !defined(_WIN64)
			int len = nLen / 2;
			unsigned short* buf = new unsigned short[len];
			memcpy(buf, pData, nLen);
			wsTempString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)buf, len);
			RELEASEARRAYOBJECTS(buf);
#endif
		}
		else
		{
			int len = nLen / 2;
			wchar_t* buf = new wchar_t[len];
			memcpy(buf, pData, nLen);
			wsTempString = std::wstring(buf, len);
			RELEASEARRAYOBJECTS(buf);
		}

#else

		if (sizeof(wchar_t) == 4)
		{
#if !defined(_WIN32) && !defined(_WIN64)
			wsTempString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)pData, nLen / 2);
#endif
		}
		else
		{
			wsTempString = std::wstring((wchar_t*)pData, nLen / 2);
		}

#endif
		pData += nLen;
		nOffset += nLen;
		return wsTempString;
	}
	inline void SkipString16(BYTE*& pData, int& nOffset, int nLen)
	{
		pData += nLen;
		nOffset += nLen;
	}
	inline std::wstring ReadString(BYTE*& pData, int& nOffset)
	{
		int nLen = 2 * ReadUSHORT(pData, nOffset);
		return ReadString16(pData, nOffset, nLen);
	}
	inline void SkipString(BYTE*& pData, int& nOffset)
	{
		int nLen = 2 * ReadUSHORT(pData, nOffset);
		SkipString16(pData, nOffset, nLen);
	}

	bool ConvertBufferToRenderer(BYTE* pBuffer, LONG lBufferLen, IMetafileToRenderter* pCorrector)
	{
		IRenderer* pRenderer = pCorrector->m_pRenderer;
		CMetafileFontPicker* pPicker = NULL;
		if (pCorrector->m_pPicker)
		{
			pPicker = (CMetafileFontPicker*)pCorrector->m_pPicker;
			pPicker->SetRenderer(pRenderer);
		}

		LONG lRendererType = 0;
		pRenderer->get_Type(&lRendererType);

		// из команд js - точные имена
		pRenderer->CommandLong(c_nUseDictionaryFonts, 0);

		CommandType eCommand = ctError;

		bool bIsPathOpened = false;
		int curindex = 0;

		bool bIsEnableBrushRect = false;

		BYTE* current = pBuffer;
		while (curindex < lBufferLen)
		{
			eCommand = (CommandType)(*current);
			current++;
			curindex++;
			switch (eCommand)
			{
			case ctPageWidth:
			{
				pRenderer->put_Width(ReadInt(current, curindex) / 100000.0);
				break;
			}
			case ctPageHeight:
			{
				pRenderer->put_Height(ReadInt(current, curindex) / 100000.0);
				break;
			}
			case ctPageStart:
			{
				pRenderer->NewPage();
				pRenderer->BeginCommand(c_nPageType);

				// TODO:
				pRenderer->put_PenLineStartCap(Aggplus::LineCapFlat);
				pRenderer->put_PenLineEndCap(Aggplus::LineCapFlat);
				break;
			}
			case ctPageEnd:
			{
				if (bIsPathOpened)
				{
					pRenderer->PathCommandEnd();
					pRenderer->EndCommand(c_nPathType);
				}
				bIsPathOpened = false;

				pRenderer->EndCommand(c_nPageType);

				if (lRendererType == c_nGrRenderer)
					return true;

				break;
			}
			case ctPenColor:
			{
				pRenderer->put_PenColor(ReadInt(current, curindex));
				break;
			}
			case ctPenAlpha:
			{
				pRenderer->put_PenAlpha(*current);
				current++;
				curindex++;
				break;
			}
			case ctPenSize:
			{
				pRenderer->put_PenSize(ReadInt(current, curindex) / 100000.0);
				break;
			}
			case ctPenDashStyle:
			{
				BYTE nDashType = *current++;
				curindex++;
				switch (nDashType)
				{
				case Aggplus::DashStyleCustom:
				{
					int nCountDash = ReadInt(current, curindex);
					if (0 < nCountDash)
					{
						double* pDash = new double[nCountDash];
						for (int nDash = 0; nDash < nCountDash; ++nDash)
						{
							pDash[nDash] = ReadInt(current, curindex) / 100000.0;
						}

						pRenderer->PenDashPattern(pDash, nCountDash);
						delete[] pDash;
					}
				}
				default:
					pRenderer->put_PenDashStyle(nDashType);
					break;
				}

				break;
			}
			case ctPenLineJoin:
			{
				pRenderer->put_PenLineJoin(*current);
				current++;
				curindex++;
				break;
			}
			case ctBrushType:
			{
				pRenderer->put_BrushType(ReadInt(current, curindex));
				break;
			}
			case ctBrushColor1:
			{
				pRenderer->put_BrushColor1(ReadInt(current, curindex));
				break;
			}
			case ctBrushAlpha1:
			{
				pRenderer->put_BrushAlpha1(*current);
				current++;
				curindex++;
				break;
			}
			case ctBrushColor2:
			{
				pRenderer->put_BrushColor1(ReadInt(current, curindex));
				break;
			}
			case ctBrushAlpha2:
			{
				pRenderer->put_BrushAlpha2(*current);
				current++;
				curindex++;
				break;
			}
			case ctBrushRectable:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				double m3 = ReadInt(current, curindex) / 100000.0;
				double m4 = ReadInt(current, curindex) / 100000.0;
				pRenderer->BrushRect(bIsEnableBrushRect ? 1 : 0, m1, m2, m3, m4);
				break;
			}
			case ctBrushRectableEnabled:
			{
				bIsEnableBrushRect = (1 == *current) ? true : false;

				if (!bIsEnableBrushRect)
					pRenderer->BrushRect(bIsEnableBrushRect ? 1 : 0, 0, 0, 1, 1);

				current += 1;
				curindex += 1;
				break;
			}
			case ctBrushTexturePathOld:
			{
				int nLen = 2 * ReadUSHORT(current, curindex);
				std::wstring sTempPath = ReadString16(current, curindex, nLen);

				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);
				pRenderer->put_BrushTexturePath(sImagePath);
				break;
			}
			case ctBrushTexturePath:
			{
				int nLen = ReadInt(current, curindex);
				std::wstring sTempPath = ReadString16(current, curindex, nLen);

				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);
				pRenderer->put_BrushTexturePath(sImagePath);
				break;
			}
			case ctBrushGradient:
			{
				current++;
				curindex++;

				while (true)
				{
					BYTE _command = *current;
					current++;
					curindex++;

					if (251 == _command)
						break;

					switch (_command)
					{
					case 0:
					{
						current += 5;
						curindex += 5;
						double x0 = ReadInt(current, curindex) / 100000.0;
						double y0 = ReadInt(current, curindex) / 100000.0;
						double x1 = ReadInt(current, curindex) / 100000.0;
						double y1 = ReadInt(current, curindex) / 100000.0;

						pCorrector->SetLinearGradiant(x0, y0, x1, y1);
						break;
					}
					case 1:
					{
						current++;
						curindex++;
						double x0 = ReadInt(current, curindex) / 100000.0;
						double y0 = ReadInt(current, curindex) / 100000.0;
						double x1 = ReadInt(current, curindex) / 100000.0;
						double y1 = ReadInt(current, curindex) / 100000.0;
						double r0 = ReadInt(current, curindex) / 100000.0;
						double r1 = ReadInt(current, curindex) / 100000.0;
						pCorrector->SetRadialGradiant(x0, y0, r0, x1, y1, r1);
						break;
					}
					case 2:
					{
						LONG lColorsCount = (LONG)ReadInt(current, curindex);
						if (0 >= lColorsCount)
						{
							pRenderer->put_BrushGradientColors(NULL, NULL, 0);
						}
						else
						{
							LONG* pColors = new LONG[lColorsCount];
							double* pPositions = new double[lColorsCount];

							if (!pColors)
								break;

							if (!pPositions)
							{
								delete[] pColors;
								break;
							}

							for (LONG lIndex = 0; lIndex < lColorsCount; lIndex++)
							{
								pPositions[lIndex] = ReadInt(current, curindex) / 100000.0;
								pColors[lIndex] = ReadInt(current, curindex);
							}

							pRenderer->put_BrushGradientColors(pColors, pPositions, lColorsCount);

							delete[] pColors;
							delete[] pPositions;
						}

						break;
					}
					default:
					{
						break;
					}
					};
				}

				break;
			}
			case ctBrushTextureMode:
			{
				LONG lMode = (LONG)(*current);
				pRenderer->put_BrushTextureMode(lMode);

				current += 1;
				curindex += 1;
				break;
			}
			case ctBrushTextureAlpha:
			{
				LONG lAlpha = (LONG)(*current);
				pRenderer->put_BrushTextureAlpha(lAlpha);

				current += 1;
				curindex += 1;
				break;
			}
			case ctSetTransform:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				double m3 = ReadInt(current, curindex) / 100000.0;
				double m4 = ReadInt(current, curindex) / 100000.0;
				double m5 = ReadInt(current, curindex) / 100000.0;
				double m6 = ReadInt(current, curindex) / 100000.0;
				pRenderer->SetTransform(m1, m2, m3, m4, m5, m6);
				break;
			}
			case ctPathCommandStart:
			{
				if (bIsPathOpened)
				{
					pRenderer->PathCommandEnd();
					pRenderer->EndCommand(c_nPathType);
				}

				pRenderer->BeginCommand(c_nPathType);
				pRenderer->PathCommandStart();

				bIsPathOpened = true;
				break;
			}
			case ctPathCommandMoveTo:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				pRenderer->PathCommandMoveTo(m1, m2);
				break;
			}
			case ctPathCommandLineTo:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				pRenderer->PathCommandLineTo(m1, m2);
				break;
			}
			case ctPathCommandCurveTo:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				double m3 = ReadInt(current, curindex) / 100000.0;
				double m4 = ReadInt(current, curindex) / 100000.0;
				double m5 = ReadInt(current, curindex) / 100000.0;
				double m6 = ReadInt(current, curindex) / 100000.0;
				pRenderer->PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
				break;
			}
			case ctPathCommandClose:
			{
				pRenderer->PathCommandClose();
				break;
			}
			case ctPathCommandEnd:
			{
				if (bIsPathOpened)
				{
					pRenderer->PathCommandEnd();
					pRenderer->EndCommand(c_nPathType);
					bIsPathOpened = false;
				}
				break;
			}
			case ctDrawPath:
			{
				pRenderer->DrawPath(ReadInt(current, curindex));
				break;
			}
			case ctDrawImageFromFile:
			{
				int nLen = ReadInt(current, curindex);
				std::wstring sTempPath = ReadString16(current, curindex, nLen);
				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);

				double m1 = ReadInt(current, curindex) / 100000.0;
				double m2 = ReadInt(current, curindex) / 100000.0;
				double m3 = ReadInt(current, curindex) / 100000.0;
				double m4 = ReadInt(current, curindex) / 100000.0;

				try
				{
					pRenderer->DrawImageFromFile(sImagePath, m1, m2, m3, m4);
				}
				catch (...)
				{
				}

				break;
			}
			case ctFontName:
			{
				int _sLen = 2 * (int)ReadUSHORT(current, curindex);
				std::wstring wsTempString = ReadString16(current, curindex, _sLen);
				pRenderer->put_FontName(wsTempString);
				break;
			}
			case ctFontSize:
			{
				double m1 = ReadInt(current, curindex) / 100000.0;
				pRenderer->put_FontSize(m1);
				break;
			}
			case ctFontStyle:
			{
				pRenderer->put_FontStyle(ReadInt(current, curindex));
				break;
			}
			case ctDrawText:
			{
				int _sLen = 2 * (int)ReadUSHORT(current, curindex);
				std::wstring wsTempString = ReadString16(current, curindex, _sLen);

				double x = ReadInt(current, curindex) / 100000.0;
				double y = ReadInt(current, curindex) / 100000.0;

				if (!pPicker)
					pRenderer->CommandDrawText(wsTempString, x, y, 0, 0);
				else
					pPicker->FillText(wsTempString, x, y, 0, 0);
				break;
			}
			case ctDrawTextCodeGid:
			{
				unsigned int nGid = (unsigned int)ReadInt(current, curindex);
				double x = ReadInt(current, curindex) / 100000.0;
				double y = ReadInt(current, curindex) / 100000.0;

				unsigned int nCountUnicodes = (unsigned int)ReadInt(current, curindex);
				unsigned int* pCodePoints = (0 != nCountUnicodes) ? new unsigned int[nCountUnicodes] : NULL;
				for (unsigned int nCodePointIndex = 0; nCodePointIndex < nCountUnicodes; ++nCodePointIndex)
					pCodePoints[nCodePointIndex] = ReadInt(current, curindex);

				pRenderer->CommandDrawTextCHAR2(pCodePoints, nCountUnicodes, nGid, x, y, 0, 0);

				if (pCodePoints)
					delete [] pCodePoints;
				break;
			}
			case ctBeginCommand:
			{
				if (bIsPathOpened)
				{
					pRenderer->PathCommandEnd();
					pRenderer->EndCommand(4);
					bIsPathOpened = false;
				}
				pRenderer->BeginCommand((DWORD)(ReadInt(current, curindex)));
				break;
			}
			case ctEndCommand:
			{
				if (bIsPathOpened)
				{
					pRenderer->EndCommand(4);
					bIsPathOpened = false;
				}
				pRenderer->EndCommand((DWORD)(ReadInt(current, curindex)));
				pRenderer->PathCommandEnd();
				break;
			}
			case ctGradientFill:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = ReadInt(current, curindex);

				std::wstring sXml, sXmlStop;
				if (0 == gradientType)	//	linearGradient
				{
					double x1 = ReadInt(current, curindex) / 100000.0;
					double x2 = ReadInt(current, curindex) / 100000.0;
					double y1 = ReadInt(current, curindex) / 100000.0;
					double y2 = ReadInt(current, curindex) / 100000.0;

					int stops = ReadInt(current, curindex);

					for (int i = 0; i < stops; ++i)
					{
						INT32 color   = static_cast<INT32>(*current);
						double opacity = static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

						current  += 6 * 4; // 4 + 1 + 1
						curindex += 6 * 4;
					}
				}
				else if (1 == gradientType)
				{
					double cx = ReadInt(current, curindex) / 100000.0;
					double cy = ReadInt(current, curindex) / 100000.0;
					double r  = ReadInt(current, curindex) / 100000.0;
					double fx = ReadInt(current, curindex) / 100000.0;
					double fy = ReadInt(current, curindex) / 100000.0;

					int stops = ReadInt(current, curindex);

					for (int i = 0; i < stops; ++i)
					{
						INT32 color   = static_cast<INT32>(*current);
						double opacity = static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

						current  += 6 * 4;		//	4 + 1 + 1
						curindex += 6 * 4;
					}
				}
				break;
			}
			case ctGradientFillXML:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = ReadInt(current, curindex);
				int _sLen = ReadInt(current, curindex);
				std::wstring wsTempString = ReadString16(current, curindex, _sLen);
				break;
			}
			case ctGradientStroke:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = ReadInt(current, curindex);
				if (0 == gradientType)	//	linearGradient
				{
					double x1 = ReadInt(current, curindex) / 100000.0;
					double x2 = ReadInt(current, curindex) / 100000.0;
					double y1 = ReadInt(current, curindex) / 100000.0;
					double y2 = ReadInt(current, curindex) / 100000.0;
					int stops = ReadInt(current, curindex);

					for (int i = 0; i < stops; ++i)
					{
						INT32 color   = static_cast<INT32>(*current);
						double opacity = static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

						current  += 6 * 4; // 4 + 1 + 1
						curindex += 6 * 4;
					}
				}
				else if (1 == gradientType)
				{
					double cx = ReadInt(current, curindex) / 100000.0;
					double cy = ReadInt(current, curindex) / 100000.0;
					double r  = ReadInt(current, curindex) / 100000.0;
					double fx = ReadInt(current, curindex) / 100000.0;
					double fy = ReadInt(current, curindex) / 100000.0;
					int stops = ReadInt(current, curindex);

					for (int i = 0; i < stops; ++i)
					{
						INT32 color   = static_cast<INT32>(*current);
						double opacity = static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

						current  += 6 * 4; // 4 + 1 + 1
						curindex += 6 * 4;
					}
				}
				break;
			}
			case ctGradientStrokeXML:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = ReadInt(current, curindex);
				int _sLen = (int)ReadInt(current, curindex);
				std::wstring wsTempString = ReadString16(current, curindex, _sLen);
				break;
			}
			case ctHyperlink:
			{
				double dX = ReadDouble(current, curindex);
				double dY = ReadDouble(current, curindex);
				double dW = ReadDouble(current, curindex);
				double dH = ReadDouble(current, curindex);

				std::wstring wsUrl     = ReadString(current, curindex);
				std::wstring wsTooltip = ReadString(current, curindex);

				pRenderer->AddHyperlink(dX, dY, dW, dH, wsUrl, wsTooltip);
				break;
			}
			case ctLink:
			{
				double dX = ReadDouble(current, curindex);
				double dY = ReadDouble(current, curindex);
				double dW = ReadDouble(current, curindex);
				double dH = ReadDouble(current, curindex);

				double dDestX = ReadDouble(current, curindex);
				double dDestY = ReadDouble(current, curindex);
				int    nPage  = ReadInt(current, curindex);

				pRenderer->AddLink(dX, dY, dW, dH, dDestX, dDestY, nPage);
				break;
			}
			case ctFormField:
			{
				BYTE* nStartPos   = current;
				int   nStartIndex = curindex;

				int nLen = ReadInt(current, curindex);

				double dX = ReadDouble(current, curindex);
				double dY = ReadDouble(current, curindex);
				double dW = ReadDouble(current, curindex);
				double dH = ReadDouble(current, curindex);

				CFormFieldInfo oInfo;
				oInfo.SetBounds(dX, dY, dW, dH);
				oInfo.SetBaseLineOffset(ReadDouble(current, curindex));

				int nFlags = ReadInt(current, curindex);

				if (nFlags & 1)
					oInfo.SetKey(ReadString(current, curindex));

				if (nFlags & 2)
					oInfo.SetHelpText(ReadString(current, curindex));

				oInfo.SetRequired(nFlags & 4);
				oInfo.SetPlaceHolder(nFlags & 8);

				if (nFlags & (1 << 6))
				{
					int nBorderType = ReadInt(current, curindex);
					double dBorderSize = ReadDouble(current, curindex);
					unsigned char unR = ReadByte(current, curindex);
					unsigned char unG = ReadByte(current, curindex);
					unsigned char unB = ReadByte(current, curindex);
					unsigned char unA = ReadByte(current, curindex);

					oInfo.SetBorder(nBorderType, dBorderSize, unR, unG, unB, unA);
				}

				if (nFlags & (1 << 9))
				{
					unsigned char unR = ReadByte(current, curindex);
					unsigned char unG = ReadByte(current, curindex);
					unsigned char unB = ReadByte(current, curindex);
					unsigned char unA = ReadByte(current, curindex);
					oInfo.SetShd(unR, unG, unB, unA);
				}

				if (nFlags & (1 << 10))
				{
					oInfo.SetJc(ReadByte(current, curindex));
				}

				oInfo.SetType(ReadInt(current, curindex));

				if (oInfo.IsTextField())
				{
					CFormFieldInfo::CTextFormPr* pPr = oInfo.GetTextFormPr();
					pPr->SetComb(nFlags & (1 << 20));

					if (nFlags & (1 << 21))
						pPr->SetMaxCharacters(ReadInt(current, curindex));

					if (nFlags & (1 << 22))
						pPr->SetTextValue(ReadString(current, curindex));

					pPr->SetMultiLine(nFlags & (1 << 23));
					pPr->SetAutoFit(nFlags & (1 << 24));

					if (nFlags & (1 << 25))
						pPr->SetPlaceHolder(ReadString(current, curindex));

					if (nFlags & (1 << 26))
					{
						CFormFieldInfo::CTextFormFormat* pFormat = pPr->GetFormat();
						pFormat->SetType((CFormFieldInfo::EFormatType)ReadByte(current, curindex));

						unsigned int unSymbolsCount = ReadInt(current, curindex);
						for (unsigned int unSymbolIndex = 0; unSymbolIndex < unSymbolsCount; ++unSymbolIndex)
						{
							pFormat->AddSymbol(ReadInt(current, curindex));
						}

						pFormat->SetValue(ReadString(current, curindex));
					}
				}
				else if (oInfo.IsDropDownList())
				{
					CFormFieldInfo::CDropDownFormPr* pPr = oInfo.GetDropDownFormPr();
					pPr->SetEditComboBox(nFlags & (1 << 20));

					int nItemsCount = ReadInt(current, curindex);
					for (int nIndex = 0; nIndex < nItemsCount; ++nIndex)
					{
						pPr->AddComboBoxItem(ReadString(current, curindex));
					}

					int nSelectedIndex = ReadInt(current, curindex);

					if (nFlags & (1 << 22))
						pPr->SetTextValue(ReadString(current, curindex));

					if (nFlags & (1 << 23))
						pPr->SetPlaceHolder(ReadString(current, curindex));
				}
				else if (oInfo.IsCheckBox())
				{
					CFormFieldInfo::CCheckBoxFormPr* pPr = oInfo.GetCheckBoxFormPr();
					pPr->SetChecked(nFlags & (1 << 20));
					pPr->SetType(ReadInt(current, curindex));
					pPr->SetCheckedSymbol(ReadInt(current, curindex));
					pPr->SetCheckedFont(ReadString(current, curindex));
					pPr->SetUncheckedSymbol(ReadInt(current, curindex));
					pPr->SetUncheckedFont(ReadString(current, curindex));

					if (nFlags & (1 << 21))
						pPr->SetGroupKey(ReadString(current, curindex));
				}
				else if (oInfo.IsPicture())
				{
					CFormFieldInfo::CPictureFormPr* pPr = oInfo.GetPictureFormPr();
					pPr->SetConstantProportions(nFlags & (1 << 20));
					pPr->SetRespectBorders(nFlags & (1 << 21));
					pPr->SetScaleType(CFormFieldInfo::EScaleType((nFlags >> 24) & 0xF));
					LONG lShiftX = ReadInt(current, curindex);
					LONG lShiftY = ReadInt(current, curindex);
					pPr->SetShift(lShiftX, lShiftY);

					if (nFlags & (1 << 22))
						pPr->SetPicturePath(pCorrector->GetImagePath(ReadString(current, curindex)));
				}
				else if (oInfo.IsSignature())
				{
					CFormFieldInfo::CSignatureFormPr* pPr = oInfo.GetSignatureFormPr();

					// Поля Настройки подписи
					// Сведения о подписывающем

					// Имя
					if (nFlags & (1 << 20))
						pPr->SetName(ReadString(current, curindex));

					// Должность Игнорируется

					// Адрес электронной почты
					if (nFlags & (1 << 21))
						pPr->SetContact(ReadString(current, curindex));

					// Инструкция для подписывающего Игнорируется

					// Показывать дату подписи в строке подписи
					pPr->SetDate(nFlags & (1 << 22));

					// Цель подписания документа (причина)
					if (nFlags & (1 << 23))
						pPr->SetReason(ReadString(current, curindex));

					// Картинка
					if (nFlags & (1 << 24))
						pPr->SetPicturePath(pCorrector->GetImagePath(ReadString(current, curindex)));

					// Необходимо передать сертификат, пароль, ключ, пароль ключа
					if (nFlags & (1 << 25))
						pPr->SetCert(ReadString(current, curindex));
				}

				if (oInfo.IsValid())
					pRenderer->AddFormField(&oInfo);

				current  = nStartPos + nLen;
				curindex = nStartIndex + nLen;

				break;
			}
			case ctDocInfo:
			{
				int nFlags = ReadInt(current, curindex);

				std::wstring wsTitle, wsCreator, wsSubject, wsKeywords;
				if (nFlags & 1)
					wsTitle    = ReadString(current, curindex);
				if (nFlags & 2)
					wsCreator  = ReadString(current, curindex);
				if (nFlags & 4)
					wsSubject  = ReadString(current, curindex);
				if (nFlags & 8)
					wsKeywords = ReadString(current, curindex);

				pRenderer->DocInfo(wsTitle, wsCreator, wsSubject, wsKeywords);
				break;
			}
			default:
			{
				break;
			}
			}; // switch (eCommand)
		} // while (curindex < len)

		return true;
	}

	void CMetafilePagesInfo::CheckBuffer(BYTE* pBuffer, LONG lBufferLen)
	{
		CommandType eCommand = ctError;

		bool bIsPathOpened = false;
		int curindex = 0;

		BYTE* current = pBuffer;
		while (curindex < lBufferLen)
		{
			eCommand = (CommandType)(*current);
			current++;
			curindex++;
			switch (eCommand)
			{
			case ctPageWidth:
			{
				arSizes[PagesCount - 1].width = (ReadInt(current, curindex) / 100000.0);
				break;
			}
			case ctPageHeight:
			{
				arSizes[PagesCount - 1].height = (ReadInt(current, curindex) / 100000.0);
				break;
			}
			case ctPageStart:
			{
				AddPage();
				arSizes[PagesCount - 1].data = current;
				break;
			}
			case ctPageEnd:
			{
				break;
			}
			case ctPenColor:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctPenAlpha:
			{
				current++;
				curindex++;
				break;
			}
			case ctPenSize:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctPenDashStyle:
			{
				BYTE nDashType = *current++;
				curindex++;
				switch (nDashType)
				{
				case Aggplus::DashStyleCustom:
				{
					int nCountDash = ReadInt(current, curindex);
					if (0 < nCountDash)
					{
						SkipInt(current, curindex, nCountDash);
					}
				}
				default:
					break;
				}

				break;
			}
			case ctPenLineJoin:
			{
				current++;
				curindex++;
				break;
			}
			case ctBrushType:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctBrushColor1:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctBrushAlpha1:
			{
				current++;
				curindex++;
				break;
			}
			case ctBrushColor2:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctBrushAlpha2:
			{
				current++;
				curindex++;
				break;
			}
			case ctBrushRectable:
			{
				SkipInt(current, curindex, 4);
				break;
			}
			case ctBrushRectableEnabled:
			{
				current += 1;
				curindex += 1;
				break;
			}
			case ctBrushTexturePathOld:
			{
				int nLen = 2 * ReadUSHORT(current, curindex);
				SkipString16(current, curindex, nLen);
				break;
			}
			case ctBrushTexturePath:
			{
				int nLen = ReadInt(current, curindex);
				SkipString16(current, curindex, nLen);
				break;
			}
			case ctBrushGradient:
			{
				current++;
				curindex++;

				while (true)
				{
					BYTE _command = *current;
					current++;
					curindex++;

					if (251 == _command)
						break;

					switch (_command)
					{
					case 0:
					{
						current += 5;
						curindex += 5;
						SkipInt(current, curindex, 4);
						break;
					}
					case 1:
					{
						current++;
						curindex++;
						SkipInt(current, curindex, 6);
						break;
					}
					case 2:
					{
						LONG lColorsCount = (LONG)ReadInt(current, curindex);
						if (0 <= lColorsCount)
						{
							SkipInt(current, curindex, 2 * lColorsCount);
						}

						break;
					}
					default:
					{
						break;
					}
					};
				}

				break;
			}
			case ctBrushTextureMode:
			{
				current += 1;
				curindex += 1;
				break;
			}
			case ctBrushTextureAlpha:
			{
				current += 1;
				curindex += 1;
				break;
			}
			case ctSetTransform:
			{
				SkipInt(current, curindex, 6);
				break;
			}
			case ctPathCommandStart:
			{
				break;
			}
			case ctPathCommandMoveTo:
			{
				SkipInt(current, curindex, 2);
				break;
			}
			case ctPathCommandLineTo:
			{
				SkipInt(current, curindex, 2);
				break;
			}
			case ctPathCommandCurveTo:
			{
				SkipInt(current, curindex, 6);
				break;
			}
			case ctPathCommandClose:
			{
				break;
			}
			case ctPathCommandEnd:
			{
				break;
			}
			case ctDrawPath:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctDrawImageFromFile:
			{
				int nLen = ReadInt(current, curindex);
				SkipString16(current, curindex, nLen);

				SkipInt(current, curindex, 4);
				break;
			}
			case ctFontName:
			{
				int nLen = 2 * (int)ReadUSHORT(current, curindex);
				SkipString16(current, curindex, nLen);
				break;
			}
			case ctFontSize:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctFontStyle:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctDrawText:
			{
				int nLen = 2 * (int)ReadUSHORT(current, curindex);
				SkipString16(current, curindex, nLen);

				SkipInt(current, curindex, 2);
				break;
			}
			case ctDrawTextCodeGid:
			{
				SkipInt(current, curindex);
				SkipDouble(current, curindex, 2);
				int nCountUnicodes = ReadInt(current, curindex);
				SkipInt(current, curindex, nCountUnicodes);
				break;
			}
			case ctBeginCommand:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctEndCommand:
			{
				SkipInt(current, curindex);
				break;
			}
			case ctGradientFill:
			case ctGradientFillXML:
			case ctGradientStroke:
			case ctGradientStrokeXML:
			{
				// TODO: Эта команда не должна приходить
				return;
			}
			case ctHyperlink:
			{
				SkipDouble(current, curindex, 4);
				SkipString(current, curindex);
				SkipString(current, curindex);
				break;
			}
			case ctLink:
			{
				SkipDouble(current, curindex, 6);
				SkipInt(current, curindex);
				break;
			}
			case ctFormField:
			{
				BYTE* nStartPos   = current;
				int   nStartIndex = curindex;

				int nLen = ReadInt(current, curindex);

				current  = nStartPos + nLen;
				curindex = nStartIndex + nLen;
				break;
			}
			case ctDocInfo:
			{
				int nFlags = ReadInt(current, curindex);

				std::wstring wsTitle, wsCreator, wsSubject, wsKeywords;
				if (nFlags & 1)
					SkipString(current, curindex);
				if (nFlags & 2)
					SkipString(current, curindex);
				if (nFlags & 4)
					SkipString(current, curindex);
				if (nFlags & 8)
					SkipString(current, curindex);
				break;
			}
			default:
			{
				break;
			}
			}; // switch (eCommand)
		} // while (curindex < len)
	}
}
