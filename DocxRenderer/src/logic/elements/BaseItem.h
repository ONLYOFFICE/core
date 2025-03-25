#pragma once

#include "../../../../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
	// взаимное расположение по вертикали со следующим объектом
	enum class eVerticalCrossingType
	{
		vctUnknown,
		vctCurrentInsideNext,
		vctCurrentOutsideNext,
		vctCurrentAboveNext,
		vctCurrentBelowNext,
		vctDublicate,
		vctTopAndBottomBordersMatch,
		vctTopBorderMatch,
		vctBottomBorderMatch,
		vctNoCrossingCurrentAboveNext,
		vctNoCrossingCurrentBelowNext
	};

	// взаимное расположение по горизонтали со следующим объектом
	enum class eHorizontalCrossingType
	{
		hctUnknown,
		hctCurrentInsideNext,
		hctCurrentOutsideNext,
		hctCurrentLeftOfNext,
		hctCurrentRightOfNext,
		hctDublicate,
		hctLeftAndRightBordersMatch,
		hctLeftBorderMatch,
		hctRightBorderMatch,
		hctNoCrossingCurrentLeftOfNext,
		hctNoCrossingCurrentRightOfNext
	};

	class CBaseItem
	{
	public:
		double m_dTop    {0.0};
		double m_dBot    {0.0};
		double m_dLeft   {0.0};
		double m_dRight  {0.0};
		double m_dHeight {0.0};
		double m_dWidth  {0.0};

		CBaseItem();
		CBaseItem(const CBaseItem& other);
		CBaseItem(CBaseItem&& other);
		virtual ~CBaseItem();

		CBaseItem& operator=(const CBaseItem& other);
		CBaseItem& operator=(CBaseItem&& other);

		virtual eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* pBaseItem) const;
		virtual eHorizontalCrossingType GetHorizontalCrossingType(const CBaseItem* pBaseItem) const;
		virtual void RecalcWithNewItem(const CBaseItem* pBaseItem);

		bool AreObjectsNoCrossingByVertically(const CBaseItem* pBaseItem) const noexcept;
		bool AreObjectsNoCrossingByHorizontally(const CBaseItem* pBaseItem) const noexcept;
		bool IsEqual(double dTop, double dBot, double dLeft, double dRight) const noexcept;

		bool operator==(const CBaseItem& other);
	};

	class IOoxmlItem
	{
	public:
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const = 0;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const = 0;
	};

	enum class eBaseItemCmpType
	{
		bictVertical,
		bictHorizontal
	};

	template <eBaseItemCmpType CmpType>
	struct CBaseItemCmp
	{
		bool operator() (const CBaseItem& item1, const CBaseItem& item2) const
		{
			if (CmpType == eBaseItemCmpType::bictVertical)
				return item1.m_dBot < item2.m_dBot;
			if (CmpType == eBaseItemCmpType::bictHorizontal)
				return item1.m_dLeft < item2.m_dLeft;
		}
	};
}
