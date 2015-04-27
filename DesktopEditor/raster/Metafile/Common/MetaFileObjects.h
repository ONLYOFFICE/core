#ifndef _METAFILE_OBJECTS_H
#define _METAFILE_OBJECTS_H

namespace MetaFile
{
	typedef enum
	{
		METAFILE_OBJECT_UNKNOWN = 0x00,
		METAFILE_OBJECT_BRUSH = 0x01,
		METAFILE_OBJECT_FONT = 0x02,
		METAFILE_OBJECT_PEN = 0x03,
		METAFILE_OBJECT_PALETTE = 0x04
	} EMetaFileObjectType;

	class CMetaFileObjectBase
	{
	public:
		CMetaFileObjectBase(){}
		virtual ~CMetaFileObjectBase(){}
		virtual EMetaFileObjectType GetType()
		{
			return METAFILE_OBJECT_UNKNOWN;
		}
	};
}

#endif //_METAFILE_OBJECTS_H