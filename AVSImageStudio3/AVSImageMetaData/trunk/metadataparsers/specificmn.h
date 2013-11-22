#pragma once
#ifndef _AVS_IMAGEMETADATA_SPECIFICMN_H_INCLUDED_
#define _AVS_IMAGEMETADATA_SPECIFICMN_H_INCLUDED_

#include "exifmakernote.h"

namespace Exif
{
	class ExifTagEntry;
}

namespace AVS
{
	namespace Image
	{

		/*!	\class CCasioMakerNote
		*	\brief Casio Makernote parser class 
		*
		*	\todo	Надо найти пример снимка и протестировать, те что встречал были без тега или модель была
		*			Olympus (это теперь одна фирма?)
		*/
		class CCasioMakerNote: public CMakerNoteIfd
		{
		public:
			CCasioMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

		private:
			std::wstring m_internalXMLEntry;
		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CCasioMakerNote());
			}
		};


		/*!	\class CNikonMakerNote
		*	\brief Nikon Makernote parser class 
		*
		*/
		class CNikonMakerNote: public CMakerNoteIfd
		{
		public:
			CNikonMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

			int CheckType(char * buffer_, size_t buffer_size,size_t & headersize);

		private:
			std::wstring m_internalXMLEntry;
		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CNikonMakerNote());
			}
		};

		/*!	\class CSonyMakerNote
		*	\brief Sony Makernote parser class 
		*
		*	\todo	Надо найти пример снимка и протестировать, те что встречал были без тега или модель была
		*			Olympus (это теперь одна фирма?)
		*/
		class CSonyMakerNote: public CMakerNoteIfd
		{
		public:
			CSonyMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;


		private:
			int CheckType(char * buffer_, size_t buffer_size,size_t & headersize);
			std::wstring m_internalXMLEntry;
		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CSonyMakerNote());
			}
		};


		/*!	\class CCanonMakerNote
		*	\brief Canon Makernote parser class 
		*/
		class CCanonMakerNote: public CMakerNoteIfd
		{
		public:
			CCanonMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

		private:
			std::wstring m_internalXMLEntry;

			std::wstring _ParseCanonTags(USHORT TagId, Exif::ExifTagEntry* Tag, bool & bResult);

		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CCanonMakerNote());
			}
		};

		/*!	\class CFujiMakerNote
		*	\brief Fuji Makernote parser class 
		*/
		class CFujiMakerNote: public CMakerNoteIfd
		{
		public:
			CFujiMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

		private:
			std::wstring m_internalXMLEntry;

			//std::wstring _ParseCanonTags(USHORT TagId, Exif::ExifTagEntry* Tag, bool & bResult);

		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CFujiMakerNote());
			}
		};


		/*!	\class COlumpusMakerNote
		*	\brief Olumpus Makernote parser class 
		*/
		class COlumpusMakerNote: public CMakerNoteIfd
		{
		public:
			COlumpusMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

		private:
			std::wstring m_internalXMLEntry;

			//std::wstring _ParseCanonTags(USHORT TagId, Exif::ExifTagEntry* Tag, bool & bResult);

		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new COlumpusMakerNote());
			}
		};

		/*!	\class CPanasonicMakerNote
		*	\brief Panasonic MakerNote parser class 
		*/
		class CPanasonicMakerNote: public CMakerNoteIfd
		{
		public:
			CPanasonicMakerNote()
			{}

		private:
			virtual bool _DoParse(Exif::ExifIO * ExifData, unsigned int base_offset, unsigned int offset, unsigned int flags) override;
			virtual std::wstring _DoGetXML() override;

		private:
			std::wstring m_internalXMLEntry;

		public:
			static CMakerNote::AutoPtr createMakerNote()
			{
				return CMakerNote::AutoPtr(new CPanasonicMakerNote());
			}
		};


	}
}

#endif