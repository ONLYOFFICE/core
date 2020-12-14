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

#include "../Records/CurrentUserAtom.h"

//using namespace NSOfficePPT;

class CUserEdit
{ 
public:
	_UINT32 m_nOffsetLastEdit; 
	_UINT32 m_nOffsetPersistDirectory;  
	                          
	_UINT32 m_nDocumentRef; 
	_UINT32 m_nEncryptRef;
	_UINT32 m_nMaxPersistWritten;

	CUserEdit()
	{
		m_nOffsetLastEdit			= 0;
		m_nOffsetPersistDirectory	= 0;
		m_nDocumentRef				= 0;
		m_nMaxPersistWritten		= 0;
		m_nEncryptRef				= 0;
	}
	~CUserEdit()
	{
	}

	CUserEdit& operator =(const CUserEdit& oSrc)
	{
		m_nOffsetLastEdit			= oSrc.m_nOffsetLastEdit;
		m_nOffsetPersistDirectory	= oSrc.m_nOffsetPersistDirectory;
		m_nDocumentRef				= oSrc.m_nDocumentRef;
		m_nMaxPersistWritten		= oSrc.m_nMaxPersistWritten;
		m_nEncryptRef				= oSrc.m_nEncryptRef;
		return (*this);
	}

	void FromAtom(CRecordUserEditAtom* pAtom)
	{
		m_nOffsetLastEdit			= pAtom->m_nOffsetLastEdit;
		m_nOffsetPersistDirectory	= pAtom->m_nOffsetPersistDirectory;
		m_nDocumentRef				= pAtom->m_nOffsetDocPersistIdRef;
		m_nMaxPersistWritten		= pAtom->m_nPersistIdSeed;
		m_nEncryptRef				= pAtom->m_nEncryptSessionPersistIdRef;
	}
}; 

class CCurrentUser 
{ 
public:
	bool			m_bIsEncrypt;  
	_UINT32			m_nOffsetToCurrentEdit;
	std::wstring	m_sName;

	CCurrentUser()
	{
		m_bIsEncrypt			= false;
		m_nOffsetToCurrentEdit	= 0;
	}
	~CCurrentUser()
	{
	}

	CCurrentUser& operator =(const CCurrentUser& oSrc)
	{
		m_bIsEncrypt			= oSrc.m_bIsEncrypt;
		m_nOffsetToCurrentEdit	= oSrc.m_nOffsetToCurrentEdit;
		m_sName					= oSrc.m_sName;
		return (*this);
	}

	void FromAtom(CRecordCurrentUserAtom* pAtom)
	{
		m_bIsEncrypt			= (ENCRYPT == pAtom->m_nToken);
		
		m_nOffsetToCurrentEdit	= pAtom->m_nOffsetToCurEdit;
		m_sName					= pAtom->m_strUNICODEUserName;
	}
};


///----------- constants----------
const int MST_FLAG_OBJECTS		= 0x01; 
const int MST_FLAG_SCHEME		= 0x02; 
const int MST_FLAG_BACKGROUND	= 0x04; 

#define MAX_OBJECTS_IN_LAYOUT 8

const int S_HEADERFOOTER_DATE =        0x01; 
const int S_HEADERFOOTER_TODAYDATE =   0x02; 
const int S_HEADERFOOTER_USERDATE =    0x04; 
const int S_HEADERFOOTER_SLIDENUMBER = 0x08; 
const int S_HEADERFOOTER_HEADER =      0x10; 
const int S_HEADERFOOTER_FOOTER =      0x20;

#define PSR_NAMEDSHOW_SIZE    32 

//---------------------------------

struct SDocInfoAtom 
{ 
    SColorAtom  PenColor;       
    INT         RestartTime; 
    SHORT       StartSlide; 
    SHORT       EndSlide; 
    USHORT      NamedShow[PSR_NAMEDSHOW_SIZE]; 
    BYTE        Flags;
}; 
 
struct SSlideInfoAtom 
{ 
	INT        SlideTime;     // how long to show the slide in ticks 
	UINT       SoundRef;      
	USHORT     Effect;        // type of transition (2 character signature) 
	USHORT     Flags;         // set of flags that determine type of build 
	BYTE	   Speed;         // speed of transition 
};  
 
struct PSR_AnimationInfoAtom 
{ 
	SColorAtom  DimColor; 
	UINT        Flags; 
	UINT        SoundRef;      
	USHORT      OrderID;     
	USHORT      DelayTime; 
	USHORT      SlideCount; 
	BYTE        BuildType; 
	BYTE        FlyMethod; 
	BYTE        FlyDirection;      
	BYTE        AfterEffect;      
	BYTE        SubEffect;     
	BYTE        OleVerb; 
}; 
 
struct SInteractiveInfoAtom 
{ 
	UINT  SoundRef;      
	UINT  ExHyperlinkID; 
	BYTE  Action; 
	BYTE  OleVerb; 
	BYTE  Jump; 
	BYTE  Flags; 
};

