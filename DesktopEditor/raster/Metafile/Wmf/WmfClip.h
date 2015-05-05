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
		CWmfClipCommandIntersect(double dL, double dT, double dR, double dB)
		{
			m_dL = dL;
			m_dT = dT;
			m_dR = dR;
			m_dB = dB;
		}
		~CWmfClipCommandIntersect()
		{
		}
		EWmfClipCommandType GetType()
		{
			return WMF_CLIPCOMMAND_INTERSECT;
		}

	public:

		double m_dL;
		double m_dT;
		double m_dR;
		double m_dB;
	};
	class CWmfClipCommandExclude : public CWmfClipCommandBase
	{
	public:
		CWmfClipCommandExclude(double dL, double dT, double dR, double dB, double dWindowL, double dWindowT, double dWindowR, double dWindowB)
		{
			m_dL = dL;
			m_dT = dT;
			m_dR = dR;
			m_dB = dB;
			m_dWindowL = dWindowL;
			m_dWindowT = dWindowT;
			m_dWindowR = dWindowR;
			m_dWindowB = dWindowB;
		}
		~CWmfClipCommandExclude()
		{
		}
		EWmfClipCommandType GetType()
		{
			return WMF_CLIPCOMMAND_EXCLUDE;
		}

	public:

		double m_dL;
		double m_dT;
		double m_dR;
		double m_dB;
		double m_dWindowL;
		double m_dWindowT;
		double m_dWindowR;
		double m_dWindowB;
	};

	class CWmfClip : public IClip
	{
	public:
		CWmfClip();
		~CWmfClip();

		void operator=(CWmfClip& oClip);
		void Reset();
		bool Intersect(double dL, double dT, double dR, double dB);
		bool Exclude(double dL, double dT, double dR, double dB, double dLbb, double dTbb, double dRbb, double dBbb);

		// IClip
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CWmfClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_WMF_WMFCLIP_H