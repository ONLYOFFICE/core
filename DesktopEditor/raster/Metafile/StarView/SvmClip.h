/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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