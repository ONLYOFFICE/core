#pragma once
#ifndef ODP2DRAWINGXML_CONVERTER_INCLUDE_H_
#define ODP2DRAWINGXML_CONVERTER_INCLUDE_H_

#include "stdafx.h"
#include "Odp.h"
#include "../../ASCPresentationEditor/OfficeDrawing/Document.h"
#include "nullable_property.h"
#include "property.h"
#include <vector>
using namespace NSPresentationEditor;

namespace Odp2DrawingXML
{
	class Converter
	{
	public:
		Converter();
		~Converter();
	public:
		DWORD Convert(const Odp::Folder& pFolder, CDocument& lDocument);
	private:
		Odp::Folder const* m_Folder;
		//static OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::string& str);

		static WORD GetTextAlignFromStr(const std::string& str);
		static WORD GetTextAnchorFromStr(const std::string& str);
		//static WORD GetFontAlignFromStr(const std::string& str);
		//static WORD GetPattFillValFromStr(const std::string& str);
		//static BYTE GetTransitionSpeedFromStr(const std::string& str);
		//static DWORD GetTLRestartFromStr(const std::string& str);
		//static DWORD GetTLPresetClassFromStr(const std::string& str);
		//static DWORD GetTLNodeTypeFromStr(const std::string& str);
		//static DWORD GetTLCalcModeFromStr(const std::string& str);
		//static DWORD GetTLValueTypeFromStr(const std::string& str);

		void FillPenBrush(const Odt::Logic::Properties& pProps, CShapeElement& pShape)const;
		void FillBackground(const Odt::Logic::Properties& pProps, CBrush& pBrush)const;
		//static void LnToPen(const nullable_property<OOX::Logic::Ln>& Line, NSAttributes::CPen_& Pen);
		//static void LnToPen(const property<OOX::Logic::Ln>& Line, NSAttributes::CPen_& Pen);
		//static void LnToPen(const OOX::Logic::Ln& Line, DWORD BGRA, NSAttributes::CPen_& Pen);
		//static void FillToBrush(const OOX::Logic::UniFill& Fill, NSAttributes::CBrush_& Brush);
		//static void FillToBrush(const OOX::Logic::UniFill& Fill, DWORD BGRA, NSAttributes::CBrush_& Brush);

		void SlideConvert(const DWORD SldNum, double* Duration, CDocument& pDocument)const;
		void AddBackground(const std::string& MasterName, CSlide& pEContainer)const;
		void DrawingElementConvert(const Odp::Content::DrawingElement& pDrawElem, CSlide& pEContainer, bool master)const;
		void GroupConvert(const Odp::Content::Group& pGroup, CSlide& pEContainer, bool master)const;
		void FrameConvert(const Odp::Content::Frame& pFrame, CSlide& pEContainer, bool master)const;
		void LineConvert(const Odp::Content::Line& pLine, CSlide& pEContainer, bool master)const;
		void RectConvert(const Odp::Content::Rect& pRect, CSlide& pEContainer, bool master)const;
		void CustomShapeConvert(const Odp::Content::CustomShape& pCustShape, CSlide& pEContainer, bool master)const;
		void TextVectorConvert(const std::vector<Odt::Content::Text>& pTextBox, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, bool master)const;
		void TextConvert(const Odt::Content::Text& pTextItem, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, const Odt::Logic::ListStyle listStyle, const int level, bool master)const;
		void ParagraphConvert(const Odt::Content::Paragraph& pParagraph, CTextAttributesEx& pText, const Odt::Logic::Properties& ParentProps, const Odt::Logic::ListStyle listStyle, const int level, bool master)const;
		void TableConvert(const Odt::Content::Table& pTable, CSlide& pEContainer, const Odt::Logic::Properties& ParentProps, long x, long y, bool master)const;
		void TableCellConvert(const Odt::Content::TableCell& pTableCell, CSlide& pEContainer, const Odt::Logic::Properties& ParentProps, long x, long y, long w, long h, bool master)const;
		//static void AddParentPics(OOX::Logic::SpTree& Group, CElementsContainer& pEContainer);
		//static void GroupConvert(OOX::Logic::SpTree& Group, CElementsContainer& pEContainer);
		//static void ShapeConvert(OOX::Logic::Shape& pShape, CElementsContainer& pEContainer);
		//static void TextParse(const OOX::Logic::TxBody& pShapeText, CTextAttributesEx& pText, const OOX::Logic::ShapeProperties& TextProps, double AutoFit);
		//static void PicConvert(const OOX::Logic::Pic& pPic, CElementsContainer& pEContainer);
		//static void VideoElementConvert(const OOX::Logic::Pic& pPic, CElementsContainer& pEContainer);
		//static void ConnectorConvert(const OOX::Logic::CxnSp& pCxn, CElementsContainer& pEContainer);

		void TransitionConvert(const DWORD SldNum, CDocument& pDocument)const;
		void TransitionParse(const Odp::Content::TransitionFilter& pTransition, const DWORD SldNum, const std::string& SlideId, CDocument& pDocument)const;
		//static void TimingConvert(const DWORD SLdNum, const OOX::Logic::Timing& pTiming, CDrawingDocument& pDocument);
		//static void TimeNodeListConvert(const OOX::Logic::TnLst& tnLst, Animations::ODExtTimeNodeContainer& tnContainer);
		//static void TimeNodeBaseConvert(const OOX::Logic::TimeNodeBase& timeNode, Animations::ODExtTimeNodeContainer& tnContainer);
		//static void CTN2TimeNodeAtom(const OOX::Logic::CTn& cTn, Animations::ODExtTimeNodeContainer& container);
		//static void AddCondLst(const OOX::Logic::CondLst condLst, Animations::ODExtTimeNodeContainer& container);
		//static void CBhvr2TimeBehaviorContainer(const OOX::Logic::CBhvr& bhvr, Animations::ODTimeBehaviorContainer& container);
		//static int ParseTLTime(const std::string& str);
	};

} // namespace Odp2DrawingXML

#endif //ODP2DRAWINGXML_CONVERTER_INCLUDE_H_