#include "stdafx.h"
#include "exifparser.h"
#include "..\openexif\src\ExifImageFile.h"
#include "..\openexif\src\ExifRawAppSeg.h"

namespace AVS
{
	namespace Image
	{
		bool CImageJpegExifParser::_DoParseFile(std::wstring & filename)
		{
			Exif::ExifStatus RetVal;
			m_internalXMLEntry = L"";
			Reset();
			_ASSERTE(NULL != m_ExifImageFile.get());

			if (Exif::EXIF_OK != (RetVal = m_ExifImageFile->open(filename.c_str(), "r")))
			{
				m_ExifImageFile->close();
				return false;
			}

			DumpXML();
			RetVal = m_ExifImageFile->close();
			return true;
		}

		std::wstring CImageJpegExifParser::_DoGetXML()
		{
			std::wstringstream resultXMLString;
			resultXMLString<< L"<?xml version=\"1.0\" encoding=\"utf-8\"?>" <<
				L"<AVSImageMetaDataExif>" << m_internalXMLEntry
				<< L"</AVSImageMetaDataExif>";
			return resultXMLString.str();
		}


		void CImageJpegExifParser::DumpXML()
		{
			_ASSERTE(NULL != m_ExifImageFile.get());
			m_internalXMLEntry = L"";
			std::wstringstream outputXMLEntry;
			Exif::ExifStatus RetVal;
			std::string makeString, modelString;

			
			// Получаем "сырые" app-сегменты
			typedef std::vector<Exif::ExifAppSegment*> ExifAppSegmentArray;
			ExifAppSegmentArray RawAppSeg = m_ExifImageFile->getAllRawAppSegs(false);

			Exif::ExifAppSegManager ExifRawAppSegs;
			// создаем копию "сырых" сегментов, так как потом они будут преобразованы и данные потеряются
			for (ExifAppSegmentArray::iterator iRAppSeg = RawAppSeg.begin();
				iRAppSeg != RawAppSeg.end();
				++iRAppSeg)
			{
				Exif::ExifRawAppSeg* newRawAppSeg = dynamic_cast<Exif::ExifRawAppSeg*>((*iRAppSeg)->clone());
				if (newRawAppSeg)
					ExifRawAppSegs.insert(newRawAppSeg);
			}

			AppSegDescArray::const_iterator iAppSegDesc = m_AppSegDescArray.begin();
			AppSegDescArray::const_iterator iAppSegWithMakernote; // для сегмента который содержит makernote
			Exif::ExifTagEntry * exifMakerNoteUndef = NULL;

			while (iAppSegDesc != m_AppSegDescArray.end())
			{
				Exif::ExifPathsTags appPathsTags;
				if (Exif::EXIF_OK == (RetVal = m_ExifImageFile->getAllTags(iAppSegDesc->first, iAppSegDesc->second, appPathsTags)))
				{
					Exif::ExifPathsTagsIter crntPathsTags = appPathsTags.begin();
					Exif::ExifPathsTagsIter endPathsTags = appPathsTags.end();
					int i = 0;
					while( crntPathsTags != endPathsTags )
					{
						i++;
						Exif::ExifTagsIter crnt = (*crntPathsTags).second.begin() ;
						Exif::ExifTagsIter end = (*crntPathsTags).second.end() ;
						while( crnt != end )
						{
							Exif::ExifTagEntry* tag = *(crnt);

							// makernote выводим потом, после разбора остальных тегов
							if (tag->getTagNum() != EXIFTAG_MAKERNOTE)
							{
							//	tag->PrintXMLEntryEx(outputXMLEntry, Utils::lexical_cast<std::wstring>((*crntPathsTags).first));
								std::wstring categoryString = L"";
								if (1 == crntPathsTags->first.back().second)
									categoryString = L"thumbnail";


								if (0 == crntPathsTags->first.back().first)
									categoryString += L"";
								if (EXIFTAG_EXIFIFDPOINTER == crntPathsTags->first.back().first)
									categoryString += L"exif";
								else if (EXIFTAG_EXIFINTEROPIFDPOINTER == crntPathsTags->first.back().first)
									categoryString += L"interoperability";
								else if (EXIFTAG_GPSINFO == crntPathsTags->first.back().first)
									categoryString += L"gps";
								else if (EXIFTAG_CAMERAINFOIFD == crntPathsTags->first.back().first)
									categoryString += L"camerainfo";
								else if (EXIFTAG_SPECIALEFFECTSIFD == crntPathsTags->first.back().first)
									categoryString += L"effects";
								else if (EXIFTAG_BORDERSIFD == crntPathsTags->first.back().first)
									categoryString += L"borders";
								else if (330 == crntPathsTags->first.back().first)
									categoryString += L"sub";

								tag->PrintXMLEntryEx(outputXMLEntry, categoryString);

							}
							else
							{
								exifMakerNoteUndef = tag;	
								iAppSegWithMakernote = iAppSegDesc;
							}

							// отдельно сохраняем Make и Model, чтобы правильно разобрать makernote

							if (tag->getTagNum() == EXIFTAG_MAKE)
							{
								
								std::stringstream tmp_;
								tag->PrintValue(tmp_);
								makeString = tmp_.str();
							}

							if (tag->getTagNum() == EXIFTAG_MODEL)
							{
								std::stringstream tmp_;
								tag->PrintValue(tmp_);
								modelString = tmp_.str();
							}

							outputXMLEntry << std::endl;
							crnt++ ;
						}
						crntPathsTags++;
					}
				}
				iAppSegDesc++;
			}

			
			do{
				// имеется makernote
				if (exifMakerNoteUndef)
				{
					Exif::ExifRawAppSeg * MakerNoteAppSeg = dynamic_cast<Exif::ExifRawAppSeg *>(ExifRawAppSegs.getRawAppSeg(iAppSegWithMakernote->first, iAppSegWithMakernote->second.c_str(), false));
					if (!MakerNoteAppSeg)
						break;

					ExifTagEntryVector * exifMakerNote = dynamic_cast<ExifTagEntryVector *>(exifMakerNoteUndef);
					_ASSERTE(NULL != exifMakerNote);
					ExifVector & MakernoteValue = exifMakerNote->getValue();

					Exif::ExifStatus status;

					std::vector<char> dataBuffer(MakerNoteAppSeg->getLength() + MakernoteValue.size());
					memcpy_s(&(dataBuffer[0]), dataBuffer.size(),  MakerNoteAppSeg->getRawData(), MakerNoteAppSeg->getLength());
					memcpy_s(&(dataBuffer[MakerNoteAppSeg->getLength()]), dataBuffer.size() - MakerNoteAppSeg->getLength(), &(MakernoteValue[0]), MakernoteValue.size());
					
					std::auto_ptr<Exif::ExifIO> ExifIORawData(new Exif::ExifIO(status, &(dataBuffer[0]), dataBuffer.size(), "r+", dataBuffer.size()));
					CMakerNote::AutoPtr makerNoteParser = CMakerNoteFactory::Сreate(makeString, modelString);
					unsigned int offset = MakerNoteAppSeg->getLength();

					if (makerNoteParser->Parse(ExifIORawData.get(), 0, offset, 0))
						outputXMLEntry << makerNoteParser->GetXML();
				}
			}while(0);

			m_internalXMLEntry = outputXMLEntry.str();
		}

		void CImageJpegExifParser::Reset()
		{
			m_ExifImageFile.reset(new Exif::ExifImageFile());
			m_internalXMLEntry = L"";
			FillTagsDesc();
		}
	}
}
