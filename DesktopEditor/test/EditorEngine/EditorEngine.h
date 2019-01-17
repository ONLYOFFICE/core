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
// EditorEngine.h : Declaration of the CEditorEngine

#pragma once
#include "resource.h"       // main symbols
#include "stdafx.h"

// images ------------------------------------------------------------
#ifndef _DEBUG
#pragma comment(lib, "../../cximage/jasper/Release/jasper.lib")
#pragma comment(lib, "../../cximage/jbig/Release/jbig.lib")
#pragma comment(lib, "../../cximage/jpeg/Release/jpeg.lib")
#pragma comment(lib, "../../cximage/mng/Release/mng.lib")
#pragma comment(lib, "../../cximage/png/Release/png.lib")
#pragma comment(lib, "../../cximage/libpsd/Release/libpsd.lib")
#pragma comment(lib, "../../cximage/raw/Release/libdcr.lib")
#pragma comment(lib, "../../cximage/tiff/Release/tiff.lib")
#pragma comment(lib, "../../cximage/zlib/Release/zlib.lib")
#pragma comment(lib, "../../cximage/CxImage/Release/cximage.lib")

#pragma comment(lib, "../../raster/Release/raster.lib")
#else
#pragma comment(lib, "../../cximage/jasper/Debug/jasper.lib")
#pragma comment(lib, "../../cximage/jbig/Debug/jbig.lib")
#pragma comment(lib, "../../cximage/jpeg/Debug/jpeg.lib")
#pragma comment(lib, "../../cximage/mng/Debug/mng.lib")
#pragma comment(lib, "../../cximage/png/Debug/png.lib")
#pragma comment(lib, "../../cximage/libpsd/Debug/libpsd.lib")
#pragma comment(lib, "../../cximage/raw/Debug/libdcr.lib")
#pragma comment(lib, "../../cximage/tiff/Debug/tiff.lib")
#pragma comment(lib, "../../cximage/zlib/Debug/zlib.lib")
#pragma comment(lib, "../../cximage/CxImage/Debug/cximage.lib")

#pragma comment(lib, "../../raster/Debug/raster.lib")
#endif

#include "../../raster/ImageFileFormatChecker.h"
#include "../../raster/BgraFrame.h"
// -------------------------------------------------------------------

// fonts -------------------------------------------------------------
#ifndef _DEBUG
#pragma comment(lib, "../../freetype-2.5.2/objs/win32/vc2008/freetype252ST.lib")
#pragma comment(lib, "../../fontengine/Release/font_engine.lib")
#else
#pragma comment(lib, "../../freetype-2.5.2/objs/win32/vc2008/freetype252ST_D.lib")
#pragma comment(lib, "../../fontengine/Debug/font_engine.lib")
#endif

#include "../../fontengine/ApplicationFonts.h"
// -------------------------------------------------------------------

// graphics ----------------------------------------------------------
#ifndef _DEBUG
#pragma comment(lib, "../../agg-2.4/Release/agg2d.lib")
#pragma comment(lib, "../../graphics/Release/graphics.lib")
#else
#pragma comment(lib, "../../agg-2.4/Debug/agg2d.lib")
#pragma comment(lib, "../../graphics/Debug/graphics.lib")
#endif

#include "../../graphics/GraphicsRenderer.h"
// -------------------------------------------------------------------

#include "../../common/File.h"
#include "../../graphics/Timer.h"
#include "../../editor/PageDrawer.h"

// v8 libraries
#pragma comment(lib, "../../../../../../../../../../v8/build/Release/lib/icui18n.lib")
#pragma comment(lib, "../../../../../../../../../../v8/build/Release/lib/icuuc.lib")
#pragma comment(lib, "../../../../../../../../../../v8/build/Release/lib/v8_base.ia32.lib")
#pragma comment(lib, "../../../../../../../../../../v8/build/Release/lib/v8_nosnapshot.ia32.lib")
#pragma comment(lib, "../../../../../../../../../../v8/build/Release/lib/v8_snapshot.lib")

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

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

    ctError						    = 255
};

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IEditorEngine
[ object, uuid("022286B8-798C-44fe-ABE3-3AED2844E7B0"), dual, pointer_default(unique) ]
__interface IEditorEngine : IDispatch
{
	[id(100)]	HRESULT ImageTest([in] BSTR bsFileName);
	[id(101)]	HRESULT PageTest([in] BSTR bsFileName, [in] BSTR bsTestFile, [in] double dWidth, [in] double dHeight);
	
//----- Для дополнительных функций ----------------------------------------------------------------
	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};

// CEditorEngine
[coclass, default(IEditorEngine), threading(apartment), vi_progid("EditorEngine.EditorEngine"), progid("EditorEngine.EditorEngine.1"), version(1.0), uuid("3A671302-97C5-4b4f-8D2A-B3D66590427F") ]
class ATL_NO_VTABLE CEditorEngine : public IEditorEngine
{
private:
	
public:
	CEditorEngine()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(SetAdditionalParam)(BSTR bsParamName, VARIANT vParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR bsParamName, VARIANT *pvParamValue)
	{
		return S_OK;
	}
	
	STDMETHOD(ImageTest)(BSTR bsFileName)
	{
		/*
		std::wstring sFile(bsFileName);
		CBgraFrame oFrame;
		oFrame.OpenFile(sFile);

		if (FALSE)
		{
			CBgraFrame oFrame2;
			oFrame2.OpenFile(L"\\\\korshul\\Exchange\\1234\\загруженное.png");

			BYTE* pData = oFrame2.get_Data();
			int nCount = oFrame2.get_Width() * oFrame2.get_Height() * 4;
			CStringA strImage = "";
			for (int i = 0; i < nCount; ++i)
			{
				CStringA s = "";
				s.Format("%d;", pData[i]);
				strImage += s;
			}

			FILE* ff = fopen("c:\\image_dump", "a+");
			fprintf(ff, strImage.GetBuffer());
			fclose(ff);
		}

		oFrame.SaveFile(L"D:\\Exchange\\1234\\RENDERER\\1.png", _CXIMAGE_FORMAT_PNG);
		*/

		return S_OK;
	}
	STDMETHOD(PageTest)(BSTR bsFileName, BSTR bsTestFile, double dWidth, double dHeight)
	{
		// !!!
		DrawDumpPage();
		return S_OK;

		// system fonts
		CApplicationFonts oApplicationFonts;
		oApplicationFonts.Initialize();

		// font manager
		CFontManager* pManager = oApplicationFonts.GenerateFontManager();

		// images cache
		CImageFilesCache oImageCache;
		
		CGraphicsRenderer oRenderer;
		oRenderer.SetFontManager(pManager);
		oRenderer.SetImageCache(&oImageCache);

		std::wstring sFile(bsFileName);
		std::wstring sImagesDir = L"D:\\Exchange\\1234\\RENDERER\\media";
		std::wstring sDstImage(bsTestFile);
		
		CFontStream oStream;
		oStream.CreateFromFile(sFile);

		int nPixWidth = (int)(96.0 * 297.0 / 25.4);
		int nPixHeight = (int)(96.0 * 420.0 / 25.4);
		int nStride = 4 * nPixWidth;
		BYTE* pPixData = new BYTE[4 * nPixWidth * nPixHeight];

		CBgraFrame oFrame;
		oFrame.put_Data(pPixData);
		oFrame.put_Width(nPixWidth);
		oFrame.put_Height(nPixHeight);
		oFrame.put_Stride(-nStride);

		memset(pPixData, 0xFF, 4 * nPixWidth * nPixHeight);

		oRenderer.CreateFromBgraFrame(&oFrame);
		
		ParsePageBinary(&oRenderer, oStream.m_pData, oStream.m_lSize, sImagesDir, false);				
		
		RELEASEINTERFACE(pManager);

		oFrame.SaveFile(sDstImage, _CXIMAGE_FORMAT_PNG);
		return S_OK;
	}

private:

	void DrawDumpPage()
	{
		CApplicationFonts oApplicationFonts;
		oApplicationFonts.InitializeFromFolder(L"X:\\AVS\\Sources\\TeamlabOffice\\trunk\\OfficeWeb\\Fonts\\native");

		// font manager
		CFontManager* pManager = oApplicationFonts.GenerateFontManager();

		// images cache
		CImageFilesCache oImageCache;

		LONG lWidth			= 815;
		LONG lHeight		= 1055;
		double dWidthMM		= 215.9;
		double dHeightMM	= 279.4;

		BYTE* pBits = new BYTE[4 * lWidth * lHeight];

		CGraphicsRenderer oRenderer;
		oRenderer.SetFontManager(pManager);
		oRenderer.SetImageCache(&oImageCache);

		CBgraFrame oFrame;
		oFrame.put_Data(pBits);
		oFrame.put_Width(lWidth);
		oFrame.put_Height(lHeight);
		oFrame.put_Stride(-4 * lWidth);

		oRenderer.CreateFromBgraFrame(&oFrame);
		oFrame.put_Data(NULL);
			
		oRenderer.put_Width(dWidthMM);
		oRenderer.put_Height(dHeightMM);

		// file
		NSFile::CFileBinary oFileDump;
		oFileDump.OpenFile(L"C:\\Page_dump.bin");
		int nLen = oFileDump.GetFileSize();
		BYTE* pData = new BYTE[nLen];
		DWORD dwRead = 0;
		oFileDump.ReadFile(pData, (DWORD)nLen, dwRead);
		oFileDump.CloseFile();
		NSMemoryStream::CMemoryStream oStream;
		oStream.WriteBuffer(pData, nLen);
		RELEASEARRAYOBJECTS(pData);
		// ---

		NSPageDrawer::CPageDrawer oDrawer;
		oDrawer.m_pStream = &oStream;
		oDrawer.m_sFontsDirectory	= L"X:\\AVS\\Sources\\TeamlabOffice\\trunk\\OfficeWeb\\Fonts\\native\\";
		oDrawer.m_sImagesDirectory	= L"X:\\AVS\\Sources\\TeamlabOffice\\trunk\\test\\media\\";
		BOOL bBreak = FALSE;
		
		for (int i = 0; i < 50; ++i)
		{
			memset(pBits, 0xFF, 4 * lWidth * lHeight);
			oDrawer.DrawOnRenderer(&oRenderer, &bBreak);
		}
				
		oDrawer.m_pStream = NULL;
	}

	void ParsePageBinary(IRenderer* pRenderer, BYTE* pOutput, int lOutputLen, std::wstring strImagesDirectory, bool bIsPDF = false)
	{
		int* m = NULL;
		USHORT* ms = NULL;
        int _sLen = 0;

        double m1 = 0;
        double m2 = 0;
        double m3 = 0;
        double m4 = 0;
        double m5 = 0;
        double m6 = 0;

		std::wstring imgPath	= L"";
		std::wstring base64Temp = L"";

		bool bIsPathOpened	= false;
		int nCountPages		= 0;
		int curindex		= 0;
		BYTE* current		= pOutput;

		CommandType eCommand;
        while (curindex < lOutputLen)
        {
            eCommand = (CommandType)(*current);
            current++;
            curindex++;
            switch (eCommand)
            {
                case ctPageWidth:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					pRenderer->put_Width((*m) / 100000.0);
                    break;
				}
                case ctPageHeight:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					pRenderer->put_Height((*m) / 100000.0);
                    break;
				}
                case ctPageStart:
				{
					pRenderer->NewPage();
					pRenderer->BeginCommand(1);
                    ++nCountPages;
                    break;
				}
                case ctPageEnd:
				{
					if (bIsPathOpened)
                    {
						pRenderer->PathCommandEnd();
						pRenderer->EndCommand(4);                        
                    }

                    bIsPathOpened = false;
					pRenderer->EndCommand(1);
                    break;
				}
                case ctPenColor:
				{
                    m = (int*)current;
					pRenderer->put_PenColor(*m);
                    current += 4;
                    curindex += 4;
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
                    m = (int*)current;
					pRenderer->put_PenSize(*m / 100000.0);
                    current += 4;
                    curindex += 4;
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
                    m = (int*)current;
					pRenderer->put_BrushType(*m);
                    current += 4;
                    curindex += 4;
                    break;
				}
                case ctBrushColor1:
				{
                    m = (int*)current;
					pRenderer->put_BrushColor1(*m);
                    current += 4;
                    curindex += 4;
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
                    m = (int*)current;
					pRenderer->put_BrushColor2(*m);
                    current += 4;
                    curindex += 4;
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
                    m = (int*)current;
                    current += 4 * 4;
                    curindex += 4 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;

					// TODO:
					//pRenderer->BrushRect(0, m1, m2, m3, m4);                    
                    break;
				}
                case ctBrushRectableEnabled:
				{
					/*
                    VARIANT var;
					var.vt = VT_BOOL;
					var.boolVal = (1 == *current) ? VARIANT_TRUE : VARIANT_FALSE;
                    pRenderer->SetAdditionalParam(L"BrushFillBoundsEnable", var);
					*/

                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctBrushTexturePath:
				{
					ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
					std::wstring s((wchar_t*)current, _sLen);
                    imgPath = s;

					if (0 != s.find(L"http:") &&
						0 != s.find(L"https:") &&
                        0 != s.find(L"ftp:") &&
                        0 != s.find(L"file:"))
                    {
						imgPath = strImagesDirectory + L"\\" + s;

                        int _len = imgPath.length();
                        int ind = imgPath.find(L".svg");
                        if (ind != -1)
                        {
                            if (ind == (_len - 4))
                            {
								std::wstring sInterest = imgPath.substr(0, ind);

								NSFile::CFileBinary oFile;
								if (oFile.OpenFile(sInterest + _T(".emf")))
                                    imgPath = sInterest + L".emf";
								else if (oFile.OpenFile(sInterest + _T(".wmf")))
                                    imgPath = sInterest + L".wmf";
                            }
                        }
                    }

                    base64Temp = L"";
                    if (0 == s.find(L"data:"))
                    {
                        // TODO:
                    }

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

					pRenderer->put_BrushTexturePath(imgPath);
                    break;
				}
                case ctBrushGradient:
				{
                    current++;
                    curindex++;

					std::wstring strAttrMain = L"";
                    std::wstring strColors = L"";
                    bool bIsLinear = true;

                    while (true)
                    {
                        BYTE _command = *current;
                        current++;
                        curindex++;

                        if (251 == _command)
                            break;

						LONG _R = 0;
						LONG _G = 0;
						LONG _B = 0;
						LONG _A = 0;


                        switch (_command)
                        {
                            case 0:
                                {
                                    current += 5;
                                    curindex += 5;

                                    m = (int*)current;
                                    current += 4 * 4;
                                    curindex += 4 * 4;

                                    double d1 = (*m++) / 100000.0;
                                    double d2 = (*m++) / 100000.0;
                                    double d3 = (*m++) / 100000.0;
                                    double d4 = (*m++) / 100000.0;

									strAttrMain = NSStringFormat::Format(L"x1=\"%.2lf\" y1=\"%.2lf\" x2=\"%.2lf\" y2=\"%.2lf\" gradientUnits=\"userSpaceOnUse\"", d1, d2, d3, d4);
									//strAttrMain.replace(L",", L".");
                                    break;
                                }
                            case 1:
                                {
                                    bIsLinear = false;

                                    current++;
                                    curindex++;

                                    m = (int*)current;
                                    current += 6 * 4;
                                    curindex += 6 * 4;

                                    double d1 = (*m++) / 100000.0;
                                    double d2 = (*m++) / 100000.0;
                                    double d3 = (*m++) / 100000.0;
                                    double d4 = (*m++) / 100000.0;
                                    double d5 = (*m++) / 100000.0;
                                    double d6 = (*m++) / 100000.0;

									strAttrMain = NSStringFormat::Format(L"cx=\"%.2lf\" cy=\"%.2lf\" r0=\"%.2lf\" r1=\"%.2lf\" rx=\"%.2lf\" ry=\"%.2lf\" gradientUnits=\"userSpaceOnUse\"", d1, d2, d5, d6, d1, d2);
                                    //strAttrMain.replace(L",", L".");
                                    break;
                                }
                            case 2:
                                {
                                    int nCountColors = *((int*)current);

                                    current += 4;
                                    curindex += 4;

                                    for (int nI = 0; nI < nCountColors; ++nI)
                                    {
                                        int pos = *((int*)current);
                                        current += 4;
                                        curindex += 4;

                                        double dPos = pos / 100000.0;
                                        BYTE _r = *current++;
                                        BYTE _g = *current++;
                                        BYTE _b = *current++;
                                        BYTE _a = *current++;

										_R += _r;
										_G += _g;
										_B += _b;
										_A += _a;

                                        curindex += 4;

                                        int _color = ((_b << 16) & 0xFF0000) | ((_g << 8) & 0xFF00) | _r;
										std::wstring sColor = NSStringFormat::Format(L"<stop stop-color=\"%d\" stop-opacity=\"%.2lf\" offset=\"%.2lf\" />", _color, _a / 255.0, dPos);

                                        //sColor.replace(L",", L".");
                                        strColors += sColor;
                                    }

									if (nCountColors != 0)
									{
										// TODO:
										pRenderer->put_BrushType(1000);

										_R = (BYTE)(_R / nCountColors);
										_G = (BYTE)(_G / nCountColors);
										_B = (BYTE)(_B / nCountColors);
										_A = (BYTE)(_A / nCountColors);

										LONG _Color = _B * 256 * 256 + _G * 256 + _R;
										pRenderer->put_BrushColor1(_Color);
										pRenderer->put_BrushAlpha1(_A);
									}

                                    break;
                                }
                            default:
                                break;
                        };
                    }

					std::wstring strXml = L"";

					// TODO:
                    if (bIsLinear)
                    {
                        strXml = L"<linearGradient " + strAttrMain + L">" + strColors + L"</linearGradient>";
						//pRenderer->SetAdditionalParam(L"Fill-LinearGradient", strXml);						
                    }
                    else
                    {
                        strXml = L"<radialGradient " + strAttrMain + L">" + strColors + L"</radialGradient>";
                        //pRenderer->SetAdditionalParam(L"Fill-RadialGradient", strXml);
                    }                                        

                    break;
				}
                case ctBrushTextureMode:
				{
                    int mode = (int)(*current);
					pRenderer->put_BrushTextureMode(mode);
                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctBrushTextureAlpha:
				{
                    int txalpha = (int)(*current);
					pRenderer->put_BrushTextureAlpha(txalpha);

                    current += 1;
                    curindex += 1;
                    break;
				}
                case ctSetTransform:
				{
                    m = (int*)current;
                    current += 6 * 4;
                    curindex += 6 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;
                    m5 = (*m++) / 100000.0;
                    m6 = (*m++) / 100000.0;

                    pRenderer->SetTransform(m1, m2, m3, m4, m5, m6);
                    break;
				}
                case ctPathCommandStart:
				{
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        pRenderer->BeginCommand(4);
                        pRenderer->PathCommandStart();
                    }
                    else
                    {
                        pRenderer->BeginCommand(4);
                        pRenderer->PathCommandStart();
                    }
                    bIsPathOpened = true;
                    break;
				}
                case ctPathCommandEnd:
				{
                    if (bIsPathOpened)
                    {
                        pRenderer->PathCommandEnd();
                        pRenderer->EndCommand(4);
                        bIsPathOpened = false;
                    }
                    break;
				}
                case ctPathCommandMoveTo:
				{
                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    pRenderer->PathCommandMoveTo(m1, m2);
                    break;
				}
                case ctPathCommandLineTo:
				{
                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    pRenderer->PathCommandLineTo(m1, m2);
                    break;
				}
                case ctPathCommandCurveTo:
				{
                    m = (int*)current;
                    current += 6 * 4;
                    curindex += 6 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;
                    m5 = (*m++) / 100000.0;
                    m6 = (*m++) / 100000.0;

                    pRenderer->PathCommandCurveTo(m1, m2, m3, m4, m5, m6);
                    break;
				}
                case ctPathCommandClose:
				{
                    pRenderer->PathCommandClose();
                    break;
				}
                case ctDrawPath:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    pRenderer->DrawPath(*m);
                    break;
				}
                case ctDrawImageFromFile:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    _sLen = (int)(*m);
                    _sLen /= 2;
					std::wstring s((wchar_t*)current, _sLen);

                    imgPath = s;

					if (0 != s.find(L"http:") &&
                        0 != s.find(L"https:") &&
                        0 != s.find(L"ftp:") &&
                        0 != s.find(L"file:"))
                    {
						imgPath = strImagesDirectory + L"\\" + s;

                        int _len = imgPath.length();
                        int ind = imgPath.find(L".svg");
                        if (ind != -1)
                        {
                            if (ind == (_len - 4))
                            {
								std::wstring sInterest = imgPath.substr(0, ind);

								NSFile::CFileBinary oFile;
								if (oFile.OpenFile(sInterest + _T(".emf")))
                                    imgPath = sInterest + L".emf";
								else if (oFile.OpenFile(sInterest + _T(".wmf")))
                                    imgPath = sInterest + L".wmf";
                            }
                        }
                    }

                    if (0 == s.find(L"data:"))
                    {
                        // TODO:
                    }

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

                    m = (int*)current;
                    current += 4 * 4;
                    curindex += 4 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;
                    m3 = (*m++) / 100000.0;
                    m4 = (*m++) / 100000.0;

                    try
                    {
                        pRenderer->DrawImageFromFile(imgPath, m1, m2, m3, m4);
                    }
                    catch (...) 
					{
					}
                    break;
				}
                case ctFontName:
				{
					ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
					std::wstring s((wchar_t*)current, _sLen);

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

					pRenderer->put_FontName(s);
                    break;
				}
                case ctFontSize:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

                    m1 = (*m++) / 100000.0;
                    // в PDF стоит ограничение на размер шрифта
					pRenderer->put_FontSize(min(m1, 1000));
                    break;
				}
                case ctFontStyle:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

					pRenderer->put_FontStyle(*m);
                    break;
				}
                case ctDrawText:
				{
                    ms = (USHORT*)current;
                    current += 2;
                    curindex += 2;

                    _sLen = (int)(*ms);
					std::wstring s((wchar_t*)current, _sLen);

                    current += 2 * _sLen;
                    curindex += 2 * _sLen;

                    m = (int*)current;
                    current += 2 * 4;
                    curindex += 2 * 4;

                    m1 = (*m++) / 100000.0;
                    m2 = (*m++) / 100000.0;

					pRenderer->CommandDrawText(s, m1, m2, 0, 0, 0);
                    break;
				}
                case ctBeginCommand:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;

					if (bIsPDF)
					{
						if (bIsPathOpened)
						{
							pRenderer->PathCommandEnd();
							pRenderer->EndCommand(4);
							bIsPathOpened = false;
						}
						pRenderer->BeginCommand(*m);
					}
					else
					{
						pRenderer->BeginCommand(*m);
					}
                    break;
				}
                case ctEndCommand:
				{
                    m = (int*)current;
                    current += 4;
                    curindex += 4;
					
					if (bIsPDF)
					{
						if (bIsPathOpened)
						{
							pRenderer->PathCommandEnd();
							pRenderer->EndCommand(4);
							bIsPathOpened = false;
						}
						pRenderer->EndCommand(*m);
						pRenderer->PathCommandEnd();
					}
					else
					{
						pRenderer->EndCommand(*m);
					}
                    
                    break;
				}
                default:
                    break;
            };

            //if (nCountPages == 1 && eCommand == CommandType.ctPageEnd)
            //    break;
        }

		/*
		if (NULL != m_pPainter)
		{
			VARIANT var;
			var.vt = VT_I4;
			m_pPainter->SetAdditionalParam(L"OnCompletePage", var);
		}
		*/
	}

};

bool agg::svg::detail::isdigit(unsigned int)
{
	return false;
}