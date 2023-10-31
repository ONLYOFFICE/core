#pragma once
#include "Paragraph.h"

namespace NSDocxRenderer
{
	class CPeak
	{
	public:
		//
		// VI     II      V
		//         |
		// III <- Peak -> I
		//         |
		// VII     IV     VIII
		//note Направления V-VIII потребуются для реализации перечеркиваний ячек (m_bIsDiagonalDownBorder/m_bIsDiagonalUpBorder)
		enum eDirections
		{
			dI    = 0,
			dII   = 1,
			dIII  = 2,
			dIV   = 3,
			dV    = 4,
			dVI   = 5,
			dVII  = 6,
			dVIII = 7,
			dNumDirections = 8
		};

	public:
		CShape* const m_pPeak; //привязка к конкретному шейпу, определенному в качестве peak
		CShape* m_pLines[dNumDirections]; //сюда записываем линии/шейпы, которые подходят к peak с разных сторон

	public:
		CPeak(CShape* pPeak) : m_pPeak(pPeak) {}
	};

	class CCell : public CBaseItem
	{
	public:
		//Corners
		// IV------III
		// |        |
		// |  Cell  |
		// |        |
		// I--------II
		//У каждой ячейки может быть только 4 угла )
		enum eCorners
		{
			cI    = 0,
			cII   = 1,
			cIII  = 2,
			cIV   = 3,
			cNumCorners = 4
		};
	public:
		bool m_bIsvMergeStart {false};
		bool m_bIsvMerge {false};

		bool m_bIsTopBorder {false};
		bool m_bIsLeftBorder {false};
		bool m_bIsBottomBorder {false};
		bool m_bIsRightBorder {false};

		bool m_bIsDiagonalDownBorder {false};
		bool m_bIsDiagonalUpBorder {false};

		UINT m_uGridSpan {1};

		CPeak *m_pPeaks[cNumCorners]; //сюда записываем peak, распределяя их по углам ячейки

		//todo пока логика подразумевает, что в каждой ячейке создается новая линия с новыми символами
		//- дополнительное  выделение памяти. Возможно нужно просто делать move.
		std::vector<CTextLine*> m_arTextLine;
		//todo Подразумевается, что здесь хранятся параграфы. Добавить реализацию вывода картинок в ToXml.
		//Возможно подойдет вывод из CShape::BuildPictureProperties
		std::vector<CBaseItem*> m_arOutputObjects;

	public:
		CCell() = default;
		virtual ~CCell();
		virtual void Clear() override final;
		virtual void AddContent(CBaseItem* pItem) override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;

		void SetParameters(CPeak *pPeak, eCorners eCorner);
	};
}
