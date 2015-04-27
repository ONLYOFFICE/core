#ifndef _METAFILE_COMMON_METAFILE_H
#define _METAFILE_COMMON_METAFILE_H

namespace MetaFile
{
	class CMetaFileBase
	{
	public:
		CMetaFileBase()
		{
		}
		virtual ~CMetaFileBase()
		{
		}

		virtual void SetError()
		{
		}
	};
}

#endif //_METAFILE_COMMON_METAFILE_H
