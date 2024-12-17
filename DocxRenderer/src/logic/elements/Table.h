#pragma once

#include "BaseItem.h"

namespace NSDocxRenderer
{
    class CTable : public CBaseItem
	{
	public:
		class CRow : CBaseItem
		{
		public:
			CRow() = default;
			virtual ~CRow() = default;
			virtual void Clear() override final;
			virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
			virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		};

		class CCell : CBaseItem
		{
		public:
			CCell() = default;
			virtual ~CCell() = default;
			virtual void Clear() override final;
			virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
			virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		};

		CTable() = default;
		virtual ~CTable() = default;
		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
	};
} // namespace NSDocxRenderer



