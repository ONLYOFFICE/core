/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef CCLIP_H
#define CCLIP_H

#include "MetaFileTypes.h"
#include "CPath.h"

namespace MetaFile
{
	typedef enum
	{
		CLIP_COMMAND_UNKNOWM,
		CLIP_COMMAND_INSTERSECT,
		CLIP_COMMAND_SETPATH,
		CLIP_COMMAND_EXCLUDE
	} EClipCommandType;

	class CClipCommandBase
	{
	public:
		CClipCommandBase();
		virtual ~CClipCommandBase();
		virtual EClipCommandType GetType() const;
	};

	class CClipCommandIntersect : public CClipCommandBase
	{
	public:
		CClipCommandIntersect(const TRectD& oRect);
		CClipCommandIntersect(const CClipCommandIntersect& oCommandIntersect);
		virtual ~CClipCommandIntersect();
		virtual EClipCommandType GetType() const override;

		const TRectD& GetRect() const;
	private:
		TRectD m_oRect;
	};

	class CClipCommandPath : public CClipCommandBase
	{
	public:
		CClipCommandPath(const CPath& oPath, unsigned int unMode, const TXForm& oTransform = TXForm());
		CClipCommandPath(const CClipCommandPath& oCommandPath);
		virtual ~CClipCommandPath();
		virtual EClipCommandType GetType() const override;

		const CPath&  GetPath()      const;
		const TXForm& GetTransform() const;
		unsigned int  GetMode()      const;
	private:
		CPath        m_oPath;
		TXForm       m_oTransform;
		unsigned int m_unMode;
	};
	
	class CClipCommandExlude : public CClipCommandBase
	{
	public:
		CClipCommandExlude(const TRectD& oClip, const TRectD& oBB);
		CClipCommandExlude(const CClipCommandExlude& oCommandExlude);
		virtual ~CClipCommandExlude();
		virtual EClipCommandType GetType() const override;

		const TRectD& GetClip() const;
		const TRectD& GetBB()   const;
	private:
		TRectD m_oClip;
		TRectD m_oBB;
	};

	class CClip
	{
	public:
		CClip();
		CClip(const CClip& oClip);
		~CClip();

		void Copy(const CClip& oClip);
		bool Empty() const;

		void Reset();
		bool Intersect(const TRectD& oRect);
		bool Exclude(const TRectD& oClip, const TRectD& oBB);
		bool SetPath(const CPath& oPath, unsigned int unMode, const TXForm& oTransform = TXForm());

		void DrawOnRenderer(IOutputDevice* pOutput) const;

		const std::vector<CClipCommandBase*> GetCommands() const;
	private:
		bool AddCommand(CClipCommandBase* pCommande);
		void Clear();

		std::vector<CClipCommandBase*> m_arCommands;
	};
}

#endif // CCLIP_H
