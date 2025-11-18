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
#pragma once

#include "agg_math.h"
#include "./MetafileToRenderer.h"
#include "../fontengine/FontManager.h"
#include "../raster/BgraFrame.h"
#include "../common/StringExt.h"
#include "../GraphicsPath.h"

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
		bool bIsEnableBrushRect = false;

		double old_t1, old_t2, old_t3, old_t4, old_t5, old_t6;

		CBufferReader oReader(pBuffer, lBufferLen);
		Aggplus::CGraphicsPath path;
		Aggplus::CMatrix transMatrRot;
		Aggplus::RectF clipRect;
		bool isClose = false;
		bool isTexture = false;
		while (oReader.Check())
		{
			eCommand = (CommandType)(oReader.ReadByte());
			switch (eCommand)
			{
			case ctPageWidth:
			{
				pRenderer->put_Width(oReader.ReadDouble());
				break;
			}
			case ctPageHeight:
			{
				pRenderer->put_Height(oReader.ReadDouble());
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
				pRenderer->put_PenColor(oReader.ReadInt());
				break;
			}
			case ctPenAlpha:
			{
				pRenderer->put_PenAlpha(oReader.ReadByte());
				break;
			}
			case ctPenSize:
			{
				pRenderer->put_PenSize(oReader.ReadDouble());
				break;
			}
			case ctPenDashStyle:
			{
				BYTE nDashType = oReader.ReadByte();
				switch (nDashType)
				{
				case Aggplus::DashStyleCustom:
				{
					int nCountDash = oReader.ReadInt();
					if (0 < nCountDash)
					{
						double* pDash = new double[nCountDash];
						for (int nDash = 0; nDash < nCountDash; ++nDash)
						{
							pDash[nDash] = oReader.ReadDouble();
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
				pRenderer->put_PenLineJoin(oReader.ReadByte());
				break;
			}
			case ctBrushType:
			{
				pRenderer->put_BrushType(oReader.ReadInt());
				break;
			}
			case ctBrushColor1:
			{
				pRenderer->put_BrushColor1(oReader.ReadInt());
				break;
			}
			case ctBrushAlpha1:
			{
				pRenderer->put_BrushAlpha1(oReader.ReadByte());
				break;
			}
			case ctBrushColor2:
			{
				pRenderer->put_BrushColor1(oReader.ReadInt());
				break;
			}
			case ctBrushAlpha2:
			{
				pRenderer->put_BrushAlpha2(oReader.ReadByte());
				break;
			}
			case ctBrushRectable:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				double m3 = oReader.ReadDouble();
				double m4 = oReader.ReadDouble();

				clipRect = Aggplus::RectF(m1, m2, m3, m4);

				pRenderer->BrushRect(bIsEnableBrushRect ? 1 : 0, m1, m2, m3, m4);
				break;
			}
			case ctBrushRectableEnabled:
			{
				bIsEnableBrushRect = oReader.ReadBool();

				if (!bIsEnableBrushRect)
					pRenderer->BrushRect(bIsEnableBrushRect ? 1 : 0, 0, 0, 1, 1);
				break;
			}
			case ctBrushTexturePathOld:
			{
				int nLen = 2 * oReader.ReadUShort();
				std::wstring sTempPath = oReader.ReadString16(nLen);
				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);
				pRenderer->put_BrushTexturePath(sImagePath);
				break;
			}
			case ctBrushTexturePath:
			{
				int nLen = oReader.ReadInt();
				std::wstring sTempPath = oReader.ReadString16(nLen);
				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);
				pRenderer->put_BrushTexturePath(sImagePath);
				isTexture = true;
				break;
			}
			case ctBrushGradient:
			{
				oReader.Skip(1);

				while (true)
				{
					BYTE _command = oReader.ReadByte();

					if (251 == _command)
						break;

					switch (_command)
					{
					case 0:
					{
						oReader.Skip(5);
						double x0 = oReader.ReadDouble();
						double y0 = oReader.ReadDouble();
						double x1 = oReader.ReadDouble();
						double y1 = oReader.ReadDouble();

						pCorrector->SetLinearGradiant(x0, y0, x1, y1);
						break;
					}
					case 1:
					{
						oReader.Skip(1);
						double x0 = oReader.ReadDouble();
						double y0 = oReader.ReadDouble();
						double x1 = oReader.ReadDouble();
						double y1 = oReader.ReadDouble();
						double r0 = oReader.ReadDouble();
						double r1 = oReader.ReadDouble();
						pCorrector->SetRadialGradiant(x0, y0, r0, x1, y1, r1);
						break;
					}
					case 2:
					{
						LONG lColorsCount = (LONG)oReader.ReadInt();
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
								pPositions[lIndex] = oReader.ReadDouble();
								pColors[lIndex] = oReader.ReadInt();
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
				LONG lMode = (LONG)oReader.ReadByte();
				pRenderer->put_BrushTextureMode(lMode);
				break;
			}
			case ctBrushTextureAlpha:
			{
				LONG lAlpha = (LONG)oReader.ReadByte();
				pRenderer->put_BrushTextureAlpha(lAlpha);
				break;
			}
			case ctBrushResetRotation:
			{
				Aggplus::RectF rect;
				bool rectable;
				pRenderer->get_BrushRect(rect, rectable);

				pRenderer->GetTransform(&old_t1, &old_t2, &old_t3, &old_t4, &old_t5, &old_t6);
				Aggplus::CMatrix mtr(old_t1, old_t2, old_t3, old_t4, old_t5, old_t6);

				double rot = mtr.rotation();
				mtr.Rotate(-agg::rad2deg(rot));
				mtr.Translate((cos(atan2(rect.Height, rect.Width) + rot) * sqrt(rect.Width * rect.Width + rect.Height * rect.Height) - rect.Width) / 2.0,
							  (sin(atan2(rect.Height, rect.Width) + rot) * sqrt(rect.Width * rect.Width + rect.Height * rect.Height) - rect.Height) / 2.0);

				pRenderer->SetTransform(mtr.sx(), mtr.shy(), mtr.shx(), mtr.sy(), mtr.tx(), mtr.ty());
				transMatrRot.Rotate(agg::rad2deg(rot), Aggplus::MatrixOrderPrepend);
				transMatrRot.Translate((cos(atan2(rect.Height, rect.Width) - rot) * sqrt(rect.Width * rect.Width + rect.Height * rect.Height) - rect.Width) / 2.0,
									   (sin(atan2(rect.Height, rect.Width) - rot) * sqrt(rect.Width * rect.Width + rect.Height * rect.Height) - rect.Height) / 2.0);
				break;
			}
			case ctSetTransform:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				double m3 = oReader.ReadDouble();
				double m4 = oReader.ReadDouble();
				double m5 = oReader.ReadDouble();
				double m6 = oReader.ReadDouble();
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
				path.StartFigure();

				bIsPathOpened = true;
				break;
			}
			case ctPathCommandMoveTo:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				path.MoveTo(m1, m2);
				break;
			}
			case ctPathCommandLineTo:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				path.LineTo(m1, m2);
				break;
			}
			case ctPathCommandCurveTo:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				double m3 = oReader.ReadDouble();
				double m4 = oReader.ReadDouble();
				double m5 = oReader.ReadDouble();
				double m6 = oReader.ReadDouble();
				path.CurveTo(m1, m2, m3, m4, m5, m6);
				break;
			}
			case ctPathCommandClose:
			{
				path.CloseFigure();
				isClose = true;
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
			case ctPathCommandOffset:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();

				pRenderer->put_BrushOffset(m1, m2);
				break;
			}
			case ctPathCommandScale:
			{
				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();

				pRenderer->put_BrushScale(true, m1, m2);
				break;
			}
			case ctDrawPath:
			{
				if (isTexture)
				{
					Aggplus::CGraphicsPath clipPath;
					Aggplus::CGraphicsPath drawPath;
					clipPath.AddRectangle(clipRect.X, clipRect.Y, clipRect.Width, clipRect.Height);
					clipPath.Transform(&transMatrRot);

					if (!transMatrRot.IsIdentity())
					{
						double left, top, width, height;
						clipPath.GetBounds(left, top, width, height);
						pRenderer->BrushRect(true, left, top, width, height);
						drawPath.AddRectangle(left, top, width, height);
					}
					else
						drawPath = path;

					path = Aggplus::CalcBooleanOperation(drawPath, clipPath, Aggplus::Intersection);
					isTexture = false;
				}

				pRenderer->AddPath(path);

				if (isClose)
				{
					pRenderer->PathCommandClose();
					isClose = false;
				}

				pRenderer->DrawPath(oReader.ReadInt());

				if (!transMatrRot.IsIdentity())
					pRenderer->SetTransform(old_t1, old_t2, old_t3, old_t4, old_t5, old_t6);

				path.Reset();
				transMatrRot.Reset();
				break;
			}
			case ctDrawImageFromFile:
			{
				int nLen = oReader.ReadInt();
				std::wstring sTempPath = oReader.ReadString16(nLen);
				std::wstring sImagePath = pCorrector->GetImagePath(sTempPath);

				double m1 = oReader.ReadDouble();
				double m2 = oReader.ReadDouble();
				double m3 = oReader.ReadDouble();
				double m4 = oReader.ReadDouble();

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
				int nLen = 2 * (int)oReader.ReadUShort();
				std::wstring wsTempString = oReader.ReadString16(nLen);
				pRenderer->put_FontName(wsTempString);
				break;
			}
			case ctFontSize:
			{
				pRenderer->put_FontSize(oReader.ReadDouble());
				break;
			}
			case ctFontStyle:
			{
				pRenderer->put_FontStyle(oReader.ReadInt());
				break;
			}
			case ctDrawText:
			{
				int nLen = 2 * (int)oReader.ReadUShort();
				std::wstring wsTempString = oReader.ReadString16(nLen);

				double x = oReader.ReadDouble();
				double y = oReader.ReadDouble();

				if (!pPicker)
					pRenderer->CommandDrawText(wsTempString, x, y, 0, 0);
				else
					pPicker->FillText(wsTempString, x, y, 0, 0);
				break;
			}
			case ctDrawTextCodeGid:
			{
				unsigned int nGid = (unsigned int)oReader.ReadInt();
				double x = oReader.ReadDouble();
				double y = oReader.ReadDouble();

				unsigned int nCountUnicodes = (unsigned int)oReader.ReadInt();
				unsigned int* pCodePoints = (0 != nCountUnicodes) ? new unsigned int[nCountUnicodes] : NULL;
				for (unsigned int nCodePointIndex = 0; nCodePointIndex < nCountUnicodes; ++nCodePointIndex)
					pCodePoints[nCodePointIndex] = oReader.ReadInt();

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
				pRenderer->BeginCommand((DWORD)(oReader.ReadInt()));
				break;
			}
			case ctEndCommand:
			{
				if (bIsPathOpened)
				{
					pRenderer->EndCommand(4);
					bIsPathOpened = false;
				}
				pRenderer->EndCommand((DWORD)(oReader.ReadInt()));
				pRenderer->PathCommandEnd();
				break;
			}
			case ctGradientFill:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = oReader.ReadInt();

				std::wstring sXml, sXmlStop;
				if (0 == gradientType)	//	linearGradient
				{
					double x1 = oReader.ReadDouble();
					double x2 = oReader.ReadDouble();
					double y1 = oReader.ReadDouble();
					double y2 = oReader.ReadDouble();

					int stops = oReader.ReadInt();

					for (int i = 0; i < stops; ++i)
					{
						INT32 color    = static_cast<INT32>(oReader.ReadByte());
						double opacity = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						oReader.Skip(6 * 4 - 3);
					}
				}
				else if (1 == gradientType)
				{
					double cx = oReader.ReadDouble();
					double cy = oReader.ReadDouble();
					double r  = oReader.ReadDouble();
					double fx = oReader.ReadDouble();
					double fy = oReader.ReadDouble();

					int stops = oReader.ReadInt();

					for (int i = 0; i < stops; ++i)
					{
						INT32 color    = static_cast<INT32>(oReader.ReadByte());
						double opacity = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						oReader.Skip(6 * 4 - 3);
					}
				}
				break;
			}
			case ctGradientFillXML:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = oReader.ReadInt();
				int nLen = oReader.ReadInt();
				std::wstring wsTempString = oReader.ReadString16(nLen);
				break;
			}
			case ctGradientStroke:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = oReader.ReadInt();
				if (0 == gradientType)	//	linearGradient
				{
					double x1 = oReader.ReadDouble();
					double x2 = oReader.ReadDouble();
					double y1 = oReader.ReadDouble();
					double y2 = oReader.ReadDouble();

					int stops = oReader.ReadInt();

					for (int i = 0; i < stops; ++i)
					{
						INT32 color    = static_cast<INT32>(oReader.ReadByte());
						double opacity = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						oReader.Skip(6 * 4 - 3);
					}
				}
				else if (1 == gradientType)
				{
					double cx = oReader.ReadDouble();
					double cy = oReader.ReadDouble();
					double r  = oReader.ReadDouble();
					double fx = oReader.ReadDouble();
					double fy = oReader.ReadDouble();

					int stops = oReader.ReadInt();

					for (int i = 0; i < stops; ++i)
					{
						INT32 color    = static_cast<INT32>(oReader.ReadByte());
						double opacity = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						double offset  = static_cast<double>(static_cast<INT32>(oReader.ReadByte())) / 255.0;
						oReader.Skip(6 * 4 - 3);
					}
				}
				break;
			}
			case ctGradientStrokeXML:
			{
				// TODO: Эта команда не должна приходить
				INT32 gradientType = oReader.ReadInt();
				int nLen = (int)oReader.ReadInt();
				std::wstring wsTempString = oReader.ReadString16(nLen);
				break;
			}
			// дополнительные команды. из-за совместимости версий не можем менят формат.
			// но все следующие - должны быть по одной схеме
			case ctHyperlink:
			{
				if (S_OK == pRenderer->IsSupportAdvancedCommand(IAdvancedCommand::AdvancedCommandType::Hyperlink))
				{
					IAdvancedCommand* pCommand = oReader.Read(ctHyperlink, pCorrector);
					if (pCommand)
					{
						pRenderer->AdvancedCommand(pCommand);
						delete pCommand;
					}
				}
				else
				{
					oReader.SkipDouble(4);
					oReader.SkipString();
					oReader.SkipString();
				}
				break;
			}
			case ctLink:
			{
				if (S_OK == pRenderer->IsSupportAdvancedCommand(IAdvancedCommand::AdvancedCommandType::Link))
				{
					IAdvancedCommand* pCommand = oReader.Read(ctLink, pCorrector);
					if (pCommand)
					{
						pRenderer->AdvancedCommand(pCommand);
						delete pCommand;
					}
				}
				else
				{
					oReader.SkipDouble(6);
					oReader.SkipInt();
				}
				break;
			}
			case ctDocInfo:
			{
				if (S_OK == pRenderer->IsSupportAdvancedCommand(IAdvancedCommand::AdvancedCommandType::DocInfo))
				{
					IAdvancedCommand* pCommand = oReader.Read(ctDocInfo, pCorrector);
					pRenderer->AdvancedCommand(pCommand);
					delete pCommand;
				}
				else
				{
					int nFlags = oReader.ReadInt();
					if (nFlags & 1)
						oReader.SkipString();
					if (nFlags & 2)
						oReader.SkipString();
					if (nFlags & 4)
						oReader.SkipString();
					if (nFlags & 8)
						oReader.SkipString();
				}
				break;
			}
			case ctFormField:
			case ctAnnotField:
			case ctAnnotFieldDelete:
			case ctWidgetsInfo:
			case ctShapeStart:
			case ctShapeEnd:
			case ctPageClear:
			case ctPageRotate:
			case ctHeadings:
			{
				IAdvancedCommand::AdvancedCommandType eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::Undefined;
				switch (eCommand)
				{
				case ctFormField:        eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::FormField;   break;
				case ctAnnotField:       eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::Annotaion;   break;
				case ctAnnotFieldDelete: eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::DeleteAnnot; break;
				case ctWidgetsInfo:      eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::WidgetsInfo; break;
				case ctShapeStart:       eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::ShapeStart;  break;
				case ctShapeEnd:         eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::ShapeEnd;    break;
				case ctPageClear:        eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::PageClear;   break;
				case ctPageRotate:       eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::PageRotate;  break;
				case ctHeadings:         eAdvancedCommandType = IAdvancedCommand::AdvancedCommandType::Headings;    break;
				default:
					break;
				}

				BYTE* cur = oReader.GetCurrentBuffer();
				int nLen = oReader.ReadInt();

				if ((IAdvancedCommand::AdvancedCommandType::Undefined != eAdvancedCommandType) &&
					(S_OK == pRenderer->IsSupportAdvancedCommand(eAdvancedCommandType)))
				{
					IAdvancedCommand* pCommand = oReader.Read(eCommand, pCorrector);
					if (pCommand)
					{
						pRenderer->AdvancedCommand(pCommand);
						delete pCommand;
					}
				}

				oReader.SetCurrentBuffer(cur + nLen);
				break;
			}
			default:
			{
				BYTE* cur = oReader.GetCurrentBuffer();
				oReader.SetCurrentBuffer(cur + oReader.ReadInt());
				break;
			}
			}; // switch (eCommand)
		}

		return true;
	}

	void CMetafilePagesInfo::CheckBuffer(BYTE* pBuffer, LONG lBufferLen)
	{
		CommandType eCommand = ctError;

		bool bIsPathOpened = false;
		CBufferReader oReader(pBuffer, lBufferLen);
		while (oReader.Check())
		{
			eCommand = (CommandType)(oReader.ReadByte());
			switch (eCommand)
			{
			case ctPageWidth:
			{
				arSizes[PagesCount - 1].width = oReader.ReadDouble();
				break;
			}
			case ctPageHeight:
			{
				arSizes[PagesCount - 1].height = oReader.ReadDouble();
				break;
			}
			case ctPageStart:
			{
				AddPage();
				arSizes[PagesCount - 1].data = oReader.GetCurrentBuffer();
				break;
			}
			case ctPageEnd:
			{
				break;
			}
			case ctPenColor:
			{
				oReader.SkipInt();
				break;
			}
			case ctPenAlpha:
			{
				oReader.Skip(1);
				break;
			}
			case ctPenSize:
			{
				oReader.SkipInt();
				break;
			}
			case ctPenDashStyle:
			{
				BYTE nDashType = oReader.ReadByte();
				switch (nDashType)
				{
				case Aggplus::DashStyleCustom:
				{
					int nCountDash = oReader.ReadInt();
					if (0 < nCountDash)
						oReader.SkipInt(nCountDash);
				}
				default:
					break;
				}

				break;
			}
			case ctPenLineJoin:
			{
				oReader.Skip(1);
				break;
			}
			case ctBrushType:
			{
				oReader.SkipInt();
				break;
			}
			case ctBrushColor1:
			{
				oReader.SkipInt();
				break;
			}
			case ctBrushAlpha1:
			{
				oReader.Skip(1);
				break;
			}
			case ctBrushColor2:
			{
				oReader.SkipInt();
				break;
			}
			case ctBrushAlpha2:
			{
				oReader.Skip(1);
				break;
			}
			case ctBrushRectable:
			{
				oReader.SkipInt(4);
				break;
			}
			case ctBrushRectableEnabled:
			{
				oReader.Skip(1);
				break;
			}
			case ctBrushTexturePathOld:
			{
				int nLen = 2 * oReader.ReadUShort();
				oReader.SkipString16(nLen);
				break;
			}
			case ctBrushTexturePath:
			{
				int nLen = oReader.ReadInt();
				oReader.SkipString16(nLen);
				break;
			}
			case ctBrushGradient:
			{
				oReader.Skip(1);

				while (true)
				{
					BYTE _command = oReader.ReadByte();
					if (251 == _command)
						break;

					switch (_command)
					{
					case 0:
					{
						oReader.Skip(5 + (4 * 4));
						break;
					}
					case 1:
					{
						oReader.Skip(1 + (6 * 4));
						break;
					}
					case 2:
					{
						LONG lColorsCount = (LONG)oReader.ReadInt();
						if (0 <= lColorsCount)
							oReader.SkipInt(2 * lColorsCount);
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
				oReader.Skip(1);
				break;
			}
			case ctBrushTextureAlpha:
			{
				oReader.Skip(1);
				break;
			}
			case ctBrushResetRotation:
			{
				break;
			}
			case ctSetTransform:
			{
				oReader.SkipInt(6);
				break;
			}
			case ctPathCommandStart:
			{
				break;
			}
			case ctPathCommandMoveTo:
			{
				oReader.SkipInt(2);
				break;
			}
			case ctPathCommandLineTo:
			{
				oReader.SkipInt(2);
				break;
			}
			case ctPathCommandCurveTo:
			{
				oReader.SkipInt(6);
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
			case ctPathCommandOffset:
			{
				oReader.SkipInt(2);
				break;
			}
			case ctPathCommandScale:
			{
				oReader.SkipInt(2);
				break;
			}
			case ctDrawPath:
			{
				oReader.SkipInt();
				break;
			}
			case ctDrawImageFromFile:
			{
				int nLen = oReader.ReadInt();
				oReader.SkipString16(nLen);
				oReader.SkipInt(4);
				break;
			}
			case ctFontName:
			{
				int nLen = 2 * (int)oReader.ReadUShort();
				oReader.SkipString16(nLen);
				break;
			}
			case ctFontSize:
			{
				oReader.SkipInt();
				break;
			}
			case ctFontStyle:
			{
				oReader.SkipInt();
				break;
			}
			case ctDrawText:
			{
				int nLen = 2 * (int)oReader.ReadUShort();
				oReader.SkipString16(nLen);
				oReader.SkipInt(2);
				break;
			}
			case ctDrawTextCodeGid:
			{
				oReader.SkipInt();
				oReader.SkipDouble(2);
				int nCountUnicodes = oReader.ReadInt();
				oReader.SkipInt(nCountUnicodes);
				break;
			}
			case ctBeginCommand:
			{
				oReader.SkipInt();
				break;
			}
			case ctEndCommand:
			{
				oReader.SkipInt();
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
				oReader.SkipDouble(4);
				oReader.SkipString();
				oReader.SkipString();
				break;
			}
			case ctLink:
			{
				oReader.SkipDouble(6);
				oReader.SkipInt();
				break;
			}
			case ctDocInfo:
			{
				int nFlags = oReader.ReadInt();

				if (nFlags & 1)
					oReader.SkipString();
				if (nFlags & 2)
					oReader.SkipString();
				if (nFlags & 4)
					oReader.SkipString();
				if (nFlags & 8)
					oReader.SkipString();
				break;
			}
			case ctFormField:
			case ctAnnotField:
			case ctAnnotFieldDelete:
			case ctWidgetsInfo:
			case ctShapeStart:
			case ctShapeEnd:
			case ctPageRotate:
			case ctHeadings:
			default:
			{
				BYTE* cur = oReader.GetCurrentBuffer();
				oReader.SetCurrentBuffer(cur + oReader.ReadInt());
				break;
			}
			}; // switch (eCommand)
		} // while (curindex < len)
	}
}
