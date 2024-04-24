/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "CClip.h"

namespace MetaFile
{
	CClipCommandBase::CClipCommandBase()
	{}

	CClipCommandBase::~CClipCommandBase()
	{}

	EClipCommandType CClipCommandBase::GetType() const
	{
		return CLIP_COMMAND_UNKNOWM;
	}

	CClipCommandIntersect::CClipCommandIntersect(const TRectD &oRect)
		: m_oRect(oRect)
	{}

	CClipCommandIntersect::CClipCommandIntersect(const CClipCommandIntersect &oCommandIntersect)
		: m_oRect(oCommandIntersect.m_oRect)
	{}

	CClipCommandIntersect::~CClipCommandIntersect()
	{}

	EClipCommandType CClipCommandIntersect::GetType() const
	{
		return CLIP_COMMAND_INSTERSECT;
	}

	const TRectD &CClipCommandIntersect::GetRect() const
	{
		return m_oRect;
	}

	CClipCommandPath::CClipCommandPath(const CPath &oPath, unsigned int unMode, const TXForm& oTransform)
		: m_oPath(oPath), m_oTransform(oTransform), m_unMode(unMode)
	{}

	CClipCommandPath::CClipCommandPath(const CClipCommandPath &oCommandPath)
		: m_oPath(oCommandPath.m_oPath)
	{}

	CClipCommandPath::~CClipCommandPath()
	{}

	EClipCommandType CClipCommandPath::GetType() const
	{
		return CLIP_COMMAND_SETPATH;
	}

	const CPath &CClipCommandPath::GetPath() const
	{
		return m_oPath;
	}

	const TXForm &CClipCommandPath::GetTransform() const
	{
		return m_oTransform;
	}

	unsigned int CClipCommandPath::GetMode() const
	{
		return m_unMode;
	}

	CClipCommandExlude::CClipCommandExlude(const TRectD &oClip, const TRectD &oBB)
		: m_oClip(oClip), m_oBB(oBB)
	{}

	CClipCommandExlude::CClipCommandExlude(const CClipCommandExlude &oCommandExlude)
		: m_oClip(oCommandExlude.m_oClip), m_oBB(oCommandExlude.m_oBB)
	{}

	CClipCommandExlude::~CClipCommandExlude()
	{}

	EClipCommandType CClipCommandExlude::GetType() const
	{
		return CLIP_COMMAND_EXCLUDE;
	}

	const TRectD &CClipCommandExlude::GetClip() const
	{
		return m_oClip;
	}

	const TRectD &CClipCommandExlude::GetBB() const
	{
		return m_oBB;
	}

	CClip::CClip()
	{}

	CClip::CClip(const CClip &oClip)
	{
		Copy(oClip);
	}

	CClip::~CClip()
	{
		Clear();
	}

	void CClip::Copy(const CClip &oClip)
	{
		Clear();
		
		for (const CClipCommandBase* pCommand : m_arCommands)
		{
			switch(pCommand->GetType())
			{
			case CLIP_COMMAND_INSTERSECT:
			{
				AddCommand(new CClipCommandIntersect(*(CClipCommandIntersect*)pCommand));
				break;
			}
			case CLIP_COMMAND_SETPATH:
			{
				AddCommand(new CClipCommandPath(*(CClipCommandPath*)pCommand));
				break;
			}
			case CLIP_COMMAND_EXCLUDE:
			{
				AddCommand(new CClipCommandExlude(*(CClipCommandExlude*)pCommand));
				break;
			}
			case CLIP_COMMAND_UNKNOWM:
			break;
			}
		}
	}
	
	bool CClip::Empty() const
	{
		return m_arCommands.empty();
	}

	void CClip::Reset()
	{
		Clear();
	}

	bool CClip::Intersect(const TRectD &oRect)
	{
		return AddCommand(new CClipCommandIntersect(oRect));
	}

	bool CClip::Exclude(const TRectD &oClip, const TRectD &oBB)
	{
		return AddCommand(new CClipCommandExlude(oClip, oBB));
	}

	bool CClip::SetPath(const CPath &oPath, unsigned int unMode, const TXForm &oTransform)
	{
		if (RGN_COPY == unMode)
			Reset();

		return AddCommand(new CClipCommandPath(oPath, unMode, oTransform));
	}

	void CClip::DrawOnRenderer(IOutputDevice *pOutput) const
	{
		if (NULL == pOutput)
			return;

		pOutput->ResetClip();

		for (const CClipCommandBase* pCommand : m_arCommands)
		{
			switch(pCommand->GetType())
			{
			case CLIP_COMMAND_INSTERSECT:
			{
				CClipCommandIntersect *pCommandInstersect = (CClipCommandIntersect*)pCommand;
				pOutput->IntersectClip(pCommandInstersect->GetRect());
				break;
			}
			case CLIP_COMMAND_SETPATH:
			{
				CClipCommandPath* pCommandPath = (CClipCommandPath*)pCommand;
				pOutput->PathClip(pCommandPath->GetPath(), pCommandPath->GetMode(), (TXForm*)&pCommandPath->GetTransform());
				break;
			}
			case CLIP_COMMAND_EXCLUDE:
			{
				CClipCommandExlude* pCommandExclude = (CClipCommandExlude*)pCommand;
				pOutput->ExcludeClip(pCommandExclude->GetClip(), pCommandExclude->GetBB());
				break;
			}
			case CLIP_COMMAND_UNKNOWM:
			break;
			}
		}
	}
	
	const std::vector<CClipCommandBase *> CClip::GetCommands() const
	{
		return m_arCommands;
	}

	bool CClip::AddCommand(CClipCommandBase *pCommande)
	{
		if (NULL == pCommande)
			return false;

		m_arCommands.push_back(pCommande);
		return true;
	}

	void CClip::Clear()
	{
		for (CClipCommandBase* pCommand : m_arCommands)
			delete pCommand;

		m_arCommands.clear();
	}
}

