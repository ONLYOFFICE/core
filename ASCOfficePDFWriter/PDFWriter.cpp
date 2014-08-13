#include "stdafx.h"
#include "PDFWriter.h"

HRESULT CPDFWriter::OnlineWordToPdf (BSTR sPathXml, BSTR sDstFile, BSTR sHtmlPlace, LONG nReg)
{
	using namespace NOnlineOfficeBinToPdf;
	HRESULT hRes = S_OK;

	NOnlineOfficeBinToPdf::CommandType eCommand = NOnlineOfficeBinToPdf::ctError;
	CString sTempLogo;

	AVSGraphics::IASCWinFonts *piWinFonts = NULL;

	try
	{
		hRes = CoCreateInstance (AVSGraphics::CLSID_CASCWinFonts, NULL, CLSCTX_ALL, AVSGraphics::IID_IASCWinFonts, (void **) &piWinFonts);

#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
		piWinFonts->Init(L"", TRUE, TRUE);
#endif

		if (S_OK != hRes)
			throw "Can't create IAVSWinFonts object!";

		hRes = CreatePDF();
		if (FAILED (hRes))
			throw "CreatePDF failed!";

		hRes = SetPDFCompressionMode(15);
		if (FAILED (hRes))
			throw "SetPDFCompressionMode(15) failed!";

		bool bIsPathOpened = false;

		int nCountPages = 0;
		CString sHypers;
		sHypers.Append(_T("<linker>"));

		// read file
		CString sOpenPathName;
		LPTSTR pOpenPathName = sOpenPathName.GetBufferSetLength (MAX_PATH);
		GetLongPathName (sPathXml, pOpenPathName, MAX_PATH);
		sOpenPathName.ReleaseBuffer();

		//FILE *pFile = _wfopen (sOpenPathName, L"r, ccs=UTF-8");
		HANDLE hFile = ::CreateFile (sOpenPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			throw "OnlineWordToPdf: Opening input file failed!";


		DWORD dwFileSize = ::GetFileSize(hFile, NULL);
		CStringA aFileContent;
		LPSTR pFileContent = aFileContent.GetBufferSetLength (dwFileSize);
		DWORD dwRead;
		BOOL bReadRes = ::ReadFile (hFile, pFileContent, dwFileSize, &dwRead, NULL);
		aFileContent.ReleaseBuffer();
		::CloseHandle (hFile);
		if (dwRead != dwFileSize)
		{
			throw "OnlineWordToPdf: wrong file read";
		}

		CAtlArray<CStringA> asResFile;

		// here we can to divide file into some lines of text (divided by '\r' symbol)
		asResFile.Add (aFileContent);
		aFileContent.Empty();

		for (size_t nLine = 0; nLine < asResFile.GetCount(); ++nLine)
		{
			CPDFWriter &oWriter = *this;

			DWORD len (0);
			BYTE *dstArray = NULL;

			if (DecodeBase64 (asResFile[nLine], &dstArray, &len)
				&& (NULL != dstArray))
			{
				ATLTRACE2 ("** new Base64line ***\n");
				if (true)
				{
					int curindex = 0;

					INT32* m = NULL;
					USHORT* ms = NULL;
					int _sLen = 0;
					CString s;
					INT32 gradientType = 0;

					double m1 = 0;
					double m2 = 0;
					double m3 = 0;
					double m4 = 0;
					double m5 = 0;
					double m6 = 0;

					BYTE* p = dstArray;
					{
						BYTE* current = p;
						while (curindex < len)
						{
							eCommand = (CommandType)(*current);
							current++;
							curindex++;
							switch (eCommand)
							{
							case ctPageWidth:
								ATLTRACE2 ("CommandType::ctPageWidth:\n");
								m = (INT32*) current;
								current += 4;
								curindex += 4;
								oWriter.put_Width ((*m) / 100000.0);
								break;
							case ctPageHeight:
								ATLTRACE2 ("CommandType::ctPageHeight:\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;
								oWriter.put_Height ((*m) / 100000.0);
								break;
							case ctPageStart:
								ATLTRACE2 ("\nCommandType::ctPageStart:\n");
								oWriter.NewPage();
								oWriter.BeginCommand(1);
								++nCountPages;
								break;
							case ctPageEnd:
								ATLTRACE2 ("CommandType::ctPageEnd:\n");
								if (bIsPathOpened)
								{
									oWriter.PathCommandEnd();
									oWriter.EndCommand(4);
								}

								bIsPathOpened = false;

								if (!nReg)
								{
									double ww;
									oWriter.get_Width(&ww);
									double wh;
									oWriter.get_Height(&wh);

									double dR = ww - 13;
									double dB = wh - 5;
									double k = 1.0;

									if (ww <= wh)
									{
										// ориентация книжная. 
										double k1 = ww / 210;
										double k2 = wh / 297;
										k = min(k1, k2);
									}
									else
									{
										double k1 = ww / 297;
										double k2 = wh / 210;
										k = min(k1, k2);
									}
									double dW = 46 * k;
									double dH = 15 * k;

									double dKoef = 72 / 25.4;

									CString sTempHyper;
									sTempHyper.Format(_T("<link><source x=\"%i\" y=\"%i\" width=\"%i\" height=\"%i\" page=\"%i\"/>"), (int)((dR - dW) * dKoef), (int)((dB - dH) * dKoef), (int)(dW * dKoef), (int)(dH * dKoef), nCountPages - 1);

									sHypers += sTempHyper;
									sHypers += _T("<target url=\"www.yourdocs.com\"/></link>");

									oWriter.ResetTransform();

									BOOL bWriteTextFile = TRUE;
									if (sTempLogo.IsEmpty())
									{
										sTempLogo = GetTempFileName();
										bWriteTextFile = WriteFile (sTempLogo, (BYTE*) cpLogo1);
									}
									if (bWriteTextFile)
									{
										BSTR bstrTempLogo = sTempLogo.AllocSysString();
										DrawImageFromFile (bstrTempLogo, dR - dW, dB - dH, dW, dH);
										SysFreeString (bstrTempLogo);
									}
								}

								oWriter.EndCommand(1);
								break;
							case ctPenColor:
								ATLTRACE2 ("CommandType::ctPenColor:\n");
								m = (INT32*)current;
								oWriter.put_PenColor (*m);
								current += 4;
								curindex += 4;
								break;
							case ctPenAlpha:
								ATLTRACE2 ("CommandType::ctPenAlpha:\n");
								oWriter.put_PenAlpha (*current);
								current++;
								curindex++;
								break;
							case ctPenSize:
								ATLTRACE2 ("CommandType::ctPenSize:\n");
								m = (INT32*)current;
								oWriter.put_PenSize (*m / 100000.0);
								current += 4;
								curindex += 4;
								break;
							case ctPenLineJoin:
								ATLTRACE2 ("CommandType::ctPenLineJoin:\n");
								oWriter.put_PenLineJoin (*current);
								current++;
								curindex++;
								break;
							case ctBrushType:
								ATLTRACE2 ("CommandType::ctBrushType:\n");
								m = (INT32*)current;
								oWriter.put_BrushType (*m);
								current += 4;
								curindex += 4;
								break;
							case ctBrushColor1:
								ATLTRACE2 ("CommandType::ctBrushColor1:\n");
								m = (INT32*)current;
								oWriter.put_BrushColor1 (*m);
								current += 4;
								curindex += 4;
								break;
							case ctBrushAlpha1:
								ATLTRACE2 ("CommandType::ctBrushAlpha1:\n");
								oWriter.put_BrushAlpha1 (*current);
								current++;
								curindex++;
								break;
							case ctBrushColor2:
								ATLTRACE2 ("CommandType::ctBrushColor2:\n");
								m = (INT32*)current;
								oWriter.put_BrushColor1 (*m);
								current += 4;
								curindex += 4;
								break;
							case ctBrushAlpha2:
								ATLTRACE2 ("CommandType::ctBrushAlpha2:\n");
								oWriter.put_BrushAlpha2 (*current);
								current++;
								curindex++;
								break;
							case ctBrushRectable:
								ATLTRACE2 ("CommandType::ctBrushRectable:\n");

								m = (INT32*)current;
								current += 4 * 4;
								curindex += 4 * 4;

								m1 = (*m++) / 100000.0;
								m2 = (*m++) / 100000.0;
								m3 = (*m++) / 100000.0;
								m4 = (*m++) / 100000.0;

								oWriter.BrushRect(0, m1, m2, m3, m4);
								break;
							case ctBrushRectableEnabled:
								{
									ATLTRACE2 ("CommandType::ctBrushRectableEnabled:\n");

									VARIANT bEn;
									bEn.vt = VT_BOOL;
									bEn.boolVal = (1 == *current) ? VARIANT_TRUE : VARIANT_FALSE;

									oWriter.SetAdditionalParam(L"BrushFillBoundsEnable", bEn);

									current += 1;
									curindex += 1;
								}
								break;
							case ctBrushTexturePath:
								{
									ATLTRACE2 ("CommandType::ctBrushTexturePath:\n");

									ms = (USHORT*)current;
									current += 2;
									curindex += 2;

									_sLen = (int)(*ms);
									CStringW sTempStr;
									sTempStr.Append((WCHAR*)current, _sLen);
									s = sTempStr;	
#ifdef _DEBUG
									// only for test
									//s = L"X:\\AVS\\Sources\\AVSOfficeStudio\\AVSOfficePDFWriter\\Tests\\TestSvgToPDF\\bin\\files\\coffe.jpg";
#endif
									CString imgPath = s;

									if (0 != s.Find(_T("http:")) &&
										0 != s.Find(_T("https:")) &&
										0 != s.Find(_T("ftp:")) &&
										0 != s.Find(_T("file:")))
									{
										if (CString(sHtmlPlace).GetLength())
											imgPath = CComBSTR (sHtmlPlace) + _T("/") + s;
										else
											imgPath = s;

										imgPath.Replace( _T("\\"), _T("/") );
									}

									if (0 == s.Find(_T("file:///")))
									{
										imgPath.Replace(_T("file:///"), _T(""));
										imgPath.Replace( _T("\\"), _T("/") );
									}

									CString base64TempFile;
									if (0 == s.Find(_T("data:")))
									{
										try
										{
											int nFind = s.Find(_T(","));
											s = s.Right(s.GetLength() - (nFind + 1));

											base64TempFile = GetTempFileName();
											CStringA sBase64MultyByte;
											sBase64MultyByte = s;
											BYTE *byteIm = NULL;
											DWORD nBuffLen = 0;
											bool bDecoderRes = DecodeBase64 (sBase64MultyByte, &byteIm, &nBuffLen);
											if (bDecoderRes)
											{
												WriteFile (base64TempFile, byteIm);
												imgPath = base64TempFile;
											}
											else throw;
										}
										catch (...)
										{
											ATLTRACE2 ("CommandType::ctBrushTexturePath:base64Decoder failed!\n");
										}
									}

									current += 2 * _sLen;
									curindex += 2 * _sLen;

									BSTR bstrImgPath = imgPath.AllocSysString();
									oWriter.put_BrushTexturePath(bstrImgPath);
									SysFreeString (bstrImgPath);

									if (!base64TempFile.IsEmpty())
									{												
										DeleteFile(base64TempFile);
									}
								}
								break;

							case ctBrushGradient:
								{
									current++;
									curindex++;

									CString strAttrMain = L"";
									CString strColors = L"";
									BOOL bIsLinear = TRUE;

									while (TRUE)
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

												m = (INT32*)current;
												current += 4 * 4;
												curindex += 4 * 4;

												double d1 = (*m++) / 100000.0;
												double d2 = (*m++) / 100000.0;
												double d3 = (*m++) / 100000.0;
												double d4 = (*m++) / 100000.0;

												strAttrMain.Format (L"x1 = '%f' y1 = '%f' x2 = '%f' y2 = '%f' gradientUnits='userSpaceOnUse' ", d1, d2, d3, d4);
												break;
											}
										case 1:
											{
												bIsLinear = false;

												current++;
												curindex++;

												m = (INT32*)current;
												current += 6 * 4;
												curindex += 6 * 4;

												double d1 = (*m++) / 100000.0;
												double d2 = (*m++) / 100000.0;
												double d3 = (*m++) / 100000.0;
												double d4 = (*m++) / 100000.0;
												double d5 = (*m++) / 100000.0;
												double d6 = (*m++) / 100000.0;

												strAttrMain.Format (L"cx = '%f' cy = '%f' r0 = '%f' r1 = '%f' rx = '%f' ry = '%f' gradientUnits='userSpaceOnUse' ", d1, d2, d5, d6, d1, d2);
												break;
											}
										case 2:
											{
												int nCountColors = *((INT32*)current);

												current += 4;
												curindex += 4;

												for (int nI = 0; nI < nCountColors; ++nI)
												{
													int pos = *((INT32*)current);
													current += 4;
													curindex += 4;

													double dPos = pos / 100000.0;
													byte _r = *current++;
													byte _g = *current++;
													byte _b = *current++;
													byte _a = *current++;

													curindex += 4;

													int _color = ((_b << 16) & 0xFF0000) | ((_g << 8) & 0xFF00) | _r;

													CString sColor;
													sColor.Format (L"<stop stop-color='%d' stop-opacity='%f' offset='%f' />", _color, _a / 255.0, dPos);
													strColors += sColor;
												}

												break;
											}
										default:
											break;
										};
									}

									CString strXml = L"";

									if (bIsLinear)
									{
										strXml = L"<linearGradient " + strAttrMain + L">" + strColors + L"</linearGradient>";

										VARIANT oVar;
										oVar.vt			=	VT_BSTR;
										oVar.bstrVal	=	CComBSTR(strXml);
										oWriter.SetAdditionalParam(L"Fill-LinearGradient", oVar);
									}
									else
									{
										strXml = L"<radialGradient " + strAttrMain + L">" + strColors + L"</radialGradient>";

										VARIANT oVar;
										oVar.vt			=	VT_BSTR;
										oVar.bstrVal	=	CComBSTR(strXml);
										oWriter.SetAdditionalParam(L"Fill-RadialGradient", oVar);
									}  
								}

								break;

							case ctBrushTextureMode:
								{
									ATLTRACE2 ("CommandType::ctBrushTextureMode:\n");

									LONG mode = (LONG)(*current);
									oWriter.put_BrushTextureMode(mode);

									current += 1;
									curindex += 1;
								}
								break;
							case ctBrushTextureAlpha:
								{
									ATLTRACE2 ("CommandType::ctBrushTextureAlpha:\n");

									LONG txalpha = (LONG)(*current);
									oWriter.put_BrushTextureAlpha(txalpha);

									current += 1;
									curindex += 1;
								}
								break;
							case ctSetTransform:
								ATLTRACE2 ("CommandType::ctSetTransform:\n");
								m = (INT32*)current;
								current += 6 * 4;
								curindex += 6 * 4;

								m1 = (*m++) / 100000.0;
								m2 = (*m++) / 100000.0;
								m3 = (*m++) / 100000.0;
								m4 = (*m++) / 100000.0;
								m5 = (*m++) / 100000.0;
								m6 = (*m++) / 100000.0;

								oWriter.SetTransform(m1, m2, m3, m4, m5, m6);
								break;
							case ctPathCommandStart:
								ATLTRACE2 ("CommandType::ctPathCommandStart:\n");
								if (bIsPathOpened)
								{
									oWriter.PathCommandEnd();
									oWriter.EndCommand(4);
									oWriter.BeginCommand(4);
									oWriter.PathCommandStart();
								}
								else
								{
									oWriter.BeginCommand(4);
									oWriter.PathCommandStart();
								}
								bIsPathOpened = true;
								break;
							case ctPathCommandMoveTo:
								ATLTRACE2 ("CommandType::ctPathCommandMoveTo:\n");
								m = (INT32*)current;
								current += 2 * 4;
								curindex += 2 * 4;

								m1 = (*m++) / 100000.0;
								m2 = (*m++) / 100000.0;
								oWriter.PathCommandMoveTo(m1, m2);
								break;
							case ctPathCommandLineTo:
								ATLTRACE2 ("CommandType::ctPathCommandLineTo:\n");
								m = (INT32*)current;
								current += 2 * 4;
								curindex += 2 * 4;

								m1 = (*m++) / 100000.0;
								m2 = (*m++) / 100000.0;
								oWriter.PathCommandLineTo(m1, m2);
								break;
							case ctPathCommandCurveTo:
								ATLTRACE2 ("CommandType::ctPathCommandCurveTo:\n");
								m = (INT32*)current;
								current += 6 * 4;
								curindex += 6 * 4;

								m1 = (*m++) / 100000.0;
								m2 = (*m++) / 100000.0;
								m3 = (*m++) / 100000.0;
								m4 = (*m++) / 100000.0;
								m5 = (*m++) / 100000.0;
								m6 = (*m++) / 100000.0;

								oWriter.PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
								break;
							case ctPathCommandClose:
								ATLTRACE2 ("CommandType::ctPathCommandClose:\n");
								oWriter.PathCommandClose();
								break;
							case ctDrawPath:
								ATLTRACE2 ("CommandType::ctDrawPath\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;

								oWriter.DrawPath(*m);
								break;
							case ctDrawImageFromFile:
								{
									ATLTRACE2 ("CommandType::ctDrawImageFromFile:\n");
									m = (INT32*)current;
									current += 2;
									curindex += 2;

									_sLen = (int)(*m);
									CStringW sTempStr;											
									sTempStr.Append((WCHAR*)current, _sLen);
									s = sTempStr;

									CString imgPath = s;

									if (0 != s.Find(_T("http:")) &&
										0 != s.Find(_T("https:")) &&
										0 != s.Find(_T("ftp:")) &&
										0 != s.Find(_T("file:")))
									{
										if (CString(sHtmlPlace).GetLength())
											imgPath = CComBSTR (sHtmlPlace) + _T("/") + s;
										else
											imgPath = s;

										imgPath.Replace( _T("\\"), _T("/") );
									}

									if (0 == s.Find(_T("file:///")))
									{
										imgPath.Replace(_T("file:///"), _T(""));
										imgPath.Replace( _T("\\"), _T("/") );
									}

									CString base64TempFile;
									if (0 == s.Find(_T("data:")))
									{
										try
										{
											int nFind = s.Find(_T(","));
											s = s.Right(s.GetLength() - (nFind + 1));

											base64TempFile = GetTempFileName();
											CStringA sBase64MultyByte;
											sBase64MultyByte = s;
											BYTE *byteIm = NULL;
											DWORD nBuffLen = 0;
											bool bDecoderRes = DecodeBase64 (sBase64MultyByte, &byteIm, &nBuffLen);
											if (bDecoderRes)
											{
												WriteFile (base64TempFile, byteIm);
												imgPath = base64TempFile;
											}
											else throw;
										}
										catch (...)
										{
											ATLTRACE2 ("CommandType::ctDrawImageFromFile:base64Decoder failed!\n");
										}
									}

									current += _sLen;
									curindex += _sLen;

									m = (INT32*)current;
									current += 4 * 4;
									curindex += 4 * 4;

									m1 = (*m++) / 100000.0;
									m2 = (*m++) / 100000.0;
									m3 = (*m++) / 100000.0;
									m4 = (*m++) / 100000.0;

									try
									{
										BSTR bstrImgPath = imgPath.AllocSysString();
										oWriter.DrawImageFromFile(bstrImgPath, m1, m2, m3, m4);
										SysFreeString (bstrImgPath);
									}
									catch (...)
									{
										ATLTRACE2 ("oWriter.DrawImageFromFile failed!\n");
									}

									if (!base64TempFile.IsEmpty())
									{
										DeleteFile(base64TempFile);
									}
								}
								break;
							case ctFontName:
								{
									ATLTRACE2 ("CommandType::ctFontName\n");
									ms = (USHORT*)current;
									current += 2;
									curindex += 2;

									_sLen = (int)(*ms);
									CStringW sTempFontNameW;
									sTempFontNameW.Append((WCHAR*)current, _sLen);
									s = sTempFontNameW;

									current += 2 * _sLen;
									curindex += 2 * _sLen;
									BSTR bstrFontName = s.AllocSysString();
									oWriter.put_FontName (bstrFontName);
									SysFreeString (bstrFontName);
								}
								break;
							case ctFontSize:
								ATLTRACE2 ("CommandType::ctFontSize:\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;

								m1 = (*m++) / 100000.0;
								oWriter.put_FontSize (m1);
								break;
							case ctFontStyle:
								ATLTRACE2 ("CommandType::ctFontStyle:\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;

								oWriter.put_FontStyle (*m);
								break;
							case ctDrawText:
								{
									ms = (USHORT*)current;
									current += 2;
									curindex += 2;

									_sLen = (int)(*ms);
									CStringW sTempDrawText;
									sTempDrawText.Append ((WCHAR*)current, _sLen);
									s = sTempDrawText;

									//CStringA aDebugText;
									//aDebugText = sTempDrawText;
									//ATLTRACE2 ("CommandType::ctDrawText: %s\n", aDebugText);

									current += 2 * _sLen;
									curindex += 2 * _sLen;

									m = (INT32*)current;
									current += 2 * 4;
									curindex += 2 * 4;

									m1 = (*m++) / 100000.0;
									m2 = (*m++) / 100000.0;

									BSTR bstrText = s.AllocSysString();
									oWriter.CommandDrawText(bstrText, m1, m2, 0, 0, 0);
									SysFreeString (bstrText);
								}
								break;
							case ctBeginCommand:
								ATLTRACE2 ("CommandType::ctBeginCommand:\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;
								if (bIsPathOpened)
								{
									oWriter.PathCommandEnd();
									oWriter.EndCommand(4);
									bIsPathOpened = false;
								}
								oWriter.BeginCommand((DWORD)(*m));
								break;
							case ctGradientFill:
								{
									gradientType	=	(INT32)(*current);
									current			+=	4;
									curindex		+=	4;

									CString sXml, sXmlStop;

									if (0 == gradientType)	//	linearGradient
									{
										m			=	(INT32*)current;
										current		+=	4 * 4;
										curindex	+=	4 * 4;

										double x1	=	(*m++) / 100000.0;
										double x2	=	(*m++) / 100000.0;						
										double y1	=	(*m++) / 100000.0;
										double y2	=	(*m++) / 100000.0;

										int stops	=	(INT32)(*current);
										current		+=	4;
										curindex	+=	4;

										sXml.Format(_T("<linearGradient x1='%f' x2='%f' y1='%f' y2='%f'>"), x1, x2, y1, y2);

										for (int i = 0; i < stops; ++i)
										{
											INT32 color		=	static_cast<INT32>(*current);
											double opacity	=	static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
											double offset	=	static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

											current			+=	6 * 4;		//	4 + 1 + 1
											curindex		+=	6 * 4;

											sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
											sXml += sXmlStop;
										}

										sXml += _T("</linearGradient>"); 

										if (stops)
										{
											VARIANT oVar;
											oVar.vt			=	VT_BSTR;
											oVar.bstrVal	=	CComBSTR(sXml);
											oWriter.SetAdditionalParam(L"Fill-LinearGradient", oVar);
										}
									}
									else if (1 == gradientType)
									{
										m			=	(INT32*)current;
										current		+=	5 * 4;
										curindex	+=	5 * 4;

										double cx	=	(*m++) / 100000.0;
										double cy	=	(*m++) / 100000.0;						
										double r	=	(*m++) / 100000.0;
										double fx	=	(*m++) / 100000.0;
										double fy	=	(*m++) / 100000.0;

										int stops	=	(INT32)(*current);
										current		+=	4;
										curindex	+=	4;

										sXml.Format(_T("<radialGradient cx='%f' cy='%f' r='%f' fx='%f' fy='%f' >"), cx, cy, r, fx, fy);

										for (int i = 0; i < stops; ++i)
										{
											INT32 color		=	static_cast<INT32>(*current);
											double opacity	=	static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
											double offset	=	static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

											current			+=	6 * 4;		//	4 + 1 + 1
											curindex		+=	6 * 4;

											sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
											sXml += sXmlStop;
										}

										sXml += _T("</radialGradient>"); 

										if (stops)
										{
											VARIANT oVar;
											oVar.vt			=	VT_BSTR;
											oVar.bstrVal	=	CComBSTR(sXml);
											oWriter.SetAdditionalParam(L"Fill-RadialGradient", oVar);
										}
									}
								}
								break;
							case ctGradientFillXML:
								{
									gradientType	=	(INT32)(*current);
									current			+=	4;
									curindex		+=	4;

									m				=	(INT32*)current;
									current			+=	4;
									curindex		+=	4;

									_sLen		=	(int)(*m);
									CStringW sTempStr;											
									sTempStr.Append((WCHAR*)current, _sLen * 0.5);
									s = sTempStr;											

									if (0 == gradientType)			//	linearGradient
										CreateLinearGradientFromSvgXml(s, TRUE);
									else if (1 == gradientType)	//	radialGradient	
										CreateRadialGradientFromSvgXml(s, TRUE);
								}
								break;		
							case ctGradientStroke:
								{
									gradientType	=	(INT32)(*current);
									current			+=	4;
									curindex		+=	4;

									CString sXml, sXmlStop;

									if (0 == gradientType)	//	linearGradient
									{
										m			=	(INT32*)current;
										current		+=	4 * 4;
										curindex	+=	4 * 4;

										double x1	=	(*m++) / 100000.0;
										double x2	=	(*m++) / 100000.0;						
										double y1	=	(*m++) / 100000.0;
										double y2	=	(*m++) / 100000.0;

										int stops	=	(INT32)(*current);
										current			+=	4;
										curindex		+=	4;

										sXml.Format(_T("<linearGradient x1='' x2='' y1='' y2=''>"), x1, x2, y1, y2);

										for (int i = 0; i < stops; ++i)
										{
											INT32 color		=	static_cast<INT32>(*current);
											double opacity	=	static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
											double offset	=	static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

											current			+=	6 * 4;		//	4 + 1 + 1
											curindex		+=	6 * 4;

											sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
											sXml += sXmlStop;
										}

										sXml += _T("</linearGradient>"); 

										if (stops)
										{
											VARIANT oVar;
											oVar.vt			=	VT_BSTR;
											oVar.bstrVal	=	CComBSTR(sXml);
											oWriter.SetAdditionalParam(L"Stroke-LinearGradient", oVar);
										}
									}
									else if (1 == gradientType)
									{
										m			=	(INT32*)current;
										current		+=	5 * 4;
										curindex	+=	5 * 4;

										double cx	=	(*m++) / 100000.0;
										double cy	=	(*m++) / 100000.0;						
										double r	=	(*m++) / 100000.0;
										double fx	=	(*m++) / 100000.0;
										double fy	=	(*m++) / 100000.0;

										int stops	=	(INT32)(*current);
										current		+=	4;
										curindex	+=	4;

										sXml.Format(_T("<radialGradient cx='%f' cy='%f' r='%f' fx='%f' fy='%f' >"), cx, cy, r, fx, fy);

										for (int i = 0; i < stops; ++i)
										{
											INT32 color		=	static_cast<INT32>(*current);
											double opacity	=	static_cast<double>(static_cast<INT32>(*(current + 1))) / 255.0;
											double offset	=	static_cast<double>(static_cast<INT32>(*(current + 2))) / 255.0;

											current			+=	6 * 4;		//	4 + 1 + 1
											curindex		+=	6 * 4;

											sXmlStop.Format(_T("<stop stop-color='%d' offset='%f' stop-opacity='%f' />"), color, offset);
											sXml += sXmlStop;
										}

										sXml += _T("</radialGradient>"); 

										if (stops)
										{
											VARIANT oVar;
											oVar.vt			=	VT_BSTR;
											oVar.bstrVal	=	CComBSTR(sXml);
											oWriter.SetAdditionalParam(L"Stroke-RadialGradient", oVar);
										}
									}
								}
								break;		
							case ctGradientStrokeXML:
								{
									gradientType	=	(INT32)(*current);
									current			+=	4;
									curindex		+=	4;

									m				=	(INT32*)current;
									current			+=	4;
									curindex		+=	4;

									_sLen		=	(int)(*m);
									CStringW sTempStr;											
									sTempStr.Append((WCHAR*)current, _sLen * 0.5);
									s = sTempStr;											

									if (0 == gradientType)			//	linearGradient
										CreateLinearGradientFromSvgXml(s, FALSE);
									else if (1 == gradientType)		//	radialGradient
										CreateRadialGradientFromSvgXml(s, FALSE);
								}
								break;							

							case ctEndCommand:
								ATLTRACE2 ("CommandType::ctEndCommand:\n");
								m = (INT32*)current;
								current += 4;
								curindex += 4;
								if (bIsPathOpened)
								{
									oWriter.PathCommandEnd();
									oWriter.EndCommand(4);
									bIsPathOpened = false;
								}
								oWriter.EndCommand((DWORD)(*m));
								oWriter.PathCommandEnd();
								break;
							default:
								ATLTRACE2 ("CommandType:DEFAULT!\n");
								break;
							}; // switch (eCommand)
						} // while (curindex < len)
					} // BYTE* p = dstArray;
					delete [] dstArray;
				} // if (TRUE == bDecoderResult)


				sHypers += _T("</linker>");

				if (nReg)
				{
					oWriter.BeginCommand(8);
					BSTR strHypers = sHypers.AllocSysString();
					oWriter.CommandDrawText(strHypers, 0, 0, 0, 0, 0);
					SysFreeString (strHypers);
					oWriter.EndCommand(8);
				}

				if (sTempLogo != "")
				{
					DeleteFile (sTempLogo);
				}

				oWriter.SaveToFile(sDstFile);

			} // if (NULL != dstArray)

		} // for (size_t nLine = 0; nLine < asResFile.GetCount(); ++nLine)
	} // try
	catch (char *pcError)
	{
		ATLTRACE2 (pcError);			
	}
	if (NULL != piWinFonts)
	{
		piWinFonts->Release();
		piWinFonts = NULL;
	}

	return hRes;
}

// help functions

inline static PDF::Matrix BuildFromSvgMatrix(CString sXml)
{
	if (sXml.GetLength())
	{		
		int st		=	0;
		int del		=	sXml.Tokenize(_T("("), st).GetLength();

		CString sTransform = sXml.Mid(del + 1, sXml.GetLength() - del - 2);
		if (sTransform.GetLength())
		{					
			CAtlArray<double> numbers;
			if (ParseUtils::DoubleValues(sTransform, numbers))
			{
				if (6 == numbers.GetCount())
				{
					return PDF::Matrix(numbers[0], numbers[1], numbers[4], numbers[2], numbers[3], numbers[5], 0, 0, 1);								
				}
			}
		}
	}

	return PDF::Matrix();
}

BOOL CPDFWriter::ApplyFillGradient()
{		
	PDF::Rect pageBounds	=	PageBounds();		
	PDF::Rect pathBounds	=	PathBounds();

	PDF::XForm* form		=	NULL;
	int maskRefId			=	-1;
	CString formName		=	L"";
	CString stateName		=	L"";
	CString alphaExt		=	L"";

	PDF::Pattern* pattern = m_oPatternState.GetFill();
	if (pattern)
	{
		ATLTRACE2(_T("ApplyFillGradient\n"));

		//	%	fill 

		//	/Pattern cs
		//	/P12345 scn

		CString sVal;
		sVal.Format(L"/P%d scn\012", pattern->GetRefIndex());

		if (PDF::ShadingPatternType == pattern->GetType())
		{
			// NOTE! прозрачность задается через сам градиент или маску 
			if (m_pCurrentExtGState)
			{
				ExtGState disableAlpha		=	CreateExtGState(m_pDocument);
				if (disableAlpha)
				{								
					disableAlpha->sBuffer	=	new CString(L"<< /Type /ExtGState\012	/CA 1\012	/ca 1\012>>");
					alphaExt				=	CString(PageGetExtGStateName(m_pDocument->pCurPage, disableAlpha));
				}
			}

			PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(pattern))->GetTopShading();
			if (shading)
			{
				shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
					m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
					0, 0, 1));

				shading->CalculateCoords(pathBounds, pageBounds.Height);
			}

			PDF::Pattern* alpha = pattern->GetAlphaPattern();
			if (alpha)
			{
				if (PDF::ShadingPatternType == alpha->GetType())
				{
					PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(alpha))->GetTopShading();
					if (shading)
					{
						shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
							m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
							0, 0, 1));

						shading->CalculateCoords(pathBounds, pageBounds.Height);

						PDF::PatternAlphaGroup* alpha	=	m_oPatterns.GetAlphaGroupTop();
						if (alpha)
						{
							form = new PDF::XForm();
							if (form)
							{
								ExtGState smask			=	CreateExtGState(m_pDocument);
								if (smask)
								{								
									CString str; str.Format(L"<< /Type /ExtGState\012   /SMask %d 0 R\012   /ca 1\012>>", alpha->GetMask()->GetId());
									smask->sBuffer		=	new CString(str);

									stateName			=	CString(PageGetExtGStateName(m_pDocument->pCurPage, smask));

									Proxy proxyForm		=	GenXRef<PDF::XForm>(form, OCLASS_XFORM);

									form->SetBound(pageBounds);

									alpha->SetFormId(form->GetId());
									alpha->GetMask()->SetStateId(smask->pHeader.nObjId & (~OTYPE_INDIRECT));

									m_oPatterns.AddXForm(form);

									XrefEntry entry		=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 1);
									if (entry)
									{
										XObject obj		=  (XObject)entry->pObj;
										if (obj)
										{
											formName	=	CString(PageGetXObjectName(m_pDocument->pCurPage, obj));
										}
									}

									RebuildResources();

									maskRefId			=	 alpha->GetMask()->GetStateId();
								}
							}
						}
					}
				}
			}
		}

		PageAttr attr				=	(PageAttr)m_pDocument->pCurPage->pAttr;
		if (attr)
		{
			m_strPatternFill		=	CString (L"/Pattern cs\012") + sVal; 
			if (alphaExt.GetLength())
			{
				m_strPatternFill	=	L"/" + alphaExt + L" gs \012" + m_strPatternFill;
			}

			StreamWriteStr(attr->pStream, CStringA(m_strPatternFill));
		}

		if (form)
		{
			// q /s13 gs /x14 Do Q
			// к x-N объекту применяется маска s-N

			m_MaskWithFormCommands.Format(L"q\012/%s gs /%s Do\012Q\012", stateName, formName);

			m_XForm		=	form;
		}

		m_oPatternState.SetFill(NULL);

		return TRUE;
	}

	return FALSE;
}
BOOL CPDFWriter::ApplyStrokeGradient()
{			
	PDF::Rect pageBounds	=	PageBounds();		
	PDF::Rect pathBounds	=	PathBounds();

	PDF::XForm* form		=	NULL;
	int maskRefId			=	-1;
	CString formName		=	L"";
	CString stateName		=	L"";
	CString alphaExt		=	L"";

	PDF::Pattern* pattern = m_oPatternState.GetStroke();
	if (pattern)
	{
		ATLTRACE2(_T("ApplyStrokeGradient\n"));

		//	%	stroke 

		//	/Pattern CS
		//	/P12345 SCN

		CString sVal;
		sVal.Format(L"/P%d SCN\012", pattern->GetRefIndex());

		if (PDF::ShadingPatternType == pattern->GetType())
		{
			// NOTE! прозрачность задается через сам градиент или маску 
			if (m_pCurrentExtGState)
			{
				ExtGState disableAlpha		=	CreateExtGState(m_pDocument);
				if (disableAlpha)
				{								
					disableAlpha->sBuffer	=	new CString(L"<< /Type /ExtGState\012	/CA 1\012	/ca 1\012>>");
					alphaExt				=	CString(PageGetExtGStateName(m_pDocument->pCurPage, disableAlpha));
				}
			}


			PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(pattern))->GetTopShading();
			if (shading)
			{
				shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
					m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
					0, 0, 1));

				shading->CalculateCoords(pathBounds, pageBounds.Height);
			}

			PDF::Pattern* alpha = pattern->GetAlphaPattern();
			if (alpha)
			{
				if (PDF::ShadingPatternType == alpha->GetType())
				{
					PDF::Shading* shading = (static_cast<PDF::ShadingPattern*>(alpha))->GetTopShading();
					if (shading)
					{
						shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC, MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
							m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD, MMToPDFCoords(m_oCommandParams.oMatrix.fF), 
							0, 0, 1));

						shading->CalculateCoords(pathBounds, pageBounds.Height);

						PDF::PatternAlphaGroup* alpha	=	m_oPatterns.GetAlphaGroupTop();
						if (alpha)
						{
							form = new PDF::XForm();
							if (form)
							{
								ExtGState smask			=	CreateExtGState(m_pDocument);
								if (smask)
								{								
									CString str; str.Format(L"<< /Type /ExtGState\012   /SMask %d 0 R\012   /ca 1\012>>", alpha->GetMask()->GetId());
									smask->sBuffer		=	new CString(str);

									stateName			=	CString(PageGetExtGStateName(m_pDocument->pCurPage, smask));

									Proxy proxyForm		=	GenXRef<PDF::XForm>(form, OCLASS_XFORM);

									form->SetBound(pageBounds);

									alpha->SetFormId(form->GetId());
									alpha->GetMask()->SetStateId(smask->pHeader.nObjId & (~OTYPE_INDIRECT));

									m_oPatterns.AddXForm(form);

									XrefEntry entry		=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 1);
									if (entry)
									{
										XObject obj		=  (XObject)entry->pObj;
										if (obj)
										{
											formName	=	CString(PageGetXObjectName(m_pDocument->pCurPage, obj));
										}
									}

									RebuildResources();

									maskRefId			=	 alpha->GetMask()->GetStateId();
								}
							}
						}
					}
				}
			}
		}

		PageAttr attr			=	(PageAttr)m_pDocument->pCurPage->pAttr;
		if (attr)
		{
			m_strPatternStroke	=	CString (L"/Pattern CS\012") + sVal; 
			if (alphaExt.GetLength())
			{
				m_strPatternFill	=	L"/" + alphaExt + L" gs \012" + m_strPatternStroke;
			}
			
			StreamWriteStr(attr->pStream, CStringA(m_strPatternStroke));
		}

		if (form)
		{
			// q /s13 gs /x14 Do Q
			// к x-N объекту применяется маска s-N

			m_MaskWithFormCommands.Format(L"q /%s gs /%s Do Q", stateName, formName);

			m_XForm	=	form;
		}

		m_oPatternState.SetStroke(NULL);

		return TRUE;
	}

	return FALSE;
}
BOOL CPDFWriter::ApplyTileFill()
{
	IUnknown* pImage = ImageUtils::LoadImage(m_oBrush.TexturePath);
	if (pImage)
	{
		ATLTRACE2(_T("ApplyTileFill\n"));

		LONG width  =	0;
		LONG height	=	0;

		m_bUseImageTextureAlpha		=	TRUE;	

		if ((m_bUseImageTextureAlpha || m_bIsWhiteBackImage) && (NULL != pImage))
		{
			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			if (SUCCEEDED(pImage->QueryInterface(&pFrame))) 
			{
				if (pFrame)
				{
					BYTE* buffer	=	NULL;

					pFrame->get_Width(&width);
					pFrame->get_Height(&height);
					pFrame->get_Buffer(&buffer);

					LONG lCountPix = width * height;

					if (m_bIsWhiteBackImage)
					{
						while (lCountPix > 0)
						{
							if (0 == buffer[3])
								memset(buffer, 0xFF, 4);

							--lCountPix;
							buffer += 4;
						}
					}
					else if (m_bUseImageTextureAlpha)
					{
						double alpha =	((double)m_oBrush.TextureAlpha / 255.0);

						while (lCountPix > 0)
						{
							buffer[3] *= alpha;
							--lCountPix;
							buffer += 4;
						}
					}

					RELEASEINTERFACE(pFrame);
				}
			}
		}

		m_bUseImageTextureAlpha	=	FALSE;	

		if (SUCCEEDED(LoadImageFromInterface(&pImage, TRUE)))
		{
			if (m_pCurrentXObject)
			{
				int RefImageId = m_pCurrentXObject->pHeader.nObjId;

				PDF::Rect pageBounds	=	PageBounds();		
				PDF::Rect pathBounds	=	PathBounds();

				PDF::TilePattern* shading = new PDF::TilePattern();
				if (shading)
				{
					// create pdf ref index
					
					Proxy ref = GenXRef<PDF::TilePattern>(shading, OCLASS_PATTERN_REF);

					// настройка заливки

					shading->SetRefIdX(RefImageId & (~OTYPE_INDIRECT));
					shading->SetBox(PDF::Rect(0.0f, 0.0f, floor ( MMToPDFCoords(PxToMM(width)) ), floor ( MMToPDFCoords(PxToMM(height))) ) );
					shading->SetId(ref->pHeader.nObjId & (~OTYPE_INDIRECT));
					shading->SetTransform(PDF::Matrix(m_oCommandParams.oMatrix.fA, m_oCommandParams.oMatrix.fC,
						MMToPDFCoords(m_oCommandParams.oMatrix.fE), 
						m_oCommandParams.oMatrix.fB, m_oCommandParams.oMatrix.fD,
						MMToPDFCoords(m_oCommandParams.oMatrix.fF),
						0, 0, 1));

					shading->CalculateCoords(pathBounds, pageBounds.Height);

					m_oPatternState.SetTiling(shading);					
					m_oPatterns.Add(shading);

					RebuildResources();

					// заливка для фигуры

					CString sVal;
					sVal.Format(L"/P%d scn\012", shading->GetRefIndex());

					PageAttr attr = (PageAttr)m_pDocument->pCurPage->pAttr;
					if (attr)
					{
						StreamWriteStr(attr->pStream, "/Pattern cs\012");
						StreamWriteStr(attr->pStream, CStringA(sVal));
					}

					m_oPatternState.SetFill(NULL);

					RELEASEINTERFACE(pImage);

					return TRUE;
				}
			}
		}

		RELEASEINTERFACE(pImage);
	}

	return FALSE;
}

BOOL CPDFWriter::CreateLinearGradientFromSvgXml(const CString& sXml, BOOL fill)
{
	ATLTRACE2(_T("CreateLinearGradientFromSvgXml\n"));

	m_oBrush.TexturePath	=	L"";
	m_oBrush.Type			=	1000;

	int FormId		=	-1;

	XrefEntry entry	=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 2);
	if (entry)
	{
		XObject obj	=	(XObject)entry->pObj;
		if (obj)
		{
			FormId	=	(obj->pHeader.nObjId & (~OTYPE_INDIRECT));
		}
	}

	XmlUtils::CXmlNode oXml;
	if (oXml.FromXmlString(sXml))
	{
		PDF::ShadingPattern* alphaShading	=	NULL;

		PDF::ShadingPattern* shading		=	new PDF::ShadingPattern();
		if (shading)
		{
			PDF::AxialShading* axial		=	new PDF::AxialShading();
			if (axial)
			{
				// единицы измерения (координаты документа или нормализованые)

				BOOL userSpaceOnUse			=	(L"userSpaceOnUse" == oXml.GetAttribute(L"gradientUnits"));
				BOOL isSvgElement			=	(L"svg" == oXml.GetAttribute(L"sourceType"));

				// направление

				double x1					=	_wtof(oXml.GetAttributeOrValue(L"x1", L"0"));
				double y1					=	_wtof(oXml.GetAttributeOrValue(L"y1", L"0"));	
				double x2					=	_wtof(oXml.GetAttributeOrValue(L"x2", L"1"));
				double y2					=	_wtof(oXml.GetAttributeOrValue(L"y2", L"0"));

				if (FALSE == isSvgElement)
				{
					if (userSpaceOnUse)
					{
						x1					=	MMToPDFCoords(x1);
						y1					=	MMToPDFCoords(y1);
						x2					=	MMToPDFCoords(x2);
						y2					=	MMToPDFCoords(y2);	
					}
				}
			
				axial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			

				// матрица

				PDF::Matrix matrix			=	BuildFromSvgMatrix(oXml.GetAttributeOrValue(L"gradientTransform"));
				axial->SetTransform(matrix);

				float width					=	0.0f;	GetWidth(&width);
				float height				=	0.0f;	GetHeight(&height);	

				GenXRef<PDF::ShadingPattern>(shading, OCLASS_PATTERN_REF);

				XmlUtils::CXmlNodes oNodes;
				if (oXml.GetNodes(L"stop", oNodes))
				{
					CAtlArray<PDF::ExponentialInterpolationFunction*> functions;

					CAtlArray<PDF::float3> colors;
					CAtlArray<float> offsets;
					CAtlArray<float> opacity;	

					BOOL alphaPatternEnable = FALSE;

					for (int i = 0; i < oNodes.GetCount(); ++i)
					{									
						XmlUtils::CXmlNode oNode;
						if(oNodes.GetAt(i, oNode))
						{
							long color	=	_wtol(oNode.GetAttributeOrValueBase(L"stop-color", L"0"));
							float off	=	(float)(_wtof(oNode.GetAttributeOrValueBase(L"offset", L"0")));
							float alpha	=	(float)(min(max(_wtof(oNode.GetAttributeOrValueBase(L"stop-opacity", L"1")), 0.0f), 1.0));
							
							colors.Add(PDF::float3((0xFF & color) / 255.0f, (0xFF & (color >> 8)) / 255.0f, (0xFF & (color >> 16)) / 255.0f));
							offsets.Add(min(max(off, 0.0f), 1.0));
							opacity.Add(alpha);
							
							if (fabs(alpha - 1.0) > DBL_EPSILON)
							{
								alphaPatternEnable	=	TRUE;
							}
						}
					}

					if (colors.GetCount())//  && FALSE == isSvgElement)
					{
						if (offsets[0] > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[0];
							float alpha			=	opacity[0];

							colors.InsertAt(0, color);
							offsets.InsertAt(0, 0.0f);
							opacity.InsertAt(0, alpha);
						}

						long count = offsets.GetCount() - 1;

						if (fabs(offsets[count] - 1.0) > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[count];
							float alpha			=	opacity[count];

							colors.Add(color);
							offsets.Add(1.0f);
							opacity.Add(alpha);
						}
					}

					// простой градиент из двух цветов
					
					if (2 == colors.GetCount() && 2 == offsets.GetCount())
					{
						PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
						if (func)
						{
							func->AddC(3, colors[0].f1, colors[0].f2, colors[0].f3);
							func->AddC(3, colors[1].f1, colors[1].f2, colors[1].f3);

							func->AddOffset(PDF::float2(offsets[0], offsets[1]));										
							func->AddDomain(PDF::float2(0.0f, 1.0f));	
							func->AddN(1);

							GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

							axial->AddFunc(func);

							if (alphaPatternEnable)
							{
								alphaShading	=	new PDF::ShadingPattern();
								if (alphaShading)
								{
									PDF::AxialShading* alphaAxial	=	new PDF::AxialShading();
									if (alphaAxial)
									{	
										alphaAxial->SetColorSpace(L"DeviceGray");
										alphaAxial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			
										alphaAxial->SetTransform(matrix);	

										PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
										if (alphaFunc)
										{
											alphaFunc->AddC(1, opacity[0]);
											alphaFunc->AddC(1, opacity[1]);

											alphaFunc->AddOffset(PDF::float2(offsets[0], offsets[1]));										
											alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
											alphaFunc->AddN(1);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

											alphaAxial->AddFunc(alphaFunc);
											alphaShading->AddShading(alphaAxial);
										}
									}
								}
							}
						}
					}
					else
					{
						// составной градиент из нескольких цветов

						PDF::StitchFunction* stitch = new PDF::StitchFunction();
						if (stitch)
						{
							stitch->AddDomain(PDF::float2(0.0f, 1.0f));		

							for (size_t i = 0; i < colors.GetCount() - 1; ++i)
							{
								PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
								if (func)
								{
									func->AddC(3, colors[i].f1, colors[i].f2, colors[i].f3);
									func->AddC(3, colors[i + 1].f1, colors[i + 1].f2, colors[i + 1].f3);

									func->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
									func->AddDomain(PDF::float2(0.0f, 1.0f));	
									func->AddN(1);

									GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

									axial->AddFunc(func);

									stitch->AddFunc(func);
									stitch->AddEncode(PDF::float2(0.0f, 1.0f));

									if (i >= 1)
									{
										stitch->AddBounds(offsets[i]);
									}
								}
							}

							if (stitch->IsValid())
							{
								GenXRef<PDF::StitchFunction>(stitch, OCLASS_FUNC_REF);
								axial->AddFunc(stitch);

								if (alphaPatternEnable)
								{
									alphaShading	=	new PDF::ShadingPattern();
									if (alphaShading)
									{
										PDF::AxialShading* alphaAxial	=	new PDF::AxialShading();
										if (alphaAxial)
										{	
											alphaAxial->SetColorSpace(L"DeviceGray");
											alphaAxial->SetGradientVector(PDF::float4(x1, y1, x2, y2), !userSpaceOnUse);			
											alphaAxial->SetTransform(matrix);

											PDF::StitchFunction* alphaStitch = new PDF::StitchFunction();
											if (alphaStitch)
											{
												alphaStitch->AddDomain(PDF::float2(0.0f, 1.0f));		

												int count = opacity.GetCount();
												for (size_t i = 0; i < count - 1; ++i)
												{
													PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
													if (alphaFunc)
													{
														alphaFunc->AddC(1, opacity[i]);
														alphaFunc->AddC(1, opacity[(i + 1)]);

														alphaFunc->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
														alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
														alphaFunc->AddN(1);

														GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

														alphaAxial->AddFunc(alphaFunc);

														alphaStitch->AddFunc(alphaFunc);
														alphaStitch->AddEncode(PDF::float2(0.0f, 1.0f));

														if (i >= 1)
														{
															alphaStitch->AddBounds(offsets[i]);
														}
													}
												}
											}
																					
											GenXRef<PDF::StitchFunction>(alphaStitch, OCLASS_FUNC_REF);
											alphaAxial->AddFunc(alphaStitch);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											alphaShading->AddShading(alphaAxial);
										}
									}
								}
							}
							else
							{
								RELEASEOBJECT(stitch);
							}
						}
					}
				}

				shading->AddShading(axial);

				if (fill)
					m_oPatternState.SetFill(shading);						
				else
					m_oPatternState.SetStroke(shading);						

				m_oPatterns.Add(shading);
				
				if (alphaShading)
				{
					shading->SetAlphaPattern(alphaShading);

					// как маска будет отрисована ( паттерно в ч.б цветовом пространстве )

					PDF::PatternAlphaGroup* mask = new PDF::PatternAlphaGroup();
					if (mask)
					{						
						mask->SetFillId(alphaShading->GetId());
						mask->SetBound(PDF::Rect(0, 0, width, height));
						mask->SetFormId(FormId);

						GenXRef<PDF::PatternAlphaGroup>(mask, OCLASS_MASK_FORM);

						// определение маски
						
						if (mask->GetMask())
						{				
							GenXRef<PDF::SoftMask>(mask->GetMask(), OCLASS_MASK_REF);
							mask->GetMask()->SetGroupId(mask->GetId());
						}

						m_oPatterns.AddMask(mask);
					}

					m_oPatterns.Add(alphaShading);
				}

				RebuildResources();

				return TRUE;
			}
		}
	}

	return FALSE;
}
BOOL CPDFWriter::CreateRadialGradientFromSvgXml(const CString& sXml, BOOL fill)
{
	ATLTRACE2(_T("CreateRadialGradientFromSvgXml\n"));

	m_oBrush.TexturePath	=	L"";
	m_oBrush.Type			=	1000;

	int FormId		=	-1;

	XrefEntry entry	=	XrefGetEntry(m_pDocument->pXref, m_pDocument->pXref->pEntries->nCount - 2);
	if (entry)
	{
		XObject obj	=	(XObject)entry->pObj;
		if (obj)
		{
			FormId	=	(obj->pHeader.nObjId & (~OTYPE_INDIRECT));
		}
	}

	XmlUtils::CXmlNode oXml;
	if (oXml.FromXmlString(sXml))
	{
		PDF::ShadingPattern* alphaShading	=	NULL;

		PDF::ShadingPattern* shading	=	new PDF::ShadingPattern();
		if (shading)
		{
			PDF::RadialShading* radial	=	new PDF::RadialShading();
			if (radial)
			{
				// единицы измерения (координаты документа или нормализованые)

				BOOL userSpaceOnUse		=	(L"userSpaceOnUse" == oXml.GetAttribute(L"gradientUnits"));
				BOOL isSvgElement		=	(L"svg" == oXml.GetAttribute(L"sourceType"));

				// направление

				double cx				=	_wtof(oXml.GetAttributeOrValue(L"cx", L"0"));
				double cy				=	_wtof(oXml.GetAttributeOrValue(L"cy", L"0"));	
				double r				=	_wtof(oXml.GetAttributeOrValue(L"r",  L"0"));
				double r0				=	_wtof(oXml.GetAttributeOrValue(L"r1", L"0"));
				double r1				=	_wtof(oXml.GetAttributeOrValue(L"r0", L"0"));
				double fx				=	_wtof(oXml.GetAttributeOrValue(L"fx", L"0"));
				double fy				=	_wtof(oXml.GetAttributeOrValue(L"fy", L"0"));

				if (oXml.GetAttribute(L"rx").GetLength())
					fx					=	_wtof(oXml.GetAttributeOrValue(L"rx", L"0"));
				if (oXml.GetAttribute(L"ry").GetLength())
					fy					=	_wtof(oXml.GetAttributeOrValue(L"ry", L"0"));

				if (FALSE == isSvgElement)
				{
					if (userSpaceOnUse)
					{
						cx				=	MMToPDFCoords(cx);
						cy				=	MMToPDFCoords(cy);
						r				=	MMToPDFCoords(r);
						r0				=	MMToPDFCoords(r0);
						r1				=	MMToPDFCoords(r1);
						fx				=	MMToPDFCoords(fx);	
						fy				=	MMToPDFCoords(fy);	
					}

					if (oXml.GetAttribute(L"r0").GetLength() && oXml.GetAttribute(L"r1").GetLength()) 
					{
						if (fabs(r0 - r1) < 0.0001)
							r1 = r0 + 0.0001;

						radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(r1 , r0),!userSpaceOnUse);			
					}
					else
					{
						radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(r * 0.5, r * 0.5 * 2),!userSpaceOnUse);			
					}
				}
				else
				{
					radial->SetGradientCircles(PDF::float2(cx, cy), PDF::float2(fx, fy), PDF::float2(0, r),!userSpaceOnUse);			
				}

				// матрица

				PDF::Matrix matrix		=	BuildFromSvgMatrix(oXml.GetAttributeOrValue(L"gradientTransform"));
				radial->SetTransform(matrix);
	
				float width				=	0.0f;	GetWidth(&width);
				float height			=	0.0f;	GetHeight(&height);	

				GenXRef<PDF::ShadingPattern>(shading, OCLASS_PATTERN_REF);

				XmlUtils::CXmlNodes oNodes;
				if (oXml.GetNodes(L"stop", oNodes))
				{
					CAtlArray<PDF::ExponentialInterpolationFunction*> functions;

					CAtlArray<PDF::float3> colors;
					CAtlArray<float> offsets;
					CAtlArray<float> opacity;								

					BOOL alphaPatternEnable = FALSE;

					for (int i = 0; i < oNodes.GetCount(); ++i)
					{									
						XmlUtils::CXmlNode oNode;
						if (oNodes.GetAt(i, oNode))
						{
							long color		=	_wtol(oNode.GetAttributeOrValueBase(L"stop-color", L"0"));
							double offset	=	min(max(_wtof(oNode.GetAttributeOrValueBase(L"offset", L"0")), 0.0f), 1.0);
							float alpha		=	(float)(min(max(_wtof(oNode.GetAttributeOrValueBase(L"stop-opacity", L"1")), 0.0f), 1.0));

							// если есть несколько нулевых смещение идущих друг за другом, отсечем их как не нужные

							if (offset < DBL_EPSILON && colors.GetCount())
							{
								colors.RemoveAll();
								offsets.RemoveAll();
								opacity.RemoveAll();
							}

							colors.Add(PDF::float3((0xFF & color) / 255.0f, (0xFF & (color >> 8)) / 255.0f, (0xFF & (color >> 16)) / 255.0f));
							offsets.Add(min(max(_wtof(oNode.GetAttributeOrValueBase(L"offset", L"0")), 0.0f), 1.0));
							opacity.Add(alpha);

							if (fabs(alpha - 1.0) > DBL_EPSILON)
							{
								alphaPatternEnable	=	TRUE;
							}
						}
					}

					if (colors.GetCount())
					{
						if (offsets[0] > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[0];
							float alpha			=	opacity[0];

							colors.InsertAt(0, color);
							offsets.InsertAt(0, 0.0f);
							opacity.InsertAt(0, alpha);
						}

						long count = offsets.GetCount() - 1;

						if (fabs(offsets[count] - 1.0) > DBL_EPSILON)
						{
							PDF::float3 color	=	colors[count];
							float alpha			=	opacity[count];

							colors.Add(color);
							offsets.Add(1.0f);
							opacity.Add(alpha);
						}
					}

					// простой градиент из двух цветов
					
					if (2 == colors.GetCount() && 2 == offsets.GetCount())
					{
						PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
						if (func)
						{
							func->AddC(3, colors[0].f1, colors[0].f2, colors[0].f3);
							func->AddC(3, colors[1].f1, colors[1].f2, colors[1].f3);

							func->AddOffset(PDF::float2(offsets[0], offsets[1]));										
							func->AddDomain(PDF::float2(0.0f, 1.0f));	
							func->AddN(1);

							GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);

							radial->AddFunc(func);
						}

						if (alphaPatternEnable)
						{
							alphaShading	=	new PDF::ShadingPattern();
							if (alphaShading)
							{
								PDF::RadialShading* alphaRadial	= new PDF::RadialShading();
								if (alphaRadial)
								{	
									alphaRadial->SetColorSpace(L"DeviceGray");
									alphaRadial->SetGradientCircles(radial->GetC0(), radial->GetC1(), radial->GetR(), !userSpaceOnUse);			
									alphaRadial->SetTransform(matrix);	

									PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
									if (alphaFunc)
									{
										alphaFunc->AddC(1, offsets[0]);
										alphaFunc->AddC(1, offsets[1]);

										alphaFunc->AddOffset(PDF::float2(offsets[0], offsets[1]));										
										alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
										alphaFunc->AddN(1);

										GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
										GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

										alphaRadial->AddFunc(alphaFunc);
										alphaShading->AddShading(alphaRadial);
									}
								}
							}
						}
					}
					else
					{
						// составной градиент из нескольких цветов
						PDF::StitchFunction* stitch = new PDF::StitchFunction();
						if (stitch)
						{
							stitch->AddDomain(PDF::float2(0.0f, 1.0f));	

							for (size_t i = 0; i < colors.GetCount() - 1; ++i)
							{
								PDF::ExponentialInterpolationFunction* func = new PDF::ExponentialInterpolationFunction();
								if (func)
								{
									func->AddC(3, colors[i].f1, colors[i].f2, colors[i].f3);
									func->AddC(3, colors[i + 1].f1, colors[i + 1].f2, colors[i + 1].f3);

									func->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
									func->AddDomain(PDF::float2(0.0f, 1.0f));	
									func->AddN(1);

									GenXRef<PDF::ExponentialInterpolationFunction>(func, OCLASS_FUNC_REF);
								
									radial->AddFunc(func);

									stitch->AddFunc(func);
									stitch->AddEncode(PDF::float2(0.0f, 1.0f));

									if (i >= 1)
									{
										stitch->AddBounds(offsets[i]);
									}
								}
							}

							if (stitch->IsValid())
							{
								GenXRef<PDF::StitchFunction>(stitch, OCLASS_FUNC_REF);
								radial->AddFunc(stitch);

								if (alphaPatternEnable)
								{
									alphaShading	=	new PDF::ShadingPattern();
									if (alphaShading)
									{
										PDF::RadialShading* alphaRadial	= new PDF::RadialShading();
										if (alphaRadial)
										{	
											alphaRadial->SetColorSpace(L"DeviceGray");
											alphaRadial->SetGradientCircles(radial->GetC0(), radial->GetC1(), radial->GetR(), !userSpaceOnUse);			
											alphaRadial->SetTransform(matrix);

											PDF::StitchFunction* alphaStitch = new PDF::StitchFunction();
											if (alphaStitch)
											{
												alphaStitch->AddDomain(PDF::float2(0.0f, 1.0f));		

												int count = opacity.GetCount();
												for (size_t i = 0; i < count - 1; ++i)
												{
													PDF::ExponentialInterpolationFunction* alphaFunc = new PDF::ExponentialInterpolationFunction();
													if (alphaFunc)
													{
														alphaFunc->AddC(1, opacity[i]);
														alphaFunc->AddC(1, opacity[(i + 1)]);

														alphaFunc->AddOffset(PDF::float2(offsets[i], offsets[i + 1]));										
														alphaFunc->AddDomain(PDF::float2(0.0f, 1.0f));	
														alphaFunc->AddN(1);

														GenXRef<PDF::ExponentialInterpolationFunction>(alphaFunc, OCLASS_FUNC_REF);

														alphaRadial->AddFunc(alphaFunc);

														alphaStitch->AddFunc(alphaFunc);
														alphaStitch->AddEncode(PDF::float2(0.0f, 1.0f));

														if (i >= 1)
														{
															alphaStitch->AddBounds(offsets[i]);
														}
													}
												}
											}
																					
											GenXRef<PDF::StitchFunction>(alphaStitch, OCLASS_FUNC_REF);
											alphaRadial->AddFunc(alphaStitch);

											GenXRef<PDF::ShadingPattern>(alphaShading, OCLASS_PATTERN_REF);
											alphaShading->AddShading(alphaRadial);
										}
									}
								}
							}
							else
							{
								RELEASEOBJECT(stitch);
							}
						}
					}
				}

				shading->AddShading(radial);

				if (fill)
					m_oPatternState.SetFill(shading);						
				else
					m_oPatternState.SetStroke(shading);						

				m_oPatterns.Add(shading);

				if (alphaShading)
				{
					shading->SetAlphaPattern(alphaShading);

					// как маска будет отрисована ( паттерно в ч.б цветовом пространстве )

					PDF::PatternAlphaGroup* mask = new PDF::PatternAlphaGroup();
					if (mask)
					{						
						mask->SetFillId(alphaShading->GetId());
						mask->SetBound(PDF::Rect(0, 0, width, height));
						mask->SetFormId(FormId);

						GenXRef<PDF::PatternAlphaGroup>(mask, OCLASS_MASK_FORM);

						// определение маски
						
						if (mask->GetMask())
						{				
							GenXRef<PDF::SoftMask>(mask->GetMask(), OCLASS_MASK_REF);
							mask->GetMask()->SetGroupId(mask->GetId());
						}

						m_oPatterns.AddMask(mask);
					}

					m_oPatterns.Add(alphaShading);
				}

				RebuildResources();

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CPDFWriter::RebuildResources()
{
	Dict element			=	(Dict)(GetElement(m_pDocument->pCurPage, "Resources")->pValue);
	if (element)
	{
		Dict patternDict	=	DictNew(m_pDocument->oMMgr);
		if (patternDict)
		{

			DictAdd(element, "Pattern", patternDict);

			RELEASEOBJECT(patternDict->sBuffer);
			patternDict->sBuffer = new CString(m_oPatterns.Defines());

			return TRUE;
		}
	}

	return FALSE;
}

template <typename T> Proxy CPDFWriter::GenXRef(T* object, int Type)
{
	Proxy ref = ProxyNew(m_pDocument->oMMgr, object);
	if (ref)
	{
		ref->pHeader.nObjClass = Type;	
		XrefAdd(m_pDocument->pXref, ref);
		object->SetId(ref->pHeader.nObjId & (~OTYPE_INDIRECT));	
	}

	return ref;
}

PDF::Rect CPDFWriter::PathBounds()
{
	PDF::Rect rect;
	
	if (m_pCurPath)
	{
		m_pCurPath->GetBounds(&rect.X, &rect.Y, &rect.Width, &rect.Height);

		rect.X		=	MMToPDFCoords(rect.X);
		rect.Y		=	MMToPDFCoords(rect.Y);
		rect.Width	=	MMToPDFCoords(rect.Width);
		rect.Height	=	MMToPDFCoords(rect.Height);
	}

	if (m_bUseTextureRect)
	{
		rect.X		=	MMToPDFCoords(m_oBrush.Rect.X);
		rect.Y		=	MMToPDFCoords(m_oBrush.Rect.Y);
		rect.Width	=	MMToPDFCoords(m_oBrush.Rect.Width) + rect.X * 2;
		rect.Height	=	MMToPDFCoords(m_oBrush.Rect.Height)+ rect.Y * 2;

		rect.X		=	0;
		rect.Y		=	0;
	}

	return rect;
}
PDF::Rect CPDFWriter::PageBounds()
{
	PDF::Rect rect;
	
	GetHeight(&rect.Height);	
	GetWidth(&rect.Width);	

	return rect;
}