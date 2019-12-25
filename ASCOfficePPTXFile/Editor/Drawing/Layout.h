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
#include "Element.h"

namespace PPT_FORMAT
{
	enum eSlideSize
	{
		SS_Screen		= 0,
		SS_LetterPaper	= 1,
		SS_A4Paper		= 2,
		SS_35mm			= 3,
		SS_Overhead		= 4,
		SS_Banner		= 5,
		SS_Custom		= 6
	};

	enum ePlaceholderType
	{
		PT_None						= 0,
		PT_MasterTitle				= 1,
		PT_MasterBody				= 2,
		PT_MasterCenterTitle		= 3,
		PT_MasterSubTitle			= 4,
		PT_MasterNotesSlideImage	= 5,
		PT_MasterNotesBody			= 6,
		PT_MasterDate				= 7,
		PT_MasterSlideNumber		= 8,
		PT_MasterFooter				= 9,
		PT_MasterHeader				= 10,
		PT_NotesSlideImage			= 11,
		PT_NotesBody				= 12,
		PT_Title					= 13,
		PT_Body						= 14,
		PT_CenterTitle				= 15,
		PT_SubTitle					= 16,
		PT_VerticalTitle			= 17,
		PT_VerticalBody				= 18,
		PT_Object					= 19,
		PT_Graph					= 20,
		PT_Table					= 21,
		PT_ClipArt					= 22,
		PT_OrgChart					= 23,
		PT_Media					= 24,
		PT_Body_Empty				= 25
	};

	enum Instances
	{
		CollectionOfSlides			= 0,
		CollectionOfMasterSlides	= 1,
		CollectionOfNotesSlides		= 2
	};

	enum eSlideLayoutType
	{
		SL_TitleSlide			= 0,
		SL_TitleBody			= 1,
		SL_MasterTitle			= 2,
		SL_TitleOnly			= 7,
		SL_TwoColumns			= 8,
		SL_TwoRows				= 9,
		SL_ColumnTwoRows		= 10,
		SL_TwoRowsColumn		= 11,
		SL_TwoColumnsRow		= 13,
		SL_FourObjects			= 14,
		SL_BigObject			= 15,
		SL_Blank				= 16,
		SL_VerticalTitleBody	= 17,
		SL_VerticalTwoRows		= 18
	};

	enum eTextType
	{
		Tx_TYPE_TITLE		= 0,
		Tx_TYPE_BODY		= 1,
		Tx_TYPE_NOTES		= 2,
		Tx_TYPE_OTHER		= 4,
		Tx_TYPE_CENTERBODY	= 5,
		Tx_TYPE_CENTERTITLE	= 6,
		Tx_TYPE_HALFBODY	= 7,
		Tx_TYPE_QUARTERBODY	= 8,

		NoPresent			= 0xFF
	};
	static void CorrectPlaceholderType(int & type)
	{
		switch (type)
		{
		case PT_MasterTitle:			type = PT_Title;			break;
		case PT_MasterBody:				type = PT_Body;				break;
		case PT_MasterCenterTitle:		type = PT_CenterTitle;		break;
		case PT_MasterSubTitle:			type = PT_SubTitle;			break;
		case PT_MasterNotesSlideImage:	type = PT_NotesSlideImage;	break;
		case PT_MasterNotesBody:		type = PT_NotesBody;		break;
		}
	}

	static bool isTitlePlaceholder(int type)
	{
		switch (type)
		{
		case PT_MasterTitle:			
		case PT_MasterCenterTitle:
		case PT_Title:
		case PT_CenterTitle:
		case PT_VerticalTitle:
			return true;
		default:
			return false;
		}
	}
	static bool isBodyPlaceholder(int type)
	{
		switch (type)
		{
		case PT_MasterBody:			
		case PT_Body:
		case PT_VerticalBody:
			return true;
		default:
			return false;
		}
	}

	class CLayout
	{
	public:

		std::vector<CElementPtr>		m_arElements;
		std::vector<CColor>				m_arColorScheme;
		std::multimap<int, CElementPtr>	m_mapPlaceholders;

		bool					m_bIsTitleMaster;
		bool					m_bUseThemeColorScheme;

		bool					m_bIsBackground;
		CBrush					m_oBackground;

		bool					m_bHasDate;
		bool					m_bHasSlideNumber;
		bool					m_bHasFooter;
		int						m_nFormatDate;
		vector_string			m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

		bool					m_bShowMasterShapes;
		std::wstring			m_strLayoutType;

		std::wstring			m_sName;

		CLayout() 
		{
			Clear();
		}
		void Clear()
		{
			m_arElements.clear();
			m_mapPlaceholders.clear();

			m_bIsTitleMaster	= false;
			
			m_bHasDate			= false;
			m_bHasSlideNumber	= false;
			m_bHasFooter		= false;
			m_nFormatDate		= 1;
			
			for (int i = 0 ; i < 3 ; i++) 
				m_PlaceholdersReplaceString[i].clear();

			m_bUseThemeColorScheme	= true;
			m_bShowMasterShapes		= true;
			m_strLayoutType			= _T("obj");
			m_bIsBackground			= false;
		}

		void CreateDublicateElements()
		{
			// просто из всех своих элементов делаем дубликата

			for (size_t nIndex = 0; nIndex < m_arElements.size(); ++nIndex)
			{
				CElementPtr pElem = m_arElements[nIndex];
				if (NULL != pElem)
				{
					m_arElements[nIndex] = pElem->CreateDublicate();
				}
			}
		}

		CElementPtr GetPlaceholder(LONG lID)
		{
			size_t nCount = m_arElements.size();
			
			for (size_t i = 0; i < nCount; ++i)
			{
				CElementPtr pElem = m_arElements[i];

				if (pElem->m_lPlaceholderType == lID)
				{
					return pElem;
				}
			}
			
			return CElementPtr();
		}

		LONG GetCountPlaceholderWithType(LONG lType)
		{
			LONG lFound = 0;

			for (size_t i = 0; i < m_arElements.size(); ++i)
			{
				if (m_arElements[i]->m_lPlaceholderType == lType)
				{
					++lFound;
				}
			}
			
			return lFound;
		}
		ODRAW::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return ODRAW::CColor();
		}

        static void CheckPlaceholderStyle(std::wstring strStyleName, LONG& lType, LONG& lLevel, LONG& lTypeStyle)
		{
            size_t		nLen  = strStyleName.length();
            wchar_t*    pData = (wchar_t*)strStyleName.c_str();

			lType		= 0;
			lLevel		= 0;
			lTypeStyle	= -1;

            wchar_t* pDataEnd = pData + nLen;
            wchar_t* pDataMem = pData + 1;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lType *= 10;
                lType += (*pDataMem - ((wchar_t)'0'));
			}

			pDataMem += 4;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lLevel *= 10;
                lLevel += (*pDataMem - ((wchar_t)'0'));
			}

			++pDataMem;
			if (pDataMem + 1 < pDataEnd)
			{
                if ((wchar_t('c') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
					lTypeStyle = 1;
                if ((wchar_t('p') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
					lTypeStyle = 0;
			}
		}
	};	
	typedef boost::shared_ptr<CLayout> CLayoutPtr;
}
