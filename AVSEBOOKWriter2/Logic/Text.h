#pragma once
#include "../Common/FontManager.h"

namespace NSEBook
{
	using namespace NSCommon;

	class ILogicItem
	{
	public:
		enum ELogicItemType
		{
			elitTextLine		= 0,
			elitTable			= 1,
			elitGraphic			= 2,
			elitTextParagraph	= 3
		};

		ELogicItemType	m_eType;
		CDoubleRect		m_rcBounds;
		bool m_bIsPageBreakAfter;

	public:
		ILogicItem()
		{
			m_eType = elitTextLine;
			m_bIsPageBreakAfter = false;
		}
		~ILogicItem()
		{
		}

		virtual double GetPositionY()			= 0;
		virtual double GetPositionX()			= 0;
	};

	const long g_lNewNoJustifySpace = 5;
}