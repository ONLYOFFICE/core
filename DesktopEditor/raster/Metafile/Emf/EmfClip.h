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
		EMF_CLIPCOMMAND_SETPATH   = 0x02,
		EMF_CLIPCOMMAND_EXCLUDE   = 0x03
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
		CEmfClipCommandIntersect(TRectD& oRect) : m_oRect(oRect)
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
		TRectD m_oRect;
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
	class CEmfClipCommandExclude : public CEmfClipCommandBase
	{
	public:
		CEmfClipCommandExclude(TRectD& oClip, TRectD& oBB) : m_oClip(oClip), m_oBB(oBB)
		{
		}
		~CEmfClipCommandExclude()
		{
		}
		EEmfClipCommandType GetType()
		{
			return EMF_CLIPCOMMAND_INTERSECT;
		}

	public:
		TRectD m_oClip;
		TRectD m_oBB;
	};

	class CEmfClip : public IClip
	{
	public:
		CEmfClip();
		~CEmfClip();

		void operator=(CEmfClip& oClip);
		void Reset();
		bool Intersect(TRectD& oRect);
		bool Exclude(TRectD& oClip, TRectD& oBB);
		bool SetPath(CEmfPath* pPath, unsigned int umMode);
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CEmfClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_EMF_EMFCLIP_H