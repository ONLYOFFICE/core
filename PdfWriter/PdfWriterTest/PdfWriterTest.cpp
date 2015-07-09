// PdfWriterTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../PdfRenderer.h"

#include "../Src/Streams.h"
#include "../Src/Utils.h"
#include "../Src/Objects.h"
#include "../Src/Encrypt.h"
#include "../Src/Info.h"
#include "../Src/Document.h"
#include "../Src/Outline.h"
#include "../Src/Destination.h"
#include "../Src/Pages.h"
#include "../Src/Annotation.h"
#include "../Src/Image.h"
#include "../Src/Font.h"
#include "../Src/Font14.h"
#include "../Src/FontCidTT.h"
#include "../Src/Pattern.h"

#include <iostream>

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/common/File.h"

#include "../../DesktopEditor/cximage/CxImage/ximage.h"
#include "../../DesktopEditor/cximage/CxImage/ximatif.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

#include "../../DesktopEditor/cximage/tiff/tiffio.h"

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"
#include <vector>
#include <ctime>

#include "../Src/FastStringToDouble.h"

#ifdef DrawText
#undef DrawText
#endif

using namespace PdfWriter;

void TestStreams()
{
	CFileStream oFS;
	oFS.OpenFile(L"D:/Test Files/1.txt", false);

	while (!oFS.IsEof())
	{
		char nChar = oFS.ReadChar();
		std::cout << nChar;
	}

	CMemoryStream oMS;
	oMS.WriteStream(&oFS, 0, NULL);

	CFileStream oTS;
	oTS.OpenFile(L"D:/Test Files/TestStreams.tmp", true);
	oTS.WriteStream(&oMS, 0, NULL);
	oTS.WriteChar(' ');
	oTS.WriteChar('c');
	oTS.WriteChar(' ');
	oTS.WriteBinary((const BYTE*)"123", 3, NULL);
	oTS.WriteChar(' ');
	oTS.WriteEscapeName((const char*)"EscapeName");
	oTS.WriteChar(' ');
	oTS.WriteEscapeText((const BYTE*)"EscapeTest", StrLen("EscapeTest", 12));
	oTS.WriteChar(' ');
	oTS.WriteHex(251, 2);
	oTS.WriteChar(' ');
	oTS.WriteHex(251, 4);
	oTS.WriteChar(' ');
	oTS.WriteInt(63);
	oTS.WriteChar(' ');
	oTS.WriteReal(0.12);
	oTS.WriteChar(' ');
	oTS.WriteStr("String");
	oTS.WriteChar(' ');
	oTS.Close();

	CFileStream oTS2;
	oTS2.OpenFile(L"D:/Test Files/TestStreamsFilter.tmp", true);
	oTS2.WriteStream(&oMS, STREAM_FILTER_FLATE_DECODE, NULL);
	oTS2.Close();
}
void TestObjects()
{
	CStream* pStream = new CFileStream();
	((CFileStream*)pStream)->OpenFile(L"D:/Test Files/TestObjects.tmp", true);

	CNullObject oNull;
	pStream->Write(&oNull);
	pStream->WriteChar('\n');

	CBoolObject oBool = true;
	pStream->Write(&oBool);
	pStream->WriteChar('\n');

	CNumberObject oNumber = 75;
	pStream->Write(&oNumber);
	pStream->WriteChar('\n');

	CRealObject oReal = 13.756;
	pStream->Write(&oReal);
	pStream->WriteChar('\n');

	CNameObject oName = "Name";
	pStream->Write(&oName);
	pStream->WriteChar('\n');

	CStringObject oString("String");
	pStream->Write(&oString, NULL);
	pStream->WriteChar('\n');

	CBinaryObject oBinary((BYTE*)"123", 3);
	pStream->Write(&oBinary, NULL);
	pStream->WriteChar('\n');

	CArrayObject* pArray = new CArrayObject();
	pArray->Add(12);
	pArray->Add(0.13);
	pArray->Add(13.1);
	pStream->Write(pArray, NULL);
	pStream->WriteChar('\n');

	CProxyObject oProxy(pArray);
	oProxy.SetRef(123, 0);
	pStream->Write(&oProxy, NULL);
	pStream->WriteChar('\n');

	CDictObject oDict;
	oDict.Add("Bool", true);
	oDict.Add("Real", 0.12);
	oDict.Add("Number", 12);
	oDict.Add("Name", "SomeName");
	oDict.Add("Array", pArray);
	pStream->Write(&oDict, NULL);
	pStream->WriteChar('\n');

	delete pStream;
}
void TestEncrypt()
{
	CEncrypt oEncrypt;
	oEncrypt.Init();
	oEncrypt.InitKey(1231, 0);

	CFileStream oFS;
	oFS.OpenFile(L"D:/Test Files/TestStreams.tmp", false);

	CFileStream oTS;
	oTS.OpenFile(L"D:/Test Files/TestEncrypt.tmp", true);
	oTS.WriteStream(&oFS, 0, &oEncrypt);
	oTS.Close();

	oFS.Close();
}
void TestDict()
{
	CStream* pStream = new CFileStream();
	((CFileStream*)pStream)->OpenFile(L"D:/Test Files/TestDict.tmp", true);

	CXref oXref(0);

	CDictObject* pDict = new CDictObject(&oXref);
	pDict->Add("Bool", true);
	pDict->Add("Real", 0.12);
	pDict->Add("Number", 12);
	pDict->Add("Name", "SomeName");
	pDict->Add("Array", new CArrayObject());

	CStream* pDictStream = pDict->GetStream();
	pDictStream->WriteEscapeText((BYTE*)"TestTest", 8);
	pDictStream->WriteBinary((BYTE*)"TestTest", 8, NULL);

	CDictObject* pDict2 = new CDictObject(&oXref);
	pDict2->Add("Bool", true);
	pDict2->Add("Real", 0.12);
	pDict2->Add("Number", 12);
	pDict2->Add("Name", "SomeName");
	pDict2->Add("Array", new CArrayObject());

	pDict2->SetFilter(STREAM_FILTER_FLATE_DECODE);

	CStream* pDictStream2 = pDict2->GetStream();
	pDictStream2->WriteEscapeText((BYTE*)"TestTest", 8);
	pDictStream2->WriteBinary((BYTE*)"TestTest", 8, NULL);

	TDate oDate;
	MemSet(&oDate, 0, sizeof(oDate));
	oDate.nDay     = 18;
	oDate.nMonth   = 10;
	oDate.nYear    = 1986;
	oDate.nHour    = 12;
	oDate.nMinutes = 0;
	oDate.nSeconds = 0;
	oDate.nInd     = '-';

	CInfoDict* pInfo = new CInfoDict(&oXref);
	pInfo->SetInfo(EInfoType::InfoCreator, "Ilya");
	pInfo->SetInfo(EInfoType::InfoCreationDate, oDate);

	oXref.WriteToStream(pStream, NULL);

	delete pStream;
}
void TestDocument1()
{
	// PageLabels and Encryption
	CDocument oPdf;
	oPdf.CreateNew();
	oPdf.AddPage();
	oPdf.AddPageLabel(pagenumstyle_Decimal, 1, "Dec-");
	oPdf.AddPage();
	oPdf.AddPage();
	oPdf.AddPageLabel(2, pagenumstyle_UpperRoman, 21, "UppRom-");

	oPdf.SetPassword("123", "qwe");
	oPdf.SetPermission(ENABLE_READ);
	oPdf.SetEncryptionMode(encryptmode_R3);

	oPdf.SaveToFile(L"D:/Test Files/Test1.pdf");
	oPdf.Close();
}
void TestDocument2()
{
	// Outline
	CDocument oPdf;
	oPdf.CreateNew();
	oPdf.AddPage();
	oPdf.AddPage();
	oPdf.AddPage();
	oPdf.AddPage();

	COutline* pOutline1   = oPdf.CreateOutline(NULL, "Test1");
	COutline* pOutline11  = oPdf.CreateOutline(pOutline1, "Test1.1");
	COutline* pOutline111 = oPdf.CreateOutline(pOutline11, "Test1.1.1");
	COutline* pOutline12  = oPdf.CreateOutline(pOutline1, "Test1.2");
	COutline* pOutline2   = oPdf.CreateOutline(NULL, "Test2");
	COutline* pOutline21  = oPdf.CreateOutline(pOutline2, "Test21");
	COutline* pOutline22  = oPdf.CreateOutline(pOutline2, "Test22");
	COutline* pOutline23  = oPdf.CreateOutline(pOutline2, "Test23");
	COutline* pOutline221 = oPdf.CreateOutline(pOutline22, "Test223");

	CDestination* pDest = oPdf.CreateDestination(2);
	pDest->SetXYZ(0, 792, 0);
	pOutline21->SetDestination(pDest);

	pDest = oPdf.CreateDestination(3);
	pDest->SetFit();
	pOutline11->SetDestination(pDest);


	oPdf.SaveToFile(L"D:/Test Files/Test2.pdf");
	oPdf.Close();
}
void TestDocument3()
{
	CDocument oPdf;
	oPdf.CreateNew();

	CPage* pPage;

	pPage = oPdf.AddPage();

	pPage->SetHeight(100);
	pPage->SetWidth(100);

	pPage->MoveTo(10, 10);
	pPage->LineTo(20, 20);
	pPage->CurveTo(70, 30, 30, 20, 50, 50);
	pPage->ClosePath();
	pPage->Fill();

	pPage = oPdf.AddPage();
	pPage->SetHeight(400);
	pPage->SetWidth(600);

	pPage->Ellipse(200, 200, 50, 100);
	pPage->Fill();

	pPage->GrSave();
	double dAngle = 30 * 3.141592f / 180;
	pPage->Concat(cos(dAngle), -sin(dAngle), sin(dAngle), cos(dAngle), -50, 50);
	pPage->MoveTo(400, 200);
	pPage->EllipseArcTo(400, 200, 50, 100, 0, 145);
	pPage->Stroke();
	pPage->GrRestore();



	double pPattern[] ={ 1, 5, 5, 2 };
	pPage->SetDash((const double*)pPattern, 4, 5);
	pPage->MoveTo(10, 10);
	pPage->LineTo(20, 20);
	pPage->CurveTo(70, 30, 30, 20, 50, 50);
	pPage->ClosePath();
	pPage->Stroke();

	pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(400);

	pPage->MoveTo(10, 10);
	pPage->LineTo(350, 10);
	pPage->LineTo(350, 350);
	pPage->LineTo(10, 350);
	pPage->ClosePath();
	pPage->Stroke();

	pPage->SetLineWidth(4);

	pPage->GrSave();
	pPage->SetLineCap(linecap_Butt);
	pPage->MoveTo(20, 20);
	pPage->LineTo(330, 20);
	pPage->Stroke();

	pPage->SetLineCap(linecap_Round);
	pPage->MoveTo(20, 40);
	pPage->LineTo(330, 40);
	pPage->Stroke();

	pPage->SetLineCap(linecap_ProjectingSquare);
	pPage->MoveTo(20, 60);
	pPage->LineTo(330, 60);
	pPage->Stroke();
	pPage->GrRestore();

	pPage->GrSave();
	double pPattern1[] ={ 1, 5, 5, 2 };
	pPage->SetDash((const double*)pPattern1, 4, 0);
	pPage->MoveTo(20, 80);
	pPage->LineTo(330, 80);
	pPage->Stroke();
	double pPattern2[] ={ 1, 5, 5, 2 };
	pPage->SetDash((const double*)pPattern2, 4, 5);
	pPage->MoveTo(20, 100);
	pPage->LineTo(330, 100);
	pPage->Stroke();
	pPage->GrRestore();

	pPage->GrSave();
	pPage->SetStrokeColor(255, 0, 0);
	pPage->SetLineJoin(linejoin_Bevel);
	pPage->MoveTo(20, 120);
	pPage->LineTo(100, 120);
	pPage->LineTo(100, 180);
	pPage->Stroke();

	pPage->SetStrokeColor(0, 255, 0);
	pPage->SetLineJoin(linejoin_Miter);
	pPage->SetMiterLimit(30);
	pPage->MoveTo(120, 120);
	pPage->LineTo(210, 120);
	pPage->LineTo(210, 180);
	pPage->Stroke();

	pPage->SetStrokeColor(0, 0, 255);
	pPage->SetLineJoin(linejoin_Round);
	pPage->MoveTo(220, 120);
	pPage->LineTo(330, 120);
	pPage->LineTo(330, 180);
	pPage->Stroke();
	pPage->GrRestore();

	pPage->GrSave();
	pPage->SetFillColor(120, 15, 15);
	pPage->MoveTo(30, 210);
	pPage->LineTo(140, 210);
	pPage->LineTo(140, 240);
	pPage->LineTo(30, 240);
	pPage->ClosePath();
	pPage->MoveTo(20, 200);
	pPage->LineTo(150, 200);
	pPage->LineTo(150, 250);
	pPage->LineTo(20, 250);
	pPage->ClosePath();
	pPage->EoFillStroke();

	//pPage->SetExtGrState(oPdf.GetExtGState(0.8, 0.8));
	pPage->SetStrokeAlpha(20);
	pPage->SetFillAlpha(20);
	pPage->SetFillColor(15, 15, 120);
	pPage->MoveTo(230, 210);
	pPage->LineTo(320, 210);
	pPage->LineTo(320, 240);
	pPage->LineTo(230, 240);
	pPage->ClosePath();
	pPage->MoveTo(220, 200);
	pPage->LineTo(330, 200);
	pPage->LineTo(330, 250);
	pPage->LineTo(220, 250);
	pPage->ClosePath();
	pPage->FillStroke();
	pPage->GrRestore();


	pPage->MoveTo(230, 310);
	pPage->LineTo(320, 310);
	pPage->LineTo(280, 340);
	pPage->ClosePath();
	pPage->Clip();
	pPage->EndPath();

	pPage->SetStrokeAlpha(122);
	pPage->SetFillAlpha(122);
	//pPage->SetExtGrState(oPdf.GetExtGState(0.5, 0.5));
	pPage->MoveTo(230, 310);
	pPage->LineTo(320, 310);
	pPage->LineTo(320, 340);
	pPage->LineTo(230, 340);
	pPage->ClosePath();
	pPage->MoveTo(220, 300);
	pPage->LineTo(330, 300);
	pPage->LineTo(330, 350);
	pPage->LineTo(220, 350);
	pPage->ClosePath();
	pPage->FillStroke();



	oPdf.SaveToFile(L"D:/Test Files/Test3.pdf");
	oPdf.Close();
}
void TestDocument4()
{
	CDocument oPdf;
	oPdf.CreateNew();
	CPage* pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(600);

	pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(600);

	CDestination* pDest = oPdf.CreateDestination(1);
	pDest->SetXYZ(0, 792, 0);
	CAnnotation* pAnnot = oPdf.CreateLinkAnnot(0, TRect(0, 100, 100, 0), pDest);
	pAnnot = oPdf.CreateUriLinkAnnot(0, TRect(0, 200, 100, 100), "www.rbc.ru");

	oPdf.SaveToFile(L"D:/Test Files/Test4.pdf");
	oPdf.Close();
}
void TestDocument5()
{
	CDocument oPdf;
	oPdf.CreateNew();
	CPage* pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(600);

	//CBgraFrame oFrame;
	//oFrame.OpenFile(L"D:/Test Files/Test.jb2");

	//TIFF* pTiff = TIFFOpenW(L"D:/Test Files/Test.tiff", "w+");
	//TIFFSetField(pTiff, TIFFTAG_IMAGEWIDTH, 800);
	//TIFFSetField(pTiff, TIFFTAG_IMAGELENGTH, 1000);
	//TIFFSetField(pTiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	//TIFFSetField(pTiff, TIFFTAG_XRESOLUTION, 200.0);
	//TIFFSetField(pTiff, TIFFTAG_YRESOLUTION, 200.0);
	//TIFFSetField(pTiff, TIFFTAG_COMPRESSION, COMPRESSION_CCITT_T6);

	//for (int nY = 0; nY < 1000; ++nY)
	//{
	//	BYTE pBuffer[800];
	//	for (int nX = 0; nX < 100; ++nX)
	//	{
	//		pBuffer[nX] = (nX > nY ? 0 : 255);
	//	}

	//	TIFFWriteScanline(pTiff, pBuffer, nY);
	//}
	//TIFFClose(pTiff);

	//CImageDict* pImage = oPdf.CreateImage();
	//pImage->LoadCCITT4(L"D:/Test Files/Test.tiff", 800, 1000);
	//pPage->DrawImage(pImage, 100, 100, 200, 200);

	BYTE* pBuffer = new BYTE[100 * 800];
	for (int nY = 0; nY < 800; ++nY)
	{
		for (int nX = 0; nX < 100; ++nX)
		{
			pBuffer[nX + nY * 100] = rand() % 255;// (nX * 8 > nY ? 0 : 255);
		}
	}

	CImageDict* pImage = oPdf.CreateImage();
	pImage->LoadBW(pBuffer, 800, 800, 100);
	delete pBuffer;
	pPage->DrawImage(pImage, 100, 100, 200, 200);

	pBuffer = new BYTE[100 * 800];
	for (int nY = 0; nY < 800; ++nY)
	{
		for (int nX = 0; nX < 100; ++nX)
		{
			pBuffer[nX + nY * 100] = rand() % 255;// (nX * 8 > nY ? 0 : 255);
		}
	}

	pImage = oPdf.CreateImage();
	pImage->LoadBW(pBuffer, 800, 800, 100);
	delete pBuffer;
	pPage->DrawImage(pImage, 300, 300, 200, 200);


	//CxImage oImage;
	//oImage.Load(L"D:/Test Files/Test.jb2");

	//CImageDict* pJb2Image = oPdf.CreateImage();
	//pJb2Image->LoadJb2(L"D:/Test Files/Test.jb2", 1728, 2376);
	//pPage->DrawImage(pJb2Image, 100, 100, 200, 200);

	//CImageDict* pJpegImage = oPdf.CreateImage();
	//pJpegImage->LoadJpeg(L"D:/Test Files/Test.jpg", 670, 473);
	//pPage->DrawImage(pJpegImage, 100, 100, 200, 200);

	//BYTE* pAlpha = new BYTE[4 * 400 * 300];
	//for (int nY = 0; nY < 300; nY++)
	//{
	//	for (int nX = 0; nX < 400; nX++)
	//	{
	//		int nIndex = 4 * (nY * 400 + nX);

	//		if (nY > 300 / 2)
	//		{
	//			pAlpha[nIndex + 0] = 255;
	//			pAlpha[nIndex + 1] = 0;
	//			pAlpha[nIndex + 2] = 0;
	//			pAlpha[nIndex + 3] = 255;
	//		}
	//		else
	//		{
	//			pAlpha[nIndex + 0] = 0;
	//			pAlpha[nIndex + 1] = 0;
	//			pAlpha[nIndex + 2] = 255;
	//			pAlpha[nIndex + 3] = 128;
	//		}	
	//	}
	//}

	//CImageDict* pJpxImage = oPdf.CreateImage();
	//pJpxImage->LoadJpx(L"D:/Test Files/Test.jp2", 400, 300);
	//pJpxImage->LoadSMask(pAlpha, 400, 300);
	//pPage->DrawImage(pJpxImage, 300, 100, 200, 200);

	//delete[] pAlpha;

	////CImageDict* pJb2Image = oPdf.CreateImage();
	////pJb2Image->LoadJb2(L"D:/Test Files/Test.jbig2", 400, 300);
	////pPage->DrawImage(pJb2Image, 300, 300, 200, 200);

	//CImageDict* pRawImage = oPdf.CreateImage();

	//BYTE* pBgra = new BYTE[4 * 100 * 100];
	//for (int nY = 0; nY < 100; nY++)
	//{
	//	for (int nX = 0; nX < 100; nX++)
	//	{
	//		int nIndex = 4 * (nY * 100 + nX);

	//		if (nX > nY)
	//		{
	//			pBgra[nIndex + 0] = 255;
	//			pBgra[nIndex + 1] = 0;
	//			pBgra[nIndex + 2] = 0;
	//			pBgra[nIndex + 3] = 255;
	//		}
	//		else
	//		{
	//			pBgra[nIndex + 0] = 0;
	//			pBgra[nIndex + 1] = 0;
	//			pBgra[nIndex + 2] = 255;
	//			pBgra[nIndex + 3] = 20;
	//		}
	//	}
	//}

	//pRawImage->LoadRaw(pBgra, 100, 100);
	//pRawImage->LoadSMask(pBgra, 100, 100);
	//pPage->DrawImage(pRawImage, 300, 300, 200, 200);
	//delete[] pBgra;

	oPdf.SaveToFile(L"D:/Test Files/Test5.pdf");
	oPdf.Close();
}
void TestDocument6()
{
	CDocument oPdf;
	oPdf.CreateNew();
	CPage* pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(600);

	CFontDict* pFont = oPdf.CreateFont14(standard14fonts_CourierOblique);

	pPage->BeginText();
	pPage->SetFontAndSize(pFont, 15);
	pPage->DrawText(10, 100, (const BYTE*)"Ilya", 4);

	pPage->SetFontAndSize(pFont, 30);
	pPage->DrawText(100, 100, (const BYTE*)"Ilya", 4);


	pPage->SetCharSpace(6);
	pPage->DrawText(100, 200, (const BYTE*)"Ilya", 4);
	pPage->SetCharSpace(0);

	pPage->SetHorizontalScalling(200.0);
	pPage->DrawText(100, 300, (const BYTE*)"Ilya", 4);
	pPage->SetHorizontalScalling(100.0);

	double dAngle = 30 * 3.141592f / 180;
	pPage->SetTextMatrix(cos(dAngle), -sin(dAngle), sin(dAngle), cos(dAngle), 0, 0);
	pPage->DrawText(100, 400, (const BYTE*)"Ilya", 4);

	pPage->SetTextMatrix(1, 0, 0, 1, 0, 0);
	pPage->DrawText(100, 500, (const BYTE*)"Ilya", 4);

	pPage->EndText();

	pFont = oPdf.CreateFont14(standard14fonts_TimesBold);

	pPage->BeginText();


	pPage->SetFontAndSize(pFont, 30);
	pPage->SetTextRenderingMode(textrenderingmode_Fill);
	pPage->DrawText(300, 100, (const BYTE*)"Ilya", 4);


	pPage->SetTextRenderingMode(textrenderingmode_Stroke);
	pPage->DrawText(300, 200, (const BYTE*)"Ilya", 4);
	pPage->SetCharSpace(0);

	pPage->SetFillColor(255, 0, 0);
	pPage->SetStrokeColor(0, 0, 255);
	pPage->SetTextRenderingMode(textrenderingmode_FillThenStroke);
	pPage->DrawText(300, 300, (const BYTE*)"Ilya", 4);
	pPage->SetHorizontalScalling(100.0);

	pPage->SetTextRenderingMode(textrenderingmode_Invisible);
	pPage->DrawText(300, 400, (const BYTE*)"Ilya", 4);

	pPage->SetTextRenderingMode(textrenderingmode_StrokeClipping);
	pPage->DrawText(300, 500, (const BYTE*)"Ilya", 4);

	pPage->EndText();

	pPage->MoveTo(300, 505);
	pPage->LineTo(400, 505);
	pPage->LineTo(400, 515);
	pPage->LineTo(300, 515);
	pPage->ClosePath();
	pPage->Fill();

	oPdf.SaveToFile(L"D:/Test Files/Test6.pdf");
	oPdf.Close();
}
void TestDocument7()
{
	struct TRange
	{
		unsigned int rangeStart;
		unsigned int rangeEnd;
	};

	int nRangesCount = 10;
	const TRange arrRanges[] =
	{
		{ 0x1D400, 0x1D4FF },
		{ 0x1D500, 0x1D5FF },
		{ 0x1D600, 0x1D6FF },
		{ 0x1D700, 0x1D7FF },

		{ 0x10000, 0x1007F },
		{ 0x10380, 0x1039F },
		{ 0x0020, 0x007F },
		{ 0x0400, 0x04FF },
		{ 0x3040, 0x309F },
		{ 0x30A0, 0x30FF }
	};

	CDocument oPdf;
	oPdf.CreateNew();
	CPage* pPage = oPdf.AddPage();
	pPage->SetHeight(600);
	pPage->SetWidth(1000);

	//CFontCidTrueType* pFont = oPdf.CreateTrueTypeFont(L"D:/Test Files/Test.ttf", 0);
	CFontCidTrueType* pFont = oPdf.CreateTrueTypeFont(L"D:/Test Files/cambria.ttc", 1);

	pPage->BeginText();

	int nX = 10;
	int nY = 590;

	for (int nRangeIndex = 0; nRangeIndex < nRangesCount; nRangeIndex++)
	{
		unsigned int rangeStart = arrRanges[nRangeIndex].rangeStart;
		unsigned int rangeEnd   = arrRanges[nRangeIndex].rangeEnd;
		unsigned int rangeLen = rangeEnd - rangeStart + 1;
		unsigned int* pRange = new unsigned int[rangeLen];
		for (unsigned int unIndex = rangeStart; unIndex <= rangeEnd; unIndex++)
		{
			pRange[unIndex - rangeStart] = unIndex;
		}

		pPage->SetFontAndSize(pFont, 10);

		unsigned char* pString = pFont->EncodeString(pRange, rangeLen);
		pPage->DrawText(nX, nY, (const BYTE*)pString, rangeLen * 2);

		delete[] pString;
		delete[] pRange;

		nY -= 10;
	}

	pPage->EndText();

	oPdf.SaveToFile(L"D:/Test Files/Test7.pdf");
	oPdf.Close();
}
void TestDocument8()
{
	CDocument oPdf;
	oPdf.CreateNew();

	CPage* pPage;

	pPage = oPdf.AddPage();

	pPage->SetHeight(600);
	pPage->SetWidth(600);

	
	pPage->GrSave();
	pPage->Ellipse(200, 200, 50, 100);
	pPage->Clip();
	pPage->EndPath();
	
	int nCount = 2;
	unsigned char pColors[] = 
	{
		255, 0, 0,
		0, 0, 255
	};

	unsigned char pAlphas[] ={ 255, 255 };
	CExtGrState* pExtGrState = NULL;

	double pPoints[] ={0, 1};
	CShading* pShading = oPdf.CreateAxialShading(pPage, 200, 150, 200, 250, pColors, pAlphas, pPoints, 2, pExtGrState);
	pPage->DrawShading(pShading);
	pPage->GrRestore();

	pPage->GrSave();
	pPage->Ellipse(400, 200, 50, 100);
	pPage->Clip();
	pPage->EndPath();

	int nCount2 = 4;
	unsigned char pColors2[] =
	{
		255, 0, 0,
		0, 255, 0,
		255, 255, 255,
		0, 0, 255
	};
	unsigned char pAlphas2[] ={ 255, 255, 255, 255 };

	double pPoints2[] ={ 0, 0.3, 0.7, 1 };

	CShading* pShading2 = oPdf.CreateAxialShading(pPage, 400, 150, 400, 250, pColors2, pAlphas2, pPoints2, nCount2, pExtGrState);
	pPage->DrawShading(pShading2);
	pPage->GrRestore();


	pPage->GrSave();
	pPage->Ellipse(200, 400, 100, 100);
	pPage->Clip();
	pPage->EndPath();

	int nCount3 = 3;
	unsigned char pColors3[] =
	{
		255, 0, 0,
		255, 255, 0,
		0, 0, 255
	};
	unsigned char pAlphas3[] ={ 255, 255, 255 };

	double pPoints3[] ={ 0, 0.5, 1 };

	CShading* pShading3 = oPdf.CreateRadialShading(pPage, 200, 375, 20, 200, 425, 100, pColors3, pAlphas3, pPoints3, nCount3, pExtGrState);
	pPage->DrawShading(pShading3);
	pPage->GrRestore();


	oPdf.SaveToFile(L"D:/Test Files/Test8.pdf");
	oPdf.Close();
}
void TestDocument9()
{
	CDocument oPdf;
	oPdf.CreateNew();

	CPage* pPage;

	pPage = oPdf.AddPage();

	pPage->SetHeight(600);
	pPage->SetWidth(600);

	CImageDict* pJpegImage = oPdf.CreateImage();
	pJpegImage->LoadJpeg(L"D:/Test Files/Test.jpg", 600, 400);

	CImageTilePattern* pPattern = oPdf.CreateImageTilePattern(70, 70, pJpegImage, NULL, imagetilepatterntype_InverseX);

	pPage->GrSave();
	pPage->SetPatternColorSpace(pPattern);
	pPage->Ellipse(300, 300, 200, 150);
	pPage->Clip();
	pPage->Fill();
	pPage->GrRestore();


	oPdf.SaveToFile(L"D:/Test Files/Test9.pdf");
	oPdf.Close();
}

std::vector<std::wstring> GetAllFilesInFolder(std::wstring wsFolder, std::wstring wsExt)
{
	std::vector<std::wstring> vwsNames;

	std::wstring wsSearchPath = wsFolder;
	wsSearchPath.append(L"*.");
	wsSearchPath.append(wsExt);

	WIN32_FIND_DATA oFindData;
	HANDLE hFind = ::FindFirstFile(wsSearchPath.c_str(), &oFindData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(oFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				vwsNames.push_back(oFindData.cFileName);
			}
		} while (::FindNextFile(hFind, &oFindData));
		::FindClose(hFind);
	}
	return vwsNames;
}
void ConvertFolder(std::wstring wsFolderPath, const int nType)
{
	CApplicationFonts oFonts;
	oFonts.Initialize();

	MetaFile::CMetaFile oMetaFile(&oFonts);
	CPdfRenderer oRenderer(&oFonts);
	
	oMetaFile.Close();

	std::wstring sExt;

	switch (nType)
	{
		case MetaFile::c_lMetaEmf: sExt = L"emf"; break;
		case MetaFile::c_lMetaWmf: sExt = L"wmf"; break;
		case MetaFile::c_lMetaSvm: sExt = L"svm"; break;
	}
	double dPx2Mm = 25.4 / 96;
	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, sExt);
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		oRenderer.NewPage();

		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		if (oMetaFile.LoadFromFile(wsFilePath.c_str()))
		{
			double dW, dH, dX, dY;
			oMetaFile.GetBounds(&dX, &dY, &dW, &dH);

			dW *= dPx2Mm;
			dH *= dPx2Mm;
			dX *= dPx2Mm;
			dY *= dPx2Mm;

			double dAspect = dH / dW;
			dW = 595.27;
			dH = dAspect * dW;

			oRenderer.put_Width(dW);
			oRenderer.put_Height(dH);
			//oMetaFile.DrawOnRenderer(&oRenderer, -dX, -dY, dW, dH);
			oMetaFile.DrawOnRenderer(&oRenderer, 0, 0, dW, dH);
			oMetaFile.Close();
		}

		printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
	}

	oRenderer.SaveToFile(wsFolderPath + L"Out.pdf");
}
void TestMetafile()
{
	ConvertFolder(L"D://Test Files//Emf//", MetaFile::c_lMetaEmf);
	//ConvertFolder(L"D://Test Files//Wmf//", MetaFile::c_lMetaWmf);
}
void TestOnlineBin()
{
	//std::wstring wsFolderPath = L"D://Test Files//Txt//";
	std::wstring wsFolderPath = L"D://Test Files//Txt//IvanovaVeronica//";
	std::wstring wsTempFolder = L"D://Test Files//Temp//";

	CApplicationFonts oFonts;
	oFonts.Initialize();

	clock_t oBeginTime = clock();
	double dPx2Mm = 25.4 / 96;
	std::vector<std::wstring> vFiles = GetAllFilesInFolder(wsFolderPath, L"txt");
	for (int nIndex = 0; nIndex < vFiles.size(); nIndex++)
	{
		std::wstring wsFilePath = wsFolderPath;
		wsFilePath.append(vFiles.at(nIndex));
		std::wstring wsOutPath = wsFolderPath + L"Out.pdf";

		CPdfRenderer oRenderer(&oFonts);
		oRenderer.SetTempFolder(wsTempFolder);
		oRenderer.OnlineWordToPdf(wsFilePath, wsOutPath);		

		printf("%d of %d %S\n", nIndex, vFiles.size(), vFiles.at(nIndex).c_str());
	}

	clock_t oEndTime = clock();
	double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("%f\n", dElapsedSecs);
}

void TestDouble()
{
	const int nMaxInt = NSFastIntToString::c_nMaxInt;
	const int nCount = 100000;
	double pDoubles[nCount];
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pDoubles[nIndex] = (rand() % (nMaxInt - 1) + (rand() % (nMaxInt - 1)) / (double)nMaxInt);
	}
	pDoubles[0] = 120.0012;
	pDoubles[1] = 12.012312;
	pDoubles[2] = 0.00012312;

	clock_t oBeginTime = clock();

	char pBuffer[32];
	for (int nI = 0; nI < 10; nI++)
	{
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			double dValue = pDoubles[nIndex];
			memset(pBuffer, 0x00, 32);
			FtoA(pBuffer, dValue, pBuffer + 31);
		}
	}

	clock_t oEndTime = clock();
	double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("FtoA %f\n", dElapsedSecs);

	oBeginTime = clock();

	for (int nI = 0; nI < 10; nI++)
	{

		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			int nResLen = 0;
			double dValue = pDoubles[nIndex];
			int nIVal = (int)dValue;
			int nFVal = (int)(abs(dValue - nIVal) * 10000);

			int nLen = 0;
			const char* sString = NSFastIntToString::GetString(abs(nIVal), nLen);
			if (nIVal < 0)
				pBuffer[nResLen++] = '-';

			memcpy(pBuffer + nResLen, sString, nLen);
			nResLen += nLen;

			if (nFVal)
			{
				sString = NSFastIntToString::GetString(nFVal, nLen);

				pBuffer[nResLen++] = '.';
				int nZeros = 4 - nLen;
				if (nZeros > 0)
				{
					memcpy(pBuffer + nResLen, NSFastIntToString::GetZeros(nZeros), nLen);
					nResLen += nZeros;
				}

				memcpy(pBuffer + nResLen, sString, nLen);
			}
		}
	}

	oEndTime = clock();
	dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
	printf("Fast %f\n", dElapsedSecs);
}

void main()
{
	//TestStreams();
	//TestObjects();
	//TestEncrypt();
	//TestDict();
	//TestDocument1();
	//TestDocument2();
	//TestDocument3();
	//TestDocument4();
	//TestDocument5();
	//TestDocument6();
	//TestDocument7();
	//TestDocument8();
	//TestDocument9();
	//TestMetafile();
	TestOnlineBin();
	//TestDouble();

	//std::string wsTest = "";

	//wsTest = "static const char c_nPrecisionLen[] = {";

	//int nMax = 100000;
	//for (int nIndex = 0; nIndex < nMax; nIndex++)
	//{

	//	std::string sInt = std::to_string(nIndex);
	//	while (sInt.length() > 1 && '0' == sInt.at(sInt.length() - 1))
	//		sInt.pop_back();

	//	wsTest += std::to_string(sInt.length());

	//	if (nIndex != nMax - 1)
	//		wsTest += ",";
	//}

	//wsTest += "};";


	char q;
	std::cin >> q;
}
