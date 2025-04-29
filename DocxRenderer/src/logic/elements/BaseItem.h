#pragma once

#include <memory>

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

	// using template to avoid downcasting
	template <typename T>
	class CBaseItemGroup : public CBaseItem
	{
	public:
		std::vector<std::shared_ptr<T>> m_arItems;

		CBaseItemGroup()
		{
			static_assert(std::is_base_of<CBaseItem, T>::value, "T should has base of CBaseItem!");
		}
		CBaseItemGroup(const CBaseItemGroup<T>& other) : CBaseItemGroup()
		{
			for (const auto value : other.m_arItems)
				m_arItems.push_back(value);
		}
		CBaseItemGroup(CBaseItemGroup<T>&& other) : CBaseItemGroup()
		{
			m_arItems = std::move(other);
		}
		virtual ~CBaseItemGroup() {}

		CBaseItemGroup<T>& operator=(const CBaseItemGroup<T>& other)
		{
			if (this == &other)
				return *this;

			m_arItems.clear();
			for (const auto value : other.m_arItems)
				m_arItems.push_back(value);

			return *this;
		}
		CBaseItemGroup<T>& operator=(CBaseItemGroup<T>&& other)
		{
			if (this == &other)
				return *this;

			m_arItems = std::move(other);
			return *this;
		}

		void AddItem(const std::shared_ptr<T>& pItem)
		{
			CBaseItem::RecalcWithNewItem(pItem.get());
			m_arItems.push_back(pItem);
		}
		void AddItem(std::shared_ptr<T>&& pItem)
		{
			CBaseItem::RecalcWithNewItem(pItem.get());
			m_arItems.push_back(std::move(pItem));
		}
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
