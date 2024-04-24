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
