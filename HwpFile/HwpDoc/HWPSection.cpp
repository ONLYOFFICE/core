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

#include "Paragraph/CellParagraph.h"
#include "Paragraph/CapParagraph.h"

#include "HWPElements/HWPRecordParaText.h"
#include "HWPElements/HWPRecordParaRangeTag.h"
#include "HWPElements/HWPRecordCtrlHeader.h"
#include "HWPElements/HWPRecordListHeader.h"

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
	int nHeader, nTagNum, nLevel, nSize;

	while (oBuffer.CanRead())
	{
		oBuffer.ReadInt(nHeader);
		nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
		nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
		nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

		if (nLevel > 0)
		{
			return true;
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

		if (0 == nLevel && EHWPTag::HWPTAG_PARA_HEADER == eTag)
		{
			CHWPPargraph *pCurrPara = CHWPPargraph::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

			if (nullptr != pCurrPara)
				m_arParas.push_back(pCurrPara);
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


int CHWPSection::ParseRecurse(CHWPPargraph& oCurrPara, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
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

		if (nLevel > nRunLevel)
		{
			switch(eTag)
			{
				case HWPTAG_PARA_HEADER:
				case HWPTAG_PARA_TEXT:
				case HWPTAG_PARA_CHAR_SHAPE:
				case HWPTAG_PARA_LINE_SEG:
				case HWPTAG_PARA_RANGE_TAG:
				case HWPTAG_CTRL_HEADER:
				{
					ParseRecurse(oCurrPara, nLevel, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_TABLE:
				{
					CCtrlTable *pTable = nullptr;

					const VECTOR<CCtrl*> arCtrls = oCurrPara.GetCtrls();

					FIND_LAST_ELEMENT(CCtrlTable, pTable, CCtrl, arCtrls);

					if (nullptr != pTable)
						ParseCtrlRecurse(*pTable, nLevel, oBuffer, 0, nVersion);

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
					CCtrlSectionDef *pCtrlSecDef = dynamic_cast<CCtrlSectionDef*>(oCurrPara.FindLastElement(L"dces"));

					if (nullptr != pCtrlSecDef)
						ParseCtrlRecurse(*pCtrlSecDef, nLevel, oBuffer, 0, nVersion);

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
					CCtrlGeneralShape *pCtrlGeneral = dynamic_cast<CCtrlGeneralShape*>(oCurrPara.FindLastElement(L" osg"));

					if (nullptr != pCtrlGeneral)
						ParseCtrlRecurse(*pCtrlGeneral, nLevel, oBuffer, 0, nVersion);

					break;
				}
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				default:
				{
					CCtrlCommon *pCtrlCommon = nullptr;

					const VECTOR<CCtrl*> arCtrls = oCurrPara.GetCtrls();

					FIND_LAST_ELEMENT(CCtrlCommon, pCtrlCommon, CCtrl, arCtrls);

					if (nullptr != pCtrlCommon)
						ParseCtrlRecurse(*pCtrlCommon, nLevel, oBuffer, 0, nVersion);

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
					if (nullptr != dynamic_cast<CCellParagraph*>(&oCurrPara))
					{
						oBuffer.Skip(-nHeaderSize);
						return oBuffer.GetDistanceToLastPos();
					}
					else if (nullptr != dynamic_cast<CCapParagraph*>(&oCurrPara))
					{
						CHWPPargraph::Parse(oCurrPara, nSize, oBuffer, 0, nVersion);
						break;
					}
					else
					{
						if (1 == nRunLevel) //TODO:: проверить
						{
							oBuffer.Skip(nSize);
							return oBuffer.GetDistanceToLastPos();
						}

						oBuffer.Skip(-nHeaderSize);
						return oBuffer.GetDistanceToLastPos();
					}
				}
				case HWPTAG_PARA_TEXT:
				{
					oCurrPara.AddCtrls(CHWPRecordParaText::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion));
					break;
				}
				case HWPTAG_PARA_CHAR_SHAPE:
				{
					if (0 == oCurrPara.GetCountCtrls())
						oCurrPara.AddCtrl(new CCtrlCharacter(L"   _", ECtrlCharType::PARAGRAPH_BREAK));

					CCharShape::FillCharShape(nTagNum, nLevel, nSize, oBuffer, 0, nVersion, oCurrPara.GetCtrls());
				}
				case HWPTAG_PARA_LINE_SEG:
				{
					oCurrPara.SetLineSeg(new CLineSeg(nTagNum, nLevel, nSize, oBuffer, 0, nVersion));
					break;
				}
				case HWPTAG_PARA_RANGE_TAG:
				{
					CHWPRecordParaRangeTag::Parse(oCurrPara, nTagNum, nLevel, nSize, oBuffer, 0, nVersion);
					break;
				}
				case HWPTAG_CTRL_HEADER:
				{
					CCtrl *pCtrl = CHWPRecordCtrlHeader::Parse(nTagNum, nLevel, nSize, oBuffer, 0, nVersion);

					if (nullptr != dynamic_cast<CCtrlGeneralShape*>(pCtrl))
						((CCtrlGeneralShape*)pCtrl)->SetParent(&oCurrPara);

					unsigned int nIndex;
					CCtrl* pCtrlOrigOp = oCurrPara.FindFirstElement(pCtrl->GetID(), pCtrl->FullFilled(), nIndex);

					if (nullptr != pCtrlOrigOp)
						oCurrPara.SetCtrl(pCtrl, nIndex);

					if (nullptr != dynamic_cast<CCtrlHeadFoot*>(pCtrl))
					{
						CCtrlSectionDef* pSecD = dynamic_cast<CCtrlSectionDef*>(oCurrPara.FindLastElement(L"dces"));

						if (nullptr != pSecD)
							pSecD->AddHeadFoot((CCtrlHeadFoot*)pCtrl);
					}

					if (nullptr != dynamic_cast<CCtrlTable*>(pCtrl))
					{
						oBuffer.Skip(nSize);
						ParseCtrlRecurse(*pCtrl, nLevel, oBuffer, 0, nVersion);
					}
					else
					{
						oBuffer.Skip(nSize);
						ParseCtrlRecurse(*pCtrl, nLevel, oBuffer, 0, nVersion);
					}
					break;
				}
				case HWPTAG_TABLE:
				{
					oBuffer.Skip(-nHeaderSize);
					return oBuffer.GetDistanceToLastPos();
				}
				case HWPTAG_LIST_HEADER:
				{
					if (1 == nRunLevel)
					{
						oBuffer.Skip(nSize);
						return oBuffer.GetDistanceToLastPos();
					}
					oBuffer.Skip(-nHeaderSize);
					return oBuffer.GetDistanceToLastPos();
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
					return oBuffer.GetDistanceToLastPos();
				}
				case HWPTAG_CTRL_DATA:
				case HWPTAG_FORM_OBJECT:
				case HWPTAG_MEMO_SHAPE:
				case HWPTAG_MEMO_LIST:
				case HWPTAG_CHART_DATA:
				case HWPTAG_VIDEO_DATA:
				case HWPTAG_SHAPE_COMPONENT_UNKNOWN:
				default:
				{
					oBuffer.Skip(nSize);
					break;
				}
			}
		}
	}

	return oBuffer.GetDistanceToLastPos();
}

int CHWPSection::ParseCtrlRecurse(CCtrl& oCurrCtrl, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return 0;
}

int CHWPSection::ParseContainerRecurse(CCtrlContainer& oContainer, int nRunLevel, CHWPStream& oBuffer, int nOff, int nVersion)
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

		if (nLevel > nRunLevel)
		{
			oBuffer.Skip(nHeaderSize);

			#define CHECK_SHAPE(shapeType) \
			const VECTOR<CCtrlGeneralShape*> arCtrls = oContainer.GetShapes(); \
			shapeType *pCtrl = nullptr; \
			FIND_LAST_ELEMENT(shapeType, pCtrl, CCtrlGeneralShape, arCtrls); \
			if (nullptr == pCtrl) \
			{ \
				pCtrl = new shapeType(); \
				oContainer.AddShape(pCtrl); \
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
					CCtrlCommon* pCtrl = oContainer.GetLastShape();
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
						ParseCtrlRecurse(*pCtrl, nLevel, oBuffer, 0, nVersion);
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

					oContainer.AddShape(pNewCtrl);

					if (nullptr == dynamic_cast<CCtrlContainer*>(pNewCtrl))
						break;

					ParseContainerRecurse((CCtrlContainer&)(*pNewCtrl), nLevel, oBuffer, 0, nVersion);
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
				case HWPTAG_CHART_DATA:
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

					oContainer.AddShape(pNewCtrl);

					if (nullptr == dynamic_cast<CCtrlContainer*>(pNewCtrl))
						break;

					ParseContainerRecurse((CCtrlContainer&)(*pNewCtrl), nLevel, oBuffer, 0, nVersion);
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

	return oBuffer.GetDistanceToLastPos();
}

int CHWPSection::ParseListAppend(CCtrl& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (L"dces" == oObj.GetID())
		oBuffer.Skip(nSize - 6);
	else if (L"daeh" == oObj.GetID() || L"toof" == oObj.GetID())
		CCtrlHeadFoot::ParseListHeaderAppend((CCtrlHeadFoot&)(oObj), nSize, oBuffer, 0, nVersion);

	// else if ("  nf" == oObj.GetID())

	return nSize;
}
}
