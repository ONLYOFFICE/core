#include "HWPSection.h"

#include "HWPElements/HWPTag.h"

#include "HwpDoc/Paragraph/CtrlEqEdit.h"
#include "Paragraph/CtrlTable.h"
#include "Paragraph/CtrlSectionDef.h"
#include "Paragraph/CtrlCommon.h"
#include "Paragraph/CtrlCharacter.h"
#include "Paragraph/CharShape.h"
#include "Paragraph/CtrlHeadFoot.h"
#include "Paragraph/CtrlTable.h"
#include "Paragraph/CtrlNote.h"
#include "Paragraph/CtrlForm.h"

#include "Paragraph/CtrlGeneralShape.h"
#include "Paragraph/CtrlShapeRect.h"
#include "Paragraph/CtrlShapePolygon.h"
#include "Paragraph/CtrlShapeEllipse.h"
#include "Paragraph/CtrlShapePic.h"
#include "Paragraph/CtrlShapeLine.h"
#include "Paragraph/CtrlShapeArc.h"
#include "Paragraph/CtrlShapeCurve.h"
#include "Paragraph/CtrlShapeOle.h"
#include "Paragraph/CtrlShapeTextArt.h"
#include "Paragraph/CtrlShapeVideo.h"

#include "Paragraph/CellParagraph.h"
#include "Paragraph/CapParagraph.h"

#include "HWPElements/HWPRecordParaText.h"
#include "HWPElements/HWPRecordParaRangeTag.h"
#include "HWPElements/HWPRecordCtrlHeader.h"
#include "HWPElements/HWPRecordListHeader.h"
#include "HWPElements/HWPRecordFormObject.h"
#include "HWPElements/HWPRecordCtrlData.h"

#ifdef _DEBUG
#include <iostream>
#define PRINT_HWPTAG(prefix, tag, postfix) \
	do \
	{ \
	std::cout << prefix; \
	switch (eTag) \
	{ \
		case HWPTAG_DOCUMENT_PROPERTIES: std::cout << "HWPTAG_DOCUMENT_PROPERTIES"; break; \
		case HWPTAG_ID_MAPPINGS: std::cout << "HWPTAG_ID_MAPPINGS"; break; \
		case HWPTAG_BIN_DATA: std::cout << "HWPTAG_BIN_DATA"; break; \
		case HWPTAG_FACE_NAME: std::cout << "HWPTAG_FACE_NAME"; break; \
		case HWPTAG_BORDER_FILL: std::cout << "HWPTAG_BORDER_FILL"; break; \
		case HWPTAG_HWP_CHAR_SHAPE: std::cout << "HWPTAG_HWP_CHAR_SHAPE"; break; \
		case HWPTAG_TAB_DEF: std::cout << "HWPTAG_TAB_DEF"; break; \
		case HWPTAG_NUMBERING: std::cout << "HWPTAG_NUMBERING"; break; \
		case HWPTAG_BULLET: std::cout << "HWPTAG_BULLET"; break; \
		case HWPTAG_PARA_SHAPE: std::cout << "HWPTAG_PARA_SHAPE"; break; \
		case HWPTAG_STYLE: std::cout << "HWPTAG_STYLE"; break; \
		case HWPTAG_DOC_DATA: std::cout << "HWPTAG_DOC_DATA"; break; \
		case HWPTAG_DISTRIBUTE_DOC_DATA: std::cout << "HWPTAG_DISTRIBUTE_DOC_DATA"; break; \
		case HWPTAG_COMPATIBLE_DOCUMENT: std::cout << "HWPTAG_COMPATIBLE_DOCUMENT"; break; \
		case HWPTAG_LAYOUT_COMPATIBILITY: std::cout << "HWPTAG_LAYOUT_COMPATIBILITY"; break; \
		case HWPTAG_TRACKCHANGE: std::cout << "HWPTAG_TRACKCHANGE"; break; \
		case HWPTAG_PARA_HEADER: std::cout << "HWPTAG_PARA_HEADER"; break; \
		case HWPTAG_PARA_TEXT: std::cout << "HWPTAG_PARA_TEXT"; break; \
		case HWPTAG_PARA_HWP_CHAR_SHAPE: std::cout << "HWPTAG_PARA_HWP_CHAR_SHAPE"; break; \
		case HWPTAG_PARA_LINE_SEG: std::cout << "HWPTAG_PARA_LINE_SEG"; break; \
		case HWPTAG_PARA_RANGE_TAG: std::cout << "HWPTAG_PARA_RANGE_TAG"; break; \
		case HWPTAG_CTRL_HEADER: std::cout << "HWPTAG_CTRL_HEADER"; break; \
		case HWPTAG_LIST_HEADER: std::cout << "HWPTAG_LIST_HEADER"; break; \
		case HWPTAG_PAGE_DEF: std::cout << "HWPTAG_PAGE_DEF"; break; \
		case HWPTAG_FOOTNOTE_SHAPE: std::cout << "HWPTAG_FOOTNOTE_SHAPE"; break; \
		case HWPTAG_PAGE_BORDER_FILL: std::cout << "HWPTAG_PAGE_BORDER_FILL"; break; \
		case HWPTAG_SHAPE_COMPONENT: std::cout << "HWPTAG_SHAPE_COMPONENT"; break; \
		case HWPTAG_TABLE: std::cout << "HWPTAG_TABLE"; break; \
		case HWPTAG_SHAPE_COMPONENT_LINE: std::cout << "HWPTAG_SHAPE_COMPONENT_LINE"; break; \
		case HWPTAG_SHAPE_COMPONENT_RECTANGLE: std::cout << "HWPTAG_SHAPE_COMPONENT_RECTANGLE"; break; \
		case HWPTAG_SHAPE_COMPONENT_ELLIPSE: std::cout << "HWPTAG_SHAPE_COMPONENT_ELLIPSE"; break; \
		case HWPTAG_SHAPE_COMPONENT_ARC: std::cout << "HWPTAG_SHAPE_COMPONENT_ARC"; break; \
		case HWPTAG_SHAPE_COMPONENT_POLYGON: std::cout << "HWPTAG_SHAPE_COMPONENT_POLYGON"; break; \
		case HWPTAG_SHAPE_COMPONENT_CURVE: std::cout << "HWPTAG_SHAPE_COMPONENT_CURVE"; break; \
		case HWPTAG_SHAPE_COMPONENT_OLE: std::cout << "HWPTAG_SHAPE_COMPONENT_OLE"; break; \
		case HWPTAG_SHAPE_COMPONENT_PICTURE: std::cout << "HWPTAG_SHAPE_COMPONENT_PICTURE"; break; \
		case HWPTAG_SHAPE_COMPONENT_CONTAINER: std::cout << "HWPTAG_SHAPE_COMPONENT_CONTAINER"; break; \
		case HWPTAG_CTRL_DATA: std::cout << "HWPTAG_CTRL_DATA"; break; \
		case HWPTAG_EQEDIT: std::cout << "HWPTAG_EQEDIT"; break; \
		case HWPTAG_SHAPE_COMPONENT_TEXTART: std::cout << "HWPTAG_SHAPE_COMPONENT_TEXTART"; break; \
		case HWPTAG_FORM_OBJECT: std::cout << "HWPTAG_FORM_OBJECT"; break; \
		case HWPTAG_MEMO_SHAPE: std::cout << "HWPTAG_MEMO_SHAPE"; break; \
		case HWPTAG_MEMO_LIST: std::cout << "HWPTAG_MEMO_LIST"; break; \
		case HWPTAG_FORBIDDEN_HWP_CHAR: std::cout << "HWPTAG_FORBIDDEN_HWP_CHAR"; break; \
		case HWPTAG_HWP_CHART_DATA: std::cout << "HWPTAG_HWP_CHART_DATA"; break; \
		case HWPTAG_TRACK_CHANGE: std::cout << "HWPTAG_TRACK_CHANGE"; break; \
		case HWPTAG_TRACK_CHANGE_AUTHOR: std::cout << "HWPTAG_TRACK_CHANGE_AUTHOR"; break; \
		case HWPTAG_VIDEO_DATA: std::cout << "HWPTAG_VIDEO_DATA"; break; \
		case HWPTAG_SHAPE_COMPONENT_UNKNOWN:  std::cout << "HWPTAG_SHAPE_COMPONENT_UNKNOWN"; break; \
		default: std::cout << "UNKNOWN"; break; \
	} \
	std::cout << postfix << std::endl; \
	} \
	while (false)
#else
#define PRINT_HWPTAG(prefix, tag, postfix) do {} while(false)
#endif

namespace HWP
{
CHWPSection::CHWPSection()
{}

CHWPSection::~CHWPSection()
{
	CLEAR_ARRAY(CHWPPargraph, m_arParas);
}

bool CHWPSection::Parse(CHWPStream& oBuffer, int nVersion)
{
	oBuffer.MoveToStart();

	int nHeader, nTagNum, nLevel, nSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		oBuffer.Skip(-4);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (nLevel > 0)
		{
			if (m_arParas.empty())
				return false;

			ParseRecurse(m_arParas.back(), nLevel, oBuffer, 0, nVersion);
			continue;
		}

		if (0xFFF == nSize)
		{
			//TODO:: buf[off+7]<<24&0xFF000000 | buf[off+6]<<16&0xFF0000 | buf[off+5]<<8&0xFF00 | buf[off+4]&0xFF;
			oBuffer.Skip(4);
			oBuffer.ReadInt(nSize);
		}
		else
			oBuffer.Skip(4);

		EHWPTag eTag = GetTagFromNum(nTagNum);

		PRINT_HWPTAG("Main parse: ", eTag, " : size=" << std::to_string(nSize) << " level=" << std::to_string(nLevel));

 		if (0 == nLevel && HWPTAG_PARA_HEADER == eTag)
		{
			CHWPPargraph *pCurrPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

			if (nullptr != pCurrPara)
				m_arParas.push_back(pCurrPara);

			oBuffer.Skip(nSize);
		}
	}

	return true;
}

#define FIND_LAST_ELEMENT(type, ptr, container_type, container) \
	for (VECTOR<container_type*>::const_reverse_iterator itCtrl = container.crbegin(); itCtrl != container.crend(); ++itCtrl) \
	{ \
		if (nullptr != dynamic_cast<type*>(*itCtrl)) \
			ptr = (type*)(*itCtrl); \
	}


int CHWPSection::ParseRecurse(CHWPPargraph* pCurrPara, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	int nHeader, nTagNum, nLevel, nSize, nHeaderSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		oBuffer.Skip(-4);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (0xFFF == nSize)
		{
			nHeaderSize = 8;
			//TODO:: buf[off+7]<<24&0xFF000000 | buf[off+6]<<16&0xFF0000 | buf[off+5]<<8&0xFF00 | buf[off+4]&0xFF;
			oBuffer.Skip(4);
			oBuffer.ReadInt(nSize);
			oBuffer.Skip(-8);
		}
		else
		{
			nHeaderSize = 4;
		}

		if (nLevel < nRunLevel)
			break;

		EHWPTag eTag = GetTagFromNum(nTagNum);

		PRINT_HWPTAG("Recurse parse: ", eTag, " : size=" << std::to_string(nSize) << " level=" << std::to_string(nLevel));

		if (nLevel > nRunLevel)
		{
			switch(eTag)
			{
				case HWPTAG_PARA_HEADER:
				case HWPTAG_PARA_TEXT:
				case HWPTAG_PARA_HWP_CHAR_SHAPE:
				case HWPTAG_PARA_LINE_SEG:
				case HWPTAG_PARA_RANGE_TAG:
				case HWPTAG_CTRL_HEADER:
				{
					ParseRecurse(pCurrPara, nLevel, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_TABLE:
				{
					CCtrlTable *pTable = nullptr;

					const VECTOR<CCtrl*> arCtrls = pCurrPara->GetCtrls();

					FIND_LAST_ELEMENT(CCtrlTable, pTable, CCtrl, arCtrls);

					if (nullptr != pTable)
						ParseCtrlRecurse(pTable, nLevel, oBuffer, 0, nVersion);

					break;
				}
				case HWPTAG_LIST_HEADER:
				{
					oBuffer.Skip(nHeaderSize);
					oBuffer.Skip(nSize);
					break;
				}
				case HWPTAG_PAGE_DEF:
				case HWPTAG_FOOTNOTE_SHAPE:
				case HWPTAG_PAGE_BORDER_FILL:
				{
					CCtrlSectionDef *pCtrlSecDef = dynamic_cast<CCtrlSectionDef*>(pCurrPara->FindLastElement(L"dces"));

					if (nullptr != pCtrlSecDef)
						ParseCtrlRecurse(pCtrlSecDef, nLevel, oBuffer, 0, nVersion);

					break;
				}
				case HWPTAG_SHAPE_COMPONENT:
				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				case HWPTAG_SHAPE_COMPONENT_LINE:
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				case HWPTAG_SHAPE_COMPONENT_ARC:
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				case HWPTAG_SHAPE_COMPONENT_OLE:
				case HWPTAG_EQEDIT:
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				{
					CCtrlGeneralShape *pCtrlGeneral = dynamic_cast<CCtrlGeneralShape*>(pCurrPara->FindLastElement(L" osg"));

					if (nullptr != pCtrlGeneral)
						ParseCtrlRecurse(pCtrlGeneral, nLevel, oBuffer, 0, nVersion);

					break;
				}
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_HWP_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				default:
				{
					CCtrlCommon *pCtrlCommon = nullptr;

					const VECTOR<CCtrl*> arCtrls = pCurrPara->GetCtrls();

					FIND_LAST_ELEMENT(CCtrlCommon, pCtrlCommon, CCtrl, arCtrls);

					if (nullptr != pCtrlCommon)
						ParseCtrlRecurse(pCtrlCommon, nLevel, oBuffer, 0, nVersion);
					else
						oBuffer.Skip(nHeaderSize);

					break;
				}
			}
		}
		else if (nLevel == nRunLevel)
		{
			oBuffer.Skip(nHeaderSize);

			switch (eTag)
			{
				case HWPTAG_PARA_HEADER:
				{
					if (nullptr != dynamic_cast<CCellParagraph*>(pCurrPara))
					{
						oBuffer.Skip(-nHeaderSize);
						return oBuffer.GetDistanceToLastPos(true);
					}
					else if (nullptr != dynamic_cast<CCapParagraph*>(pCurrPara))
					{
						CHWPPargraph::Parse(*pCurrPara, nSize, oBuffer, 0, nVersion);
						break;
					}
					else
					{
						if (1 == nRunLevel) //TODO:: проверить
						{
							oBuffer.Skip(nSize);
							return oBuffer.GetDistanceToLastPos(true);
						}

						oBuffer.Skip(-nHeaderSize);
						return oBuffer.GetDistanceToLastPos(true);
					}
				}
				case HWPTAG_PARA_TEXT:
				{
					pCurrPara->AddCtrls(CHWPRecordParaText::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion));
					break;
				}
				case HWPTAG_PARA_HWP_CHAR_SHAPE:
				{
					if (0 == pCurrPara->GetCountCtrls())
						pCurrPara->AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));

					CCharShape::FillCharShape(nTagNum, nLevel, nSize, oBuffer, 0, nVersion, pCurrPara->GetCtrls());
					break;
				}
				case HWPTAG_PARA_LINE_SEG:
				{
					pCurrPara->SetLineSeg(new CLineSeg(nTagNum, nLevel, nSize, oBuffer, 0, nVersion));
					break;
				}
				case HWPTAG_PARA_RANGE_TAG:
				{
					CHWPRecordParaRangeTag::Parse(*pCurrPara, nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_CTRL_HEADER:
				{
					oBuffer.SavePosition();

					CCtrl *pCtrl = CHWPRecordCtrlHeader::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

					if (nullptr == pCtrl)
					{
						oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
						break;
					}

					if (nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
						((CCtrlGeneralShape*)pCtrl)->SetParent(pCurrPara);

					unsigned int nIndex;
					CCtrl* pCtrlOrigOp = pCurrPara->FindFirstElement(pCtrl->GetID(), false, nIndex);

					if (nullptr != pCtrlOrigOp)
						pCurrPara->SetCtrl(pCtrl, nIndex);

					if (nullptr != dynamic_cast<CCtrlHeadFoot*>(pCtrl))
					{
						CCtrlSectionDef* pSecD = dynamic_cast<CCtrlSectionDef*>(pCurrPara->FindLastElement(L"dces"));

						if (nullptr != pSecD)
							pSecD->AddHeadFoot((CCtrlHeadFoot*)pCtrl);
					}

					oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());
					ParseCtrlRecurse(pCtrl, nLevel, oBuffer, 0, nVersion);

					oBuffer.RemoveLastSavedPos();
					break;
				}
				case HWPTAG_TABLE:
				{
					oBuffer.Skip(-nHeaderSize);
					return oBuffer.GetDistanceToLastPos(true);
				}
				case HWPTAG_LIST_HEADER:
				{
					if (1 == nRunLevel)
					{
						oBuffer.Skip(nSize);
						return oBuffer.GetDistanceToLastPos(true);
					}
					oBuffer.Skip(-nHeaderSize);
					return oBuffer.GetDistanceToLastPos(true);
				}
				case HWPTAG_SHAPE_COMPONENT:
				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				case HWPTAG_SHAPE_COMPONENT_LINE:
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				case HWPTAG_SHAPE_COMPONENT_ARC:
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				case HWPTAG_SHAPE_COMPONENT_OLE:
				case HWPTAG_EQEDIT:
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				{
					oBuffer.Skip(-nHeaderSize);
					return oBuffer.GetDistanceToLastPos(true);
				}
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_HWP_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				{
					oBuffer.Skip(nSize);
					break;
				}
				default: {}
			}
		}
	}

	return oBuffer.GetDistanceToLastPos(true);
}

int CHWPSection::ParseCtrlRecurse(CCtrl* pCurrCtrl, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CCtrl* pCtrl = pCurrCtrl;

	oBuffer.SavePosition();

	int nHeader, nTagNum, nLevel, nSize, nHeaderSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		oBuffer.Skip(-4);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (0xFFF == nSize)
		{
			nHeaderSize = 8;
			//TODO:: buf[off+7]<<24&0xFF000000 | buf[off+6]<<16&0xFF0000 | buf[off+5]<<8&0xFF00 | buf[off+4]&0xFF;
			oBuffer.Skip(4);
			oBuffer.ReadInt(nSize);
			oBuffer.Skip(-8);
		}
		else
		{
			nHeaderSize = 4;
		}

		if (nLevel < nRunLevel)
			break;

		EHWPTag eTag = GetTagFromNum(nTagNum);

		PRINT_HWPTAG("Recurse ctrl parse: ", eTag, " : size=" << std::to_string(nSize) << " level=" << std::to_string(nLevel));

		if (nLevel > nRunLevel)
		{
			switch(eTag)
			{
				case HWPTAG_PARA_HEADER:
				{
					if (nullptr != dynamic_cast<CCtrlCommon*>(pCtrl))
						ParseCtrlRecurse((CCtrlCommon*)pCtrl, nLevel, oBuffer, 0, nVersion);
					else
					{
						oBuffer.Skip(nHeaderSize);
						oBuffer.Skip(nSize);
					}
					break;
				}
				case HWPTAG_PARA_TEXT:
				{
					if (nullptr != dynamic_cast<CCtrlCommon*>(pCtrl))
					{
						CHWPPargraph* pLastPara = ((CCtrlCommon*)pCtrl)->GetLastPara();

						if (nullptr == pLastPara)
						{
							oBuffer.Skip(nHeaderSize);
							oBuffer.Skip(nSize);
							break;
						}

						ParseRecurse(pLastPara, nLevel, oBuffer, 0, nVersion);
					}
					else
					{
						oBuffer.Skip(nHeaderSize);
						oBuffer.Skip(nSize);
					}
					break;
				}
				case HWPTAG_LIST_HEADER:
				{
					oBuffer.Skip(nHeaderSize);

					int nSubParaCount = CHWPRecordListHeader::GetCount(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

					if (nullptr != dynamic_cast<CCtrlTable*>(pCtrl))
					{
						ParseListAppend(*(CCtrlCommon*)pCtrl, nSize - 6, oBuffer, 0, nVersion);
						CCtrlTable *pCtrlTable = (CCtrlTable*)pCtrl;
						if (pCtrlTable->HaveCells())
						{
							//TODO:: проверить
							break;
						}

						CCapParagraph* pNewPara = new CCapParagraph();
						pCtrlTable->AddCaption(pNewPara);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlShapeRect*>(pCtrl) ||
					         nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
					{
						CCtrlCommon* pCtrlCommon = (CCtrlCommon*)pCtrl;
						oBuffer.Skip(-6);

						pCtrlCommon->SetTextVerAlign(CHWPRecordListHeader::GetVertAlign(6, oBuffer, 0, nVersion));

						ParseListAppend(*pCtrlCommon, nSize - 6, oBuffer, 0, nVersion);
						ParseCtrlRecurse(pCtrlCommon, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlHeadFoot*>(pCtrl) ||
					         nullptr != dynamic_cast<CCtrlNote*>(pCtrl))
					{
						ParseListAppend(*pCtrl, nSize - 6, oBuffer, 0, nVersion);
						ParseCtrlRecurse(pCtrl, nLevel, oBuffer, 0, nVersion);
					}
					else
						oBuffer.Skip(nSize - 6);

					break;
				}
				case HWPTAG_PAGE_DEF:
				case HWPTAG_FOOTNOTE_SHAPE:
				case HWPTAG_PAGE_BORDER_FILL:
				{
					if (nullptr != dynamic_cast<CCtrlSectionDef*>(pCtrl))
						ParseCtrlRecurse((CCtrlSectionDef*)pCtrl, nLevel, oBuffer, 0, nVersion);

					break;
				}
				case HWPTAG_SHAPE_COMPONENT:
				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				case HWPTAG_SHAPE_COMPONENT_LINE:
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				case HWPTAG_SHAPE_COMPONENT_ARC:
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				case HWPTAG_SHAPE_COMPONENT_OLE:
				case HWPTAG_EQEDIT:
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				{
					if (nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
						ParseCtrlRecurse((CCtrlGeneralShape*)pCtrl, nLevel, oBuffer, 0, nVersion);
					else
						return oBuffer.GetDistanceToLastPos(true);

					break;
				}
				case HWPTAG_TABLE:
				{
					ParseCtrlRecurse(pCtrl, nLevel, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_CTRL_HEADER:
				{
					return oBuffer.GetDistanceToLastPos(true);
				}
				case HWPTAG_PARA_RANGE_TAG:
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_HWP_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				default:
				{
					ParseCtrlRecurse(pCtrl, nLevel, oBuffer, 0, nVersion);
					break;
				}
			}
		}
		else if (nLevel == nRunLevel)
		{
			oBuffer.Skip(nHeaderSize);

			switch (eTag)
			{
				case HWPTAG_PARA_HEADER:
				{
					if (nullptr != dynamic_cast<CCtrlTable*>(pCtrl))
					{
						CCtrlTable *pCtrltable = (CCtrlTable*)pCtrl;
						if (!pCtrltable->HaveCells())
						{
							CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
							pCtrltable->AddParagraph(pNewPara);
							oBuffer.Skip(nSize);
							ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
						}
						else
						{
							CTblCell* pCell = pCtrltable->GetLastCell();

							CCellParagraph* pNewPara = new CCellParagraph();
							pCell->AddParagraph(pNewPara);

							CHWPPargraph::Parse(*pNewPara, nSize, oBuffer, 0, nVersion);
							ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
						}
					}
					else if (nullptr != dynamic_cast<CCtrlShapeRect*>(pCtrl))
					{
						CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
						((CCtrlCommon*)pCtrl)->AddParagraph(pNewPara);
						CHWPPargraph::Parse(*pNewPara, nSize, oBuffer, 0, nVersion);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
					{
						CCtrlCommon *pCtrlCommon = (CCtrlCommon*)(pCtrl);
						if (0 < pCtrlCommon->GetCaptionWidth() && pCtrlCommon->CaptionsEmpty())
						{
							CCapParagraph *pNewPara = new CCapParagraph();
							pCtrlCommon->AddCaption(pNewPara);
							CHWPPargraph::Parse(*pNewPara, nSize, oBuffer, 0, nVersion);
							ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
						}
						else
						{
							CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
							pCtrlCommon->AddParagraph(pNewPara);
							CHWPPargraph::Parse(*pNewPara, nLevel, oBuffer, 0, nVersion);
							ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
						}
					}
					else if (nullptr != dynamic_cast<CCtrlHeadFoot*>(pCtrl))
					{
						CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
						((CCtrlHeadFoot*)pCtrl)->AddParagraph(pNewPara);
						oBuffer.Skip(nSize);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlSectionDef*>(pCtrl))
					{
						CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
						((CCtrlSectionDef*)pCtrl)->AddParagraph(pNewPara);
						oBuffer.Skip(nSize);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlNote*>(pCtrl))
					{
						CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
						((CCtrlNote*)pCtrl)->AddParagraph(pNewPara);
						oBuffer.Skip(nSize);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
					}
					else
					{
						//TODO:: проверить
						CHWPPargraph *pNewPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
						oBuffer.Skip(nSize);
						ParseRecurse(pNewPara, nLevel, oBuffer, 0, nVersion);
						delete pNewPara;
					}

					break;
				}
				case HWPTAG_CTRL_HEADER:
				{
					oBuffer.Skip(-nHeaderSize);
					return true;
				}
				case HWPTAG_PAGE_DEF:
				{
					if (nullptr != dynamic_cast<CCtrlSectionDef*>(pCtrl))
						((CCtrlSectionDef*)pCtrl)->SetPage(CPage::Parse(nLevel, nSize, oBuffer, 0, nVersion));

					break;
				}
				case HWPTAG_FOOTNOTE_SHAPE:
				{
					if (nullptr == dynamic_cast<CCtrlSectionDef*>(pCtrl))
						break;

					((CCtrlSectionDef*)pCtrl)->AddNoteShape(CNoteShape::Parse(nLevel, nSize, oBuffer, 0, nVersion));

					break;
				}
				case HWPTAG_PAGE_BORDER_FILL:
				{
					if (nullptr == dynamic_cast<CCtrlSectionDef*>(pCtrl))
						break;

					((CCtrlSectionDef*)pCtrl)->AddPageBorderFill(CPageBorderFill::Parse(nLevel, nSize, oBuffer, 0, nVersion));

					break;
				}
				case HWPTAG_TABLE:
				{
					int nLen = CCtrlTable::ParseCtrl(*(CCtrlTable*)pCtrl, nSize, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_LIST_HEADER:
				{
					if (nullptr == dynamic_cast<CCtrlTable*>(pCtrl))
					{
						oBuffer.Skip(nSize);
						break;
					}

					EVertAlign eVerAlign = CHWPRecordListHeader::GetVertAlign(nSize, oBuffer, 0, nVersion);

					CTblCell* pCell = new CTblCell(nSize - 6, oBuffer, 0, nVersion);
					pCell->SetVertAlign(eVerAlign);

					((CCtrlTable*)pCtrl)->AddCell(pCell);

					break;
				}
				case HWPTAG_SHAPE_COMPONENT:
				{
					if(nullptr != dynamic_cast<CCtrlContainer*>(pCtrl))
					{
						oBuffer.Skip(-nHeaderSize);
						ParseContainerRecurse((CCtrlContainer*)pCtrl, nLevel, oBuffer, 0, nVersion);
					}
					else if (nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
					{
						CCtrlGeneralShape *pNewCtrl = CCtrlGeneralShape::Parse(*(CCtrlGeneralShape*)pCtrl, nSize, oBuffer, 0, nVersion);
						CHWPPargraph *pParentPara = ((CCtrlGeneralShape*)pCtrl)->GetParent();
						int nCtrlIbdex = pParentPara->IndexOf(pCtrl);

						if (nCtrlIbdex >= 0)
							pParentPara->SetCtrl(pNewCtrl, nCtrlIbdex);
						else
							pParentPara->AddCtrl(pNewCtrl);

						//TODO:: проверить
						pCtrl = pNewCtrl;
					}

					break;
				}

				#define PROCESS_SHAPE_COMPONENT(type_conponent) \
				if (nullptr == dynamic_cast<type_conponent*>(pCtrl)) \
				{ \
					oBuffer.Skip(nSize); \
					break; \
				} \
				type_conponent::ParseElement(*(type_conponent*)pCtrl, nSize, oBuffer, 0, nVersion)

				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapePic);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_LINE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeLine);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeRect);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeEllipse);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_ARC:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeArc);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapePolygon);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeCurve);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_OLE:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeOle);
					break;
				}
				case HWPTAG_EQEDIT:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlEqEdit);
					break;
				}
				case HWPTAG_VIDEO_DATA:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeVideo);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				{
					PROCESS_SHAPE_COMPONENT(CCtrlShapeTextArt);
					break;
				}
				case HWPTAG_FORM_OBJECT:
				{
					if (nullptr == dynamic_cast<CCtrlForm*>(pCtrl))
					{
						oBuffer.Skip(nSize);
						break;
					}

					CHWPRecordFormObject::ParseCtrl(*(CCtrlForm*)pCtrl, nSize, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_CTRL_DATA:
				{
					CHWPRecordCtrlData::ParseCtrl(*pCtrl, nSize, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				default:
				{
					oBuffer.Skip(nSize);
					break;
				}
			}
		}
	}

	return 0;
}

int CHWPSection::ParseContainerRecurse(CCtrlContainer* pContainer, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	int nHeader, nTagNum, nLevel, nSize, nHeaderSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (0xFFF == nSize)
		{
			nHeaderSize = 8;
			//TODO:: buf[off+7]<<24&0xFF000000 | buf[off+6]<<16&0xFF0000 | buf[off+5]<<8&0xFF00 | buf[off+4]&0xFF;
			oBuffer.Skip(4);
			oBuffer.ReadInt(nSize);
			oBuffer.Skip(-8);
		}
		else
		{
			nHeaderSize = 4;
		}

		if (nLevel < nRunLevel)
			break;

		EHWPTag eTag = GetTagFromNum(nTagNum);

		PRINT_HWPTAG("Container parse: ", eTag, " : size=" << std::to_string(nSize) << " level=" << std::to_string(nLevel));

		if (nLevel > nRunLevel)
		{
			oBuffer.Skip(nHeaderSize);

			#define CHECK_SHAPE(shapeType) \
			const VECTOR<CCtrlGeneralShape*> arCtrls = pContainer->GetShapes(); \
			shapeType *pCtrl = nullptr; \
			FIND_LAST_ELEMENT(shapeType, pCtrl, CCtrlGeneralShape, arCtrls); \
			if (nullptr == pCtrl) \
			{ \
				pCtrl = new shapeType(); \
				pContainer->AddShape(pCtrl); \
			} \
			shapeType::ParseElement((shapeType&)(*pCtrl), nSize, oBuffer, 0, nVersion)

			switch (eTag)
			{
				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				{
					CHECK_SHAPE(CCtrlShapePic);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_LINE:
				{
					CHECK_SHAPE(CCtrlShapeLine);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				{
					CHECK_SHAPE(CCtrlShapeRect);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				{
					CHECK_SHAPE(CCtrlShapeEllipse);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_ARC:
				{
					CHECK_SHAPE(CCtrlShapeArc);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				{
					CHECK_SHAPE(CCtrlShapePolygon);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				{
					CHECK_SHAPE(CCtrlShapeCurve);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_OLE:
				{
					CHECK_SHAPE(CCtrlShapeOle);
					break;
				}
				case HWPTAG_EQEDIT:
				{
					CHECK_SHAPE(CCtrlEqEdit);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				{
					CHECK_SHAPE(CCtrlShapeTextArt);
					break;
				}
				case HWPTAG_LIST_HEADER:
				{
					CCtrlCommon* pCtrl = pContainer->GetLastShape();
					int nSubParaCount = CHWPRecordListHeader::GetCount(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
					oBuffer.Skip(6);

					bool bIsShapeRect = false, bIsShapePolygon = false;

					if (nullptr != dynamic_cast<CCtrlShapeRect*>(pCtrl))
						bIsShapeRect = true;
					else if (nullptr != dynamic_cast<CCtrlShapePolygon*>(pCtrl))
						bIsShapePolygon = true;

					if (bIsShapeRect || bIsShapePolygon)
					{
						if (bIsShapeRect)
							pCtrl->SetID(L"cer$");
						else
							pCtrl->SetID(L"lop$");

						oBuffer.Skip(-6);
						pCtrl->SetTextVerAlign(CHWPRecordListHeader::GetVertAlign(6, oBuffer, 0, nVersion));
						ParseListAppend(*pCtrl, nSize -6, oBuffer, 0, nVersion);
						ParseCtrlRecurse(pCtrl, nLevel, oBuffer, 0, nVersion);
					}
					else
						oBuffer.Skip(nSize - 6);

					break;
				}
				case HWPTAG_SHAPE_COMPONENT:
				{
					CCtrlGeneralShape oBaseCtrl;
					CCtrlGeneralShape *pNewCtrl = CCtrlGeneralShape::Parse(oBaseCtrl, nSize, oBuffer, 0, nVersion);

					if (nullptr == dynamic_cast<CCtrlGeneralShape*>(pNewCtrl))
						break;

					pContainer->AddShape(pNewCtrl);

					if (nullptr == dynamic_cast<CCtrlContainer*>(pNewCtrl))
						break;

					ParseContainerRecurse((CCtrlContainer*)pNewCtrl, nLevel, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				case HWPTAG_TABLE:
				case HWPTAG_PARA_HEADER:
				case HWPTAG_PARA_TEXT:
				case HWPTAG_PAGE_DEF:
				case HWPTAG_FOOTNOTE_SHAPE:
				case HWPTAG_PAGE_BORDER_FILL:
				case HWPTAG_PARA_RANGE_TAG:
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_HWP_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				default:
				{
					oBuffer.Skip(nSize);
					break;
				}
			}
		}
		else if (nLevel == nRunLevel)
		{
			oBuffer.Skip(nHeaderSize);

			switch (eTag)
			{
				case HWPTAG_PARA_HEADER:
				case HWPTAG_CTRL_HEADER:
				case HWPTAG_PAGE_DEF:
				case HWPTAG_FOOTNOTE_SHAPE:
				case HWPTAG_PAGE_BORDER_FILL:
				case HWPTAG_TABLE:
				case HWPTAG_LIST_HEADER:
				case HWPTAG_VIDEO_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_CTRL_DATA:
				{
					oBuffer.Skip(nSize);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT:
				{
					CCtrlGeneralShape oBaseCtrl;
					CCtrlGeneralShape *pNewCtrl = CCtrlGeneralShape::Parse(oBaseCtrl, nSize, oBuffer, 0, nVersion);

					if (nullptr == dynamic_cast<CCtrlGeneralShape*>(pNewCtrl))
						break;

					pContainer->AddShape(pNewCtrl);

					if (nullptr == dynamic_cast<CCtrlContainer*>(pNewCtrl))
						break;

					ParseContainerRecurse((CCtrlContainer*)pNewCtrl, nLevel, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_SHAPE_COMPONENT_PICTURE:
				case HWPTAG_SHAPE_COMPONENT_LINE:
				case HWPTAG_SHAPE_COMPONENT_RECTANGLE:
				case HWPTAG_SHAPE_COMPONENT_ELLIPSE:
				case HWPTAG_SHAPE_COMPONENT_ARC:
				case HWPTAG_SHAPE_COMPONENT_POLYGON:
				case HWPTAG_SHAPE_COMPONENT_CURVE:
				case HWPTAG_SHAPE_COMPONENT_OLE:
				case HWPTAG_EQEDIT:
				case HWPTAG_SHAPE_COMPONENT_TEXTART:
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				default:
				{
					oBuffer.Skip(nSize);
					break;
				}
			}
		}
	}

	return oBuffer.GetDistanceToLastPos(true);
}

const VECTOR<CHWPPargraph*> CHWPSection::GetParagraphs() const
{
	return m_arParas;
}

int CHWPSection::ParseListAppend(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	if (L"cer$" == oObj.GetID())
		CCtrlShapeRect::ParseListHeaderAppend((CCtrlShapeRect&)(oObj), nSize, oBuffer, 0, nVersion);
	else if (L" osg" == oObj.GetID())
		CCtrlGeneralShape::ParseListHeaderApend((CCtrlGeneralShape&)(oObj), nSize, oBuffer, 0, nVersion);
	else if (L" lbt" == oObj.GetID())
		CCtrlTable::ParseListHeaderAppend((CCtrlTable&)(oObj), nSize, oBuffer, 0, nVersion);
	else if (L"deqe" == oObj.GetID())
		CCtrlEqEdit::ParseListHeaderAppend((CCtrlEqEdit&)(oObj), nSize, oBuffer, 0, nVersion);
	else if (L"lop$" == oObj.GetID())
		CCtrlShapePolygon::ParseListHeaderAppend((CCtrlShapePolygon&)(oObj), nSize, oBuffer, 0, nVersion);
	else if (L"lle$" == oObj.GetID())
		CCtrlShapeEllipse::ParseListHeaderAppend((CCtrlShapeEllipse&)(oObj), nSize, oBuffer, 0, nVersion);
	else
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());

	return oBuffer.GetDistanceToLastPos(true);
}

int CHWPSection::ParseListAppend(CCtrl& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (L"dces" == oObj.GetID())
		oBuffer.Skip(nSize - 6);
	else if (L"daeh" == oObj.GetID() || L"toof" == oObj.GetID())
	{
		oBuffer.SavePosition();
		CCtrlHeadFoot::ParseListHeaderAppend((CCtrlHeadFoot&)(oObj), nSize, oBuffer, 0, nVersion);
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true) - 6);
	}
	else if (L"  nf" == oObj.GetID())
		oBuffer.Skip(nSize);

	return nSize;
}
}
