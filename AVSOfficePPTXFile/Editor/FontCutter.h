#pragma once
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"
#include "BinReaderWriterDefines.h"

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

		CAtlMap<CString, CEmbeddedFontInfo> m_mapFontsEmbeddded;
		CAtlMap<WCHAR, BOOL> m_CharMap;
		
	public:
		CString m_strEmbeddedFontsFolder;

	public:
		CEmbeddedFontsManager()
		{
			m_strEmbeddedFontsFolder = _T("");
		}

		void CheckFont(const CString& strName, AVSGraphics::IAVSFontManager* pManager)
		{
			CAtlMap<CString, CEmbeddedFontInfo>::CPair* pPair = m_mapFontsEmbeddded.Lookup(strName);
			if (NULL != pPair)
				return;

			CEmbeddedFontInfo oInfo;
			oInfo.Name = strName;

			VARIANT var;
			var.vt = VT_BSTR;
			var.bstrVal = strName.AllocSysString();

			pManager->GetAdditionalParam(L"GetAllStylesByFontName", &var);
			CString strXml = (CString)var.bstrVal;

			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlString(strXml))
			{
				XmlUtils::CXmlNodes oNodes;
				if (oNode.GetNodes(_T("font"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNodeF;
						oNodes.GetAt(i, oNodeF);

						LONG lStyle = oNodeF.ReadAttributeInt(_T("style"));
						switch (lStyle)
						{
						case 0:
							oInfo.PathRegular = oNodeF.GetAttribute(_T("path"));
							oInfo.FaceRegular = oNodeF.ReadAttributeInt(_T("faceindex"));
							break;
						case 1:
							oInfo.PathBold = oNodeF.GetAttribute(_T("path"));
							oInfo.FaceBold = oNodeF.ReadAttributeInt(_T("faceindex"));
							break;
						case 2:
							oInfo.PathItalic = oNodeF.GetAttribute(_T("path"));
							oInfo.FaceItalic = oNodeF.ReadAttributeInt(_T("faceindex"));
							break;
						case 3:
							oInfo.PathBoldItalic = oNodeF.GetAttribute(_T("path"));
							oInfo.FaceBoldItalic = oNodeF.ReadAttributeInt(_T("faceindex"));
							break;
						default:
							break;
						}
					}
				}
			}

			m_mapFontsEmbeddded.SetAt(strName, oInfo);
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
				m_CharMap.SetAt(ch1[i], TRUE);
				m_CharMap.SetAt(ch2[i], TRUE);
			}

			RELEASEARRAYOBJECTS(pCBuf1);
			RELEASEARRAYOBJECTS(pCBuf2);
		}

		template<typename T>
		void WriteEmbeddedFonts(T* pWriter)
		{
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
		}

		SAFEARRAY* GenerateSafearray()
		{
			// цифры нужны всем. остальное - каждый 
			CheckString(_T("0123456789"));
			m_CharMap.SetAt((WCHAR)0x00B0, TRUE);
			m_CharMap.SetAt((WCHAR)0x00B7, TRUE);
			m_CharMap.SetAt((WCHAR)0x00B6, TRUE);
			m_CharMap.SetAt((WCHAR)0x00A4, TRUE);
			m_CharMap.SetAt((WCHAR)0x00A0, TRUE);
			m_CharMap.SetAt((WCHAR)0x0022, TRUE);
			m_CharMap.SetAt((WCHAR)0x0032, TRUE);
			m_CharMap.SetAt((WCHAR)0x0038, TRUE);
			m_CharMap.SetAt((WCHAR)0x0097, TRUE);
			
			ULONG nCount = (ULONG)m_CharMap.GetCount();
			
			SAFEARRAYBOUND rgsab;
			rgsab.lLbound	= 0;
			rgsab.cElements	= nCount;

			SAFEARRAY* pArray = SafeArrayCreate(VT_UI2, 1, &rgsab);
			USHORT* pBuffer = (USHORT*)(pArray->pvData);

			POSITION pos = m_CharMap.GetStartPosition();
			while (NULL != pos)
			{
				CAtlMap<WCHAR, BOOL>::CPair* pPair = m_CharMap.GetNext(pos);
				*pBuffer = pPair->m_key;
				++pBuffer;
			}
			return pArray;
		}

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
	};
}

#define FONT_PICKER_BINARYDATA_SAVEEMBEDDED	0
#define FONT_PICKER_BINARYDATA_SAVEMAP		1