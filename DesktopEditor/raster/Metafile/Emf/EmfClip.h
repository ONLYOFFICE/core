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
#ifndef _METAFILE_EMF_EMFCLIP_H
#define _METAFILE_EMF_EMFCLIP_H

#include <vector>
#include "EmfTypes.h"
#include "EmfPath.h"
#include "../Common/MetaFileClip.h"

namespace MetaFile
{
	class CEmfDC;

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
		CEmfClipCommandPath(CEmfPath* pPath, unsigned int unMode, TEmfXForm* pTransform);
		~CEmfClipCommandPath()
		{
		}
		EEmfClipCommandType GetType()
		{
			return EMF_CLIPCOMMAND_SETPATH;
		}

	public:

		TEmfXForm    m_oTransform;
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
		bool SetPath(CEmfPath* pPath, unsigned int umMode, TEmfXForm* pTransform);
		void ClipOnRenderer(IOutputDevice* pOutput);

	private:

		void Clear();

	private:

		std::vector<CEmfClipCommandBase*> m_vCommands;
	};
}

#endif // _METAFILE_EMF_EMFCLIP_H
