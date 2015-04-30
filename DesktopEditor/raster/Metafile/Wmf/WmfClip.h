#ifndef _METAFILE_WMF_WMFCLIP_H
#define _METAFILE_WMF_WMFCLIP_H

#include "../Common/MetaFileClip.h"
#include "../Common/IOutputDevice.h"
#include "../Common/MetaFileTypes.h"

#include <vector>

namespace MetaFile
{
	typedef enum
	{
		WMF_CLIPCOMMAND_UNKNOWN   = 0x00,
		WMF_CLIPCOMMAND_INTERSECT = 0x01,
		WMF_CLIPCOMMAND_EXCLUDE   = 0x02
	} EWmfClipCommandType;

	class CWmfClipCommandBase
	{
	public:
		CWmfClipCommandBase()
		{
		}
		virtual ~CWmfClipCommandBase()
		{
		}
		virtual EWmfClipCommandType GetType()
		{
			return WMF_CLIPCOMMAND_UNKNOWN;
		}
	};
	class CWmfClipCommandIntersect : public CWmfClipCommandBase
	{
	public:
		CWmfClipCommandIntersect(short shL, short shT, short shR, short shB)
		{
			m_shL = shL;
			m_shT =	shT;
			m_shR =	shR;
			m_shB =	shB;
		}
		~CWmfClipCommandIntersect()
		{
		}
		EWmfClipCommandType GetType()
		{
			return WMF_CLIPCOMMAND_INTERSECT;
		}

	public:

		short m_shL;
		short m_shT;
		short m_shR;
		short m_shB;
	};
	class CWmfClipCommandExclude : public CWmfClipCommandBase
	{
	public:
		CWmfClipCommandExclude(short shL, short shT, short shR, short shB, TRect oBB)
		{
			m_shL = shL;
			m_shT =	shT;
			m_shR =	shR;
			m_shB =	shB;
			m_oBB = oBB;
		}
		~CWmfClipCommandExclude()
		{
		}
		EWmfClipCommandType GetType()
		{
			return WMF_CLIPCOMMAND_EXCLUDE;
		}

	public:

		TRect m_oBB;
		short m_shL;
		short m_shT;
		short m_shR;
		short m_shB;
	};

	class CWmfClip : public IClip
	{
	public:
		CWmfClip();
		~CWmfClip();

		void operator=(CWmfClip& oClip);
		void Reset();
		bool Intersect(short shL, short shT, short shR, short shB);
		bool Exclude(short shL, short shT, short shR, short shB, TRect oBB);

		// IClip
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CWmfClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_WMF_WMFCLIP_H