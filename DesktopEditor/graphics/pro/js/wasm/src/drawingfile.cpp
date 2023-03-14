#include <malloc.h>
#include <iostream>

#include "../../../../pro/Graphics.h"
#include "../../../../../common/Base64.h"
#include "../../../../../common/File.h"
#include "drawingfile.h"
#include "serialize.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

NSFonts::IApplicationFonts* g_applicationFonts = NULL;
WASM_EXPORT void InitializeFontsBin(BYTE* data, int size)
{
	if (!g_applicationFonts)
	{
		g_applicationFonts = NSFonts::NSApplication::Create();
		g_applicationFonts->InitializeFromBin(data, (unsigned int)size);
	}
}
WASM_EXPORT void InitializeFontsBase64(BYTE* pDataSrc, int nLenSrc)
{
	if (!g_applicationFonts)
	{
		g_applicationFonts = NSFonts::NSApplication::Create();

		int nLenDst = NSBase64::Base64DecodeGetRequiredLength(nLenSrc);
		BYTE* pDataDst = new BYTE[nLenDst];

		if (FALSE == NSBase64::Base64Decode((const char*)pDataSrc, nLenSrc, pDataDst, &nLenDst))
		{
			RELEASEARRAYOBJECTS(pDataDst);
			return;
		}

		g_applicationFonts->InitializeFromBin(pDataDst, (unsigned int)nLenDst);
		RELEASEARRAYOBJECTS(pDataDst);
	}
}
WASM_EXPORT void InitializeFontsRanges(BYTE* pDataSrc)
{
	if (g_applicationFonts && pDataSrc)
		g_applicationFonts->InitializeRanges(pDataSrc);
}
WASM_EXPORT void SetFontBinary(char* path, BYTE* data, int size)
{
	NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	if (pStorage)
	{
		std::string sPathA(path);
		pStorage->Add(UTF8_TO_U(sPathA), data, size, true);
	}
}
WASM_EXPORT int IsFontBinaryExist(char* path)
{
	NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	if (pStorage)
	{
		std::string sPathA(path);
		NSFonts::IFontStream* pStream = pStorage->Get(UTF8_TO_U(sPathA));
		if (pStream)
			return 1;
	}
	return 0;
}

WASM_EXPORT int GetType(BYTE* data, LONG size)
{
	// 0 - PDF
	// 1 - DJVU
	// 2 - XPS
	char* pFirst = strstr((char*)data, "%PDF-" );
	if (pFirst)
		return 0;
	if ( (8 <= size) && (0x41 == data[0] && 0x54 == data[1] && 0x26 == data[2] && 0x54 == data[3] &&
						 0x46 == data[4] && 0x4f == data[5] && 0x52 == data[6] && 0x4d == data[7]))
		return 1;
	return 2;
}
WASM_EXPORT CGraphicsFileDrawing* Open(BYTE* data, LONG size, const char* password)
{
	if (!g_applicationFonts)
		g_applicationFonts = NSFonts::NSApplication::Create();

	// всегда пересоздаем сторадж
	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());

	CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing(g_applicationFonts);
	pGraphics->Open(data, size, GetType(data, size), password);
	return pGraphics;
}
WASM_EXPORT int GetErrorCode(CGraphicsFileDrawing* pGraphics)
{
	if (!pGraphics)
		return -1;
	return pGraphics->GetErrorCode();
}
WASM_EXPORT void  Close     (CGraphicsFileDrawing* pGraphics)
{
	delete pGraphics;
	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NULL);
}
WASM_EXPORT BYTE* GetInfo   (CGraphicsFileDrawing* pGraphics)
{
	NSWasm::CData oRes;
	oRes.SkipLen();

	int pages_count = pGraphics->GetPagesCount();
	oRes.AddInt(pages_count);
	for (int page = 0; page < pages_count; ++page)
	{
		int nW = 0;
		int nH = 0;
		int nDpi = 0;
		pGraphics->GetPageInfo(page, nW, nH, nDpi);
		oRes.AddInt(nW);
		oRes.AddInt(nH);
		oRes.AddInt(nDpi);
	}
	std::wstring wsInfo = pGraphics->GetInfo();
	std::string sInfo = U_TO_UTF8(wsInfo);
	oRes.WriteString((BYTE*)sInfo.c_str(), sInfo.length());

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
WASM_EXPORT BYTE* GetPixmap(CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
{
	return pGraphics->GetPage(nPageIndex, nRasterW, nRasterH, nBackgroundColor);
}
WASM_EXPORT BYTE* GetGlyphs (CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
	return pGraphics->GetGlyphs(nPageIndex);
}
WASM_EXPORT BYTE* GetLinks  (CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
	return pGraphics->GetLinks(nPageIndex);
}
WASM_EXPORT BYTE* GetStructure(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->GetStructure();
}
WASM_EXPORT BYTE* GetInteractiveForms(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->GetInteractiveForms();
}
WASM_EXPORT void DestroyTextInfo(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->DestroyText();
}
WASM_EXPORT int  IsNeedCMap(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->IsNeedCMap() ? 1 : 0;
}
WASM_EXPORT void SetCMapData(CGraphicsFileDrawing* pGraphics, BYTE* data, int size)
{
	pGraphics->SetCMapData(data, size);
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_CPP_BINARY

int READ_INT(BYTE* x)
{
	return x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
}

#include "../../../../../fontengine/ApplicationFontsWorker.h"
#include "../../../../../common/Directory.h"

int main(int argc, char* argv[])
{
	// CHECK SYSTEM FONTS
	CApplicationFontsWorker oWorker;
	oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
	//oWorker.m_arAdditionalFolders.push_back(L"D:\\GIT\\core-fonts");
	oWorker.m_bIsNeedThumbnails = false;

	if (!NSDirectory::Exists(oWorker.m_sDirectory))
	{
		NSDirectory::CreateDirectory(oWorker.m_sDirectory);
		NSFonts::IApplicationFonts* pFonts = oWorker.Check();
		RELEASEINTERFACE(pFonts);
	}

	// INITIALIZE FONTS
	if (true)
	{
		BYTE* pFontSelection = NULL;
		DWORD nFontSelectionLen = 0;
		if (NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/fonts_cache/font_selection.bin", &pFontSelection, nFontSelectionLen))
		{
			char* pBase64 = NULL;
			int nBase64Len = 0;
			NSFile::CBase64Converter::Encode(pFontSelection, (int)nFontSelectionLen, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NOCRLF);

			InitializeFontsBase64((BYTE*)pBase64, nBase64Len);

			RELEASEARRAYOBJECTS(pBase64);
		}
		RELEASEARRAYOBJECTS(pFontSelection);
	}

	// OPEN FILE
	std::wstring sFilePath = NSFile::GetProcessDirectory() + L"/test.pdf";

	BYTE* pFileData = NULL;
	DWORD nFileDataLen = 0;
	if (!NSFile::CFileBinary::ReadAllBytes(sFilePath, &pFileData, nFileDataLen))
		return 1;

	CGraphicsFileDrawing* pGrFile = Open(pFileData, (LONG)nFileDataLen, "");
	int nError = GetErrorCode(pGrFile);

	if (nError != 0)
	{
		Close(pGrFile);
		if (nError == 4)
		{
			std::string sPassword = "123456";
			pGrFile = Open(pFileData, nFileDataLen, sPassword.c_str());
		}
		else
		{
			RELEASEARRAYOBJECTS(pFileData);
			return 1;
		}
	}

	// INFO
	BYTE* pInfo = GetInfo(pGrFile);
	int nLength = READ_INT(pInfo);
	nLength -= 4;

	int nPagesCount = 0;
	int nTestPage = argc > 1 ? atoi(argv[1]) : 0;
	int nWidth = 100;
	int nHeight = 100;

	if (nLength > 0)
	{
		nPagesCount = READ_INT(pInfo + 4);
		if (nPagesCount > 0)
		{
			nWidth  = READ_INT(pInfo + nTestPage * 12 + 8);
			nHeight = READ_INT(pInfo + nTestPage * 12 + 12);
			int dpi = READ_INT(pInfo + nTestPage * 12 + 16);
			//nWidth  *= (dpi / 25.4);
			//nHeight *= (dpi / 25.4);
			std::cout << "Page " << nTestPage << " width " << nWidth << " height " << nHeight << " dpi " << dpi << std::endl;

			nLength = READ_INT(pInfo + nPagesCount * 12 + 8);
			std::cout << "json "<< std::string((char*)(pInfo + nPagesCount * 12 + 12), nLength) << std::endl;;
		}
	}

	free(pInfo);

	// CMAP
	BYTE* pCMapData = NULL;
	if (IsNeedCMap(pGrFile))
	{
		DWORD nCMapDataLength = 0;
		if (NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/cmap.bin", &pCMapData, nCMapDataLength))
		{
			SetCMapData(pGrFile, pCMapData, nCMapDataLength);
		}
	}

	// RASTER
	if (true && nPagesCount > 0)
	{
		BYTE* res = NULL;
		res = GetPixmap(pGrFile, nTestPage, nWidth, nHeight, 0xFFFFFF);

		CBgraFrame oFrame;
		oFrame.put_Data(res);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(4 * nWidth);
		oFrame.put_IsRGBA(true);
		oFrame.SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
		oFrame.ClearNoAttack();

		RELEASEARRAYOBJECTS(res);
	}

	// LINKS
	if (false && nPagesCount > 0)
	{
		BYTE* pLinks = GetLinks(pGrFile, nTestPage);
		nLength = READ_INT(pLinks);
		DWORD i = 4;
		nLength -= 4;
		while (i < nLength)
		{
			DWORD nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout <<  "Link " << std::string((char*)(pLinks + i), nPathLength);
			i += nPathLength;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " Ydest " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " X " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " Y " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " W " << (double)nPathLength / 100.0;
			nPathLength = READ_INT(pLinks + i);
			i += 4;
			std::cout << " H " << (double)nPathLength / 100.0 << std::endl;
		}

		std::cout << std::endl;

		if (pLinks)
			free(pLinks);
	}

	// STRUCTURE
	if (false)
	{
		BYTE* pStructure = GetStructure(pGrFile);
		nLength = READ_INT(pStructure);
		DWORD i = 4;
		nLength -= 4;
		while (i < nLength)
		{
			DWORD nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Page " << nPathLength << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Level " << nPathLength << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pStructure + i);
			i += 4;
			std::cout << "Description " << std::string((char*)(pStructure + i), nPathLength) << std::endl;
			i += nPathLength;
		}

		std::cout << std::endl;

		if (pStructure)
			free(pStructure);
	}

	// GLYPHS
	if (false && nPagesCount > 0)
	{
		// TODO:
		BYTE* pGlyphs = GetGlyphs(pGrFile, nTestPage);
	}

	// INTERACTIVE FORMS
	if (true)
	{
		BYTE* pWidgets = GetInteractiveForms(pGrFile);
		nLength = READ_INT(pWidgets);
		DWORD i = 4;
		nLength -= 4;
		while (i < nLength)
		{
			DWORD nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Annot Flag " << nPathLength << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Name " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
			i += nPathLength;
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Page " << nPathLength << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "X1 " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Y1 " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "X2 " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Y2 " << (double)nPathLength / 100.0 << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Q " << nPathLength << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::string sType = std::string((char*)(pWidgets + i), nPathLength);
			std::cout << "Type " << sType << ", ";
			i += nPathLength;
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Field Flag " << nPathLength << ", ";
			nPathLength = READ_INT(pWidgets + i);
			i += 4;
			std::cout << "Flags " << nPathLength << ", ";
			int nFlags = nPathLength;
			if (nFlags & (1 << 0))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "TU " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 1))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "DS " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 2))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "BE C " << (double)nPathLength / 100.0 << ", ";
			}
			if (nFlags & (1 << 3))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "H " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
				i += nPathLength;
			}
			if (nFlags & (1 << 4))
			{
				int nBorderType = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "Border type " << nBorderType << " ";
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "width " << (double)nPathLength / 100.0 << ", ";
				if (nBorderType == 1)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "Dash Pattern " << (double)nPathLength / 100.0 << " ";
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << (double)nPathLength / 100.0 << ", ";
				}
			}
			if (nFlags & (1 << 5))
			{
				int nBCLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "BC ";
				for (int j = 0; j < nBCLength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << (double)nPathLength / 100.0 << " ";
				}
				std::cout << ", ";
			}
			if (nFlags & (1 << 6))
			{
				nPathLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "R " << nPathLength << ", ";
			}
			if (nFlags & (1 << 7))
			{
				int nBCLength = READ_INT(pWidgets + i);
				i += 4;
				std::cout << "BG ";
				for (int j = 0; j < nBCLength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << (double)nPathLength / 100.0 << " ";
				}
				std::cout << ", ";
			}
			if (sType == "checkbox" || sType == "radiobutton" || sType == "button")
			{
				std::cout << (nFlags & (1 << 9) ? "Yes" : "Off") << ", ";
				if (nFlags & (1 << 10))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "CA " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
				if (sType == "button")
				{
					if (nFlags & (1 << 11))
					{
						nPathLength = READ_INT(pWidgets + i);
						i += 4;
						std::cout << "RC " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
						i += nPathLength;
					}
					if (nFlags & (1 << 12))
					{
						nPathLength = READ_INT(pWidgets + i);
						i += 4;
						std::cout << "AC " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
						i += nPathLength;
					}
				}
				if (nFlags & (1 << 13))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "TP " << nPathLength << ", ";
				}
				if (nFlags & (1 << 14))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "AP.N.Yes " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
			}
			else if (sType == "text")
			{
				if (nFlags & (1 << 9))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
				if (nFlags & (1 << 10))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "MaxLen " << nPathLength << ", ";
				}
				if (nFlags & (1 << 11))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "RichValue " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
			}
			else if (sType == "combobox" || sType == "listbox")
			{
				if (nFlags & (1 << 9))
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << "Value " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;
				}
				if (nFlags & (1 << 10))
				{
					int nOptLength = READ_INT(pWidgets + i);
					i += 4;
					for (int j = 0; j < nOptLength; ++j)
					{
						nPathLength = READ_INT(pWidgets + i);
						i += 4;
						std::cout << std::to_string(j) << " Opt1 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
						i += nPathLength;
						nPathLength = READ_INT(pWidgets + i);
						i += 4;
						std::cout << std::to_string(j) << " Opt2 " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
						i += nPathLength;
					}
				}
			}
			if (nFlags & (1 << 19))
			{
				int nActLength = READ_INT(pWidgets + i);
				i += 4;
				for (int j = 0; j < nActLength; ++j)
				{
					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::cout << std::to_string(j) << " AA " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
					i += nPathLength;

					nPathLength = READ_INT(pWidgets + i);
					i += 4;
					std::string sType((char*)(pWidgets + i), nPathLength);
					std::cout << "Type " << sType << ", ";
					i += nPathLength;

					if (sType == "JavaScript")
					{
						if (nFlags & (1 << 20))
						{
							nPathLength = READ_INT(pWidgets + i);
							i += 4;
							std::cout << "JS " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
							i += nPathLength;
						}
					}
					else if (sType == "GoTo")
					{
						if (nFlags & (1 << 20))
						{
							nPathLength = READ_INT(pWidgets + i);
							i += 4;
							std::cout << "GoTo " << std::string((char*)(pWidgets + i), nPathLength) << " ";
							i += nPathLength;
							nPathLength = READ_INT(pWidgets + i);
							i += 4;
							std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
						}
					}
					else if (sType == "Named")
					{
						if (nFlags & (1 << 20))
						{
							nPathLength = READ_INT(pWidgets + i);
							i += 4;
							std::cout << "Named " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
							i += nPathLength;
						}
					}
					else if (sType == "URI")
					{
						if (nFlags & (1 << 20))
						{
							nPathLength = READ_INT(pWidgets + i);
							i += 4;
							std::cout << "URL " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
							i += nPathLength;
						}
					}
					else if (sType == "Hide")
					{
						if (nFlags & (1 << 20))
						{
							std::cout << "Hide flag " << (nFlags & (1 << 21)) << ", ";
							int nHideLength = READ_INT(pWidgets + i);
							i += 4;
							for (int j = 0; j < nHideLength; ++j)
							{
								nPathLength = READ_INT(pWidgets + i);
								i += 4;
								std::cout << std::to_string(j) << " THide " << std::string((char*)(pWidgets + i), nPathLength) << ", ";
								i += nPathLength;
							}
						}
					}
				}
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		if (pWidgets)
			free(pWidgets);
	}

	Close(pGrFile);
	RELEASEARRAYOBJECTS(pCMapData);

	return 0;
}
#endif
