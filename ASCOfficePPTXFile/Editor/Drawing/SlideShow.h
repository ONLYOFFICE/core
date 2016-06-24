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
#include "Elements.h"

namespace NSPresentationEditor
{
	class CTransition
	{
	public:
		bool	m_bAudioPresent;		// есть ли аудио
		CAudioElement m_oAudio;			// аудио

		BYTE	m_nEffectDirection;		// направление эффекта
		BYTE	m_nEffectType;			// тип перехода

		bool	m_bLoopSound;			// зациклить аудио
		bool	m_bStopSound;			// перед транзишном перестать играть все аудио до этого

		double	m_dSpeed;				// длительность в миллисекундах

	public:

		CTransition() : m_oAudio()
		{
			m_bAudioPresent		= false;
			
			m_nEffectType		= 0;
			m_nEffectDirection	= 0;

			m_bLoopSound		= false;
			m_bStopSound		= false;

			m_dSpeed			= 500.0;
		}
		~CTransition()
		{
		}

		CTransition& operator=(const CTransition& oSrc)
		{
			m_bAudioPresent		= oSrc.m_bAudioPresent;
			
			m_nEffectType		= oSrc.m_nEffectType;
			m_nEffectDirection	= oSrc.m_nEffectDirection;

			m_bLoopSound		= oSrc.m_bLoopSound;
			m_bStopSound		= oSrc.m_bStopSound;

			m_dSpeed			= oSrc.m_dSpeed;

			return *this;
		}

		CTransition(const CTransition& oSrc)
		{
			*this = oSrc;
		}
	};
	
	class CSlideShowInfo
	{
	public:
		double	m_dSlideDuration;	// длительность слайда (без учета перехода)
		bool	m_bHidden;			// нужно ли НЕ показывать слайд при проигрывании

		CTransition	m_oTransition;	// переход

		bool	m_bOnlyClick;		// переход на следующий по щелчку

	public:

		CSlideShowInfo() : m_oTransition()
		{
			m_dSlideDuration	= 30000.0;
			m_bHidden			= false;
			m_bOnlyClick		= false;
		}
		~CSlideShowInfo()
		{
		}

		CSlideShowInfo& operator=(const CSlideShowInfo& oSrc)
		{
			m_dSlideDuration	= oSrc.m_dSlideDuration;
			m_bHidden			= oSrc.m_bHidden;
			m_bOnlyClick		= oSrc.m_bOnlyClick;

			m_oTransition		= oSrc.m_oTransition;
			return *this;
		}
		CSlideShowInfo(const CSlideShowInfo& oSrc)
		{
			*this = oSrc;
		}
	};
}