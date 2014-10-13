#pragma once
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "BinReaderWriterDefines.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../Common/Base64.h"
#include "../../Common/DocxFormat/Source/Base/Nullable.h"

#include <map>

namespace NSFontCutter
{
	class CEmbeddedFontsManager
	{		
	private:
		class CEmbeddedFontInfo
		{
		public:
			CString	Name;

			CString PathRegular;
			LONG	FaceRegular;

			CString PathBold;
			LONG	FaceBold;

			CString PathItalic;
			LONG	FaceItalic;

			CString PathBoldItalic;
			LONG	FaceBoldItalic;

		public:
			CEmbeddedFontInfo()
			{
				Name = _T("");

				PathRegular = _T("");
				FaceRegular = -1;

				PathBold = _T("");
				FaceBold = -1;

				PathItalic = _T("");
				FaceItalic = -1;

				PathBoldItalic = _T("");
				FaceBoldItalic = -1;
			}

			CEmbeddedFontInfo(const CEmbeddedFontInfo& oSrc)
			{
				*this = oSrc;
			}

			CEmbeddedFontInfo& operator=(const CEmbeddedFontInfo& oSrc)
			{
				Name = oSrc.Name;

				PathRegular = oSrc.PathRegular;
				FaceRegular = oSrc.FaceRegular;

				PathBold = oSrc.PathBold;
				FaceBold = oSrc.FaceBold;

				PathItalic = oSrc.PathItalic;
				FaceItalic = oSrc.FaceItalic;

				PathBoldItalic = oSrc.PathBoldItalic;
				FaceBoldItalic = oSrc.FaceBoldItalic;

				return *this;
			}
		};

		std::map<CString, CEmbeddedFontInfo> m_mapFontsEmbeddded;
		std::map<WCHAR, BOOL> m_CharMap;
		
	public:
		CString m_strEmbeddedFontsFolder;

	public:
		CEmbeddedFontsManager()
		{
			m_strEmbeddedFontsFolder = _T("");
		}

		void CheckFont(const CString& strName, CFontManager* pManager)
		{
			std::map<CString, CEmbeddedFontInfo>::const_iterator pPair = m_mapFontsEmbeddded.find(strName);
			if (pPair != m_mapFontsEmbeddded.end())
				return;

			CEmbeddedFontInfo oInfo;
			oInfo.Name = strName;

			CArray<CFontInfo*> aFontInfos = pManager->GetAllStylesByFontName(std::wstring(strName.GetString()));
			for(int i = 0; i < aFontInfos.GetCount(); ++i)
			{
				CFontInfo* pFontInfo = aFontInfos[i];
				if(FALSE != pFontInfo->m_bBold && FALSE != pFontInfo->m_bItalic)
				{
					oInfo.PathBoldItalic = CString(pFontInfo->m_wsFontPath.c_str());
					oInfo.FaceBoldItalic = pFontInfo->m_lIndex;
				}
				else if(FALSE != pFontInfo->m_bBold)
				{
					oInfo.PathBold = CString(pFontInfo->m_wsFontPath.c_str());
					oInfo.FaceBold = pFontInfo->m_lIndex;
				}
				else if(FALSE != pFontInfo->m_bItalic)
				{
					oInfo.PathItalic = CString(pFontInfo->m_wsFontPath.c_str());
					oInfo.FaceItalic = pFontInfo->m_lIndex;
				}
				else
				{
					oInfo.PathRegular = CString(pFontInfo->m_wsFontPath.c_str());
					oInfo.FaceRegular = pFontInfo->m_lIndex;
				}
			}

			m_mapFontsEmbeddded [strName] = oInfo;
		}

		void CheckString(const nullable_string& val)
		{
			if (val.is_init())
				CheckString(*val);
		}
		void CheckString(const CString& val)
		{
			int len = val.GetLength();
			LPWSTR pCBuf1 = new WCHAR[len + 1];
			LPWSTR pCBuf2 = new WCHAR[len + 1];

			LPCWSTR pSrc = (LPCWSTR)val;
			memcpy(pCBuf1, pSrc, len * sizeof(WCHAR));
			memcpy(pCBuf2, pSrc, len * sizeof(WCHAR));

			pCBuf1[len] = 0;
			pCBuf2[len] = 0;
			
			LPWSTR ch1 = CharLower(pCBuf1);
			LPWSTR ch2 = CharUpper(pCBuf2);

			for (int i = 0; i < len; ++i)
			{
				m_CharMap [ch1[i]] = TRUE;
				m_CharMap [ch2[i]] = TRUE;
			}

			RELEASEARRAYOBJECTS(pCBuf1);
			RELEASEARRAYOBJECTS(pCBuf2);
		}

		template<typename T>
		void WriteEmbeddedFonts(T* pWriter)
		{
#ifdef BUILD_CONFIG_FULL_VERSION
			ULONG nCount = 0;
			POSITION pos = m_mapFontsEmbeddded.GetStartPosition();
			while (pos != NULL)
			{
				CEmbeddedFontInfo& oInfo = m_mapFontsEmbeddded.GetNextValue(pos);

				if (_T("") != oInfo.PathRegular)
					++nCount;

				if (_T("") != oInfo.PathBold)
					++nCount;

				if (_T("") != oInfo.PathItalic)
					++nCount;

				if (_T("") != oInfo.PathBoldItalic)
					++nCount;
			}

			pWriter->WriteULONG(nCount);

			CString strFileWrite = m_strEmbeddedFontsFolder + _T("\\fonts.js");
			CFile oFile;
			oFile.CreateFile(strFileWrite);

			CStringA strWrite = "var embedded_fonts = [\n";
			oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());

			strWrite = ",\n";
			bool bIsFirst = true;
			ULONG lIndexF = 0;

			SAFEARRAY* pArrayUnicodes = GenerateSafearray();
			Fonts::IFontConverter* pFontConverter = NULL;
			CoCreateInstance(__uuidof(Fonts::CFontConverter), NULL, CLSCTX_ALL, __uuidof(Fonts::IFontConverter), (void**)&pFontConverter);

			pos = m_mapFontsEmbeddded.GetStartPosition();
			while (pos != NULL)
			{
				CEmbeddedFontInfo& oInfo = m_mapFontsEmbeddded.GetNextValue(pos);

				if (_T("") != oInfo.PathRegular)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());
					
					bIsFirst = false;
					WriteFont(oInfo.Name, oInfo.FaceRegular, oInfo.PathRegular, &oFile, pArrayUnicodes, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(0); // regular

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathBold)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());

					bIsFirst = false;
					WriteFont(oInfo.Name, oInfo.FaceBold, oInfo.PathBold, &oFile, pArrayUnicodes, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(1); // bold

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathItalic)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());

					bIsFirst = false;
					WriteFont(oInfo.Name, oInfo.FaceItalic, oInfo.PathItalic, &oFile, pArrayUnicodes, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(2); // italic

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}

				if (_T("") != oInfo.PathBoldItalic)
				{
					if (!bIsFirst)
						oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());

					bIsFirst = false;
					WriteFont(oInfo.Name, oInfo.FaceBoldItalic, oInfo.PathBoldItalic, &oFile, pArrayUnicodes, pFontConverter);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					
					pWriter->WriteBYTE(0);
					pWriter->WriteString(oInfo.Name);
					
					pWriter->WriteBYTE(1);
					pWriter->WriteULONG(3); // bold italic

					pWriter->WriteBYTE(2);
					pWriter->WriteBYTE(1); // is cutting

					pWriter->WriteBYTE(3);
					pWriter->WriteULONG(lIndexF); // index in cutting

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					lIndexF++;
				}
			}

			strWrite = "\n];";
			oFile.WriteFile(strWrite.GetBuffer(), strWrite.GetLength());

			oFile.CloseFile();

			RELEASEARRAY(pArrayUnicodes);
			RELEASEINTERFACE(pFontConverter);
#endif
		}

		SAFEARRAY* GenerateSafearray()
		{
			// цифры нужны всем. остальное - каждый 
			CheckString(_T("0123456789"));
			m_CharMap [(WCHAR)0x00B0] = TRUE;
			m_CharMap [(WCHAR)0x00B7] = TRUE;
			m_CharMap [(WCHAR)0x00B6] = TRUE;
			m_CharMap [(WCHAR)0x00A4] = TRUE;
			m_CharMap [(WCHAR)0x00A0] = TRUE;
			m_CharMap [(WCHAR)0x0022] = TRUE;
			m_CharMap [(WCHAR)0x0032] = TRUE;
			m_CharMap [(WCHAR)0x0038] = TRUE;
			m_CharMap [(WCHAR)0x0097] = TRUE;
			
			ULONG nCount = (ULONG)m_CharMap.size();
			
			SAFEARRAYBOUND rgsab;
			rgsab.lLbound	= 0;
			rgsab.cElements	= nCount;

			// be sure that WCHAR is 2 bytes long
			SAFEARRAY* pArray = SafeArrayCreate(VT_UI2, 1, &rgsab);
			USHORT* pBuffer = (USHORT*)(pArray->pvData);

			for (std::map<WCHAR, BOOL>::const_iterator pPair = m_CharMap.begin(); pPair != m_CharMap.end(); ++pPair)
			{
				*pBuffer = pPair->first;
				++pBuffer;
			}
			return pArray;
		}

#ifdef BUILD_CONFIG_FULL_VERSION
		void WriteFont(CString& strName, LONG& lFaceIndex, CString& strFontPath, CFile* pFile, SAFEARRAY* pArrayUnicodes, Fonts::IFontConverter* pFontConverter)
		{	
			LONG lFontConverterFlag = 16; // truetype only

			BSTR bsFontIn = strFontPath.AllocSysString();
						
			CString _strName = strName + _T("_Embedded");
			BSTR bsName = _strName.AllocSysString();

			SAFEARRAY* pArrayData = NULL;

			pFontConverter->ToOTF2(bsFontIn, pArrayUnicodes, bsName, lFontConverterFlag, lFaceIndex, &pArrayData); // TrueType only

			SysFreeString(bsFontIn);
			SysFreeString(bsName);

			BYTE* pbBinBuffer = (BYTE*)pArrayData->pvData;
			int nBinBufferLen = pArrayData->rgsabound[0].cElements;
			int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
			BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen];
			if (TRUE == Base64::Base64Encode(pbBinBuffer, nBinBufferLen, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NOCRLF))
			{
				CStringA s = "\"";
				pFile->WriteFile(s.GetBuffer(), s.GetLength());

				pFile->WriteFile(pbBase64Buffer, nBase64BufferLen);

				pFile->WriteFile(s.GetBuffer(), s.GetLength());
			}

			RELEASEARRAYOBJECTS(pbBase64Buffer);

			RELEASEARRAY(pArrayData);			
		}
#endif
	};
}

#define FONT_PICKER_BINARYDATA_SAVEEMBEDDED	0
#define FONT_PICKER_BINARYDATA_SAVEMAP		1