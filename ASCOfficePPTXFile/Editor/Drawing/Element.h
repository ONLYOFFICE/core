﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

namespace NSPresentationEditor
{
	enum ElementType
	{
		etGroup		 = 0,
		etVideo		 = 1,
		etAudio		 = 2,
		etPicture    = 3,
		etShape      = 4,
		etText		 = 5
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

	class CElement;
	typedef boost::shared_ptr<CElement> CElementPtr;

	class CElement
	{
	public:
		ElementType			m_etType;
		
		CDoubleRect			m_rcChildAnchor;
		CDoubleRect			m_rcAnchor;
		CDoubleRect			m_rcGroupAnchor;

		bool				m_bChildAnchorEnabled;
		bool				m_bAnchorEnabled;
		bool				m_bGroupAnchorEnabled;
		
		double				m_dStartTime;
		double				m_dEndTime;

		CTextInteractiveInfo m_oTextActions;
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

		double				m_dRotate;			// угол поворота в градусах
		bool				m_bFlipH;			// симметричное отражение относительно горизонтальной оси
		bool				m_bFlipV;			// симметричное отражение относительно вертикальной оси

		bool				m_bLine;
		bool				m_bIsFilled;

		bool				m_bIsBackground;	
		bool				m_bHaveAnchor;	
		bool				m_bHidden;

		CTheme*				m_pTheme;
		CLayout*			m_pLayout;

		std::wstring		m_sName;
		std::wstring		m_sDescription;

		std::wstring		m_sHyperlink;

		std::vector<CElementPtr>*	m_pParentElements;
		std::vector<CElementPtr>	m_pChildElements;

		CElement()
		{
			m_bIsBackground				= false;
			m_bHaveAnchor				= true;
			m_bHidden					= false;

			m_bChildAnchorEnabled		= false;
			m_bAnchorEnabled			= false;
			m_bGroupAnchorEnabled		= false;

			m_lID						= -1;
			m_lLayoutID					= -1;

			m_lPlaceholderID			= -1;
			m_lPlaceholderType			= -1;
			m_bPlaceholderSet			= false;
			m_lPlaceholderSizePreset	= -1;
			m_lPlaceholderUserStr		= -1;
			m_nFormatDate				= 1;

			m_etType					= etShape;
						
			m_dStartTime				= 0.0;
			m_dEndTime					= 30.0;

			m_dRotate					= 0.0;
			m_bFlipH					= false;
			m_bFlipV					= false;
			m_bLine						= true;
			m_bIsFilled					= true;

			m_pTheme					= NULL;
			m_pLayout					= NULL;
			m_pParentElements			= NULL;
		}
		virtual ~CElement()
		{
		}

		virtual CElementPtr CreateDublicate() = 0;

		virtual void SetProperiesToDublicate(CElementPtr pDublicate)
		{
			if (!pDublicate)
				return;

			pDublicate->m_bIsBackground				= m_bIsBackground;	
			pDublicate->m_bHaveAnchor				= m_bHaveAnchor;
			pDublicate->m_bHidden					= m_bHidden;

			pDublicate->m_etType					= m_etType;
			
			pDublicate->m_rcChildAnchor				= m_rcChildAnchor;
			pDublicate->m_rcAnchor					= m_rcAnchor;
			pDublicate->m_rcGroupAnchor				= m_rcGroupAnchor;

			pDublicate->m_bChildAnchorEnabled		= m_bChildAnchorEnabled;
			pDublicate->m_bAnchorEnabled			= m_bAnchorEnabled;
			pDublicate->m_bGroupAnchorEnabled		= m_bGroupAnchorEnabled;

			pDublicate->m_dStartTime				= m_dStartTime;
			pDublicate->m_dEndTime					= m_dEndTime;

			pDublicate->m_lID						= m_lID;
			pDublicate->m_lLayoutID					= m_lLayoutID;

			pDublicate->m_oActions					= m_oActions;
			pDublicate->m_oTextActions				= m_oTextActions;
			pDublicate->m_oAnimations				= m_oAnimations;
			pDublicate->m_oEffects					= m_oEffects;

			pDublicate->m_lPlaceholderID			= m_lPlaceholderID;
			pDublicate->m_lPlaceholderType			= m_lPlaceholderType;
			pDublicate->m_bPlaceholderSet			= m_bPlaceholderSet;
			
			pDublicate->m_lPlaceholderSizePreset	= m_lPlaceholderSizePreset;
			pDublicate->m_lPlaceholderUserStr		= m_lPlaceholderUserStr;
			pDublicate->m_nFormatDate				= m_nFormatDate;

			pDublicate->m_dRotate					= m_dRotate;
			pDublicate->m_bFlipH					= m_bFlipH;
			pDublicate->m_bFlipV					= m_bFlipV;
			pDublicate->m_bLine						= m_bLine;
			pDublicate->m_bIsFilled					= m_bIsFilled;

			pDublicate->m_pTheme					= m_pTheme;
			pDublicate->m_pLayout					= m_pLayout;
			
			pDublicate->m_oPen						= m_oPen;
			pDublicate->m_oBrush					= m_oBrush;
			pDublicate->m_oShadow					= m_oShadow;
		}

		void NormalizeCoordsByMetric()
		{
			if (!m_bAnchorEnabled) return;
			double dScale = 1587.5; //master to emu
			
			m_rcAnchor.left		= dScale * m_rcAnchor.left;
			m_rcAnchor.right	= dScale * m_rcAnchor.right;
			m_rcAnchor.top		= dScale * m_rcAnchor.top;
			m_rcAnchor.bottom	= dScale * m_rcAnchor.bottom;
		}
	};
}
