#pragma once
#ifndef PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_
#define PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_

#include "stdafx.h"
#include "PPTX.h"
#include "OfficeDrawing\Document.h"
#include "OfficeDrawing\Shapes\BaseShape\PPTXShape\PPTXShape.h"
#include "nullable_property.h"
#include "property.h"
#include <boost/shared_ptr.hpp>
#include "Source/Structures.h"

namespace PPTX2DrawingXML
{
	class Converter
	{
	public:
		Converter();
		~Converter();
	public:
		static DWORD Convert(const PPTX::Folder& pFolder, CStringW& xml);
	private:
		static OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::string& str);

		static WORD GetTextAlignFromStr(const std::string& str);
		static WORD GetTextAnchorFromStr(const std::string& str);
		static WORD GetFontAlignFromStr(const std::string& str);
		static WORD GetPattFillValFromStr(const std::string& str);
		static DWORD GetTransitionSpeedFromStr(const std::string& str);
		static DWORD GetTLRestartFromStr(const std::string& str);
		static DWORD GetTLPresetClassFromStr(const std::string& str);
		static DWORD GetTLNodeTypeFromStr(const std::string& str);
		static DWORD GetTLCalcModeFromStr(const std::string& str);
		static DWORD GetTLValueTypeFromStr(const std::string& str);
		static BYTE GetLineEndType(const std::string& str);
		static BYTE GetLineJoinType(const PPTX::Logic::eJoin join);
		static BYTE GetDashStyle(const std::string& str);
		static BYTE GetTextStrike(const std::string& str);

		static void LnToPen(const PPTX::Logic::Ln& Line, DWORD BGRA, NSAttributes::CPen_& Pen);
		static void FillToBrush(const PPTX::Logic::UniFill& Fill, DWORD BGRA, NSAttributes::CBrush_& Brush);

		static void SlideConvert(const boost::shared_ptr<PPTX::Slide> Slide, const DWORD SldNum, double* Duration, const int cx, const int cy, CDrawingDocument& pDocument);
		static void AddParentPics(const PPTX::Logic::SpTree& Group, CElementsContainer& pEContainer);
		static void GroupConvert(const PPTX::Logic::SpTree& Group, CElementsContainer& pEContainer);
		static void ShapeConvert(const PPTX::Logic::Shape& pShape, CElementsContainer& pEContainer);
		static void TextParse(const PPTX::Logic::TxBody& pShapeText, CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction);
		static void PicConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer);
		static void OleConvert(const PPTX::Logic::GraphicFrame& pOle, CElementsContainer& pEContainer);
		static void VideoElementConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer);
		static void AudioElementConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer);
		static void ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, CElementsContainer& pEContainer);
		static void TableConvert(const PPTX::Logic::GraphicFrame& pOle, CElementsContainer& pEContainer);
		static int TableCellConvert(const PPTX::Logic::TableCell& pTableCell, CElementsContainer& pEContainer, CShapeElement& lpShapeElement, const PPTX::Logic::TableStyle& Style, const TableCellData& CellData);
		static int TableCellHeight(const PPTX::Logic::TableCell& pTableCell, const TableCellData& CellData, const PPTX::Logic::ShapeProperties& TextProps);
		static int TableCellTextHeight(const PPTX::Logic::TableCell& pTableCell, const int WidthInEmu, const PPTX::Logic::ShapeProperties& TextProps);

		static int TransitionConvert(const DWORD SLdNum, const boost::shared_ptr<PPTX::Slide> Slide, CDrawingDocument& pDocument);
		static void TimingConvert(const DWORD SLdNum, const PPTX::Logic::Timing& pTiming, CDrawingDocument& pDocument);
		static void TimeNodeListConvert(const PPTX::Logic::TnLst& tnLst, Animations::ODExtTimeNodeContainer& tnContainer);
		static void TimeNodeBaseConvert(const PPTX::Logic::TimeNodeBase& timeNode, Animations::ODExtTimeNodeContainer& tnContainer);
		static void CTN2TimeNodeAtom(const PPTX::Logic::CTn& cTn, Animations::ODExtTimeNodeContainer& container);
		static void AddCondLst(const PPTX::Logic::CondLst& condLst, Animations::ODExtTimeNodeContainer& container);
		static void CBhvr2TimeBehaviorContainer(const PPTX::Logic::CBhvr& bhvr, Animations::ODTimeBehaviorContainer& container);
		static int ParseTLTime(const std::string& str);

		//PPTX::Folder m_Folder;
	};

} // namespace PPTX2DrawingXML

#endif //PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_