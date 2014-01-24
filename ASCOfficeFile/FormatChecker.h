#pragma once
#include "../Common/OfficeFileFormats.h"
#include "../Common/OfficeFileErrorDescription.h"
#include <string>
#include "../Common/XmlUtils.h"


#import "./../Redist/ASCOfficeUtils.dll"		 rename_namespace("AVSOfficeUtils"), raw_interfaces_only

class AVSOfficeFormatChecker{


private: AVSOfficeUtils::IOfficeUtils2*		m_pOfficeUtils;
private: static const long mc_aFormatTable[][2];
private: static const int mc_nFormatTableSize;
private: static const long mc_aViewTable[];
private: static const int mc_nViewTableSize;


public:	AVSOfficeFormatChecker()
		{
			m_pOfficeUtils = NULL;
			CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils),
								NULL, 
								CLSCTX_INPROC_SERVER, 
								__uuidof(AVSOfficeUtils::IOfficeUtils2), 
								(void **)&(m_pOfficeUtils));
		}
public: ~AVSOfficeFormatChecker()
		{
			if (m_pOfficeUtils != NULL)
			{
				m_pOfficeUtils->Release();
				m_pOfficeUtils = NULL;
			}
		}
//nSource - define исходного формата
//nDestination - define конечного формата
public: bool CanConvert( long nSource, long nDestination )
		{
			////напр€мую
			//for( int i = 0; i < mc_nFormatTableSize; i++ )
			//{
			//	if( mc_aFormatTable[i][0] == nSource && mc_aFormatTable[i][1] == nDestination  )
			//	{
			//		return true;
			//	}
			//}
			////используем docx как контейнер
			//bool bDocxContainer = false;
			//for( int i = 0; i < mc_nFormatTableSize; i++ )
			//{
			//	if( mc_aFormatTable[i][0] == nSource && mc_aFormatTable[i][1] == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX  )
			//	{
			//		bDocxContainer = true;
			//		break;
			//	}
			//}
			//if( true == bDocxContainer )
			//	for( int i = 0; i < mc_nFormatTableSize; i++ )
			//	{
			//		if( mc_aFormatTable[i][0] == AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX && mc_aFormatTable[i][1] == nDestination  )
			//		return true;
			//	}

			////используем xlsx как контейнер
			//bool bXlsxContainer = false;
			//for( int i = 0; i < mc_nFormatTableSize; i++ )
			//{
			//	if( mc_aFormatTable[i][0] == nSource && mc_aFormatTable[i][1] == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX  )
			//	{
			//		bXlsxContainer = true;
			//		break;
			//	}
			//}
			//if( true == bXlsxContainer )
			//	for( int i = 0; i < mc_nFormatTableSize; i++ )
			//	{
			//		if( mc_aFormatTable[i][0] == AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX && mc_aFormatTable[i][1] == nDestination  )
			//		return true;
			//		}
			//return false;
			return true;
		}
public: bool CanView( long nSource )
		{
			////можно сделать бинарно
			//for( int i = 0; i < mc_nViewTableSize; i++ )
			//	if( mc_aViewTable[i] == nSource )
			//		return true;
			//return false;
			return true;
		}
//sFileName - полное им€ файла
//return - формат файла беретс€ по началу файла, если это невозможно, то по расширению
public: long GetFileFormat( CString sFileName )
		{
			long lError;
			long format = AVS_OFFICESTUDIO_FILE_UNKNOWN;
			//“акой пустой файл по€вл€етс€, если создавать документ в контекстном меню по ѕ ћ
			//ќткрываем его как txt
			if( TRUE == IsEmptyFile( sFileName, lError ) )
				return AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
			//по заголовку файла
			if( TRUE == IsRTFFile( sFileName, lError ) )
				format = AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF;
			else if( TRUE == IsPDFFile( sFileName, lError ) )
				format = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
			else if ( TRUE == IsFB2File( sFileName, lError ) )
				format = AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2;
			else if( TRUE == IsDJVUFile( sFileName, lError ) )
				format = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
			else if( TRUE == IsMOBIFile( sFileName, lError ) )
				format = AVS_OFFICESTUDIO_FILE_DOCUMENT_MOBI;
			else
				format = IsImageFile(sFileName, lError);

			if (format != AVS_OFFICESTUDIO_FILE_UNKNOWN)
				return format;

			//по содержанию файла	
			IStorage* storage = IsStorageType( sFileName );
			if (storage != NULL)
			{
				if( TRUE == IsDOCFile( storage, lError ) )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC;
				else if( TRUE == IsXLSFile( storage, lError ) )
					format = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS;
				else if( TRUE == IsPPTFile( storage, lError ) )
					format = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT;
				else if( TRUE == IsMS_OFFCRYPTOFile( storage, lError ) )
					format = AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO;
				storage->Release();
			}					
			else if (IsArchiveType( sFileName ))
			{			
				if ( TRUE == IsDOCXFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX;
				else if ( TRUE == IsXLSXFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX;
				else if ( TRUE == IsPPTXFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX;
				else if ( TRUE == IsODTFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT;
				else if ( TRUE == IsODPFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP;
				else if ( TRUE == IsODSFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS;
				else if ( TRUE == IsEPUBFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB;
				else if ( TRUE == IsXPSFile( sFileName, lError ) )
					format = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
			}


			//возвращаем тип по расширению
			else
			{
				TCHAR tBuffer[256];
				_tsplitpath( sFileName, NULL,NULL,NULL, tBuffer );
				CString sExtention( tBuffer );
				CharLower( sExtention.GetBuffer() );
				sExtention.ReleaseBuffer();

				if ( _T(".mht") ==  sExtention )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT;
				else if ( _T(".txt") ==  sExtention || _T(".xml") ==  sExtention || _T(".xslt") ==  sExtention )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT;
				else if( _T(".csv") ==  sExtention )
					format = AVS_OFFICESTUDIO_FILE_SPREADSHEET_CSV;
				else if( _T(".svg") ==  sExtention )
					format = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SVG;
				else if ( _T(".html") ==  sExtention || _T(".htm") ==  sExtention )
					format = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
				else
				{
					//пробуем определить html по содержимому
					//об€зательно пускаем эту функцию в конце когда другие способы не помогли
					if ( TRUE == IsHtmlFile( sFileName, lError ) )
						format = AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML;
				}
			}
			return format;
		}
private: BOOL IsEmptyFile(const CString &sFilePath, long &lError)
		 {
			lError = NOERROR;
			HANDLE  hFile;

			hFile = ::CreateFile( sFilePath , GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE==hFile) 
			{
				lError = AVS_ERROR_FILEACCESS;
				return FALSE;
			}
			DWORD dwDataSize = GetFileSize(hFile, NULL);
			::CloseHandle(hFile);

			if(INVALID_FILE_SIZE != dwDataSize && 0 == dwDataSize)
				return TRUE;
			return FALSE;
		 }
private: BOOL IsRTFFile(const CString &sFilePath, long &lError)
		 {
			lError = NOERROR;
			BYTE    pBuffer[ 5 ];
			DWORD    dwBytesRead;
			HANDLE  hFile;

			hFile = ::CreateFile( sFilePath , GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE==hFile) 
			{
				lError = AVS_ERROR_FILEACCESS;
				return FALSE;
			}
		 
			::ReadFile( hFile, pBuffer, 5, &dwBytesRead, NULL );
			::CloseHandle(hFile); 

			if( (5 <= dwBytesRead) && '{' == pBuffer[0] && '\\' == pBuffer[1] && 'r' == pBuffer[2] && 't' == pBuffer[3] && 'f' == pBuffer[4] )
				return TRUE;

			return FALSE;
		 }
private: BOOL IsMOBIFile(const CString &sFilePath, long &lError)
		 {
			lError = NOERROR;
			BYTE    pBuffer[ 70 ];
			DWORD    dwBytesRead;
			HANDLE  hFile;

			hFile = ::CreateFile( sFilePath , GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE==hFile) 
			{
				lError = AVS_ERROR_FILEACCESS;
				return FALSE;
			}
		 
			::ReadFile( hFile, pBuffer, 70, &dwBytesRead, NULL );
			::CloseHandle(hFile); 

			if (70 > dwBytesRead)
				return FALSE;

			if ((pBuffer[60] == (BYTE)'B') &&
				(pBuffer[61] == (BYTE)'O') &&
				(pBuffer[62] == (BYTE)'O') &&
				(pBuffer[63] == (BYTE)'K') &&
				(pBuffer[64] == (BYTE)'M') &&
				(pBuffer[65] == (BYTE)'O') &&
				(pBuffer[66] == (BYTE)'B') &&
				(pBuffer[67] == (BYTE)'I'))
				return TRUE;

			if ((pBuffer[60] == (BYTE)'T') &&
				(pBuffer[61] == (BYTE)'E') &&
				(pBuffer[62] == (BYTE)'X') &&
				(pBuffer[63] == (BYTE)'t') &&
				(pBuffer[64] == (BYTE)'R') &&
				(pBuffer[65] == (BYTE)'E') &&
				(pBuffer[66] == (BYTE)'A') &&
				(pBuffer[67] == (BYTE)'d'))
				return TRUE;

			return FALSE;
		 }
private: BOOL IsPDFFile(const CString &sFilePath, long &lError)
		 {
			lError = NOERROR;
			BYTE	pBuffer[ 20 ];
			DWORD	dwBytesRead;
			HANDLE	hFile;

			hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hFile) 
			{
				lError = AVS_ERROR_FILEACCESS;
				return FALSE;
			}
			::ReadFile(hFile, pBuffer, 19, &dwBytesRead, NULL);
			::CloseHandle(hFile);

			pBuffer[dwBytesRead] = '\0';


			char* pFirst = strstr( (char*)pBuffer, "%PDF-" );
			if( NULL != pFirst )
				return TRUE;

			return FALSE;
		 }
private: BOOL IsDJVUFile(const CString &sFilePath, long &lError)
		 {
			lError = NOERROR;
			BYTE	pBuffer[ 8 ];
			DWORD	dwBytesRead;
			HANDLE	hFile;

			hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hFile) 
			{
				lError = AVS_ERROR_FILEACCESS;
				return FALSE;
			}
			::ReadFile(hFile, pBuffer, 8, &dwBytesRead, NULL);
			::CloseHandle(hFile); 

			//djvu
			//Hex: 41 54 26 54 46 4f 52 4d
			//ASCII: AT&TFORM
			
			if ( 8 <= dwBytesRead )
			{
				if ( (0x41 == pBuffer[0]) && (0x54 == pBuffer[1]) && (0x26 == pBuffer[2]) && (0x54 == pBuffer[3]) && (0x46 == pBuffer[4]) && (0x4f == pBuffer[5]) && (0x52 == pBuffer[6]) && (0x4d == pBuffer[7]))
					return TRUE;
			}
			return FALSE;
		 }


private: BOOL IsDOCFile(IStorage* storage, long &lError)
		 {
			 lError = NOERROR;
			 	
			 IStream* stream = NULL;
			 HRESULT hresult = storage->OpenStream( L"WordDocument", NULL, ( STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ),
				 NULL, &stream );
			 if ( stream != NULL )
				 stream->Release();
			 return hresult == S_OK? TRUE: FALSE;
		 }
private: int IsImageFile(const CString &sFilePath, long &lError)
		 {
			 lError = NOERROR;
			 BYTE	pBuffer[ 44 ];
			 DWORD	dwBytesRead;
			 HANDLE	hFile;

			 hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			 if (INVALID_HANDLE_VALUE == hFile) 
			 {
				 return AVS_OFFICESTUDIO_FILE_UNKNOWN;
			 }
			 ::ReadFile(hFile, pBuffer, 44, &dwBytesRead, NULL);
			 ::CloseHandle(hFile); 

			 //jpeg	
			 // Hex: FF D8 FF
			 if ( (3 <= dwBytesRead) && (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
				 return AVS_OFFICESTUDIO_FILE_IMAGE_JPG;

			 //bmp ( http://ru.wikipedia.org/wiki/BMP )
			 //Hex: 42 4D
			 //ASCII: BM
			 //Hex (position 6) : 00 00
			 //Hex (position 26): 01 00
			 //Hex (position 28): 00 || 01 || 04 || 08 || 10 || 18 || 20
			 //Hex (position 29): 00
			 //Hex (position 30): 00 || 01 || 02 || 03 || 04 || 05
			 //Hex (position 31): 00 00 00
			 if ( (34 <= dwBytesRead) && (0x42 == pBuffer[0]) && (0x4D == pBuffer[1]) && 
				 (0x00 == pBuffer[6]) && (0x00 == pBuffer[7]) && (0x01 == pBuffer[26]) && (0x00 == pBuffer[27]) && 
				 ( (0x00 == pBuffer[28]) || (0x01 == pBuffer[28]) || (0x04 == pBuffer[28]) || (0x08 == pBuffer[28]) ||
				 (0x10 == pBuffer[28]) || (0x18 == pBuffer[28]) || (0x20 == pBuffer[28]) ) && (0x00 == pBuffer[29]) && 
				 ( (0x00 == pBuffer[30]) || (0x01 == pBuffer[30]) || (0x02 == pBuffer[30]) || (0x03 == pBuffer[30]) || 
				 (0x04 == pBuffer[30]) || (0x05 == pBuffer[30]) ) && (0x00 == pBuffer[31]) && (0x00 == pBuffer[32]) && (0x00 == pBuffer[33]) )
				 return AVS_OFFICESTUDIO_FILE_IMAGE_BMP;

			 //gif 
			 //Hex: 47 49 46 38
			 //ASCII: GIF8
			 //or for GIF87a...
			 //Hex: 47 49 46 38 37 61
			 //ASCII: GIF87a
			 //or for GIF89a...
			 //Hex: 47 49 46 38 39 61
			 //ASCII: GIF89a
			 if ( (4 <= dwBytesRead) && (0 == strncmp( (char *)pBuffer, "GIF8", 4)))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_GIF;
			 if ( (6 <= dwBytesRead) && ( (0 == strncmp( (char *)pBuffer, "GIF87a", 6)) || (0 == strncmp((char *)pBuffer, "GIF89a", 6)) ) )
				 return AVS_OFFICESTUDIO_FILE_IMAGE_GIF;

			 //png 
			 //Hex: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
			 //ASCII: .PNG........IHDR
			 if ( (16 <= dwBytesRead) && (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
				 && (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
				 && (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
				 && (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_PNG;

			 //CR2
			 //Hex: 49 49 2A 00 10 00 00 00 43 52
			 //ASCII: II*.....CR
			 if ( (10 <= dwBytesRead ) && (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2])
				 && (0x00 == pBuffer[3])  && (0x10 == pBuffer[4]) && (0x00 == pBuffer[5]) && (0x00 == pBuffer[6])
				 && (0x00 == pBuffer[7]) && (0x43 == pBuffer[8]) && (0x52 == pBuffer[9]) )
				 return AVS_OFFICESTUDIO_FILE_IMAGE_CR2;

			 //tiff
			 //Hex: 49 49 2A 00
			 //ASCII:
			 //or for big endian
			 //Hex: 4D 4D 00 2A
			 //ASCII: MM.*
			 //or for little endian
			 //Hex: 49 49 2A 00
			 //ASCII: II*
			 if ( 4 <= dwBytesRead )
			 {
				 if ( ( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ) ||
					 ( (0x4D == pBuffer[0]) && (0x4D == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x2A == pBuffer[3]) ) ||
					 ( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ))
					 return AVS_OFFICESTUDIO_FILE_IMAGE_TIFF;
			 }

			 //wmf
			 //Hex: D7 CD C6 9A 00 00
			 //or for Windows 3.x
			 //Hex: 01 00 09 00 00 03
			 if ( 6 <= dwBytesRead )
			 {
				 if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) || 
					 ((0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x09 == pBuffer[2]) && (0x00 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x03 == pBuffer[5]) ))
					 return AVS_OFFICESTUDIO_FILE_IMAGE_WMF;
			 }

			 //emf ( http://wvware.sourceforge.net/caolan/ora-wmf.html )
			 //Hex: 01 00 00 00
			 //Hex (position 40): 20 45 4D 46
			 if ( (44 <= dwBytesRead) && (0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x00 == pBuffer[3]) && 
				 (0x20 == pBuffer[40]) && (0x45 == pBuffer[41]) && (0x4D == pBuffer[42]) && (0x46 == pBuffer[43]) )
				 return AVS_OFFICESTUDIO_FILE_IMAGE_EMF;

			 //pcx ( http://www.fileformat.info/format/pcx/corion.htm )
			 //Hex (position 0): 0A
			 //Hex (position 1): 00 || 01 || 02 || 03 || 04 || 05
			 //Hex (position 3): 01 || 02 || 04 || 08 ( Bytes per pixel )
			 if ( (4 <= dwBytesRead) && (0x0A == pBuffer[0]) && (0x00 == pBuffer[1] || 0x01 == pBuffer[1] ||
				 0x02 == pBuffer[1] || 0x03 == pBuffer[1] || 0x04 == pBuffer[1] || 0x05 == pBuffer[1] ) && 
				 ( 0x01 == pBuffer[3] || 0x02 == pBuffer[3] || 0x04 == pBuffer[3] || 0x08 == pBuffer[3] ))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_PCX;

			 //tga ( http://www.fileformat.info/format/tga/corion.htm )
			 //DATA TYPE 1-COLOR-MAPPED IMAGES								: Hex (position 1) : 01 01
			 //DATA TYPE 2-TRUE-COLOR IMAGES									: Hex (position 1) : 00 02
			 //DATA TYPE 3-BLACK AND WHITE(UNMAPPED) IMAGES					: Hex (position 1) : 00 03
			 //DATA TYPE 9-RUN-LENGTH ENCODED(RLE),COLOR-MAPPED IMAGES		: Hex (position 1) : 01 09
			 //DATA TYPE 10-RUN-LENGTH ENCODED(RLE),TRUE-COLOR IMAGES		: Hex (position 1) : 00 0A
			 //DATA TYPE 11-RUN-LENGTH ENCODED(RLE),BLACK AND WHITE IMAGES	: Hex (position 1) : 00 0B
			 // + Bytes per pixel											: Hex (position 16): 0x08 || 0x10 || 0x18 || 0x20
			 if ( (17 <= dwBytesRead) && ( (0x01 == pBuffer[1] && 0x01 == pBuffer[2]) || (0x00 == pBuffer[1] && 0x02 == pBuffer[2]) ||
				 (0x00 == pBuffer[1] && 0x03 == pBuffer[2]) || (0x01 == pBuffer[1] && 0x09 == pBuffer[2]) ||
				 (0x00 == pBuffer[1] && 0x0A == pBuffer[2]) || (0x00 == pBuffer[1] && 0x0B == pBuffer[2]) )
				 && ( 0x08 == pBuffer[16] || 0x10 == pBuffer[16] || 0x18 == pBuffer[16] || 0x20 == pBuffer[16] ))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_TGA;

			 //ras
			 //Hex: 59 A6 6A 95
			 //ASCII: Y
			 if ( (4 <= dwBytesRead ) && (0x59 == pBuffer[0]) && (0xA6 == pBuffer[1]) && (0x6A == pBuffer[2])&& (0x95 == pBuffer[3]))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_RAS;

			 //ipod 
			 //(None or Unknown)

			 //psd
			 //Hex: 38 42 50 53 00 01 00 00 00 00 00 00 00
			 //ASCII: 8BPS
			 if ( (13 <= dwBytesRead ) && (0x38 == pBuffer[0]) && (0x42 == pBuffer[1]) && (0x50 == pBuffer[2])
				 && (0x53 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x01 == pBuffer[5]) && (0x00 == pBuffer[6])
				 && (0x00 == pBuffer[7]) && (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10])
				 && (0x00 == pBuffer[11]) && (0x00 == pBuffer[12]))
				 return AVS_OFFICESTUDIO_FILE_IMAGE_PSD;

			 return AVS_OFFICESTUDIO_FILE_UNKNOWN;
		 }
		 

private: BOOL IsXLSFile(IStorage* storage, long &lError)
		 {
			 lError = NOERROR;
			 
			 IStream* stream = NULL;
			 HRESULT hresult = storage->OpenStream( L"Workbook", NULL, ( STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ),
				 NULL, &stream );	
			 if ( stream != NULL )
				 stream->Release();
			 return hresult == S_OK? TRUE: FALSE;
		 }


private: BOOL IsPPTFile(IStorage* storage, long &lError)
		 {
			 lError = NOERROR;
			 IStream* stream = NULL;
			 HRESULT hresult = storage->OpenStream( L"PowerPoint Document", NULL, ( STGM_READ | STGM_DIRECT | STGM_SHARE_EXCLUSIVE ),
				 NULL, &stream );
			 if ( stream != NULL )
				 stream->Release();
			 return hresult == S_OK? TRUE: FALSE;
		 }

private: BOOL IsMS_OFFCRYPTOFile(IStorage* storage, long &lError)
		 {
			 lError = NOERROR;
			 IStorage* storageDataSpaces = NULL;
			 HRESULT hresult = storage->OpenStorage( L"\006DataSpaces", NULL, STGM_SHARE_EXCLUSIVE, NULL, 0, &(storageDataSpaces) );
			 if ( storageDataSpaces != NULL )
				 storageDataSpaces->Release();
			 return hresult == S_OK? TRUE: FALSE;
		 }

private: IStorage* IsStorageType(const CString &sFilePath)
		 {
			 IStorage* storage;
			 storage = NULL;
			 StgOpenStorage( sFilePath, NULL, STGM_READ | STGM_TRANSACTED | STGM_SHARE_DENY_NONE, 
	        	               NULL, 0, &(storage) );
			 return storage;
		 }


private: BOOL IsArchiveType(const CString &sFilePath)
		 {
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->IsArchive(bstrFilePath);
				 SysFreeString( bstrFilePath );
			 }

			 return hresult == S_OK? TRUE: FALSE;
		 }


private: BOOL IsDOCXFile(const CString &sFilePath, long &lError)
		 {				 
			 lError = NOERROR;
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->IsFileExistInArchive(bstrFilePath, L"word/document.xml");
				 SysFreeString( bstrFilePath );
			 }

			 return hresult == S_OK? TRUE: FALSE;	 
		 }
private: BOOL IsXPSFile(const CString &sFilePath, long &lError)
		 {				 
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 BYTE* pBuffer = NULL;
				 const char *xpsFormatLine = "http://schemas.microsoft.com/xps/2005/06/fixedrepresentation";	
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"_rels/.rels", &pBuffer);
				 if (hresult == S_OK && pBuffer != NULL)
				 {
					 if (!strstr((char*)pBuffer, xpsFormatLine))
						 hresult = S_FALSE;

				 }
				 else
				 {//¬ XPS есть спецальный формат оптимизированный дл€ печати
					hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"_rels/.rels/[0].piece", &pBuffer);
					if (hresult != S_OK || pBuffer == NULL)
						hresult = S_FALSE;
				 }
				 SysFreeString( bstrFilePath );

				 RELEASEHEAP(pBuffer);
			 }

			 return hresult == S_OK? TRUE: FALSE; 
		 }

private: BOOL IsXLSXFile(const CString &sFilePath, long &lError)
		 {				
			 lError = NOERROR;
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->IsFileExistInArchive(bstrFilePath, L"xl/workbook.xml");
				 SysFreeString( bstrFilePath );
			 }

			 return hresult == S_OK? TRUE: FALSE;	 
		 }


private: BOOL IsPPTXFile(const CString &sFilePath, long &lError)
		 {	
			 lError = NOERROR;
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->IsFileExistInArchive(bstrFilePath, L"ppt/presentation.xml");
				 SysFreeString( bstrFilePath );
			 }

			 return hresult == S_OK? TRUE: FALSE;		 
		 }


private: BOOL IsODTFile(const CString &sFilePath, long &lError)
		 {		
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 BYTE* pBuffer = NULL;
				 const char *odtFormatLine = "application/vnd.oasis.opendocument.text";	
				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"mimetype", &pBuffer);
				 if (hresult == S_OK && pBuffer != NULL)
				 {
					 if (!strstr((char*)pBuffer, odtFormatLine))
						 hresult = S_FALSE;

				 }
				 SysFreeString( bstrFilePath );

				 RELEASEHEAP(pBuffer);
			 }

			 return hresult == S_OK? TRUE: FALSE;	
		 }


 private: BOOL IsODPFile(const CString &sFilePath, long &lError)
		 {		
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 BYTE* pBuffer = NULL;
				 const char *odtFormatLine = "application/vnd.oasis.opendocument.presentation";			 

				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"mimetype", &pBuffer);
				 if (hresult == S_OK && pBuffer != NULL)
				 {
					 if (!strstr((char*)pBuffer, odtFormatLine))
						 hresult = S_FALSE;

				 }
				 SysFreeString( bstrFilePath );

				 RELEASEHEAP(pBuffer);			 
			 }
			 return hresult == S_OK? TRUE: FALSE;	
		 }
 private: BOOL IsODSFile(const CString &sFilePath, long &lError)
		 {		
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 BYTE* pBuffer = NULL;
				 const char *odtFormatLine = "application/vnd.oasis.opendocument.spreadsheet";			 

				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"mimetype", &pBuffer);
				 if (hresult == S_OK && pBuffer != NULL)
				 {
					 if (!strstr((char*)pBuffer, odtFormatLine))
						 hresult = S_FALSE;

				 }
				 SysFreeString( bstrFilePath );

				 RELEASEHEAP(pBuffer);			 
			 }
			 return hresult == S_OK? TRUE: FALSE;	
		 }
 private: BOOL IsEPUBFile(const CString &sFilePath, long &lError)
		 {		
			 HRESULT hresult = S_FALSE;
			 if (m_pOfficeUtils != NULL)
			 {
				 BYTE* pBuffer = NULL;
				 const char *odtFormatLine = "application/epub+zip";			 

				 CStringW sTemp = sFilePath;
				 BSTR bstrFilePath = sTemp.AllocSysString();
				 hresult = m_pOfficeUtils->LoadFileFromArchive(bstrFilePath, L"mimetype", &pBuffer);
				 if (hresult == S_OK && pBuffer != NULL)
				 {
					 if (!strstr((char*)pBuffer, odtFormatLine))
						 hresult = S_FALSE;

				 }
				 SysFreeString( bstrFilePath );

				 RELEASEHEAP(pBuffer);			 
			 }
			 return hresult == S_OK? TRUE: FALSE;
		 }
private: BOOL IsFB2File(const CString &sFilePath, long &lError)
		 {
			 HRESULT hresult = S_FALSE;
			 lError = NOERROR;
			 BYTE	pBuffer[ 100 ];
			 DWORD	dwBytesRead;
			 HANDLE	hFile;

			 hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			 if (INVALID_HANDLE_VALUE == hFile)
			 {
				 lError = AVS_ERROR_FILEACCESS;
				 return FALSE;
			 }
			 ::ReadFile(hFile, pBuffer, 100, &dwBytesRead, NULL);
			 ::CloseHandle(hFile);

			 bool tagOpen = false;
			 //FB2 File is XML-file with rootElement - FictionBook 

			 for (size_t i = 0; i < 100 - 11; i++)
			 {
				 if (0x3C == pBuffer[i])
					 tagOpen = true;
				 else if (0x3E == pBuffer[i])
					 tagOpen = false;
				 else if (tagOpen && 0x46 == pBuffer[i]     && 0x69 == pBuffer[i + 1] && 0x63 == pBuffer[i + 2]
								  && 0x74 == pBuffer[i + 3] && 0x69 == pBuffer[i + 4] && 0x6F == pBuffer[i + 5]
								  && 0x6E == pBuffer[i + 6] && 0x42 == pBuffer[i + 7] && 0x6F == pBuffer[i + 8]
								  && 0x6F == pBuffer[i + 9] && 0x6B == pBuffer[i + 10])
					 return TRUE;
			 }
			 return FALSE;			 
		 }
private: BOOL IsSVGFile(const CString &sFilePath, long &lError)
		 {
			 //„тобы включить функицю надо решить проблемы:
			 //в начале файла могут быть большие коментарии
			 //слово svg может встречатьс€ во вложеннах тега и атрибутах(как например в html при сохранении drawing <img alt="" datasvg="%3Csvg%20width%3D%...)

			 HRESULT hresult = S_FALSE;
			 lError = NOERROR;
			 BYTE	pBuffer[ 2000 ];
			 DWORD	dwBytesRead;
			 HANDLE	hFile;

			 hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			 if (INVALID_HANDLE_VALUE == hFile)
			 {
				 lError = AVS_ERROR_FILEACCESS;
				 return FALSE;
			 }
			 ::ReadFile(hFile, pBuffer, 2000, &dwBytesRead, NULL);
			 ::CloseHandle(hFile);

			 bool tagOpen = false;
			 //SVG File is XML-file with rootElement - svg 
			 for (size_t i = 0; i < dwBytesRead - 2; i++)
			 {
				 if (0x3C == pBuffer[i])
					 tagOpen = true;
				 else if (0x3E == pBuffer[i])
					 tagOpen = false;
				 else if (tagOpen && 0x73 == pBuffer[i] && 0x76 == pBuffer[i + 1] && 0x67 == pBuffer[i + 2])
					 return TRUE;
			 }
			 return FALSE;			 
		 }
private: BOOL IsHtmlFile(const CString &sFilePath, long &lError)
		 {
			 HRESULT hresult = S_FALSE;
			 lError = NOERROR;
			 BYTE	pBuffer[ 2000 ];
			 DWORD	dwBytesRead;
			 HANDLE	hFile;

			 hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			 if (INVALID_HANDLE_VALUE == hFile)
			 {
				 lError = AVS_ERROR_FILEACCESS;
				 return FALSE;
			 }
			 ::ReadFile(hFile, pBuffer, 2000, &dwBytesRead, NULL);
			 ::CloseHandle(hFile);

			 bool tagOpen = false;
			 //Html File is XML-file with rootElement - html 
			 for (size_t i = 0; i < dwBytesRead - 2; i++)
			 {
				 if (0x3C == pBuffer[i] && (0x48 == pBuffer[i + 1] || 0x68 == pBuffer[i + 1]) && (0x54 == pBuffer[i + 2] || 0x74 == pBuffer[i + 2]) && (0x4d == pBuffer[i + 3] || 0x6d == pBuffer[i + 3]) && (0x4c == pBuffer[i + 4] || 0x6c == pBuffer[i + 4]))
					 return TRUE;
			 }
			 return FALSE;			 
		 }
//nSource - define исходного формата
//nDestination - define конечного формата
//aOutputFormats - массив будет очищен и наполнен define возможных конечных формата
//return - false - ни одного формата не доступно true - доступно больше 1-ого формата
public: bool GetAvailableFormats( long nSource, CAtlArray<long>& aOutputFormats )
		{
			aOutputFormats.RemoveAll();
			for( int i = 0; i < mc_nFormatTableSize; i++ )
				if( mc_aFormatTable[i][0] == nSource )
					aOutputFormats.Add( mc_aFormatTable[i][1] );
			return aOutputFormats.GetCount() > 0;
		}
//nIndex - индекс от 1 до значени€ возвращенного GetAvailableFormatCount(...)
//nSource - define исходного формата
//return - define формата, в который можно осуществить конвертацию
public: long GetAvailableFormat( int nIndex, long nSource)
		{
			int nResultCount = 0;
			for( int i = 0; i < mc_nFormatTableSize; i++ )
				if( mc_aFormatTable[i][0] == nSource )
					if( ++nResultCount == nIndex )
						return mc_aFormatTable[i][1];
			return AVS_OFFICESTUDIO_FILE_UNKNOWN;
		}
//nSource - define исходного формата
//return - количество форматов, в который можно осуществить конвертацию
public: int GetAvailableFormatCount( long nSource )
		{
			int nResultCount = 0;
			for( int i = 0; i < mc_nFormatTableSize; i++ )
				if( mc_aFormatTable[i][0] == nSource )
					nResultCount++;
			return nResultCount;
		}

private:
	AVSOfficeUtils::IOfficeUtils*		pOfficeUtils;
};

__declspec(selectany) const long AVSOfficeFormatChecker::mc_aFormatTable[][2] = {
	//Source									//Destination
	//AVS_OFFICESTUDIO_FILE_DOCUMENT,				AVS_OFFICESTUDIO_FILE_DOCUMENT,
	//AVS_OFFICESTUDIO_FILE_DOCUMENT,				AVS_OFFICESTUDIO_FILE_PRESENTATION,
	//AVS_OFFICESTUDIO_FILE_SPREADSHEET,			AVS_OFFICESTUDIO_FILE_SPREADSHEET,

	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,

	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,		AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,
	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,		AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS,
	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS,		AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,

	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC,			AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT,			AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML,		AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF,			AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_OTHER_EXTRACT_IMAGE,

	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_IMAGE_EMF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,

	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,

	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB,		AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2,			AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,

	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,		AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF
};
__declspec(selectany) const int AVSOfficeFormatChecker::mc_nFormatTableSize =  sizeof( mc_aFormatTable ) / (2 * sizeof( long ) );
__declspec(selectany) const long AVSOfficeFormatChecker::mc_aViewTable[] = {
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX, 
	AVS_OFFICESTUDIO_FILE_DOCUMENT_DOC, 
	AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_HTML,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_MHT,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_RTF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_TXT,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPT,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX,
	AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_EPUB,
	AVS_OFFICESTUDIO_FILE_IMAGE_GIF,
	AVS_OFFICESTUDIO_FILE_IMAGE_JPG,
	AVS_OFFICESTUDIO_FILE_IMAGE_PNG,
	AVS_OFFICESTUDIO_FILE_IMAGE_TGA,
	AVS_OFFICESTUDIO_FILE_IMAGE_TIFF,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_SWF,
	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLS,
	AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX,
	AVS_OFFICESTUDIO_FILE_DOCUMENT_FB2,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU,
	AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS
};
__declspec(selectany) const int AVSOfficeFormatChecker::mc_nViewTableSize =  sizeof( mc_aViewTable ) / ( sizeof( long ) );
