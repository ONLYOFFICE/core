#pragma once

#include <vector>
#include "Guiddef.h"
#include "metadatatypes.h"
#include "..\metadataparsers\metadataparser.h"

namespace AVS
{
	namespace Image
	{
		class MetaDataParserFactory
		{
		public:
			typedef ImageMetaDataParser::AutoPtr (*CreateMetaDataParserRoutine)();
			class ParserInfo
			{
			public:
				ParserInfo(GUID parserGuid, unsigned int metaDataType, CreateMetaDataParserRoutine createRoutine, GUID objectGuid)
					: m_ParserGuid(parserGuid), m_Type(metaDataType), m_CreateRoutine(createRoutine), m_ObjectGuid(objectGuid)
				{
				}

				unsigned int & Type()
				{
					return m_Type;
				};
				
				CreateMetaDataParserRoutine & CreateRoutine()
				{
					return m_CreateRoutine;
				}
				
				GUID & ObjectGuid()
				{
					return m_ObjectGuid;
				}
				
				GUID & ParserGuid()
				{
					return m_ParserGuid;
				}

			private:
				unsigned int m_Type;
				CreateMetaDataParserRoutine m_CreateRoutine;
				GUID m_ObjectGuid;
				GUID m_ParserGuid;
			};

			typedef std::vector<ParserInfo> ParserInfoArray;

		private:
			static std::auto_ptr<ParserInfoArray> _ParserInfoArray;
			static void _Init()
			{
				if (!_ParserInfoArray.get())
					_ParserInfoArray = std::auto_ptr<ParserInfoArray>(new ParserInfoArray());
			}
		
			static bool _SearchGuid(GUID & guid)
			{
				if (!_ParserInfoArray.get())
					return false;

				for (ParserInfoArray::iterator i1 = _ParserInfoArray->begin();
					i1 != _ParserInfoArray->end(); ++i1)
				{
					GUID objectGuid = (*i1).ParserGuid();
					if (0 == memcmp(&objectGuid , &guid, sizeof(GUID)))
						return true;
				}
				return false;
			}

		public:

			static void Cleanup()
			{
			}

			static void AddParser(ParserInfo & parserInfo)
			{
				_Init();
				if (_SearchGuid(parserInfo.ParserGuid()))
					return;
				_ASSERTE(NULL != _ParserInfoArray.get());
				_ParserInfoArray->push_back(parserInfo);
			}

			static int Count()
			{
				if (_ParserInfoArray.get())
					return _ParserInfoArray->size();
				else
					return 0;
			}

			static AVS::Image::ImageMetaDataParser::AutoPtr CreateParser(int index)
			{
				if ((index < 0) || (index >= Count()))
					throw std::range_error("[CreateParser]: invalid index");
				else
				{
					CreateMetaDataParserRoutine & routine = _ParserInfoArray->at(index).CreateRoutine();
					return routine();
				}
			}

			static unsigned int GetType(int index)
			{
				if ((index < 0) || (index >= Count()))
					throw std::range_error("[GetType]: invalid index");
				else
				{
					return _ParserInfoArray->at(index).Type();
				}
			}

			static GUID GetObjectGuid(int index)
			{
				if ((index < 0) || (index >= Count()))
					throw std::range_error("[GetObjectGuid]: invalid index");
				else
				{
					return _ParserInfoArray->at(index).ObjectGuid();
				}
			}
		};
	}
}