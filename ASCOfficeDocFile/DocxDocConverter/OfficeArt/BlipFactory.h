#pragma once

#include <fstream>
#include <string>

#include "OfficeArtBlipJPEG.h"
#include "OfficeArtBlipPNG.h"
#include "OfficeArtBlipEMF.h"
#include "OfficeArtBlipWMF.h"
#include "OfficeArtBlipTIFF.h"
#include "OfficeArtFBSE.h"

#include "../../Common/MD4/md4.h"

#include "../../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"


namespace OfficeArt
{
	class BlipFactory
	{
	public:

		BlipFactory () : m_bDeleteFile (FALSE)
		{

		}

		BlipFactory (std::wstring fileName) : m_bDeleteFile (FALSE)
		{
			m_sFile	= std::wstring(fileName);
		}

		~BlipFactory ()
		{
			if (m_bDeleteFile)
			{
				::DeleteFile (m_sFile.c_str());
			}
		}

		inline std::wstring GetFileNameExtension() const
		{
			std::wstring::size_type dotPosition = m_sFile.find_last_of( _T( '.' ) );
			static const std::wstring::size_type npos = -1;
			std::wstring extension;

			if (dotPosition != npos)
				extension = std::wstring( ( m_sFile.begin() + dotPosition + 1 ), m_sFile.end() );

			return extension;
		}

		inline Enumerations::MSOBLIPTYPE GetBlipType() const
		{
			Enumerations::MSOBLIPTYPE blipType	=	Enumerations::msoblipUNKNOWN;
			
			std::wstring extension				=	GetFileNameExtension();

			if ( (extension == std::wstring(L"jpg")) || (extension == std::wstring(L"jpeg")) )
			{
				blipType	=	Enumerations::msoblipJPEG;
			}
			else if (extension == std::wstring(L"png") || extension == std::wstring(L"gif"))
			{
				blipType	=	Enumerations::msoblipPNG;
			}
			else if (extension == std::wstring(L"emf"))
			{
				blipType	=	Enumerations::msoblipEMF;
			}
			else if (extension == std::wstring(L"wmf"))
			{
				blipType	=	Enumerations::msoblipWMF;
			}
			else if (extension == std::wstring(L"tiff") || extension == std::wstring(L"tif"))
			{
				blipType	=	Enumerations::msoblipTIFF;
			}
			else if (extension == std::wstring(L"bmp"))
			{
				blipType	=	Enumerations::msoblipDIB;
			}

			return blipType;
		}

		inline OfficeArtBlip* GetOfficeArtBlip()
		{
			OfficeArtBlip* officeArtBlip = NULL;

			if (!m_sFile.empty())
			{
				std::string xstr;

				std::ifstream xfile(m_sFile.c_str(), std::ios::binary);

				//узнаем размер файла, и выделяем память в строке
				xfile.seekg( 0, std::ios_base::end );
				xstr.resize( xfile.tellg() );
				xfile.seekg( 0, std::ios_base::beg );

				//копируем данные
				xfile.read(const_cast<char*>( xstr.data() ), (std::streamsize)xstr.size());

				if ( !xstr.empty() )
				{
					std::wstring extension = GetFileNameExtension();

					if ( extension == std::wstring(L"gif")
						|| extension == std::wstring(L"bmp") 
						|| extension == std::wstring(L"tiff")
						|| extension == std::wstring(L"tif") )
					{
						//	MS WORD конвертит исходник в PNG формат ( UUID берет их исходника GIF файла )
						//	MS WORD конвертит исходник в PNG формат ( UUID берет их исходника TIFF файла )
						//	MS WORD конвертит исходник в PNG формат ( UUID берет их исходника BMP файла )

						m_sOriginalData		=	xstr;
						officeArtBlip		=	GetBlipWithPngTransform();
					}
					else if ((extension == std::wstring(L"jpg")) || (extension == std::wstring(L"jpeg")))
					{
						MD4 md4Code((unsigned char*)xstr.data(), xstr.size());
						officeArtBlip		=	new OfficeArtBlipJPEG((unsigned char*)xstr.data(), xstr.size(), md4Code.GetMD4Bytes());
					}
					else if (extension == std::wstring(L"png"))
					{
						if (m_sOriginalData.length())
						{
							MD4 md4Code((unsigned char*)m_sOriginalData.data(), m_sOriginalData.size());
							officeArtBlip = new OfficeArtBlipPNG ((unsigned char*)m_sOriginalData.data(), m_sOriginalData.size(), md4Code.GetMD4Bytes());
						}
						else
						{
							MD4 md4Code((unsigned char*)xstr.data(), xstr.size());
							officeArtBlip = new OfficeArtBlipPNG ((unsigned char*)xstr.data(), xstr.size(), md4Code.GetMD4Bytes());
						}
					}
					else if (extension == std::wstring(L"emf"))
					{
						unsigned char* buffer			=	NULL;
						unsigned long comprLen	=	CompressImage( &buffer, (unsigned char*)xstr.data(), xstr.size());

						if ( ( buffer != NULL ) && ( comprLen != 0 ) )
						{
							MD4 md4Code((unsigned char*)xstr.data(), xstr.size());

							//!!!TODO!!!
							officeArtBlip = new OfficeArtBlipEMF( OfficeArtMetafileHeader( xstr.size(), RECT( 0, 0, 0, 0 ), POINT( 0, 0 ), comprLen, COMPRESSION_METHOD_DEFLATE ), buffer, md4Code.GetMD4Bytes() );

							RELEASEARRAYOBJECTS (buffer);
						}
					}
					else if (extension == std::wstring(L"wmf"))
					{
						unsigned long comprLen = 0;
						unsigned char* buffer = NULL;

						std::string metaPlaceableRecord;
						std::string wmfData;

						metaPlaceableRecord.push_back( (char)0xD7 );
						metaPlaceableRecord.push_back( (char)0xCD );
						metaPlaceableRecord.push_back( (char)0xC6 );
						metaPlaceableRecord.push_back( (char)0x9A );

						if ( equal( xstr.begin(), ( xstr.begin() + 4 ), metaPlaceableRecord.begin() ) )
						{
							wmfData.assign( ( xstr.begin() + 22 ), xstr.end() );
						}
						else
						{
							wmfData = xstr;
						}

						comprLen = CompressImage( &buffer, (unsigned char*)wmfData.data(), wmfData.size() );

						if ( ( buffer != NULL ) && ( comprLen != 0 ) )
						{
							MD4 md4Code( (unsigned char*)wmfData.data(), wmfData.size() );

							// TODO : need fix
							
							officeArtBlip = new OfficeArtBlipWMF( OfficeArtMetafileHeader( wmfData.size(), RECT( 0, 0, 0, 0 ), POINT( 0, 0 ), comprLen, COMPRESSION_METHOD_DEFLATE ), buffer, md4Code.GetMD4Bytes() );
							RELEASEARRAYOBJECTS (buffer);
						}
					}
				}
			}

			return officeArtBlip;
		}

		inline std::vector<unsigned char> Get_rgbUid1 ()
		{
			if (!m_sFile.empty())
			{
				std::string xstr;

				std::ifstream xfile(m_sFile.c_str(), std::ios::binary);

				//узнаем размер файла, и выделяем память в строке
				xfile.seekg( 0, std::ios_base::end );
				xstr.resize( xfile.tellg() );
				xfile.seekg( 0, std::ios_base::beg );

				//копируем данные
				xfile.read(const_cast<char*>(xstr.data()), (std::streamsize)xstr.size());

				if ( !xstr.empty() )
				{
					std::wstring extension = GetFileNameExtension();

					if ((extension == std::wstring(_T("jpg"))) 
						|| (extension == std::wstring(_T("jpeg"))) 
						|| (extension == std::wstring(_T("png"))) 
						|| (extension == std::wstring(_T("gif")))
						|| (extension == std::wstring(_T("tiff"))) 
						|| (extension == std::wstring(L"tif"))
						|| (extension == std::wstring(L"bmp")) )
					{
						MD4 MD4Code ((unsigned char*)xstr.data(), xstr.size());
						return MD4Code.GetMD4Bytes();
					}
					else if (extension == std::wstring(_T("emf")))
					{
						unsigned char* buffer			=	NULL;
						unsigned long comprLen	=	CompressImage (&buffer, (unsigned char*)xstr.data(), xstr.size());

						if ( ( buffer != NULL ) && ( comprLen != 0 ) )
						{
							MD4 MD4Code ((unsigned char*)xstr.data(), xstr.size());
							RELEASEARRAYOBJECTS (buffer);

							return MD4Code.GetMD4Bytes();
						}
					}
					else if (extension == std::wstring(_T("wmf")))
					{
						std::string metaPlaceableRecord;
						std::string wmfData;

						metaPlaceableRecord.push_back((char)0xD7);
						metaPlaceableRecord.push_back((char)0xCD);
						metaPlaceableRecord.push_back((char)0xC6);
						metaPlaceableRecord.push_back((char)0x9A);

						if ( equal( xstr.begin(), ( xstr.begin() + 4 ), metaPlaceableRecord.begin() ) )
						{
							wmfData.assign( ( xstr.begin() + 22 ), xstr.end() );
						}
						else
						{
							wmfData = xstr;
						}

						unsigned char* buffer			=	NULL;
						unsigned long comprLen	=	CompressImage( &buffer, (unsigned char*)wmfData.data(), wmfData.size() );

						if ( ( buffer != NULL ) && ( comprLen != 0 ) )
						{
							MD4 MD4Code ((unsigned char*)wmfData.data(), wmfData.size());
							RELEASEARRAYOBJECTS (buffer);

							return MD4Code.GetMD4Bytes();
						}
					}
				}
			}

			return std::vector<unsigned char> ();
		}

	protected:
		inline unsigned long CompressImage (unsigned char** buffer, unsigned char* imageData, unsigned int imageSize) const
		{
			unsigned long comprLen = 0;

			if ( ( buffer != NULL ) && ( imageData != NULL ) && ( imageSize != 0 ) )
			{
				comprLen	=	imageSize;
				*buffer		=	new unsigned char[comprLen];

				HRESULT hr	=	S_OK;
				COfficeUtils* pOfficeUtils = new COfficeUtils(NULL); 

					if (pOfficeUtils)
					{
						pOfficeUtils->Compress(*buffer, &comprLen, imageData, imageSize, -1);

						delete pOfficeUtils;
						pOfficeUtils = NULL;
					}
			}

			return comprLen;
		}

		OfficeArtBlip* GetBlipWithPngTransform ();

	private:

		std::wstring	m_sFile;
		bool			m_bDeleteFile;	
		std::string		m_sOriginalData;
	};
}