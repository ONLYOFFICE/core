#pragma once
#ifndef PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_
#define PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_

#include "../stdafx.h"
#include "../PPTXFormat/PPTX.h"

#include "../../AVSPresentationEditor/OfficeDrawing/Document.h"
#include "Source/Structures.h"

namespace PPTX2EditorSimple
{
	const double c_dScaleElements = 1.0 / 36000;

	class Converter
	{
	public:
		Converter();
		~Converter();
	public:
		static DWORD Convert(PPTX::Folder& pFolder, NSPresentationEditor::CDocument& oDocument);
		static DWORD ConvertToVS(PPTX::Folder& pFolder, CString& strXml);
		static DWORD ConvertToEditor(PPTX::Folder& pFolder, CString& strXml);
	private:
		static OOXMLShapes::ShapeType GetShapeTypeFromStr(const CString& str);

		static WORD GetTextAlignFromStr(const CString& str);
		static WORD GetTextAnchorFromStr(const CString& str);
		static WORD GetFontAlignFromStr(const CString& str);
		static WORD GetPattFillValFromStr(const CString& str);
		static DWORD GetTransitionSpeedFromStr(const CString& str);
		static DWORD GetTLRestartFromStr(const CString& str);
		static DWORD GetTLPresetClassFromStr(const CString& str);
		static DWORD GetTLNodeTypeFromStr(const CString& str);
		static DWORD GetTLCalcModeFromStr(const CString& str);
		static DWORD GetTLValueTypeFromStr(const CString& str);
		static BYTE GetLineEndType(const CString& str);
		static BYTE GetLineJoinType(const PPTX::Logic::eJoin join);
		static BYTE GetDashStyle(const CString& str);
		static BYTE GetTextStrike(const CString& str);

		static void LnToPen(const PPTX::Logic::Ln& Line, DWORD BGRA, NSPresentationEditor::CPen& Pen);
		static void FillToBrush(const PPTX::Logic::UniFill& Fill, DWORD BGRA, NSPresentationEditor::CBrush& Brush);

		static void SlideConvert(const smart_ptr<PPTX::Slide> Slide, const DWORD SldNum, double* Duration, const int cx, const int cy, NSPresentationEditor::CDocument& oDocument);
		static void AddParentPics(const PPTX::Logic::SpTree& Group, NSPresentationEditor::CSlide& oEContainer);
		static void GroupConvert(const PPTX::Logic::SpTree& Group, NSPresentationEditor::CSlide& oEContainer);
		static void ShapeConvert(const PPTX::Logic::Shape& pShape, NSPresentationEditor::CSlide& oEContainer);
		static void TextParse(const PPTX::Logic::TxBody& pShapeText, NSPresentationEditor::CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction);
		static void PicConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer);
		static void OleConvert(const PPTX::Logic::GraphicFrame& pOle, NSPresentationEditor::CSlide& pEContainer);
		static void VideoElementConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer);
		static void AudioElementConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer);
		static void ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, NSPresentationEditor::CSlide& pEContainer);
		static void TableConvert(const PPTX::Logic::GraphicFrame& pOle, NSPresentationEditor::CSlide& pEContainer);
		static int TableCellConvert(const PPTX::Logic::TableCell& pTableCell, NSPresentationEditor::CSlide& pEContainer, NSPresentationEditor::CShapeElement& lpShapeElement, const PPTX::Logic::TableStyle& Style, const TableCellData& CellData);
		static int TableCellHeight(const PPTX::Logic::TableCell& pTableCell, const TableCellData& CellData, const PPTX::Logic::ShapeProperties& TextProps);
		static int TableCellTextHeight(const PPTX::Logic::TableCell& pTableCell, const int WidthInEmu, const PPTX::Logic::ShapeProperties& TextProps);

		static int TransitionConvert(const DWORD SLdNum, const smart_ptr<PPTX::Slide> Slide, NSPresentationEditor::CDocument& pDocument);
		static void TimingConvert(const DWORD SLdNum, const PPTX::Logic::Timing& pTiming, NSPresentationEditor::CDocument& pDocument);
		static void TimeNodeListConvert(const PPTX::Logic::TnLst& tnLst, NSPresentationEditor::CAnimationInfo& tnContainer);
		static void TimeNodeBaseConvert(const PPTX::Logic::TimeNodeBase& timeNode, NSPresentationEditor::CAnimationInfo& tnContainer);
		static void CTN2TimeNodeAtom(const PPTX::Logic::CTn& cTn, NSPresentationEditor::CAnimationInfo& container);
		static void AddCondLst(const PPTX::Logic::CondLst& condLst, NSPresentationEditor::CAnimationInfo& container);
		static void CBhvr2TimeBehaviorContainer(const PPTX::Logic::CBhvr& bhvr, NSPresentationEditor::CAnimationInfo& container);
		static int ParseTLTime(const CString& str);

		//PPTX::Folder m_Folder;
	};

} // namespace PPTX2DrawingXML

#endif //PPTX2DRAWINGXML_CONVERTER_INCLUDE_H_