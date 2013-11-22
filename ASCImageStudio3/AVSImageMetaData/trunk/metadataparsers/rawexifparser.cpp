/*!	\file	rawexifparser.cpp
\brief	

*/
#include "stdafx.h"
#include "exifparser.h"
#include "atlbase.h"

#include <string>
#include <sstream>
#include <iostream>

#include "..\openexif\src\ExifTags.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifIFD.h"

#import "..\..\..\..\..\Redist\AVSImageRaw3.dll" rename_namespace("AVSImageRaw3")

namespace AVS
{
	namespace Image
	{
		bool CImageRawExifParser::_DoParseFile(std::wstring & RawImageFilename)
		{
			HRESULT hr;
			m_internalXMLEntry = L"";
			CComPtr<AVSImageRaw3::IImageRaw3Metadata> imageRaw3;
			if (FAILED(hr = imageRaw3.CoCreateInstance(__uuidof(AVSImageRaw3::CImageRaw3))))
				throw Exception::imageraw_create_error();

			if (S_OK != (hr = imageRaw3->raw_GetMetaData(_bstr_t(RawImageFilename.c_str()))))
				// в случае если открыли файл и не нашли метаданных, то возвращается S_FALSE
				return false;

			DWORD ifdCount = 0;			
			if FAILED(hr = imageRaw3->raw_IFDCount(0, &ifdCount))
				return false;
			if (0 == ifdCount)
				return false;

			DWORD rawFileFlags = 0;
			if FAILED(imageRaw3->raw_GetFlags(0, &rawFileFlags))
				return false;

			unsigned int tag0count = 0;
			std::wstringstream outputXmlStream;
			std::string cameraMake, cameraModel;
			for (unsigned int i = 0; i < ifdCount; i++)
			{
				USHORT Tag;
				DWORD ifdOffset, baseOffset;
				if FAILED(hr = imageRaw3->raw_IFDTag(i, &Tag))
					continue;
				if FAILED(hr = imageRaw3->raw_IFDOffset(i, &ifdOffset))
					continue;
				if FAILED(hr = imageRaw3->raw_BaseOffset(i, &baseOffset))
					continue;

				FILE * rawImageFile = NULL;
				if (0 != _wfopen_s(&rawImageFile, RawImageFilename.c_str(), L"rb"))
					return false;

				Exif::ExifStatus exifStatus;
				Exif::ExifIO exifIO(exifStatus, rawImageFile, RawImageFilename, "r+");
				if (exifStatus != Exif::EXIF_OK)
				{
					fclose(rawImageFile);
					return false;
				}

				if (rawFileFlags != 0x4949)
				{
					exifIO.flags() |= EXIF_SWAB;
					exifIO.bigEndian(true);
				}else
				{
					exifIO.flags() &= (~EXIF_SWAB);
					exifIO.bigEndian(false);
				}

				if ((0 == Tag) || (EXIFTAG_EXIFIFDPOINTER == Tag)) // TODO: makernote
				{

					Exif::ExifIFDMap ifdMap;
					Exif::ExifIFD exif_ifd(0, ifdOffset, baseOffset, 0, &ifdMap);
					exif_ifd.setLoadImage(false); // предотвращаем обработу самого изображения
					exifStatus = exif_ifd.readDirTree(&exifIO);
					if (exifStatus != Exif::EXIF_OK)
					{
						ATLTRACE2("[AVSImageMetaData]: readDirTree error\n");
						continue;
						//return false;
					}
					else
					{
						if (0 == Tag)
							tag0count++;
					}

					Exif::ExifPathsTags allTags;
					exif_ifd.getAllTags(allTags, 0);

					Exif::ExifTagEntry * exifMakerNoteUndef = NULL;
					Exif::ExifPathsTagsIter currentPathsTags = allTags.begin();
					Exif::ExifPathsTagsIter endPathsTags = allTags.end();
					while(currentPathsTags != endPathsTags)
					{
						Exif::ExifTagsIter crnt = (*currentPathsTags).second.begin() ;
						Exif::ExifTagsIter end = (*currentPathsTags).second.end() ;
						while( crnt != end )
						{
							Exif::ExifTagEntry* tag = *(crnt);

							// makernote выводим потом, после разбора остальных тегов
							if (tag->getTagNum() != EXIFTAG_MAKERNOTE)
							{
								if ((330 != Tag) && (EXIFTAG_EXIFIFDPOINTER != Tag))
								{
									std::wstring categoryString = L"";
									if (0 == currentPathsTags->first.back().first)
										categoryString = L"common";
									if (EXIFTAG_EXIFIFDPOINTER == currentPathsTags->first.back().first)
										categoryString = L"exif";
									else if (EXIFTAG_EXIFINTEROPIFDPOINTER == currentPathsTags->first.back().first)
										categoryString = L"interoperability";
									else if (EXIFTAG_GPSINFO == currentPathsTags->first.back().first)
										categoryString = L"gps";
									else if (EXIFTAG_CAMERAINFOIFD == currentPathsTags->first.back().first)
										categoryString = L"camerainfo";
									else if (EXIFTAG_SPECIALEFFECTSIFD == currentPathsTags->first.back().first)
										categoryString = L"effects";
									else if (EXIFTAG_BORDERSIFD == currentPathsTags->first.back().first)
										categoryString = L"borders";
									else if (330 == currentPathsTags->first.back().first)
										categoryString = L"sub";


									if (1 == tag0count)
									{
										if (categoryString.size() == 0)
											tag->PrintXMLEntryEx(outputXmlStream, Utils::lexical_cast<std::wstring>(currentPathsTags->first.back().first));
										else
											tag->PrintXMLEntryEx(outputXmlStream, categoryString);
									}
									else if (2 == tag0count)
									{
										// 1st IFD (Thumbnail)
										tag->PrintXMLEntryEx(outputXmlStream, std::wstring(L"thumbnail"));
									} else
									{
										tag->PrintXMLEntryEx(outputXmlStream, std::wstring(L"additional ") + AVS::Utils::lexical_cast<std::wstring>(tag0count-1));
									}
								}else if (EXIFTAG_EXIFIFDPOINTER == Tag)
								{
									//tag->PrintXMLEntryEx(outputXmlStream, std::wstring(L"exif"));
								}
							}
							else
							{
								exifMakerNoteUndef = tag;	
							}

							if (EXIFTAG_MAKE == tag->getTagNum())
							{
								std::stringstream tmp_;
								tag->PrintValue(tmp_);
								cameraMake = tmp_.str();
							}

							if (EXIFTAG_MODEL == tag->getTagNum())
							{
								std::stringstream tmp_;
								tag->PrintValue(tmp_);
								cameraModel = tmp_.str();
							}

							outputXmlStream << std::endl;
							crnt++;
						}
						currentPathsTags++;
					}
				}
				else if ((EXIFTAG_MAKERNOTE == Tag) && ((cameraMake.size() != 0) || (cameraModel.size() != 0)))
				{
					CMakerNote::AutoPtr makerNoteParser = CMakerNoteFactory::Сreate(cameraMake, cameraModel);
					makerNoteParser->setParseAppSegHeaderState(false);
					if (makerNoteParser->Parse(&exifIO, baseOffset, ifdOffset, 0))
						outputXmlStream << makerNoteParser->GetXML();
				}
			}

			imageRaw3.Release();

			m_internalXMLEntry = outputXmlStream.str();
			return true;
		}

		std::wstring CImageRawExifParser::_DoGetXML()
		{
			std::wstringstream resultXMLString;
			resultXMLString<< L"<?xml version=\"1.0\" encoding=\"utf-8\"?>" <<
				L"<AVSImageMetaDataExif>"
				<< m_internalXMLEntry
				<< L"</AVSImageMetaDataExif>";
			return resultXMLString.str();
		}
	}
}