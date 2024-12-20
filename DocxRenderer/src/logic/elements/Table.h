#pragma once

#include <vector>
#include <memory>

#include "BaseItem.h"
#include "Paragraph.h"

#include "../../resources/LinesTable.h"

namespace NSDocxRenderer
{
    class CTable : public CBaseItem
	{
	public:
		class CCell;
		class CRow;

	private:
		using cell_ptr_t = std::shared_ptr<CCell>;
		using row_ptr_t = std::shared_ptr<CRow>;
		using paragraph_ptr_t = std::shared_ptr<CParagraph>;

	public:
		class CCell : public CBaseItem
		{
		public:
			struct CBorder
			{
				double dWidth{};
				double dSpacing{};
				long lColor{};
				eLineType eLineType{};
			};

			CCell() = default;
			virtual ~CCell() = default;
			virtual void Clear() override final;
			virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
			virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;

			void AddParagraph(const paragraph_ptr_t& pParagraph);

			CBorder m_oBorderTop;
			CBorder m_oBorderBot;
			CBorder m_oBorderLeft;
			CBorder m_oBorderRight;

		private:
			std::vector<paragraph_ptr_t> m_arParagraphs;
		};
		class CRow : public CBaseItem
		{
		public:
			CRow() = default;
			virtual ~CRow() = default;
			virtual void Clear() override final;
			virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
			virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;

			void AddCell(const cell_ptr_t& pCell);

		private:
			std::vector<cell_ptr_t> m_arCells;
		};

		CTable() = default;
		virtual ~CTable() = default;
		virtual void Clear() override final;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;

		void AddRow(const row_ptr_t& pRow);

	private:
		std::vector<row_ptr_t> m_arRows;
	};
} // namespace NSDocxRenderer



