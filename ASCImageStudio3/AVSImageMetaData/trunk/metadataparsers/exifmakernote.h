/*!
*	\file	exifmakernote.h
* 
*/

#pragma once
#include <vector>
#include <string>
#include <memory>
#include "exif_utils.h"
#include "..\openexif\src\ExifTypeDefs.h"
#include "..\openexif\src\ExifIO.h"
#include "..\openexif\src\ExifIFD.h"

namespace AVS
{
	namespace Image
	{
		enum AVSMakerNoteTags
		{
			MAKERNOTE_CANON = 1,
			MAKERNOTE_NIKON3 = 2,
			MAKERNOTE_SONY = 3,
			MAKERNOTE_FUJI = 4,
			MAKERNOTE_PANASONIC = 5,
			MAKERNOTE_OLYMPUS = 6,
			MAKERNOTE_CASIO = 7,
			MAKERNOTE_NIKON1 = 8,
		};
		
		/*!	\brief	Обработчик данных makernote, абстрактный класс
		*/
		class CMakerNote
		{
		public:
			typedef std::auto_ptr<CMakerNote> AutoPtr;
			CMakerNote():m_bParseAppSegHeader(true)
			{}

		public:
			bool Parse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
			{
				return _DoParse(ExifData, base_offset, offset, flags);
			}

			std::wstring GetXML()
			{
				return _DoGetXML();
			}

			bool getParseAppSegHeaderState()
			{
				return m_bParseAppSegHeader;
			}

			void setParseAppSegHeaderState(bool bState)
			{
				m_bParseAppSegHeader = bState;
			}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags)
			{
				return true;
			}

			virtual std::wstring _DoGetXML()
			{
				return L"";
			}

			bool m_bParseAppSegHeader;

		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CMakerNote());
			}
		};

		/*!	\brief	Базовый класс для обработчиков makernote, которые имеют структуру IFD
		*/
		class CMakerNoteIfd: public CMakerNote
		{
		public:
			typedef std::vector<Exif::ExifDirEntry> ExifDirEntrysArray;
			CMakerNoteIfd(unsigned int IfdHeaderOffset_ = 0): IfdHeaderOffset(IfdHeaderOffset_)
			{
			}

			void SetIfdHeaderOffset(unsigned int IfdHeaderOffset_)
			{
				IfdHeaderOffset = IfdHeaderOffset_;
			}

			unsigned int GetIfdHeaderOffset()
			{
				return IfdHeaderOffset;
			}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;

		protected:
			ExifDirEntrysArray DirEntrys;
			unsigned int IfdHeaderOffset;

			// TODO
			template <typename T>
			static void Swab(T * value)
			{
				return;	
			}

			static bool PrintAllTags(Exif::ExifIFD * exif_ifd, Exif::ExifIO * exif_io, unsigned int makernote_id, std::wstring & output_xml)
			{
				Exif::ExifStatus status;
				status = exif_ifd->readDirTree(exif_io);
				Exif::ExifPathsTags makernoteTags;
				exif_ifd->getAllTags(makernoteTags, 0);

				std::wstringstream outputXMLEntry;
				Exif::ExifPathsTagsIter crntPathsTags = makernoteTags.begin();
				Exif::ExifPathsTagsIter endPathsTags = makernoteTags.end();
				while( crntPathsTags != endPathsTags )
				{
					Exif::ExifTagsIter crnt = (*crntPathsTags).second.begin() ;
					Exif::ExifTagsIter end = (*crntPathsTags).second.end() ;
					while( crnt != end )
					{
						Exif::ExifTagEntry* tag = *(crnt);
						tag->PrintXMLEntryEx(outputXMLEntry, std::wstring(L"makernote"), makernote_id);
						outputXMLEntry << std::endl;
						crnt++;
					}
					crntPathsTags++;
				}
				output_xml = outputXMLEntry.str();
				return true;
			}


			/*!	\brief	разбираем заголовок Ifd в данных makernote
			*			функция повторяет функциональность метода ExifIFD::readDir
			*/
			//bool ParseIfdHeader(const std::vector<char> & MakerNoteData, unsigned int IfdOffset, unsigned int flags);
		};

		

		/*!	\brief Фабрика классов обработчиков makernote
		*/
		class CMakerNoteFactory
		{
		public:
			typedef CMakerNote::AutoPtr (*CreateMakerNoteRoutine)();
			//! Type used to store model labels and %MakerNote create functions
			typedef std::vector<std::pair<std::string, CreateMakerNoteRoutine> > ModelRegistry;
			//! Type used to store a list of make labels and model registries
			typedef std::vector<std::pair<std::string, ModelRegistry*> > Registry;

			static Registry* pRegistry_;
			static void init()
			{
				if (!pRegistry_)
					pRegistry_ = new Registry();
			}

			static void cleanup()
			{
				if (pRegistry_ != 0)
				{
					Registry::iterator e = pRegistry_->end();
					for (Registry::iterator i = pRegistry_->begin(); i != e; ++i)
					{
						delete i->second;
					}
					delete pRegistry_;
					pRegistry_ = NULL;
				}
			}

			/*!	\breif Зарагестрировать новый класс-обработчик, преедаем имя производителя, модель и процедуру создания класса
			*/
			static void RegisterMakerNote(const std::string& make, const std::string& model, CreateMakerNoteRoutine createMakerNote)
			{
				init();
				ModelRegistry* pModelRegistry = 0;
				_ASSERTE(pRegistry_ != 0);
				Registry::const_iterator end1 = pRegistry_->end();
				Registry::const_iterator pos1;
				for (pos1 = pRegistry_->begin(); pos1 != end1; ++pos1)
				{
					if (pos1->first == make)
						break;
				}
				if (pos1 != end1)
				{
					pModelRegistry = pos1->second;
				}
				else
				{
					pModelRegistry = new ModelRegistry;
					pRegistry_->push_back(std::make_pair(make, pModelRegistry));
				}
				// Find or create a registry entry for model
				ModelRegistry::iterator end2 = pModelRegistry->end();
				ModelRegistry::iterator pos2;
				for (pos2 = pModelRegistry->begin(); pos2 != end2; ++pos2)
				{
					if (pos2->first == model)
						break;
				}
				if (pos2 != end2)
				{
					pos2->second = createMakerNote;
				}
				else
				{
					pModelRegistry->push_back(std::make_pair(model, createMakerNote));
				}
			}

			/*!	\brief	Создать экземпляр обработчика makernote для данного производителя и модели
			*/
			static CMakerNote::AutoPtr Сreate(const std::string& make,	const std::string& model)
			{
				// находим наилучшее соответствие производителя
				int score = 0;
				ModelRegistry* pModelRegistry = 0;
				_ASSERTE(pRegistry_ != 0);
				Registry::const_iterator end1 = pRegistry_->end();
				Registry::const_iterator pos1;
				for (pos1 = pRegistry_->begin(); pos1 != end1; ++pos1)
				{
					int rc = AVS::Utils::match(pos1->first, make);
					if (rc > score)
					{
						score = rc;
						pModelRegistry = pos1->second;
					}
				}
				if (pModelRegistry == 0)
					return CMakerNote::createMakerNote();

				// находим наилучшее соотетствие модели
				score = 0;
				CMakerNoteFactory::CreateMakerNoteRoutine createMakerNote = 0;
				ModelRegistry::const_iterator end2 = pModelRegistry->end();
				ModelRegistry::const_iterator pos2;
				for (pos2 = pModelRegistry->begin(); pos2 != end2; ++pos2)
				{
					int rc = AVS::Utils::match(pos2->first, model);
					if (rc > score)
					{
						score = rc;
						createMakerNote = pos2->second;
					}
				}
				if (createMakerNote == 0)
					return CMakerNote::createMakerNote();

				return createMakerNote();
			}

		};

	}
}