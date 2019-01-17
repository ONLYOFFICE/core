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
#pragma once	

#include <vector>
#include <string>

#include "../DocxDocConverter/Prl.h"

typedef std::vector<ASCDocFileFormat::Prl>	PrlList;

namespace DOCXTODOC
{
	class CInternalStyle
	{
	public:

		CInternalStyle () : m_bIsRenewed(FALSE)
		{

		}

		CInternalStyle (std::string strName, const PrlList& arRun, const PrlList& arParagraph) : m_bIsRenewed(FALSE), m_strName(strName)
		{
			SetPrlsLists (arRun, arParagraph);
		}

		CInternalStyle (std::string strBaseStyle) : m_bIsRenewed(FALSE)
		{

		}

		inline void SetPrlsLists (const PrlList& arRun, const PrlList& arParagraph)
		{
			m_arRun			=	arRun;
			m_arParagraph	=	arParagraph;
		}

		inline void SetName (const std::string& strName)
		{
			m_strName	=	strName;
		}

		inline bool UpdateRunPrls (const PrlList& arRun)
		{
			if (0 == arRun.size())		//	не дополняем стиль
				return FALSE;

			if (0 == m_arRun.size())	//	стиль не имеет параметров для символов
			{
				m_arRun	= arRun;
				return TRUE;
			}

			bool bModifity	=	FALSE;

			for (size_t i = 0; i < arRun.size(); ++i)
			{
				int nInd = FindSprm (arRun[i].GetSprmCode());
				if (nInd >= 0)
				{
					if (m_arRun[nInd] != arRun[i])
					{
						m_arRun[nInd]	=	arRun[i];
						bModifity		=	TRUE;
					}
				}
				else
				{
					m_arRun.push_back (arRun[i]);
					bModifity			=	TRUE;
				}
			}

			m_bIsRenewed	=	bModifity;

			return bModifity;
		}

		inline bool IsRenewed ()
		{
			return m_bIsRenewed;
		}

		inline bool operator == (const CInternalStyle& oStyle)
		{
			if ((oStyle.m_arRun.size() != m_arRun.size()) || (oStyle.m_arParagraph.size() != m_arParagraph.size()) )
				return false;

			for (size_t i = 0; i < m_arRun.size(); ++i)
			{
				if (m_arRun[i] != oStyle.m_arRun[i])
					return false;
			}
			
			for (size_t i = 0; i < m_arParagraph.size(); ++i)
			{
				if (m_arParagraph[i] != oStyle.m_arParagraph[i])
					return false;
			}

			return true; // ((oStyle.m_strName == m_strName) );//&& (oStyle.m_arRun == m_arRun) && (oStyle.m_arParagraph == m_arParagraph));
		}

		inline const PrlList& GetRun()
		{
			return m_arRun;
		}

	private:
		
		inline int FindSprm (unsigned short sprm)
		{
			for (size_t i = 0; i < m_arRun.size(); ++i)
			{
				if (sprm == m_arRun[i].GetSprmCode())
					return i;
			}

			return -1;
		}

	private:

		bool m_bIsRenewed;

		std::string m_strName;

		PrlList	m_arRun;
		PrlList	m_arParagraph;
	};

}
