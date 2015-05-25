#ifndef _METAFILE_SVM_SVMCLIP_H
#define _METAFILE_SVM_SVMCLIP_H

#include "../Common/MetaFileClip.h"
#include "../Common/IOutputDevice.h"
#include "../Common/MetaFileTypes.h"

#include <vector>

namespace MetaFile
{
	typedef enum
	{
		SVM_CLIPCOMMAND_UNKNOWN   = 0x00,
		SVM_CLIPCOMMAND_INTERSECT = 0x01,
		SVM_CLIPCOMMAND_EXCLUDE   = 0x02
	} ESvmClipCommandType;

	class CSvmClipCommandBase
	{
	public:
		CSvmClipCommandBase()
		{
		}
		virtual ~CSvmClipCommandBase()
		{
		}
		virtual ESvmClipCommandType GetType()
		{
			return SVM_CLIPCOMMAND_UNKNOWN;
		}
	};
	class CSvmClipCommandIntersect : public CSvmClipCommandBase
	{
	public:
		CSvmClipCommandIntersect(double dL, double dT, double dR, double dB)
		{
			m_dL = dL;
			m_dT = dT;
			m_dR = dR;
			m_dB = dB;
		}
		~CSvmClipCommandIntersect()
		{
		}
		ESvmClipCommandType GetType()
		{
			return SVM_CLIPCOMMAND_INTERSECT;
		}

	public:

		double m_dL;
		double m_dT;
		double m_dR;
		double m_dB;
	};
	class CSvmClipCommandExclude : public CSvmClipCommandBase
	{
	public:
		CSvmClipCommandExclude(double dL, double dT, double dR, double dB, double dWindowL, double dWindowT, double dWindowR, double dWindowB)
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
		~CSvmClipCommandExclude()
		{
		}
		ESvmClipCommandType GetType()
		{
			return SVM_CLIPCOMMAND_EXCLUDE;
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

	class CSvmClip : public IClip
	{
	public:
		CSvmClip();
		~CSvmClip();

		void operator=(CSvmClip& oClip);
		void Reset();
		bool Intersect(double dL, double dT, double dR, double dB);
		bool Exclude(double dL, double dT, double dR, double dB, double dLbb, double dTbb, double dRbb, double dBbb);

		// IClip
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CSvmClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_SVM_SVMCLIP_H