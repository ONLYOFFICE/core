/*!	\file exifparser.h
*/
#ifndef _AVS_IMAGEMETADATA_EXIFPARSER_H_INCLUDED_
#define _AVS_IMAGEMETADATA_EXIFPARSER_H_INCLUDED_
#pragma once 

#include "metadataparser.h"
#include <memory>
#include <string>
#include <sstream>

#include "exifmakernote.h"
#include "specificmn.h"
#include "..\openexif\src\ExifTagEntry.h"
#include "..\openexif\src\ExifImageFile.h"
#include "parserguids.h"

#include "guiddef.h"
/*
namespace Exif
{
	class ExifImageFile;
}*/

namespace AVS
{
	namespace Image
	{
		namespace Exception
		{
			class exif_parse_error: public metadata_parse_error
			{
			public:
				exif_parse_error()
				{}
			};

			class imageraw_create_error: public metadata_parse_error
			{
			};

			class imageraw_open_error: public metadata_parse_error
			{
			};
		}

		class CImageJpegExifParser: public ImageMetaDataParser
		{
		private:
			typedef std::vector<char> ExifVector;
			typedef Exif::ExifTagEntryT<ExifVector> ExifTagEntryVector;

		private:

			void ParseMakerNote(ExifVector & _vec)
			{
				char * buffer = &(_vec[0]);				
			}

		public:
			~CImageJpegExifParser()
			{
				CMakerNoteFactory::cleanup();
			}

			static const GUID * GetGuid()
			{
				return &PARSER_EXIF_JPEG;
			}

		private:
			virtual bool _DoParseFile(std::wstring & filename) override;
			virtual std::wstring _DoGetXML() override;
		
			typedef std::pair<unsigned short, std::string> AppSegDesc;
			typedef std::vector<AppSegDesc> AppSegDescArray;
		
		private:

			void _ParseMakerNote()
			{
					
			}

			void FillTagsDesc()
			{
				m_AppSegDescArray.clear();
				m_AppSegDescArray.push_back(AppSegDesc(0xFFE1,"Exif"));
				m_AppSegDescArray.push_back(AppSegDesc(0xFFE3,"Meta"));

				CMakerNoteFactory::RegisterMakerNote("Canon", "*", CCanonMakerNote::createMakerNote); 
				CMakerNoteFactory::RegisterMakerNote("CASIO", "*", CCasioMakerNote::createMakerNote); 
				CMakerNoteFactory::RegisterMakerNote("NIKON*", "*", CNikonMakerNote::createMakerNote); 
				CMakerNoteFactory::RegisterMakerNote("SONY*", "*", CSonyMakerNote::createMakerNote);
				CMakerNoteFactory::RegisterMakerNote("FUJIFILM", "*", CFujiMakerNote::createMakerNote);
				CMakerNoteFactory::RegisterMakerNote("OLYMPUS*", "*", COlumpusMakerNote::createMakerNote);
				CMakerNoteFactory::RegisterMakerNote("Panasonic", "*", CPanasonicMakerNote::createMakerNote);
			}

			void Reset();
			void DumpXML();

		private:
			std::wstring m_internalXMLEntry;
			std::auto_ptr<Exif::ExifImageFile> m_ExifImageFile;
			AppSegDescArray m_AppSegDescArray;
		
		public:
			static ImageMetaDataParser::AutoPtr createRoutine()
			{
				return ImageMetaDataParser::AutoPtr(new CImageJpegExifParser());
			}
		};


		class CImageRawExifParser: public ImageMetaDataParser
		{
		public:
			static const GUID * GetGuid()
			{
				return &PARSER_EXIF_RAW;
			}

		private:
			virtual bool _DoParseFile(std::wstring & filename) override;
			virtual std::wstring _DoGetXML() override;
			std::wstring m_internalXMLEntry;
		public:
			static ImageMetaDataParser::AutoPtr createRoutine()
			{
				return ImageMetaDataParser::AutoPtr(new CImageRawExifParser());
			}
		};
	}
}
#endif