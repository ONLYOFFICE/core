#pragma once
#include "RtfProperty.h"
#include "Utils.h"
#include "../Common/MediaFormatDefine.h"
class RtfPicture : public IDocumentElement
{
	//public: RtfShape m_oShapeProp;
public: typedef enum {dt_none, dt_png, dt_jpg, dt_emf, dt_wmf, dt_apm}DataType;

public: DataType eDataType;

public: int m_nWidth;
public: int m_nWidthGoal;
public: int m_nHeight;
public: int m_nHeightGoal;

public: double m_dScaleX;
public: double m_dScaleY;

public: int m_bScaled;

public: int m_nCropL;
public: int m_nCropT;
public: int m_nCropR;
public: int m_nCropB;

public: bool m_bIsCopy; // true - надо удалять m_sPicFilename, false - не надо удалять
public: CString m_sPicFilename; //всегда содержит имя картинки, тип которой поддерживает rtf

public: CAtlArray<CString> m_aTempFiles;

public: RtfPicture()
		{
			m_bIsCopy = false;
			SetDefault();
		}
public: ~RtfPicture()
		{
			SetDefault();
			for( int i = 0; i < (int)m_aTempFiles.GetCount(); i++ ) 
				Utils::RemoveDirOrFile( m_aTempFiles[i] );
		}
public: int GetType()
		{
			return TYPE_RTF_PICTURE;
		}
public: bool IsValid()
		{
			return _T("") != m_sPicFilename && dt_none != eDataType;
		}
public: void SetDefaultRtf()
		{
			SetDefault();
		}
public: void SetDefaultOOX()
		{
			SetDefault();
		}
public: void SetDefault()
		{
			eDataType = dt_none;
			DEFAULT_PROPERTY( m_nWidth )
				DEFAULT_PROPERTY( m_nWidthGoal )
				DEFAULT_PROPERTY( m_nHeight )
				DEFAULT_PROPERTY( m_nHeightGoal )

				DEFAULT_PROPERTY_DEF( m_dScaleX, 100 )
				DEFAULT_PROPERTY_DEF( m_dScaleY, 100 )
				DEFAULT_PROPERTY( m_bScaled )

				DEFAULT_PROPERTY( m_nCropL )
				DEFAULT_PROPERTY( m_nCropT )
				DEFAULT_PROPERTY( m_nCropR )
				DEFAULT_PROPERTY( m_nCropB )
				if( true == m_bIsCopy && _T("") != m_sPicFilename )
					Utils::RemoveDirOrFile( m_sPicFilename );
			m_sPicFilename = _T("");
		}
public: CString RenderToRtf(RenderParameter oRenderParameter);
public: CString RenderToOOX(RenderParameter oRenderParameter);
public: CString GenerateWMF(RenderParameter oRenderParameter);
public: static bool LoadPicture( IUnknown** piImage, CString sPath );
public: static bool SavePicture( IUnknown* piImage, CString sPath, long nFormat );
public: static DataType GetPictureType( CString sFilename )
		{
			BYTE	pBuffer[ 16 ];
			DWORD	dwBytesRead;
			HANDLE	hFile;

			hFile = ::CreateFile(sFilename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hFile) 
				return dt_none;

			::ReadFile(hFile, pBuffer, 16, &dwBytesRead, NULL);
			::CloseHandle(hFile); 

			//jpeg	
			// Hex: FF D8 FF
			if ( (3 <= dwBytesRead) && (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
				return dt_jpg;

			//png 
			//Hex: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
			//ASCII: .PNG........IHDR
			if ( (16 <= dwBytesRead) && (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
				&& (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
				&& (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
				&& (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
				return dt_png;
			//wmf (aldus placeable header (apm))
			//Hex: D7 CD C6 9A 00 00
			if ( 6 <= dwBytesRead )
			{
				if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) )
					return dt_apm;
			}
			//wmf
			//or for Windows 3.x
			//Hex: 01 00 09 00 00 03
			if ( 6 <= dwBytesRead )
			{
				if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) || 
					((0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x09 == pBuffer[2]) && (0x00 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x03 == pBuffer[5]) ))
					return dt_wmf;
			}
			//emf
			//Hex: 01 00 00 00
			if ( (4 <= dwBytesRead) && (0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x00 == pBuffer[3]) )
				return dt_emf;

			return dt_none;
		}
};
typedef boost::shared_ptr<RtfPicture> RtfPicturePtr;