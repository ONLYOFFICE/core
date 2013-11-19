#pragma once
#ifndef PPTX2OFFICEDRAWING_CONVERTER_INCLUDE_H_
#define PPTX2OFFICEDRAWING_CONVERTER_INCLUDE_H_

#include "stdafx.h"
#include "PPTX.h"

#include "Document.h"
#include "Structures.h"
#include <boost/shared_ptr.hpp>
#include "nullable_property.h"
#include "property.h"

/*
#include "OfficeDrawing\Shapes\BaseShape\PPTXShape\PPTXShape.h"
#include "Source/Structures.h"
*/
namespace PPTX2OfficeDrawing
{
	class Converter
	{
	public:
		Converter();
		~Converter();

	public:
		static DWORD Convert(const PPTX::Folder& pFolder, NSPresentationEditor::CDocument& pDocument);

	private:
		static void MasterConvert(const boost::shared_ptr<PPTX::SlideMaster> pMaster, NSPresentationEditor::CDocument& pDocument, int cx, int cy);
		static void LayoutConvert(const boost::shared_ptr<PPTX::SlideLayout> pSource, NSPresentationEditor::CLayout& pDest);
		static void SlideConvert(const boost::shared_ptr<PPTX::Slide> pSource, NSPresentationEditor::CSlide& pDest);
		static void GroupConvert(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements, bool bMergeWithLevelUp = false);
		static void BackgroundConvert(const nullable_property<PPTX::Logic::Bg> pBg, CAtlArray<IElement*>& pElements);
		static void AddNonPlaceholders(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements);

		static void ShapeConvert(const PPTX::Logic::Shape& pShape, CAtlArray<IElement*>& pElements, bool bMergeWithLevelUp = false);
		static void PicConvert(const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements);
		static void OleConvert(const PPTX::Logic::GraphicFrame& pOle, CAtlArray<IElement*>& pElements);
		static void VideoElementConvert(const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements);
		static void AudioElementConvert(const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements);
		static void ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, CAtlArray<IElement*>& pElements);

		static void TextParse(const PPTX::Logic::TxBody& pShapeText, CTextAttributesEx& pText/*, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction*/);

		static void ColorConvert(const PPTX::Logic::UniColor& Source, NSPresentationEditor::CColor& Dest, DWORD BGRA = 0);
		static void FillToBrush(const PPTX::Logic::UniFill& Fill, NSPresentationEditor::CBrush& Brush, DWORD BGRA = 0);
		static void LineToPen(const PPTX::Logic::Ln& Line, NSPresentationEditor::CPen& Pen, DWORD BGRA = 0);

		static void TextStyleConvert(const nullable_property<PPTX::Logic::TextListStyle>& pptxStyle, NSPresentationEditor::CTextStyle& pStyle);
		static void TextStyleConvert(const PPTX::Logic::TextListStyle& pptxStyle, NSPresentationEditor::CTextStyle& pStyle);
		static void TextStyleLevelConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::CTextStyleLevel& pStyleLevel);

		static void ParagraphPropertiesConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::STextPFRun& pStyleLevel);
		static void ParagraphPropertiesConvert(const nullable_property<PPTX::Logic::TextParagraphPr>& pptxStyle, NSPresentationEditor::STextPFRun& pStyleLevel);
		static void RunPropertiesConvert(const PPTX::Logic::RunProperties& pRunProps, NSPresentationEditor::STextCFRun& pCFRun);
		static void RunPropertiesConvert(const nullable_property<PPTX::Logic::RunProperties>& pRunProps, NSPresentationEditor::STextCFRun& pCFRun);

		static OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::string& str);
		static long GetPattFillValFromStr(const std::string& str);
		static long GetColorIndexFromStr(const std::string& str);
		static BYTE GetLineEndType(const std::string& str);
		static BYTE GetLineJoinType(const PPTX::Logic::eJoin join);
		static BYTE GetDashStyle(const std::string& str);
		static WORD GetTextAlignFromStr(const std::string& str);
		static BYTE GetTextStrike(const std::string& str);

/*
		static WORD GetTextAnchorFromStr(const std::string& str);
		static WORD GetFontAlignFromStr(const std::string& str);
		static DWORD GetTransitionSpeedFromStr(const std::string& str);
		static DWORD GetTLRestartFromStr(const std::string& str);
		static DWORD GetTLPresetClassFromStr(const std::string& str);
		static DWORD GetTLNodeTypeFromStr(const std::string& str);
		static DWORD GetTLCalcModeFromStr(const std::string& str);
		static DWORD GetTLValueTypeFromStr(const std::string& str);

		static void TableConvert(const PPTX::Logic::GraphicFrame pOle, CElementsContainer& pEContainer);
		static int TableCellConvert(const PPTX::Logic::TableCell& pTableCell, CElementsContainer& pEContainer, CShapeElement& lpShapeElement, const PPTX::Logic::TableStyle& Style, const TableCellData& CellData);
		static int TableCellHeight(const PPTX::Logic::TableCell& pTableCell, const TableCellData& CellData, const PPTX::Logic::ShapeProperties& TextProps);
		static int TableCellTextHeight(const PPTX::Logic::TableCell& pTableCell, const int WidthInEmu, const PPTX::Logic::ShapeProperties& TextProps);

		static int TransitionConvert(const DWORD SLdNum, const boost::shared_ptr<PPTX::Slide> Slide, CDrawingDocument& pDocument);
		static void TimingConvert(const DWORD SLdNum, const PPTX::Logic::Timing& pTiming, CDrawingDocument& pDocument);
		static void TimeNodeListConvert(const PPTX::Logic::TnLst& tnLst, Animations::ODExtTimeNodeContainer& tnContainer);
		static void TimeNodeBaseConvert(const PPTX::Logic::TimeNodeBase& timeNode, Animations::ODExtTimeNodeContainer& tnContainer);
		static void CTN2TimeNodeAtom(const PPTX::Logic::CTn& cTn, Animations::ODExtTimeNodeContainer& container);
		static void AddCondLst(const PPTX::Logic::CondLst condLst, Animations::ODExtTimeNodeContainer& container);
		static void CBhvr2TimeBehaviorContainer(const PPTX::Logic::CBhvr& bhvr, Animations::ODTimeBehaviorContainer& container);
		static int ParseTLTime(const std::string& str);
*/
	};
} // namespace PPTX2OfficeDrawing

#endif //PPTX2OFFICEDRAWING_CONVERTER_INCLUDE_H_