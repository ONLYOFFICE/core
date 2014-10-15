#pragma once

#ifdef _WIN32
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#else
#include "../../Common/DocxFormat/Source/Base/ASCString.h"
#include "../../DesktopEditor/common/Types.h"
#endif

#include <vector>
#include <map>

#define MAX_STACK_SIZE 1024

class COfficeFontPicker;
namespace NSCommon
{
	template<typename Type> class smart_ptr;
	class nullable_string;
	class nullable_bool;
	class nullable_int;
	class nullable_double;
	class nullable_sizet;
}
namespace PPTX
{
	class WritingElement;
	class FileContainer;
}
namespace BinDocxRW
{
	class CDocxSerializer;
}
namespace NSShapeImageGen
{
	class CImageManager;
}
namespace NSFontCutter
{
	class CFontDstManager;
}

namespace NSBinPptxRW
{
	class CStringWriter;
	class CCommonWriter;
	class CSeekTableEntry;

	class CMasterSlideInfo
	{
	public:
		CMasterSlideInfo();

	public:
		LONG m_lThemeIndex;
		CStringA m_strImageBase64;

        std::vector<LONG>       m_arLayoutIndexes;
        std::vector<CStringA>   m_arLayoutImagesBase64;
	};

	class CCommonWriter
	{
	public:
        std::map<size_t, LONG> themes;
        std::map<size_t, LONG> slideMasters;
        std::map<size_t, LONG> slides;
        std::map<size_t, LONG> layouts;
        std::map<size_t, LONG> notes;
        std::map<size_t, LONG> notesMasters;

        std::vector<CMasterSlideInfo> m_oRels;
        std::vector<LONG> m_oSlide_Layout_Rels;

		NSShapeImageGen::CImageManager*	m_pImageManager;
		
		//NSFontCutter::CFontDstManager	m_oFontPicker;

		NSFontCutter::CFontDstManager*	m_pNativePicker;
		COfficeFontPicker*				m_pFontPicker;
		bool m_bDeleteFontPicker;

	public:
		CCommonWriter();
		~CCommonWriter();

	public:
		void CreateFontPicker(COfficeFontPicker* pPicker);
		void CheckFontPicker();
	};

	class CImageManager2
	{
	private:
        std::map<CString, CString>	m_mapImages;
		LONG						m_lIndexNextImage;
		CString						m_strDstMedia;

	public:
		BOOL						m_bIsWord;

	public:
		CImageManager2();
		~CImageManager2();
		void Clear();
		void SetDstMedia(const CString& strDst);
		CString GetDstMedia();

	public:
		template <typename T>
		void Serialize(T* pWriter)
		{
			pWriter->WriteBYTE(m_bIsWord ? 1 : 0);
			pWriter->WriteINT(m_lIndexNextImage);
			pWriter->WriteString(m_strDstMedia);
			
            int lCount = (int)m_mapImages.size();
			pWriter->WriteINT(lCount);

            for (std::map<CString, CString>::const_iterator pPair = m_mapImages.begin(); pPair != m_mapImages.end(); ++pPair)
            {
                pWriter->WriteString(pPair->first);
                pWriter->WriteString(pPair->second);
            }
		}

		template <typename T>
		void Deserialize(T* pReader)
		{
			m_bIsWord = ((true == pReader->GetBool()) ? TRUE : FALSE);
			m_lIndexNextImage = pReader->GetLong();
			m_strDstMedia = pReader->GetString2();

			m_mapImages.clear();
			LONG lCount = pReader->GetLong();

			for (LONG i = 0; i < lCount; ++i)
			{
				CString s1 = pReader->GetString2();
				CString s2 = pReader->GetString2();

				m_mapImages [s1] = s2;
			}
		}

	public:
		CString GenerateImage(const CString& strInput, CString strBase64Image = _T(""));

		void SaveImageAsPng(const CString& strFileSrc, const CString& strFileDst);

		void SaveImageAsJPG(const CString& strFileSrc, const CString& strFileDst);

		bool IsNeedDownload(const CString& strFile);
		CString DownloadImage(const CString& strFile);
	};

	class CBinaryFileWriter
	{
	public:
		class CSeekTableEntry
		{
		public:
			LONG Type;
			LONG SeekPos;

		public:
			CSeekTableEntry();
		};

		CCommonWriter* m_pCommon;
		CString m_strMainFolder;

		NSCommon::smart_ptr<PPTX::FileContainer>* m_pCommonRels;
		BinDocxRW::CDocxSerializer* m_pMainDocument;

		NSCommon::smart_ptr<PPTX::FileContainer>* ThemeDoc;
		NSCommon::smart_ptr<PPTX::WritingElement>* ClrMapDoc;
	private:
		BYTE*		m_pStreamData;
		BYTE*		m_pStreamCur;
		ULONG		m_lSize;

		ULONG		m_lPosition;
		ULONG		m_arStack[MAX_STACK_SIZE];
		ULONG		m_lStackPosition;
		
		std::vector<CSeekTableEntry> m_arMainTables;

	public:
		LONG m_lWidthCurShape;
		LONG m_lHeightCurShape;

	public:
		BYTE* GetBuffer();
		ULONG GetPosition();
		void SetPosition(const ULONG& lPosition);
		void Skip(const ULONG& lSize);

		double GetWidthMM();
		double GetHeightMM();
		void ClearShapeCurSizes();

		// -------------------- stream simple types -----------------------
	public:
		void Clear();

		void SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc);

		void ClearNoAttack();

		void CheckBufferSize(size_t lPlus);
		void WriteBYTE(const BYTE& lValue);
		void WriteBOOL(const bool& bValue);
		void WriteUSHORT(const USHORT& lValue);
		void WriteULONG(const ULONG& lValue);
		void WriteLONG(const long& lValue);
		void WriteINT(const int& lValue);
		void WriteDouble(const double& dValue);
		void WriteDoubleReal(const double& dValue);
		void WriteStringW(const WCHAR* sBuffer);
		void WriteBYTEArray(const BYTE* pBuffer, size_t len);
		void WriteStringA(const char* sBuffer);
		void WriteStringA(CStringA& sBuffer);
		void WriteStringW(CString& sBuffer);
		void WriteStringW2(const WCHAR* sBuffer);
		void WriteStringW2(CString& sBuffer);
		void WriteStringW3(const WCHAR* sBuffer);
		void WriteStringW3(CString& sBuffer);
		// --------------------------------------------------------

	public: 
		CBinaryFileWriter();
		~CBinaryFileWriter();

		void StartRecord(LONG lType);
		void EndRecord();

		void StartMainRecord(LONG lType);
		void WriteReserved(size_t lCount);

		void WriteMainPart();

	public:
		
		void WriteString1(int type, const CString& val);
		void WriteString2(int type, const NSCommon::nullable_string& val);
		void WriteString(const CString& val);

		void WriteString1Data(int type, const WCHAR* pData, ULONG len);

		void WriteBool1(int type, const bool& val);
		void WriteBool2(int type, const NSCommon::nullable_bool& val);

		void WriteInt1(int type, const int& val);
		void WriteInt2(int type, const NSCommon::nullable_int& val);

		void WriteDouble1(int type, const double& val);
		void WriteDouble2(int type, const NSCommon::nullable_double& val);

		void WriteSize_t1(int type, const size_t& val);
		void WriteSize_t2(int type, const NSCommon::nullable_sizet& val);

		template<typename T>
		void WriteLimit1(int type, const T& val)
		{
			BYTE bType = (BYTE)type;
			WriteBYTE(bType);
			WriteBYTE(val.GetBYTECode());
		}
		template<typename T>
		void WriteLimit2(int type, const T& val)
		{
			if (val.is_init())
				WriteLimit1(type, *val);
		}

		template<typename T>
		void WriteRecord1(int type, const T& val)
		{
			StartRecord(type);
			val.toPPTY(this);
			EndRecord();
		}
		template<typename T>
		void WriteRecord2(int type, const T& val)
		{
			if (val.is_init())
			{
				StartRecord(type);
				val->toPPTY(this);
				EndRecord();
			}
		}

#ifdef _WIN32
		template<typename T>
		void WriteRecordArray(int type, int subtype, const CAtlArray<T>& val)
		{
			StartRecord(type);

			ULONG len = (ULONG)val.GetCount();
			WriteULONG(len);

			for (ULONG i = 0; i < len; ++i)
				WriteRecord1(subtype, val[i]);

			EndRecord();
		}
#endif #ifdef _WIN32
		template<typename T>
		void WriteRecordArray(int type, int subtype, const std::vector<T>& val)
		{
			StartRecord(type);

			ULONG len = (ULONG)val.size();
			WriteULONG(len);

			for (ULONG i = 0; i < len; ++i)
				WriteRecord1(subtype, val[i]);

			EndRecord();
		}

		void GetBase64File(const CString& sFile, CStringA& strDst64);

		void WriteTheme64(LONG lIndex, const CString& sFile);

		void WriteLayoutTheme64(LONG lIndexTheme, LONG lIndexLayout, const CString& sFile);

		CString GetFolderForGenerateImages();

		// embedded fonts
		void WriteEmbeddedFonts();

		// serialize ImageManagers
		LPSAFEARRAY Serialize(NSBinPptxRW::CImageManager2* pManager);
		LPSAFEARRAY Serialize(NSShapeImageGen::CImageManager* pManager);

		LPSAFEARRAY GetSafearray();
	};
	class CSlideMasterInfo
	{
	public:
		LONG			m_lThemeIndex;
		std::vector<LONG> m_arLayouts;

	public:
		CSlideMasterInfo();
		CSlideMasterInfo(const CSlideMasterInfo& oSrc);
	};

	class CRelsGenerator
	{
	private:
		CStringWriter* m_pWriter;
		int									m_lNextRelsID;
		std::map<CString, int>				m_mapImages;

		std::map<CString, int>				m_mapLinks;

	public:
		CImageManager2*						m_pManager;

	public:
		CRelsGenerator(CImageManager2* pManager);
		~CRelsGenerator();
		void Clear();

		void StartRels();

		void StartTheme();

		void StartMaster(int nIndexTheme, const CSlideMasterInfo& oInfo);
		void StartLayout(int nIndexTheme);
		void StartSlide(int nIndexSlide, int nIndexLayout);
		void StartNote(int nIndexSlide);
		void WriteMasters(int nCount);
		void WriteThemes(int nCount);
		void WriteSlides(int nCount);
		void WriteSlideComments(int nComment);
		void EndPresentationRels(const bool& bIsCommentsAuthors);
		int GetNextId();
		void CloseRels();
		void AddRels(const CString& strRels);
		void SaveRels(const CString& strFile);

		int WriteImage(const CString& strImagePath, CString strBase64Image);
		int WriteChart(int nChartNumber, LONG lDocType);

		int WriteRels(const CString& bsType, const CString& bsTarget, const CString& bsTargetMode);

		int WriteHyperlink(const CString& strLink, const bool& bIsActionInit);	
	};

	class CBinaryFileReader
	{
	private:
		BYTE* m_pData;
		LONG m_lSize;
		LONG m_lPos;
		BYTE* m_pDataCur;

		LONG m_lNextId;

	public:
		//CRelsGenerator m_oRels;
		CString m_strFolder;
		CString m_strFolderThemes;

		LONG m_lChartNumber;
		CString m_strContentTypes;

		BinDocxRW::CDocxSerializer* m_pMainDocument;

		LONG		m_lDocumentType;

		CRelsGenerator* m_pRels;
		std::vector<CRelsGenerator*> m_stackRels;
		int m_nCurrentRelsStack;
	
	public:
		CBinaryFileReader();
		~CBinaryFileReader();

		void SetMainDocument(BinDocxRW::CDocxSerializer* pMainDoc);

	public:
		void Init(BYTE* pData, LONG lStart, LONG lSize);
		LONG GenerateNextId();

	public:

		int Seek(LONG _pos);
		int Skip(LONG _skip);
		bool Peek(int nSizeToRead);
		
		// 1 bytes
		BYTE GetUChar();
		bool GetBool();

		// 2 byte
		USHORT GetUShort();

		// 4 byte
		ULONG GetULong();

		LONG GetLong();
		double GetDouble();
		// 8 byte
		double GetDoubleReal();
		//String
		CString GetString(LONG len);
		CStringA GetString1(LONG len);
		CString GetString2();
		CString GetString3(LONG len);

		LPSAFEARRAY GetArray(LONG len);

		CStringA GetString2A();
		void SkipRecord();

		LONG GetPos();

		LONG GetSize();

		BYTE* GetData();
		BYTE* GetPointer(int nSize);
	public:
		void Deserialize(NSBinPptxRW::CImageManager2* pManager, LPSAFEARRAY pArray);
		void Deserialize(NSShapeImageGen::CImageManager* pManager, LPSAFEARRAY pArray);
	};
}
