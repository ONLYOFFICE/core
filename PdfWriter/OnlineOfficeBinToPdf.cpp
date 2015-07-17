#include "OnlineOfficeBinToPdf.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/String.h"

#ifndef _INT32
typedef int _INT32;
#endif

#ifndef SHORT
typedef unsigned short USHORT;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

namespace NSOnlineOfficeBinToPdf
{
	enum CommandType
	{
		ctPenXML						= 0,
		ctPenColor						= 1,
		ctPenAlpha						= 2,
		ctPenSize						= 3,
		ctPenDashStyle					= 4,
		ctPenLineStartCap				= 5,
		ctPenLineEndCap				    = 6,
		ctPenLineJoin					= 7,
		ctPenDashPatern				    = 8,
		ctPenDashPatternCount			= 9,
		ctPenDashOffset				    = 10,
		ctPenAlign						= 11,
		ctPenMiterLimit				    = 12,

		// brush
		ctBrushXML						= 20,
		ctBrushType					    = 21,
		ctBrushColor1					= 22,
		ctBrushColor2					= 23,
		ctBrushAlpha1					= 24,
		ctBrushAlpha2					= 25,
		ctBrushTexturePath				= 26,
		ctBrushTextureAlpha			    = 27,
		ctBrushTextureMode				= 28,
		ctBrushRectable				    = 29,
		ctBrushRectableEnabled 		    = 30,
		ctBrushGradient                 = 31,

		// font
		ctFontXML						= 40,
		ctFontName						= 41,
		ctFontSize						= 42,
		ctFontStyle					    = 43,
		ctFontPath						= 44,
		ctFontGID						= 45,
		ctFontCharSpace				    = 46,

		// shadow
		ctShadowXML					    = 50,
		ctShadowVisible				    = 51,
		ctShadowDistanceX				= 52,
		ctShadowDistanceY				= 53,
		ctShadowBlurSize				= 54,
		ctShadowColor					= 55,
		ctShadowAlpha					= 56,

		// edge
		ctEdgeXML						= 70,
		ctEdgeVisible					= 71,
		ctEdgeDistance					= 72,
		ctEdgeColor					    = 73,
		ctEdgeAlpha					    = 74,

		// text
		ctDrawText						= 80,
		ctDrawTextEx					= 81,

		// pathcommands
		ctPathCommandMoveTo			    = 91,
		ctPathCommandLineTo			    = 92,
		ctPathCommandLinesTo			= 93,
		ctPathCommandCurveTo			= 94,
		ctPathCommandCurvesTo			= 95,
		ctPathCommandArcTo		        = 96,
		ctPathCommandClose				= 97,
		ctPathCommandEnd				= 98,
		ctDrawPath						= 99,
		ctPathCommandStart				= 100,
		ctPathCommandGetCurrentPoint	= 101,
		ctPathCommandText				= 102,
		ctPathCommandTextEx			    = 103,

		// image
		ctDrawImage					    = 110,
		ctDrawImageFromFile			    = 111,

		ctSetParams					    = 120,

		ctBeginCommand					= 121,
		ctEndCommand					= 122,

		ctSetTransform					= 130,
		ctResetTransform				= 131,

		ctClipMode						= 140,

		ctCommandLong1					= 150,
		ctCommandDouble1				= 151,
		ctCommandString1				= 152,
		ctCommandLong2					= 153,
		ctCommandDouble2				= 154,
		ctCommandString2				= 155,

		ctPageWidth                     = 200,
		ctPageHeight                    = 201,

		ctPageStart                     = 202,
		ctPageEnd                       = 203,

		// gradients

		ctGradientFill					= 220,
		ctGradientFillXML				= 221,

		ctGradientStroke				= 222,
		ctGradientStrokeXML				= 223,

		ctError						    = 255
	};
	static inline _INT32       ReadInt(BYTE*& pData, int& nOffset)
	{
#ifdef _ARM_ALIGN_
		_INT32 ret = 0;
		memcpy(&ret, pData, sizeof(_INT32));
		pData   += 4;
		nOffset += 4;
		return ret;
#else
		_INT32 ret = *((_INT32*)pData);
		pData   += 4;
		nOffset += 4;
		return ret;
#endif
	}
	static inline USHORT       ReadUSHORT(BYTE*& pData, int& nOffset)
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
	static inline std::wstring ReadString16(BYTE*& pData, int& nOffset, int nLen)
	{
		std::wstring wsTempString;
#ifdef _ARM_ALIGN_

		if (sizeof(wchar_t) == 4)
		{
#if !defined(_WIN32) && !defined(_WIN64)
			int len = nLen / 2;
			UTF16* buf = new UTF16[len];
			memcpy(buf, pData, nLen);
			wsTempString = stringUtf16ToWString(buf, len);
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
			wsTempString = stringUtf16ToWString((UTF16*)pData, nLen / 2);
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
	static std::wstring        ReadImagePath(CPdfRenderer* pPdf, const std::wstring& wsHtmlPlace, BYTE*& pData, int& nOffset)
	{
		int _sLen = 2 * ReadUSHORT(pData, nOffset);
		std::wstring wsTempString = ReadString16(pData, nOffset, _sLen);

		if (0 == wsTempString.find(L"data:"))
		{
			try
			{
				int nFind = wsTempString.find(L",");
				wsTempString = wsTempString.substr(nFind + 1);

				std::wstring wsBase64TempFile = pPdf->GetTempFile();
				std::string sBase64MultyByte(wsTempString.begin(), wsTempString.end());

				int nBufferLen = NSBase64::Base64DecodeGetRequiredLength(sBase64MultyByte.length());
				BYTE* pImageBuffer = new BYTE[nBufferLen + 64];

				if (NSBase64::Base64Decode(sBase64MultyByte.c_str(), sBase64MultyByte.length(), pImageBuffer, &nBufferLen))
				{
					NSFile::CFileBinary oFile;
					if (oFile.CreateFileW(wsBase64TempFile))
					{
						oFile.WriteFile(pImageBuffer, nBufferLen);
						oFile.CloseFile();
						wsTempString = wsBase64TempFile;
					}
				}
				else throw;
			}
			catch (...)
			{
			}
		}
		else
		{
			if (0 != wsTempString.find(L"http:")
				&& 0 != wsTempString.find(L"https:")
				&& 0 != wsTempString.find(L"ftp:")
				&& 0 != wsTempString.find(L"file:"))
			{
				if (0 == wsTempString.find(L"theme"))
				{
					std::wstring wsThemesPlace = pPdf->GetThemesPlace();
					if (L"" != wsThemesPlace)
						wsTempString = wsThemesPlace + L"/" + wsTempString;
				}
				else
				{
					if (wsHtmlPlace.length() > 0)
						wsTempString = wsHtmlPlace + L"/" + wsTempString;

					std::wstring wsSvgExt(L".svg");
					if (0 == wsTempString.compare(wsTempString.length() - wsSvgExt.length(), std::wstring::npos, wsSvgExt))
					{
						std::wstring wsTestPath = wsTempString.substr(0, wsTempString.length() - wsSvgExt.length());
						if (NSFile::CFileBinary::Exists(wsTestPath + L".emf"))
							wsTempString = wsTestPath + L".emf";
						else if (NSFile::CFileBinary::Exists(wsTestPath + L".wmf"))
							wsTempString = wsTestPath + L".wmf";
					}
				}

				NSStringExt::Replace(wsTempString, L"\\", L"/");
			}

			if (0 == wsTempString.find(L"file:///"))
			{
				NSStringExt::Replace(wsTempString, L"file:///", L"");
				NSStringExt::Replace(wsTempString, L"\\", L"/");
			}
		}

		return wsTempString;
	}
	static bool                ConvertBufferToPdf(CPdfRenderer* pPdf, BYTE* pBuffer, LONG lBufferLen, const std::wstring& wsHtmlPlace)
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
					pPdf->put_Width(ReadInt(current, curindex) / 100000.0);
					break;
				}
				case ctPageHeight:
				{
					pPdf->put_Height(ReadInt(current, curindex) / 100000.0);
					break;
				}
				case ctPageStart:
				{
					pPdf->NewPage();
					pPdf->BeginCommand(c_nPageType);
					break;
				}
				case ctPageEnd:
				{
					if (bIsPathOpened)
					{
						pPdf->PathCommandEnd();
						pPdf->EndCommand(c_nPathType);
					}
					bIsPathOpened = false;

					pPdf->EndCommand(c_nPageType);

					break;
				}
				case ctPenColor:
				{
					pPdf->put_PenColor(ReadInt(current, curindex));
					break;
				}
				case ctPenAlpha:
				{
					pPdf->put_PenAlpha(*current);
					current++;
					curindex++;
					break;
				}
				case ctPenSize:
				{
					pPdf->put_PenSize(ReadInt(current, curindex) / 100000.0);
					break;
				}
				case ctPenLineJoin:
				{
					pPdf->put_PenLineJoin(*current);
					current++;
					curindex++;
					break;
				}
				case ctBrushType:
				{
					pPdf->put_BrushType(ReadInt(current, curindex));
					break;
				}
				case ctBrushColor1:
				{
					pPdf->put_BrushColor1(ReadInt(current, curindex));
					break;
				}
				case ctBrushAlpha1:
				{
					pPdf->put_BrushAlpha1(*current);
					current++;
					curindex++;
					break;
				}
				case ctBrushColor2:
				{
					pPdf->put_BrushColor1(ReadInt(current, curindex));
					break;
				}
				case ctBrushAlpha2:
				{
					pPdf->put_BrushAlpha2(*current);
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
					pPdf->BrushRect(0, m1, m2, m3, m4);
					break;
				}
				case ctBrushRectableEnabled:
				{
					bool bEnable = (1 == *current) ? true : false;
					pPdf->EnableBrushRect(bEnable);

					current += 1;
					curindex += 1;
					break;
				}
				case ctBrushTexturePath:
				{
					std::wstring wsTempString = ReadImagePath(pPdf, wsHtmlPlace, current, curindex);
					pPdf->put_BrushTexturePath(wsTempString);
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
								pPdf->SetLinearGradient(x0, y0, x1, y1);
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
								pPdf->SetRadialGradient(x0, y0, r0, x1, y1, r1);
								break;
							}
							case 2:
							{
								LONG lColorsCount = (LONG)ReadInt(current, curindex);
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

								pPdf->put_BrushGradientColors(pColors, pPositions, lColorsCount);

								delete[] pColors;
								delete[] pPositions;

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
					pPdf->put_BrushTextureMode(lMode);

					current += 1;
					curindex += 1;
					break;
				}
				case ctBrushTextureAlpha:
				{
					LONG lAlpha = (LONG)(*current);
					pPdf->put_BrushTextureAlpha(lAlpha);

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
					pPdf->SetTransform(m1, m2, m3, m4, m5, m6);
					break;
				}
				case ctPathCommandStart:
				{
					if (bIsPathOpened)
					{
						pPdf->PathCommandEnd();
						pPdf->EndCommand(c_nPathType);
					}

					pPdf->BeginCommand(c_nPathType);
					pPdf->PathCommandStart();

					bIsPathOpened = true;
					break;
				}
				case ctPathCommandMoveTo:
				{
					double m1 = ReadInt(current, curindex) / 100000.0;
					double m2 = ReadInt(current, curindex) / 100000.0;
					pPdf->PathCommandMoveTo(m1, m2);
					break;
				}
				case ctPathCommandLineTo:
				{
					double m1 = ReadInt(current, curindex) / 100000.0;
					double m2 = ReadInt(current, curindex) / 100000.0;
					pPdf->PathCommandLineTo(m1, m2);
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
					pPdf->PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
					break;
				}
				case ctPathCommandClose:
				{
					pPdf->PathCommandClose();
					break;
				}
				case ctPathCommandEnd:
				{
					if (bIsPathOpened)
					{
						pPdf->PathCommandEnd();
						pPdf->EndCommand(c_nPathType);
						bIsPathOpened = false;
					}
					break;
				}
				case ctDrawPath:
				{
					pPdf->DrawPath(ReadInt(current, curindex));
					break;
				}
				case ctDrawImageFromFile:
				{
					std::wstring wsTempString = ReadImagePath(pPdf, wsHtmlPlace, current, curindex);
					double m1 = ReadInt(current, curindex) / 100000.0;
					double m2 = ReadInt(current, curindex) / 100000.0;
					double m3 = ReadInt(current, curindex) / 100000.0;
					double m4 = ReadInt(current, curindex) / 100000.0;
					pPdf->DrawImageFromFile(wsTempString, m1, m2, m3, m4);
					break;
				}
				case ctFontName:
				{
					int _sLen = 2 * (int)ReadUSHORT(current, curindex);
					std::wstring wsTempString = ReadString16(current, curindex, _sLen);
					pPdf->put_FontName(wsTempString);
					break;
				}
				case ctFontSize:
				{
					double m1 = ReadInt(current, curindex) / 100000.0;
					pPdf->put_FontSize(m1);
					break;
				}
				case ctFontStyle:
				{
					pPdf->put_FontStyle(ReadInt(current, curindex));
					break;
				}
				case ctDrawText:
				{
					int _sLen = 2 * (int)ReadUSHORT(current, curindex);
					std::wstring wsTempString = ReadString16(current, curindex, _sLen);

					double m1 = ReadInt(current, curindex) / 100000.0;
					double m2 = ReadInt(current, curindex) / 100000.0;

					pPdf->CommandDrawText(wsTempString, m1, m2, 0, 0);
					break;
				}
				case ctBeginCommand:
				{
					if (bIsPathOpened)
					{
						pPdf->PathCommandEnd();
						pPdf->EndCommand(4);
						bIsPathOpened = false;
					}
					pPdf->BeginCommand((DWORD)(ReadInt(current, curindex)));
					break;
				}
				case ctEndCommand:
				{
					if (bIsPathOpened)
					{
						pPdf->PathCommandEnd();
						pPdf->EndCommand(4);
						bIsPathOpened = false;
					}
					pPdf->EndCommand((DWORD)(ReadInt(current, curindex)));
					pPdf->PathCommandEnd();
					break;
				}
				case ctGradientFill:
				{
					// TODO: Эта команда не должна приходить
					_INT32 gradientType = ReadInt(current, curindex);

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
							_INT32 color   = static_cast<_INT32>(*current);
							double opacity = static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
							double offset  = static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;

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
							_INT32 color   = static_cast<_INT32>(*current);
							double opacity = static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
							double offset  = static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;

							current  += 6 * 4;		//	4 + 1 + 1
							curindex += 6 * 4;
						}
					}
					break;
				}
				case ctGradientFillXML:
				{
					// TODO: Эта команда не должна приходить
					_INT32 gradientType = ReadInt(current, curindex);
					int _sLen = ReadInt(current, curindex);
					std::wstring wsTempString = ReadString16(current, curindex, _sLen);
					break;
				}
				case ctGradientStroke:
				{
					// TODO: Эта команда не должна приходить
					_INT32 gradientType = ReadInt(current, curindex);
					if (0 == gradientType)	//	linearGradient
					{
						double x1 = ReadInt(current, curindex) / 100000.0;
						double x2 = ReadInt(current, curindex) / 100000.0;
						double y1 = ReadInt(current, curindex) / 100000.0;
						double y2 = ReadInt(current, curindex) / 100000.0;
						int stops = ReadInt(current, curindex);

						for (int i = 0; i < stops; ++i)
						{
							_INT32 color   = static_cast<_INT32>(*current);
							double opacity = static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
							double offset  = static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;

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
							_INT32 color   = static_cast<_INT32>(*current);
							double opacity = static_cast<double>(static_cast<_INT32>(*(current + 1))) / 255.0;
							double offset  = static_cast<double>(static_cast<_INT32>(*(current + 2))) / 255.0;

							current  += 6 * 4; // 4 + 1 + 1
							curindex += 6 * 4;
						}
					}
					break;
				}
				case ctGradientStrokeXML:
				{
					// TODO: Эта команда не должна приходить
					_INT32 gradientType = ReadInt(current, curindex);
					int _sLen = (int)ReadInt(current, curindex);
					std::wstring wsTempString = ReadString16(current, curindex, _sLen);
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
	bool ConvertBinToPdf(CPdfRenderer* pPdf, const std::wstring& wsSrcFile, const std::wstring& wsDstFile, bool bBinary)
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

		std::wstring wsHtmlPlace = NSDirectory::GetFolderPath(wsSrcFile);
		if (bBinary)
		{
			ConvertBufferToPdf(pPdf, pFileContent, dwFileSize, wsHtmlPlace);
		}
		else
		{
			int   nBufferLen = NSBase64::Base64DecodeGetRequiredLength(dwFileSize);
			BYTE* pBuffer    = new BYTE[nBufferLen];
			if (!pBuffer)
			{
				RELEASEARRAYOBJECTS(pFileContent);
				return false;
			}

			if (NSBase64::Base64Decode((const char*)pFileContent, dwFileSize, pBuffer, &nBufferLen))
			{
				ConvertBufferToPdf(pPdf, pBuffer, nBufferLen, wsHtmlPlace);
			}
			else
			{
				RELEASEARRAYOBJECTS(pBuffer);
				RELEASEARRAYOBJECTS(pFileContent);
				return false;
			}

			RELEASEARRAYOBJECTS(pBuffer);
		}
		RELEASEARRAYOBJECTS(pFileContent);

		pPdf->SaveToFile(wsDstFile);

		return true;
	}
}