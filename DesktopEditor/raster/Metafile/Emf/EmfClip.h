#ifndef _METAFILE_EMF_EMFCLIP_H
#define _METAFILE_EMF_EMFCLIP_H

#include <vector>
#include "EmfTypes.h"
#include "EmfPath.h"
#include "../Common/MetaFileClip.h"

namespace MetaFile
{
	typedef enum
	{
		EMF_CLIPCOMMAND_UNKNOWN   = 0x00,
		EMF_CLIPCOMMAND_INTERSECT = 0x01,
		EMF_CLIPCOMMAND_SETPATH   = 0x02
	} EEmfClipCommandType;

	class CEmfClipCommandBase
	{
	public:
		CEmfClipCommandBase()
		{
		}
		virtual ~CEmfClipCommandBase()
		{
		}
		virtual EEmfClipCommandType GetType()
		{
			return EMF_CLIPCOMMAND_UNKNOWN;
		}
	};
	class CEmfClipCommandIntersect : public CEmfClipCommandBase
	{
	public:
		CEmfClipCommandIntersect(TEmfRectL& oRect) : m_oRect(oRect)
		{
		}
		~CEmfClipCommandIntersect()
		{
		}
		EEmfClipCommandType GetType()
		{
			return EMF_CLIPCOMMAND_INTERSECT;
		}

	public:
		TEmfRectL m_oRect;
	};
	class CEmfClipCommandPath : public CEmfClipCommandBase
	{
	public:
		CEmfClipCommandPath(CEmfPath* pPath, unsigned int unMode) : m_oPath(pPath), m_unMode(unMode)
		{
		}
		~CEmfClipCommandPath()
		{
		}
		EEmfClipCommandType GetType()
		{
			return EMF_CLIPCOMMAND_SETPATH;
		}

	public:

		CEmfPath     m_oPath;
		unsigned int m_unMode;
	};

	class CEmfClip : public IClip
	{
	public:
		CEmfClip();
		~CEmfClip();

		void operator=(CEmfClip& oClip);
		void Reset();
		bool Intersect(TEmfRectL& oRect);
		bool SetPath(CEmfPath* pPath, unsigned int umMode);
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CEmfClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_EMF_EMFCLIP_H