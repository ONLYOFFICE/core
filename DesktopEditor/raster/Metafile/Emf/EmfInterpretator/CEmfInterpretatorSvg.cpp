#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"

#include "../../../../graphics/Image.h"

#include "../../../BgraFrame.h"

#include <algorithm>
#include <cmath>

#ifndef MININT32
#define MAXUINT32   ((UINT32)~((UINT32)0))
#define MAXINT32    ((INT32)(MAXUINT32 >> 1))
#define MININT32    ((INT32)~MAXINT32)
#endif

namespace MetaFile
{               
	CEmfInterpretatorSvg::CEmfInterpretatorSvg(CEmfParserBase* pParser, double dWidth, double dHeight)
	    : CInterpretatorSvgBase(pParser, dWidth, dHeight)
	{}

	CEmfInterpretatorSvg::~CEmfInterpretatorSvg()
	{

	}

	void CEmfInterpretatorSvg::CreateConditional(IMetaFileBase *pParser)
	{
		m_oSecondConditional.m_pParser = pParser;
	}

	void CEmfInterpretatorSvg::ChangeConditional()
	{
		if (NULL == m_oSecondConditional.m_pParser)
			return;

		std::swap(m_wsLastClipId, m_oSecondConditional.m_wsLastClipId);
		std::swap(m_pParser, m_oSecondConditional.m_pParser);
	}

	InterpretatorType CEmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		m_oViewport.dLeft   = oTEmfHeader.oFramePx.lLeft;
		m_oViewport.dTop    = oTEmfHeader.oFramePx.lTop;
		m_oViewport.dRight  = oTEmfHeader.oFramePx.lRight;
		m_oViewport.dBottom = oTEmfHeader.oFramePx.lBottom;

		m_oXmlWriter.WriteNodeBegin(L"svg", true);
		m_oXmlWriter.WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_oXmlWriter.WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");

		UpdateSize();

		if (m_oViewport.GetWidth() != 0)
			m_oXmlWriter.WriteAttribute(L"width", ConvertToWString(m_oViewport.GetWidth()));

		if (m_oViewport.GetHeight() != 0)
			m_oXmlWriter.WriteAttribute(L"height", ConvertToWString(m_oViewport.GetHeight()));

		double dXScale = 1, dYScale = 1, dXTranslate = 0, dYTranslate = 0;

		if (0 != m_oSizeWindow.x)
		{
			dXScale = m_oSizeWindow.x / m_oViewport.GetWidth();
			dXTranslate = m_oViewport.GetWidth() / 2 * std::abs(dXScale - 1);

			if (dXScale < 1)
				dXTranslate = -dXTranslate;
		}

		if (0 != m_oSizeWindow.y)
		{
			dYScale = m_oSizeWindow.y / m_oViewport.GetHeight();
			dYTranslate = m_oViewport.GetHeight() / 2 * std::abs(dYScale - 1);

			if (dYScale < 1)
				dYTranslate = -dYTranslate;
		}

		if (1 != dXScale || 1 != dYScale)
			m_oXmlWriter.WriteAttribute(L"transform", L"matrix(" + std::to_wstring(dXScale) + L",0,0," + std::to_wstring(dYScale) + L',' + ConvertToWString(dXTranslate) + L',' + ConvertToWString(dYTranslate) + L')');

		m_oXmlWriter.WriteNodeEnd(L"svg", true, false);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EOF()
	{
		m_oXmlWriter.WriteString(m_wsDefs);
		m_oXmlWriter.WriteNodeEnd(L"svg", false, false);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLPATH(const TEmfRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXCLUDECLIPRECT(const TEmfRectL &oClip)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_INTERSECTCLIPRECT(const TEmfRectL &oClip)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ANGLEARC(const TEmfPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		double dXRadius = unRadius;
		double dYRadius = unRadius;

		double dXCenter = oCenter.x;
		double dYCenter = oCenter.y;

		double dStartX = dXCenter + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = dYCenter + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = dXCenter + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = dYCenter + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + ConvertToWString(dStartX) + L' ' + ConvertToWString(dStartY);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARC(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y);

		if (NULL != m_pParser && m_pParser->GetTransform()->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		double dEndX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		TEmfPointL oStartPoint;

		dStartAngle *= -M_PI / 180;

		double dWidth  = oBox.lRight  - oBox.lLeft;
		double dHeight = oBox.lBottom - oBox.lTop;

		double dTan = atan2( sin( dStartAngle ) / dHeight / 2,  cos( dStartAngle ) / dWidth / 2 );

		oStartPoint.x = oBox.lLeft + dWidth / 2.0 + dWidth / 2 * cos(dTan);
		oStartPoint.y = oBox.lTop + dHeight / 2.0 - dHeight / 2 * sin(dTan);

		std::wstring wsValue = L"M " + ConvertToWString(oStartPoint.x) + L' ' + ConvertToWString(oStartPoint.y);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARCTO(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		double dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y);

		if (NULL != m_pParser && m_pParser->GetTransform()->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		double dStartX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = (oNewRect.dRight + oNewRect.dLeft) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.dBottom + oNewRect.dTop) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		std::wstring wsValue = L"M " + ConvertToWString(dStartX) + L' ' + ConvertToWString(dStartY);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
				ConvertToWString(dYRadius) + L' ' +
				L"0 " +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
				((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
				ConvertToWString(dEndX) + L' ' +
				ConvertToWString(dEndY);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		//TODO:: реализовать
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
									   {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
									   {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
									   {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.aEmrText.OutputString, oTEmfExtTextoutA.aEmrText.Chars);

		std::vector<double> arDx(0);

		if (NULL != oTEmfExtTextoutA.aEmrText.OutputDx)
			arDx = std::vector<double>(oTEmfExtTextoutA.aEmrText.OutputDx, oTEmfExtTextoutA.aEmrText.OutputDx + oTEmfExtTextoutA.aEmrText.Chars);

		WriteText(wsText, TPointD(oTEmfExtTextoutA.aEmrText.Reference.x, oTEmfExtTextoutA.aEmrText.Reference.y), oTEmfExtTextoutA.Bounds, TPointD(oTEmfExtTextoutA.exScale, oTEmfExtTextoutA.eyScale), arDx);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.wEmrText.OutputString, oTEmfExtTextoutW.wEmrText.Chars);

		std::vector<double> arDx(0);

		if (NULL != oTEmfExtTextoutW.wEmrText.OutputDx)
			arDx = std::vector<double>(oTEmfExtTextoutW.wEmrText.OutputDx, oTEmfExtTextoutW.wEmrText.OutputDx + oTEmfExtTextoutW.wEmrText.Chars);

		WriteText(wsText, TPointD(oTEmfExtTextoutW.wEmrText.Reference.x, oTEmfExtTextoutW.wEmrText.Reference.y), oTEmfExtTextoutW.Bounds, TPointD(oTEmfExtTextoutW.exScale, oTEmfExtTextoutW.eyScale), arDx);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TEmfPointL &oPoint)
	{
		TPointD oCurPos = GetCutPos();

		NodeAttributes arAttributes = {{L"x1", ConvertToWString(oCurPos.x)},
									   {L"y1", ConvertToWString(oCurPos.y)},
									   {L"x2", ConvertToWString(oPoint.x)},
									   {L"y2", ConvertToWString(oPoint.y)}};

		AddStroke(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"line", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PIE(const TEmfRectL &oBox, const TEmfPointL &oStart, const TEmfPointL &oEnd)
	{
		short shCenterX = (oBox.lLeft + oBox.lRight) / 2;
		short shCenterY = (oBox.lTop + oBox.lBottom) / 2;

		short shRadiusX = std::abs(oBox.lRight - oBox.lLeft) / 2;
		short shRadiusY = std::abs(oBox.lBottom - oBox.lTop) / 2;

		std::wstring wsPath = L'M' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L' ' +
		                      L'L' + ConvertToWString(oStart.x)+ L' ' + ConvertToWString(oStart.y)+ L' ' +
		                      L'A' + ConvertToWString(shRadiusX) + L' ' + ConvertToWString(shRadiusY) + L" 0, 0, 0, " + ConvertToWString(oEnd.x) + L' ' + ConvertToWString(oEnd.y) + L' ' +
		                      L'L' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L" Z";

		NodeAttributes arAttributes = {{L"d", wsPath}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].x) + L' ' + ConvertToWString(arPoints[0].y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L' ' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
						ConvertToWString(arPoints[unIndex + 1].x) + L' ' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
						ConvertToWString(arPoints[unIndex + 2].x) + L' ' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		BYTE oLastType = 0x00;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
			{
				if (oLastType != 0x02)
				{
					wsValue += L" L ";
					oLastType = 0x02;
				}
				wsValue += ConvertToWString(arPoints[unIndex].x) + L',' + ConvertToWString(arPoints[unIndex].y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L',' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
							ConvertToWString(arPoints[unIndex + 1].x) + L',' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
							ConvertToWString(arPoints[unIndex + 2].x) + L',' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].x) + L' ' + ConvertToWString(arPoints[unIndex].y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TEmfRectL &oBounds, TEmfPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
	{
		if (0 == unCount || NULL == arPoints || NULL == pAbTypes)
			return;

		std::wstring wsValue;

		BYTE oLastType = 0x00;

		for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
		{
			if (0x02 == pAbTypes[unIndex])
			{
				if (oLastType != 0x02)
				{
					wsValue += L" L ";
					oLastType = 0x02;
				}
				wsValue += ConvertToWString(arPoints[unIndex].x) + L',' + ConvertToWString(arPoints[unIndex].y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].x)     + L',' + ConvertToWString(arPoints[unIndex].y)     + L' ' +
							ConvertToWString(arPoints[unIndex + 1].x) + L',' + ConvertToWString(arPoints[unIndex + 1].y) + L' ' +
							ConvertToWString(arPoints[unIndex + 2].x) + L',' + ConvertToWString(arPoints[unIndex + 2].y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].x) + L' ' + ConvertToWString(arPoints[unIndex].y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

		wsValue.pop_back();

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.x) + L',' + ConvertToWString(oCurPos.y);

		for (const TEmfPointL& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TEmfRectL &oBounds, const std::vector<TEmfPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.x) + L',' + ConvertToWString(oCurPos.y);

		for (const TEmfPointS& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointL>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';

			for (const TEmfPointL& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointS>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';

			for (const TEmfPointS& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].x) + L',' +  ConvertToWString(oPolygon[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointL>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';

			for (const TEmfPointL& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TEmfRectL &oBounds, const std::vector<std::vector<TEmfPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TEmfPointS>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';

			for (const TEmfPointS& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.x) + L',' + ConvertToWString(oPoint.y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].x) + L',' +  ConvertToWString(oPolyline[0].y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RECTANGLE(const TEmfRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)}};

		AddStroke(arAttributes);
		AddFill(arAttributes, oNewRect.dRight - oNewRect.dLeft, oNewRect.dBottom - oNewRect.dTop);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);


		WriteNode(L"rect", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TEmfRectL &oBox, const TEmfSizeL &oCorner)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.dLeft)},
									   {L"y",		ConvertToWString(oNewRect.dTop)},
									   {L"width",	ConvertToWString(oNewRect.dRight - oNewRect.dLeft)},
									   {L"height",	ConvertToWString(oNewRect.dBottom - oNewRect.dTop)},
		                               {L"rx",		ConvertToWString((double)oCorner.cx / 2.)},
		                               {L"ry",		ConvertToWString((double)oCorner.cy / 2.)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"rect", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETPIXELV(const TEmfPointL &oPoint, const TEmfColor &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEANDFILLPATH(const TEmfRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes, std::fabs(oBounds.lRight - oBounds.lLeft), std::fabs(oBounds.lBottom - oBounds.lTop));
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEPATH(const TEmfRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath();

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLRGN(const TEmfRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			            L"L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                    ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PAINTRGN(const TEmfRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			            L"L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                    ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                    ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FRAMERGN(const TEmfRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TEmfRectL> &arRects)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_OFFSETCLIP(double dX, double dY)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_RESETCLIP()
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPPATH(short unShFlags, const CEmfPlusPath *pPath)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPRECT(short shCM, const TEmfPlusRectF &oRect)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPREGION(short shObjectIndex, short shCM, const CEmfPlusRegion *pRegion)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB &oARGB)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWARC(BYTE chOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		if (NULL != m_pParser && m_pParser->GetTransform()->M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.dRight - oNewRect.dLeft)) / 2;
		double dYRadius = std::fabs((oNewRect.dBottom - oNewRect.dTop)) / 2;

		std::wstring wsValue = L"M " + ConvertToWString(oNewRect.dLeft) + L' ' + ConvertToWString(oNewRect.dTop);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
		        ConvertToWString(dYRadius) + L' ' +
		        L"0 " +
		        ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
		        ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
		        ConvertToWString(oNewRect.dRight) + L' ' +
		        ConvertToWString(oNewRect.dBottom);

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);

		if (AD_COUNTERCLOCKWISE == m_pParser->GetArcDirection())
		{
			TXForm oXForm;
			oXForm.Copy(m_pParser->GetTransform());

			oXForm.M22 = -oXForm.M22;

			AddTransform(arAttributes, &oXForm);
		}
		else
			AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWBEZIERS(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.size() < 4)
			return;

		std::wstring wsValue = L"M " + ConvertToWString(arPoints[0].X) + L' ' + ConvertToWString(arPoints[0].Y) + L" C ";

		for (unsigned int unIndex = 1; unIndex + 2 < arPoints.size(); unIndex += 3)
			wsValue +=	ConvertToWString(arPoints[unIndex].X)     + L' ' + ConvertToWString(arPoints[unIndex].Y)     + L' ' +
			            ConvertToWString(arPoints[unIndex + 1].X) + L' ' + ConvertToWString(arPoints[unIndex + 1].Y) + L' ' +
			            ConvertToWString(arPoints[unIndex + 2].X) + L' ' + ConvertToWString(arPoints[unIndex + 2].Y) + L' ';

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWCLOSEDCURVE(short shOgjectIndex, double dTension, const std::vector<TEmfPlusPointF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWCURVE(short shOgjectIndex, double dTension, unsigned int unOffset, unsigned int unNumSegments, const std::vector<TEmfPlusPointF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWDRIVERSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unDriverStringOptionsFlags, unsigned int unMatrixPresent, TXForm *pMatrix, const std::wstring &wsString, const std::vector<TPointD> &arGlyphPos)
	{
		if (NULL == m_pParser || wsString.size() != arGlyphPos.size())
			return;

		IFont *pFont = m_pParser->GetFont();

		if (NULL == pFont)
			return;

		NodeAttributes arNodeAttributes;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		int nColor = m_pParser->GetTextColor();

		if (0 != nColor)
			arNodeAttributes.push_back({L"fill", L"rgb(" + INTCOLOR_TO_RGB(nColor) + L')'});

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = 18;

		arNodeAttributes.push_back({L"font-size", ConvertToWString(dFontHeight)});

		std::wstring wsFaceName = pFont->GetFaceName();

		if (!wsFaceName.empty())
			arNodeAttributes.push_back({L"font-family", wsFaceName});

		if (pFont->GetWeight() > 550)
			arNodeAttributes.push_back({L"font-weight", L"bold"});

		if (pFont->IsItalic())
			arNodeAttributes.push_back({L"font-style", L"italic"});

		if (pFont->IsUnderline() && pFont->IsStrikeOut())
			arNodeAttributes.push_back({L"text-decoration", L"underline line-through"});
		else if (pFont->IsUnderline())
			arNodeAttributes.push_back({L"text-decoration", L"underline"});
		else if (pFont->IsStrikeOut())
			arNodeAttributes.push_back({L"text-decoration", L"line-through"});

		std::wstring wsValue;

		for (unsigned int unIndex = 0; unIndex < arGlyphPos.size(); ++unIndex)
			wsValue += L"<tspan x=\"" + ConvertToWString(arGlyphPos[unIndex].x) + L"\" y=\"" + ConvertToWString(arGlyphPos[unIndex].y) + L"\">" + StringNormalization(std::wstring(1, wsString[unIndex])) + L"</tspan>";

		AddTransform(arNodeAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"text", arNodeAttributes, wsValue);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWELLIPSE(short shOgjectIndex, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWLINES(short shOgjectIndex, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPlusPointF& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polyline", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWPATH(short shOgjectIndex, unsigned int unPenId, const CEmfPath* pPath)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath(pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWPIE(short shOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		//TODO: Реализовать при встрече
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWRECTS(short shOgjectIndex, const std::vector<TEmfPlusRectF> &arRects)
	{
		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfPlusRectF& oRect : arRects)
		{
			oTempRect = oRect.GetRectD();

			wsValue +=	L"M "  + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) +
			            L" L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                     ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unFormatID, const std::wstring &wsString, const TEmfPlusRectF &oRect)
	{
		WriteText(wsString, TPointD(oRect.dX, oRect.dY), oRect.GetRectL(), TPointD(m_pParser->GetTransform()->M11, m_pParser->GetTransform()->M22));
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int unBrushId, double dTension, const std::vector<TEmfPlusRectF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLELLIPSE(unsigned int unBrushId, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.GetRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.dLeft   + oNewRect.dRight)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.dTop    + oNewRect.dBottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.dRight  - oNewRect.dLeft)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.dBottom - oNewRect.dTop)    / 2)}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"ellipse", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPATH(short shOgjectIndex, unsigned int unBrushId, const CEmfPlusPath *pPath)
	{
		if (NULL == m_pParser)
			return;

		std::wstring wsValue = CreatePath(pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		TRectD oPathRect = pPath->ConvertToRect();

		AddFill(arAttributes, std::fabs(oPathRect.dRight - oPathRect.dLeft), std::fabs(oPathRect.dBottom - oPathRect.dTop));
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path" , arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPIE(unsigned int unBrushId, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPOLYGON(unsigned int unBrushId, const std::vector<TEmfPlusPointF> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TEmfPlusPointF& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"polygon", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLRECTS(unsigned int unBrushId, const std::vector<TEmfPlusRectF> &arRects)
	{
		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfPlusRectF& oRect : arRects)
		{
			oTempRect = oRect.GetRectD();

			m_pParser->GetTransform()->Apply(oTempRect.dLeft,  oTempRect.dTop);
			m_pParser->GetTransform()->Apply(oTempRect.dRight, oTempRect.dBottom);

			wsValue +=	L"M "  + ConvertToWString(oTempRect.dLeft)  + L',' + ConvertToWString(oTempRect.dTop) +
			            L" L " + ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dTop) + L' ' +
			                     ConvertToWString(oTempRect.dRight) + L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dBottom) + L' ' +
			                     ConvertToWString(oTempRect.dLeft)	+ L',' + ConvertToWString(oTempRect.dTop) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);

		NodeAttributes arGAttributes;
		AddClip(arGAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeBegin(L"g", arGAttributes);

		WriteNode(L"path", arAttributes);

		if (!m_wsLastClipId.empty())
			WriteNodeEnd(L"g");
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLREGION(short shOgjectIndex, unsigned int unBrushId)
	{

	}

	void CEmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		if (NULL == pBuffer || 0 == dW || 0 == dH || 0 == unWidth || 0 == unHeight)
			return;

		if (1 == unWidth && 1 == unHeight)
		{
			NodeAttributes arAttributes = {{L"x",      ConvertToWString(dX)},
			                               {L"y",      ConvertToWString(dY)},
			                               {L"width",  ConvertToWString(dW)},
			                               {L"height", ConvertToWString(dH)},
			                               {L"fill", L"rgb(" + std::to_wstring(pBuffer[2]) + L',' + std::to_wstring(pBuffer[1]) + L',' + std::to_wstring(pBuffer[0]) + L',' + std::to_wstring(pBuffer[3]) + L')'}};

			AddTransform(arAttributes);

			WriteNode(L"rect", arAttributes);

			return;
		}

		CBgraFrame  oFrame;

		oFrame.put_Data(pBuffer);
		oFrame.put_Width(unWidth);
		oFrame.put_Height(unHeight);

		BYTE* pNewBuffer = NULL;
		int nNewSize = 0;

		oFrame.Encode(pNewBuffer, nNewSize, 4);
		oFrame.put_Data(NULL);

		if (0 < nNewSize)
		{
			int nImageSize = NSBase64::Base64EncodeGetRequiredLength(nNewSize);
			unsigned char* ucValue = new unsigned char[nImageSize];

			if (NULL == ucValue)
				return;

			NSBase64::Base64Encode(pNewBuffer, nNewSize, ucValue, &nImageSize);
			std::wstring wsValue(ucValue, ucValue + nImageSize);

			RELEASEARRAYOBJECTS(ucValue);

			NodeAttributes arAttributes = {{L"x",      ConvertToWString(dX)},
			                               {L"y",      ConvertToWString(dY)},
			                               {L"width",  ConvertToWString(dW)},
			                               {L"height", ConvertToWString(dH)},
			                               {L"xlink:href", L"data:image/png;base64," + wsValue}};

			AddTransform(arAttributes);

			NodeAttributes arGAttributes;
			AddClip(arGAttributes);

			if (!m_wsLastClipId.empty())
				WriteNodeBegin(L"g", arGAttributes);

			WriteNode(L"image", arAttributes);

			if (!m_wsLastClipId.empty())
				WriteNodeEnd(L"g");
		}

		if (NULL != pNewBuffer)
			delete [] pNewBuffer;
	}

	void CEmfInterpretatorSvg::ResetClip()
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::IntersectClip(const TRectD &oClip)
	{
		CInterpretatorSvgBase::IntersectClip(oClip);
	}

	void CEmfInterpretatorSvg::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		CInterpretatorSvgBase::ExcludeClip(oClip, oBB);
	}

	void CEmfInterpretatorSvg::PathClip(IPath *pPath, int nClipMode, TXForm *pTransform)
	{
		if (NULL == pPath || nClipMode != CombineModeIntersect)
			return;

		m_wsLastClipId = L"PATHCLIP_" + ConvertToWString(++m_unNumberDefs, 0);

		CEmfPath *pEmfPath = dynamic_cast<CEmfPath*>(pPath);

		if (NULL == pEmfPath)
			return;

		std::wstring wsPath = CreatePath(pEmfPath, pTransform);

		if (wsPath.empty())
			return;

		m_wsDefs += L"<clipPath id=\"" + m_wsLastClipId + L"\"><path d=\"" + wsPath + L"\" clip-rule=\"evenodd\"/></clipPath>";
	}

	TRectD CEmfInterpretatorSvg::TranslateRect(const TEmfRectL &oRect) const
	{
		TRectD oNewRect(oRect.lLeft, oRect.lTop, oRect.lRight, oRect.lBottom);

		if (oNewRect.dRight < oNewRect.dLeft)
		{
			double dTempValue = oNewRect.dLeft;
			oNewRect.dLeft    = oNewRect.dRight;
			oNewRect.dRight   = dTempValue;
		}

		if (oNewRect.dBottom < oNewRect.dTop)
		{
			double dTempValue = oNewRect.dTop;
			oNewRect.dTop     = oNewRect.dBottom;
			oNewRect.dBottom  = dTempValue;
		}

		return oNewRect;
	}

	std::wstring CEmfInterpretatorSvg::CreatePath(const CEmfPath* pPath, const TXForm* pTransform)
	{
		if (NULL == m_pParser)
			return std::wstring();

		const CEmfPath* pNewPath = pPath;

		if (NULL == pNewPath)
		{
			CEmfParserBase *pEmfParser = dynamic_cast<CEmfParserBase*>(m_pParser);

			if (NULL == pEmfParser)
				return std::wstring();

			pNewPath = pEmfParser->GetPath();
		}

		if (NULL == pNewPath || pNewPath->m_pCommands.empty())
			return std::wstring();

		std::wstring wsValue;
		TEmfPointD *pFirstPoint = NULL;
		BYTE oLastType = 0x00;

		TXForm oTransform;

		if (NULL != pTransform)
			oTransform.Copy(pTransform);

		TPointD oPoint;

		for (const CEmfPathCommandBase* pCommand : pNewPath->m_pCommands)
		{
			switch ((unsigned int)pCommand->GetType())
			{
				case EMF_PATHCOMMAND_MOVETO:
				{
					CEmfPathMoveTo* pMoveTo = (CEmfPathMoveTo*)pCommand;

					oPoint.x = pMoveTo->x;
					oPoint.y = pMoveTo->y;

					oTransform.Apply(oPoint.x, oPoint.y);

					wsValue += L"M " + ConvertToWString(oPoint.x) + L',' +  ConvertToWString(oPoint.y) + L' ';

					RELEASEOBJECT(pFirstPoint);
					pFirstPoint = new TEmfPointD();
					pFirstPoint->x = oPoint.x;
					pFirstPoint->y = oPoint.y;

					oLastType = EMF_PATHCOMMAND_MOVETO;

					break;
				}
				case EMF_PATHCOMMAND_LINETO:
				{
					CEmfPathLineTo* pLineTo = (CEmfPathLineTo*)pCommand;

					if (EMF_PATHCOMMAND_LINETO != oLastType)
					{
						oLastType = EMF_PATHCOMMAND_LINETO;
						wsValue += L"L ";
					}

					oPoint.x = pLineTo->x;
					oPoint.y = pLineTo->y;

					oTransform.Apply(oPoint.x, oPoint.y);

					wsValue += ConvertToWString(oPoint.x) + L',' +  ConvertToWString(oPoint.y) + L' ';

					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					break;
				}
				case EMF_PATHCOMMAND_CURVETO:
				{
					CEmfPathCurveTo* pCurveTo = (CEmfPathCurveTo*)pCommand;

					if (EMF_PATHCOMMAND_CURVETO != oLastType)
					{
						oLastType = EMF_PATHCOMMAND_CURVETO;
						wsValue += L"C ";
					}

					TPointD oPoint2(pCurveTo->x2, pCurveTo->y2), oPoint3(pCurveTo->xE, pCurveTo->yE);

					oPoint.x = pCurveTo->x1;
					oPoint.y = pCurveTo->y1;

					oTransform.Apply(oPoint.x,  oPoint.y);
					oTransform.Apply(oPoint2.x, oPoint2.y);
					oTransform.Apply(oPoint3.x, oPoint3.y);

					wsValue +=	ConvertToWString(oPoint.x)  + L',' + ConvertToWString(oPoint.y)  + L' ' +
					            ConvertToWString(oPoint2.x) + L',' + ConvertToWString(oPoint2.y) + L' ' +
					            ConvertToWString(oPoint3.x) + L',' + ConvertToWString(oPoint3.y) + L' ';

					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					break;
				}
				case EMF_PATHCOMMAND_ARCTO:
				{
					CEmfPathArcTo* pArcTo = (CEmfPathArcTo*)pCommand;

					TPointD oPoint2(pArcTo->right, pArcTo->bottom);

					oPoint.x = pArcTo->left;
					oPoint.y = pArcTo->top;

					oTransform.Apply(oPoint.x,  oPoint.y);
					oTransform.Apply(oPoint2.x, oPoint2.y);

					double dXRadius = std::fabs((oPoint2.x - oPoint.x)) / 2;
					double dYRadius = std::fabs((oPoint2.y - oPoint.y)) / 2;

					double dEndX = (oPoint2.x + oPoint.x)  / 2 + dXRadius  * cos((pArcTo->sweep) * M_PI / 180);
					double dEndY = (oPoint2.y + oPoint2.y) / 2 + dYRadius  * sin((pArcTo->sweep) * M_PI / 180);

					wsValue += L"A " + ConvertToWString(dXRadius) + L' ' +
					        ConvertToWString(dYRadius) + L' ' +
					        L"0 " +
					        ((std::fabs(pArcTo->sweep - pArcTo->start) <= 180) ? L"0" : L"1") + L' ' +
					        ((std::fabs(pArcTo->sweep - pArcTo->start) <= 180) ? L"1" : L"0") + L' ' +
					        ConvertToWString(dEndX) + L' ' +
					        ConvertToWString(dEndY) + L' ';


					if (NULL == pFirstPoint)
					{
						pFirstPoint = new TEmfPointD;
						pFirstPoint->x = oPoint.x;
						pFirstPoint->y = oPoint.y;
					}

					oLastType = EMF_PATHCOMMAND_ARCTO;

					break;
				}
				case EMF_PATHCOMMAND_CLOSE:
				{
					if (NULL != pFirstPoint)
					{
						wsValue += L"L " + ConvertToWString(pFirstPoint->x) + L',' + ConvertToWString(pFirstPoint->y) + L' ';
						oLastType = EMF_PATHCOMMAND_CLOSE;
						RELEASEOBJECT(pFirstPoint);
					}
					break;
				}
			}
		}

		if (!wsValue.empty() && wsValue[0] != L'M')
			wsValue.insert(0, L"M " + ConvertToWString(m_pParser->GetCurPos().x) + L',' + ConvertToWString(m_pParser->GetCurPos().y) + L' ');

		RELEASEOBJECT(pFirstPoint);

		return wsValue;
	}
}

