/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "TextAttributesEx.h"
#include "Interactive.h"
#include "Animations.h"
#include "ElementProperties.h"

namespace NSPresentationEditor
{
	enum ElementType
	{
		etVideo		 = 0,
		etAudio		 = 1,
		etPicture    = 2,
		etShape      = 3,
		etText		 = 4
	};

	enum enumPlaceholderType
	{
		PT_None	= 0,
		
		PT_MasterTitle,
		PT_MasterBody,
		PT_MasterCenterTitle,
		PT_MasterSubTitle, 
		PT_MasterNotesSlideImage, 
		PT_MasterNotesBody, 
		
		PT_MasterDate, 
		PT_MasterSlideNumber, 
		PT_MasterFooter, 
		PT_MasterHeader, 
		
		PT_NotesSlideImage, 
		PT_NotesBody, 
		
		PT_Title,
		PT_Body, 
		PT_CenterTitle, 
		PT_SubTitle, 
		PT_VerticalTitle, 
		PT_VerticalBody, 
		PT_Object, 
		PT_Graph, 
		PT_Table, 
		PT_ClipArt, 
		PT_OrgChart, 
		PT_Media,
		PT_VerticalObject, 
		PT_Picture 
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

	class CTheme;
	class CLayout;
	class CSlide;

	class IElement
	{
	public:
		ElementType			m_etType;
		CDoubleRect			m_rcBounds;
		CDoubleRect			m_rcBoundsOriginal;
		bool				m_bBoundsEnabled;

		double				m_dStartTime;
		double				m_dEndTime;

		CInteractiveInfo	m_oActions;
		CAnimationInfo		m_oAnimations;
		CEffects			m_oEffects;

		CPen				m_oPen;
		CBrush				m_oBrush;
		CShadow				m_oShadow;

		int					m_lID;
		int					m_lLayoutID;

		int					m_lPlaceholderID;
		int					m_lPlaceholderType;
		bool				m_bPlaceholderSet;
		int					m_lPlaceholderSizePreset;
		int					m_lPlaceholderUserStr;
		int					m_nFormatDate;

		// метрика
		CMetricInfo			m_oMetric;

		double				m_dRotate;			// угол поворота в градусах
		bool				m_bFlipH;			// симметричное отражение относительно горизонтальной оси
		bool				m_bFlipV;			// симметричное отражение относительно вертикальной оси

		bool				m_bLine;

		bool				m_bIsBackground;	
		bool				m_bHaveAnchor;	
		bool				m_bHidden;

		bool				m_bIsChangeable;	// можно ли редактировать элемент

		CElementProperties	m_oProperties;

		CTheme*				m_pTheme;
		CLayout*			m_pLayout;

		std::wstring		m_sName;
		std::wstring		m_sDescription;

		std::wstring		m_sHyperlink;

	protected:
		ULONG m_lCountRef;

	public:	
		
		virtual ULONG AddRef()
		{
			++m_lCountRef;
			return m_lCountRef;
		}
		virtual ULONG Release()
		{
			--m_lCountRef;
			if (0 == m_lCountRef)
			{
				delete this;
				return 0;
			}
			return m_lCountRef;
		}

	public:

		IElement()
		{
			m_bIsBackground				= false;
			m_bHaveAnchor				= true;
			m_bHidden					= false;

			m_bIsChangeable				= true;
			
			m_lID						= -1;
			m_lLayoutID					= -1;

			m_lPlaceholderID			= -1;
			m_lPlaceholderType			= -1;
			m_bPlaceholderSet			= false;
			m_lPlaceholderSizePreset	= -1;
			m_lPlaceholderUserStr		= -1;
			m_nFormatDate				= 1;

			m_etType					= etPicture;
			
			m_bBoundsEnabled			= true;
			
			m_rcBounds.left				= 0; 
			m_rcBounds.top				= 0;
			m_rcBounds.right			= 1; 
			m_rcBounds.bottom			= 1;

			m_rcBoundsOriginal.left		= 0; 
			m_rcBoundsOriginal.top		= 0;
			m_rcBoundsOriginal.right	= 1; 
			m_rcBoundsOriginal.bottom	= 1;

			m_dStartTime				= 0.0;
			m_dEndTime					= 30.0;

			m_dRotate					= 0.0;
			m_bFlipH					= false;
			m_bFlipV					= false;
			m_bLine						= true;

			m_lCountRef					= 1;

			m_pTheme					= NULL;
			m_pLayout					= NULL;
		}
		virtual ~IElement()
		{
		}

		virtual void NormalizeCoords(double dScaleX, double dScaleY)
		{
			m_rcBounds.left		= dScaleX * m_rcBoundsOriginal.left;
			m_rcBounds.right	= dScaleX * m_rcBoundsOriginal.right;
			m_rcBounds.top		= dScaleY * m_rcBoundsOriginal.top;
			m_rcBounds.bottom	= dScaleY * m_rcBoundsOriginal.bottom;
		}
		virtual void NormalizeCoordsByMetric()
		{
			double dScaleX				= (double)m_oMetric.m_lUnitsHor / m_oMetric.m_lMillimetresHor;
			double dScaleY				= (double)m_oMetric.m_lUnitsVer	/ m_oMetric.m_lMillimetresVer;
			
			m_rcBoundsOriginal.left		= dScaleX * m_rcBounds.left;
			m_rcBoundsOriginal.right	= dScaleX * m_rcBounds.right;
			m_rcBoundsOriginal.top		= dScaleY * m_rcBounds.top;
			m_rcBoundsOriginal.bottom	= dScaleY * m_rcBounds.bottom;
		}

	protected:
		virtual void SetProperiesToDublicate(IElement* pDublicate)
		{
			if (NULL == pDublicate)
				return;

			pDublicate->m_bBoundsEnabled			= m_bBoundsEnabled;

			pDublicate->m_bIsBackground				= m_bIsBackground;	
			pDublicate->m_bHaveAnchor				= m_bHaveAnchor;
			pDublicate->m_bHidden					= m_bHidden;
			
			pDublicate->m_bIsChangeable				= m_bIsChangeable;
			
			pDublicate->m_etType					= m_etType;
			
			pDublicate->m_rcBounds					= m_rcBounds;
			pDublicate->m_rcBoundsOriginal			= m_rcBoundsOriginal;

			pDublicate->m_dStartTime				= m_dStartTime;
			pDublicate->m_dEndTime					= m_dEndTime;

			pDublicate->m_lID						= m_lID;
			pDublicate->m_lLayoutID					= m_lLayoutID;

			pDublicate->m_oActions					= m_oActions;
			pDublicate->m_oAnimations				= m_oAnimations;
			pDublicate->m_oEffects					= m_oEffects;

			pDublicate->m_lPlaceholderID			= m_lPlaceholderID;
			pDublicate->m_lPlaceholderType			= m_lPlaceholderType;
			pDublicate->m_bPlaceholderSet			= m_bPlaceholderSet;
			
			pDublicate->m_lPlaceholderSizePreset	= m_lPlaceholderSizePreset;
			pDublicate->m_lPlaceholderUserStr		= m_lPlaceholderUserStr;
			pDublicate->m_nFormatDate				= m_nFormatDate;

			pDublicate->m_oMetric					= m_oMetric;
			pDublicate->m_oProperties				= m_oProperties;

			pDublicate->m_dRotate					= m_dRotate;
			pDublicate->m_bFlipH					= m_bFlipH;
			pDublicate->m_bFlipV					= m_bFlipV;
			pDublicate->m_bLine						= m_bLine;

			pDublicate->m_pTheme					= m_pTheme;
			pDublicate->m_pLayout					= m_pLayout;
			
			pDublicate->m_oPen						= m_oPen;
			pDublicate->m_oBrush					= m_oBrush;
			pDublicate->m_oShadow					= m_oShadow;
		}

	public:

		virtual void SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
		{
			std::map<CElementProperty::Type, CElementProperty>* pMap = &m_oProperties.m_arProperties;
			
			for (std::map<CElementProperty::Type, CElementProperty>::iterator pPair = pMap->begin(); pPair != pMap->end(); ++pPair)
			{
				CElementProperty oProperty = pPair->second;
				SetupProperty(pSlide, pTheme, pLayout, &oProperty);
			}
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)	= 0;
		virtual IElement* CreateDublicate()																			= 0;

		virtual CString SerializeToXml()
		{
			CString strElement = _T("");
			strElement.Format(_T("<Element type='%d' id='%d' lid='%d' pid='%d' ptp='%d' />"), 
				(int)m_etType, m_lID, m_lLayoutID, m_lPlaceholderID, m_lPlaceholderType);
			return strElement;
		}
	};
}