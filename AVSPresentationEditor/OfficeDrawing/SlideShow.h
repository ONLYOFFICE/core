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