#pragma once

#include "elements/Paragraph.h"
#include "elements/Shape.h"
#include "managers/ImageManager.h"
#include "managers/FontStyleManager.h"
#include "managers/ParagraphStyleManager.h"
#include "../../convert_params.h"

namespace NSDocxRenderer
{
	class CPage
	{
	public:
		struct CManagers
		{
			CManagers() = default;
			CManagers(const CManagers& other) = default;
			~CManagers() = default;

			CImageManager*          pImageManager;
			CFontStyleManager*      pFontStyleManager;
			CParagraphStyleManager* pParagraphStyleManager;
			CFontManager*           pFontManager;
			CFontSelector*			pFontSelector;
		};

		double m_dWidth {0.0};
		double m_dHeight{0.0};

		LONG m_lCurrentCommand{0};
		LONG m_lClipMode      {0};

		TextAssociationType         m_eTextAssociationType{TextAssociationType::tatPlainParagraph};
		NSFonts::IApplicationFonts* m_pAppFonts{nullptr};
		NSStructures::CFont         m_oFont{};
		NSStructures::CPen          m_oPen{};
		NSStructures::CBrush        m_oBrush{};
		NSStructures::CShadow       m_oShadow{};
		NSStructures::CEdgeText     m_oEdgeText{};
		Aggplus::CMatrix            m_oTransform{};

		bool m_bIsDeleteTextClipPage{true};
		bool m_bIsRecalcFontSize    {true};
		bool m_bIsGradient          {false};
		bool m_bUseDefaultFont      {false};
		bool m_bWriteStyleRaw       {false};

		CPage(NSFonts::IApplicationFonts* pAppFonts, const CManagers& oManagers);
		~CPage();

		void BeginCommand(DWORD lType);
		void EndCommand(DWORD lType);

		void Clear();

		void WriteImage(const std::shared_ptr<CImageInfo> pInfo, double& fX, double& fY, double& fWidth, double& fHeight);

		void PathMoveTo(double& dX, double& dY);
		void PathLineTo(double& dX, double& dY);
		void PathCurveTo(double& dX1, double& dY1, double& dX2, double& dY2, double& dX3, double& dY3);
		void PathStart();
		void PathEnd();
		void PathClose();
		void DrawPath(LONG lType, const std::shared_ptr<CImageInfo> pInfo);

		void AddText(
			const PUINT pUnicodes,
			const PUINT pGids,
			const UINT& nCount,
			const double& fX,
			const double& fY,
			const double& fWidth,
			const double& fHeight,
			const double& fBaseLineOffset);

		void Analyze();
		void Record(NSStringUtils::CStringBuilder& oWriter, bool bIsLastPage);

		std::vector<std::wstring> GetXmlShapes();
		std::vector<std::wstring> GetXmlShapesPptx();
		void AddCompleteXml(const std::wstring oXml);

	private:
		using shape_ptr_t = std::shared_ptr<CShape>;
		using cont_ptr_t = std::shared_ptr<CContText>;
		using line_ptr_t = std::shared_ptr<CTextLine>;
		using item_ptr_t = std::shared_ptr<CBaseItem>;
		using paragraph_ptr_t = std::shared_ptr<CParagraph>;

		// returns std::vector of conts with diac. symbols and remove it from m_arConts
		std::vector<cont_ptr_t> MoveDiacriticalSymbols();

		// returns std::vector of text lines builded from m_arConts
		std::vector<line_ptr_t> BuildTextLines();

		// returns std::vector of paragraphs builded from m_arTextLines
		std::vector<paragraph_ptr_t> BuildParagraphs();

		// returns std::vector of base items builded from m_arParagraphs
		std::vector<item_ptr_t> BuildOutputObjects();

		// analyze shapes (set lines type)
		void AnalyzeShapes();

		// analyze type of lines (double, wave, etc.)
		void AnalyzeLinesType();

		// analyze m_arTextLines and add effects, adds diac, super-sub scripts etc.
		void AnalyzeTextLines();

		// analyze drop caps (creates shapes)
		void AnalyzeDropCaps();

		// analyze conts in text lines
		void AnalyzeConts();

		// strikeouts, underlines, highlights, outline
		void AnalyzeEffects();

		// adds diacritical symbols in conts
		void AddDiacriticalSymbols();

		// super-sub scripts line merge
		void MergeLinesByVertAlignType();

		// remove out of bounds text lines
		void DeleteTextClipPage();

		// merging conts in text lines
		void MergeConts();

		// get horizontal and vertical lines from shapes
		void GetHorVerLines();

		// set dominant shapes
		void DetermineDominantGraphics();

		// split lines by graphics
		void SplitLines();

		// creates shapes from overlapping text lines
		void AnalyzeOverlapLines();

		// calc selected sizes of conts
		void CalcSelected();

		// merge shapes with each other
		void MergeShapes();

		// calc true shapes rotation for ooxml format
		void CalcShapesRotation();

		// for drawingml is no tag behind-doc - so we need to reorder shapes
		void ReorderShapesForPptx();

		// get lines by groups by X
		std::vector<std::vector<line_ptr_t>> GetLinesByGroups();

		bool IsLineCrossingText(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept;
		bool IsLineBelowText(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept;
		bool IsHighlight(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept;
		bool IsOutline(shape_ptr_t pShape, cont_ptr_t pCont) const noexcept;

		bool IsVerticalLineBetween(item_ptr_t pFirst, item_ptr_t pSecond) const noexcept;
		bool IsHorizontalLineBetween(item_ptr_t pFirst, item_ptr_t pSecond) const noexcept;
		bool IsVerticalLineTrough(item_ptr_t pFirst) const noexcept;
		bool IsHorizontalLineTrough(item_ptr_t pFirst) const noexcept;

		void ToXml(NSStringUtils::CStringBuilder& oWriter) const noexcept;
		void WriteSectionToFile(bool bLastPage, NSStringUtils::CStringBuilder& oWriter) const noexcept;

		static shape_ptr_t CreateSingleLineShape(line_ptr_t& pLine);
		static shape_ptr_t CreateSingleParagraphShape(paragraph_ptr_t& pParagraph);

		CManagers m_oManagers;

		CVectorGraphics m_oCurrVectorGraphics;
		CVectorGraphics m_oClipVectorGraphics;

		CContTextBuilder      m_oContBuilder;
		CHorVerLinesCollector m_oHorVerLinesCollector;

		std::vector<cont_ptr_t>      m_arConts;
		std::vector<line_ptr_t>      m_arTextLines;
		std::vector<cont_ptr_t>      m_arDiacriticalSymbols;
		std::vector<shape_ptr_t>     m_arShapes;
		std::vector<paragraph_ptr_t> m_arParagraphs;

		std::vector<item_ptr_t>   m_arOutputObjects;
		std::vector<std::wstring> m_arCompleteObjectsXml;

		size_t m_nShapeOrder = 0;
	};
}
