#include "CEmfInterpretatorSvg.h"

#include "../../Common/MetaFileUtils.h"
#include "../../../BgraFrame.h"

#include <algorithm>
#include <cmath>
#include <stdint.h>

#ifndef MININT32
#define MAXUINT32   ((uint32_t)~((uint32_t)0))
#define MAXINT32    ((int32_t)(MAXUINT32 >> 1))
#define MININT32    ((int32_t)~MAXINT32)
#endif

namespace MetaFile
{
	CEmfInterpretatorSvg::CEmfInterpretatorSvg(CEmfParserBase* pParser, double dWidth, double dHeight)
	    : CInterpretatorSvgBase(pParser, dWidth, dHeight)
	{}

	CEmfInterpretatorSvg::~CEmfInterpretatorSvg()
	{}

	void CEmfInterpretatorSvg::CreateConditional(IMetaFileBase *pParser)
	{
		m_oSecondConditional.m_pParser = pParser;
	}

	void CEmfInterpretatorSvg::ChangeConditional()
	{
		if (NULL == m_oSecondConditional.m_pParser)
			return;

		std::swap(m_pParser, m_oSecondConditional.m_pParser);
		std::swap(m_oClip, m_oSecondConditional.m_oClip);
	}

	InterpretatorType CEmfInterpretatorSvg::GetType() const
	{
		return InterpretatorType::Svg;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_HEADER(const TEmfHeader &oTEmfHeader)
	{
		m_oViewport.dLeft   = std::min(oTEmfHeader.oFramePx.Left, oTEmfHeader.oFramePx.Right );
		m_oViewport.dTop    = std::min(oTEmfHeader.oFramePx.Top,  oTEmfHeader.oFramePx.Bottom);
		m_oViewport.dRight  = std::max(oTEmfHeader.oFramePx.Left, oTEmfHeader.oFramePx.Right );
		m_oViewport.dBottom = std::max(oTEmfHeader.oFramePx.Top,  oTEmfHeader.oFramePx.Bottom);

		m_pXmlWriter->WriteNodeBegin(L"svg", true);
		m_pXmlWriter->WriteAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_pXmlWriter->WriteAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");

		UpdateSize();

		double dXScale = 1., dYScale = 1.;

		if (0 != m_oSizeWindow.X)
			dXScale = m_oSizeWindow.X / m_oViewport.GetWidth();

		if (0 != m_oSizeWindow.Y)
			dYScale = m_oSizeWindow.Y / m_oViewport.GetHeight();

		if (m_oViewport.GetWidth() != 0)
			m_pXmlWriter->WriteAttribute(L"width", ConvertToWString(m_oViewport.GetWidth() * dXScale));

		if (m_oViewport.GetHeight() != 0)
			m_pXmlWriter->WriteAttribute(L"height", ConvertToWString(m_oViewport.GetHeight() * dYScale));

		m_pXmlWriter->WriteNodeEnd(L"svg", true, false);

		if (!Equals(1., dXScale) || !Equals(1., dYScale))
		{
			m_pXmlWriter->WriteNodeBegin(L"g", true);

			m_pXmlWriter->WriteAttribute(L"transform", L"scale(" + ConvertToWString(dXScale) + L',' + ConvertToWString(dYScale) + L')');

			m_pXmlWriter->WriteNodeEnd(L"g", true, false);
		}
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EOF()
	{
		CloseClip();

		if (!m_wsDefs.empty())
			m_pXmlWriter->WriteString(L"<defs>" + m_wsDefs + L"</defs>");

		if (!Equals(m_oSizeWindow.X, m_oViewport.GetWidth()) || !Equals(m_oSizeWindow.Y, m_oViewport.GetHeight()))
			m_pXmlWriter->WriteNodeEnd(L"g", false, false);

		m_pXmlWriter->WriteNodeEnd(L"svg", false, false);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		CInterpretatorSvgBase::ResetClip();
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLPATH(const TRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		const CPath* pPath = m_pParser->GetPath();
		
		if (NULL == pPath)
			return;
		
		const std::wstring wsValue = CreatePath(*pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddClip();
		AddFill(arAttributes);
		AddTransform(arAttributes);

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXCLUDECLIPRECT(const TRectL &oClip)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETMETARGN()
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_INTERSECTCLIPRECT(const TRectL &oClip)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ANGLEARC(const TPointL &oCenter, const unsigned int &unRadius, const double &dStartAngle, const double &dSweepAngle)
	{
		double dXRadius = unRadius;
		double dYRadius = unRadius;

		double dXCenter = oCenter.X;
		double dYCenter = oCenter.Y;

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

		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARC(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oStart.X, oStart.Y);
		double dSweepAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oEnd.X, oEnd.Y);

		if (NULL != m_pParser && m_pParser->GetTransform().M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.Right - oNewRect.Left)) / 2;
		double dYRadius = std::fabs((oNewRect.Bottom - oNewRect.Top)) / 2;

		double dEndX = (oNewRect.Right + oNewRect.Left) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.Bottom + oNewRect.Top) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

		TPointL oStartPoint;

		dStartAngle *= -M_PI / 180;

		double dWidth  = oBox.Right  - oBox.Left;
		double dHeight = oBox.Bottom - oBox.Top;

		double dTan = atan2( sin( dStartAngle ) / dHeight / 2,  cos( dStartAngle ) / dWidth / 2 );

		oStartPoint.X = oBox.Left + dWidth / 2.0 + dWidth / 2 * cos(dTan);
		oStartPoint.Y = oBox.Top + dHeight / 2.0 - dHeight / 2 * sin(dTan);

		std::wstring wsValue = L"M " + ConvertToWString(oStartPoint.X) + L' ' + ConvertToWString(oStartPoint.Y);

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

		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ARCTO(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		TRectD oNewRect = TranslateRect(oBox);

		double dStartAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oStart.X, oStart.Y);
		double dSweepAngle = GetEllipseAngle(oBox.Left, oBox.Top, oBox.Right, oBox.Bottom, oEnd.X, oEnd.Y);

		if (NULL != m_pParser && m_pParser->GetTransform().M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.Right - oNewRect.Left)) / 2;
		double dYRadius = std::fabs((oNewRect.Bottom - oNewRect.Top)) / 2;

		double dStartX = (oNewRect.Right + oNewRect.Left) / 2 + dXRadius  * cos((dStartAngle) * M_PI / 180);
		double dStartY = (oNewRect.Bottom + oNewRect.Top) / 2 + dYRadius  * sin((dStartAngle) * M_PI / 180);

		double dEndX = (oNewRect.Right + oNewRect.Left) / 2 + dXRadius  * cos((dSweepAngle) * M_PI / 180);
		double dEndY = (oNewRect.Bottom + oNewRect.Top) / 2 + dYRadius  * sin((dSweepAngle) * M_PI / 180);

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

		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_CHORD(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		//TODO:: реализовать
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ELLIPSE(const TRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.Left   + oNewRect.Right)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.Top    + oNewRect.Bottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.Right  - oNewRect.Left)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.Bottom - oNewRect.Top)    / 2)}};
		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"ellipse", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTA(const TEmfExtTextoutA &oTEmfExtTextoutA)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutA.oEmrText.pOutputString, oTEmfExtTextoutA.oEmrText.unChars);

		std::vector<double> arDx(0);

		if (NULL != oTEmfExtTextoutA.oEmrText.pOutputDx)
			arDx = std::vector<double>(oTEmfExtTextoutA.oEmrText.pOutputDx, oTEmfExtTextoutA.oEmrText.pOutputDx + oTEmfExtTextoutA.oEmrText.unChars);

		WriteText(wsText, TPointD(oTEmfExtTextoutA.oEmrText.oReference.X, oTEmfExtTextoutA.oEmrText.oReference.Y), oTEmfExtTextoutA.oBounds, TPointD(oTEmfExtTextoutA.dExScale, oTEmfExtTextoutA.dEyScale), arDx);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_EXTTEXTOUTW(const TEmfExtTextoutW &oTEmfExtTextoutW)
	{
		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oTEmfExtTextoutW.oEmrText.pOutputString, oTEmfExtTextoutW.oEmrText.unChars);

		std::vector<double> arDx(0);

		if (NULL != oTEmfExtTextoutW.oEmrText.pOutputDx)
			arDx = std::vector<double>(oTEmfExtTextoutW.oEmrText.pOutputDx, oTEmfExtTextoutW.oEmrText.pOutputDx + oTEmfExtTextoutW.oEmrText.unChars);

		WriteText(wsText, TPointD(oTEmfExtTextoutW.oEmrText.oReference.X, oTEmfExtTextoutW.oEmrText.oReference.Y), oTEmfExtTextoutW.oBounds, TPointD(oTEmfExtTextoutW.dExScale, oTEmfExtTextoutW.dEyScale), arDx);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_LINETO(const TPointL &oPoint)
	{
		TPointD oCurPos = GetCutPos();

		NodeAttributes arAttributes = {{L"x1", ConvertToWString(oCurPos.X)},
		                               {L"y1", ConvertToWString(oCurPos.Y)},
		                               {L"x2", ConvertToWString(oPoint.X)},
		                               {L"y2", ConvertToWString(oPoint.Y)}};

		AddStroke(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"line", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PIE(const TRectL &oBox, const TPointL &oStart, const TPointL &oEnd)
	{
		short shCenterX = (oBox.Left + oBox.Right) / 2;
		short shCenterY = (oBox.Top + oBox.Bottom) / 2;

		short shRadiusX = std::abs(oBox.Right - oBox.Left) / 2;
		short shRadiusY = std::abs(oBox.Bottom - oBox.Top) / 2;

		std::wstring wsPath = L'M' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L' ' +
		                      L'L' + ConvertToWString(oStart.X)+ L' ' + ConvertToWString(oStart.Y)+ L' ' +
		                      L'A' + ConvertToWString(shRadiusX) + L' ' + ConvertToWString(shRadiusY) + L" 0, 0, 0, " + ConvertToWString(oEnd.X) + L' ' + ConvertToWString(oEnd.Y) + L' ' +
		                      L'L' + ConvertToWString(shCenterX) + L' ' + ConvertToWString(shCenterY) + L" Z";

		NodeAttributes arAttributes = {{L"d", wsPath}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
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
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIER(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
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
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
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
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYBEZIERTO(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
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
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TRectL &oBounds, TPointL *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
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
				wsValue += ConvertToWString(arPoints[unIndex].X) + L',' + ConvertToWString(arPoints[unIndex].Y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].X)     + L',' + ConvertToWString(arPoints[unIndex].Y)     + L' ' +
				            ConvertToWString(arPoints[unIndex + 1].X) + L',' + ConvertToWString(arPoints[unIndex + 1].Y) + L' ' +
				            ConvertToWString(arPoints[unIndex + 2].X) + L',' + ConvertToWString(arPoints[unIndex + 2].Y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].X) + L' ' + ConvertToWString(arPoints[unIndex].Y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYDRAW(const TRectL &oBounds, TPointS *arPoints, const unsigned int &unCount, const unsigned char *pAbTypes)
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
				wsValue += ConvertToWString(arPoints[unIndex].X) + L',' + ConvertToWString(arPoints[unIndex].Y) + L' ';
			}
			else if (0x04 == pAbTypes[unIndex] && unIndex + 2 < unCount)
			{
				if (oLastType != 0x02)
				{
					wsValue += L" C ";
					oLastType = 0x04;
				}
				wsValue +=	ConvertToWString(arPoints[unIndex].X)     + L',' + ConvertToWString(arPoints[unIndex].Y)     + L' ' +
				            ConvertToWString(arPoints[unIndex + 1].X) + L',' + ConvertToWString(arPoints[unIndex + 1].Y) + L' ' +
				            ConvertToWString(arPoints[unIndex + 2].X) + L',' + ConvertToWString(arPoints[unIndex + 2].Y) + L' ';

				unIndex += 3;
			}
			else if (0x06 == pAbTypes[unIndex]) //MoveTo
			{
				wsValue += L" M " + ConvertToWString(arPoints[unIndex].X) + L' ' + ConvertToWString(arPoints[unIndex].Y) + L' ';
				oLastType = 0x06;
			}
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polygon", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYGON(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polygon", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointL& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINE(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		std::wstring wsValue;

		for (const TPointS& oPoint : arPoints)
			wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

		wsValue.pop_back();

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TRectL &oBounds, const std::vector<TPointL> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.X) + L',' + ConvertToWString(oCurPos.Y);

		for (const TPointL& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYLINETO(const TRectL &oBounds, const std::vector<TPointS> &arPoints)
	{
		if (arPoints.empty())
			return;

		TPointD oCurPos = GetCutPos();

		std::wstring wsValue = ConvertToWString(oCurPos.X) + L',' + ConvertToWString(oCurPos.Y);

		for (const TPointS& oPoint : arPoints)
			wsValue += L' ' + ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y);

		NodeAttributes arAttributes = {{L"points", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TRectL &oBounds, const std::vector<std::vector<TPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TPointL>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';

			for (const TPointL& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYGON(const TRectL &oBounds, const std::vector<std::vector<TPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TPointS>& oPolygon : arPolygons)
		{
			if (oPolygon.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';

			for (const TPointS& oPoint : oPolygon)
				wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

			wsValue += ConvertToWString(oPolygon[0].X) + L',' +  ConvertToWString(oPolygon[0].Y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TRectL &oBounds, const std::vector<std::vector<TPointL>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TPointL>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].X) + L',' +  ConvertToWString(oPolyline[0].Y) + L' ';

			for (const TPointL& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].X) + L',' +  ConvertToWString(oPolyline[0].Y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_POLYPOLYLINE(const TRectL &oBounds, const std::vector<std::vector<TPointS>> &arPolygons)
	{
		std::wstring wsValue;

		for (const std::vector<TPointS>& oPolyline : arPolygons)
		{
			if (oPolyline.size() < 2)
				continue;

			wsValue += L"M " + ConvertToWString(oPolyline[0].X) + L',' +  ConvertToWString(oPolyline[0].Y) + L' ';

			for (const TPointS& oPoint : oPolyline)
				wsValue += ConvertToWString(oPoint.X) + L',' + ConvertToWString(oPoint.Y) + L' ';

			wsValue += ConvertToWString(oPolyline[0].X) + L',' +  ConvertToWString(oPolyline[0].Y) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		arAttributes.push_back({L"fill-rule", L"evenodd"});

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_RECTANGLE(const TRectL &oBox)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.Left)},
		                               {L"y",		ConvertToWString(oNewRect.Top)},
		                               {L"width",	ConvertToWString(oNewRect.Right - oNewRect.Left)},
		                               {L"height",	ConvertToWString(oNewRect.Bottom - oNewRect.Top)}};

		AddStroke(arAttributes);
		AddFill(arAttributes, oNewRect.Right - oNewRect.Left, oNewRect.Bottom - oNewRect.Top);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"rect", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_ROUNDRECT(const TRectL &oBox, const TSizeL &oCorner)
	{
		TRectD oNewRect = TranslateRect(oBox);

		NodeAttributes arAttributes = {{L"x",		ConvertToWString(oNewRect.Left)},
		                               {L"y",		ConvertToWString(oNewRect.Top)},
		                               {L"width",	ConvertToWString(oNewRect.Right - oNewRect.Left)},
		                               {L"height",	ConvertToWString(oNewRect.Bottom - oNewRect.Top)},
		                               {L"rx",		ConvertToWString((double)oCorner.X / 2.)},
		                               {L"ry",		ConvertToWString((double)oCorner.Y / 2.)}};

		AddStroke(arAttributes);
		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"rect", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SETPIXELV(const TPointL &oPoint, const TRGBA &oColor)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_SMALLTEXTOUT(const TEmfSmallTextout &oText)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEANDFILLPATH(const TRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		const CPath* pPath = m_pParser->GetPath();
		
		if (NULL == pPath)
			return;
		
		const std::wstring wsValue = CreatePath(*pPath);;

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddFill(arAttributes, std::fabs(oBounds.Right - oBounds.Left), std::fabs(oBounds.Bottom - oBounds.Top));
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_STROKEPATH(const TRectL &oBounds)
	{
		if (NULL == m_pParser)
			return;

		const CPath* pPath = m_pParser->GetPath();
		
		if (NULL == pPath)
			return;
		
		const std::wstring wsValue = CreatePath(*pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path" , arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_GRADIENTFILL(const std::vector<TTriVertex> &arVertex, const std::vector<std::pair<int, int> > &arIndexes, unsigned int unFillMode)
	{
		if (arVertex.empty())
			return;

		NodeAttributes arAttributes;

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		if (4 == arVertex.size())
		{
			arAttributes.push_back({L"x",      ConvertToWString(std::min(arVertex[0].nX, arVertex[1].nX))});
			arAttributes.push_back({L"y",      ConvertToWString(std::min(arVertex[0].nY, arVertex[2].nY))});
			arAttributes.push_back({L"width",  ConvertToWString(std::abs(arVertex[1].nX - arVertex[0].nX))});
			arAttributes.push_back({L"height", ConvertToWString(std::abs(arVertex[2].nY - arVertex[0].nY))});
			WriteNode(L"rect" , arAttributes);
		}
		else if (3 == arVertex.size())
		{
			CPath oPath;
			oPath.MoveTo(arVertex[0].nX, arVertex[0].nY);
			oPath.LineTo(arVertex[1].nX, arVertex[1].nY);
			oPath.LineTo(arVertex[2].nX, arVertex[2].nY);
			oPath.Close();

			std::wstring wsValue = CreatePath(oPath);

			if (!wsValue.empty())
				arAttributes.push_back({L"d", wsValue});

			WriteNode(L"path" , arAttributes);
		}
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FILLRGN(const TRectL &oBounds, unsigned int unIhBrush, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Top) + L' ' +
			            L"L " + ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Top) + L' ' +
			                    ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Top) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_PAINTRGN(const TRectL &oBounds, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || arRects.empty())
			return;

		std::wstring wsValue;

		TRectD oTempRect;

		for (const TRectL& oRect : arRects)
		{
			oTempRect = TranslateRect(oRect);

			wsValue +=	L"M " + ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Top) + L' ' +
			            L"L " + ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Top) + L' ' +
			                    ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Top) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMR_FRAMERGN(const TRectL &oBounds, unsigned int unIhBrush, int nWidth, int nHeight, const TRegionDataHeader &oRegionDataHeader, const std::vector<TRectL> &arRects)
	{
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_OFFSETCLIP(double dX, double dY)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_RESETCLIP()
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPPATH(short unShFlags, const CEmfPlusPath *pPath)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPRECT(short shCM, const TEmfPlusRectF &oRect)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_SETCLIPREGION(short shObjectIndex, short shCM, const CEmfPlusRegion *pRegion)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_ENDOFFILE()
	{
		CloseClip();
	}
	
	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_GETDC()
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_CLEAR(const TEmfPlusARGB &oARGB)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWARC(BYTE chOgjectIndex, double dStartAngle, double dSweepAngle, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.ToRectD();

		if (NULL != m_pParser && m_pParser->GetTransform().M22 < 0)
		{
			dStartAngle *= -1;
			dSweepAngle *= -1;
		}

		double dXRadius = std::fabs((oNewRect.Right - oNewRect.Left)) / 2;
		double dYRadius = std::fabs((oNewRect.Bottom - oNewRect.Top)) / 2;

		std::wstring wsValue = L"M " + ConvertToWString(oNewRect.Left) + L' ' + ConvertToWString(oNewRect.Top);

		wsValue += L" A " + ConvertToWString(dXRadius) + L' ' +
		           ConvertToWString(dYRadius) + L' ' +
		           L"0 " +
		           ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"0" : L"1") + L' ' +
		           ((std::fabs(dSweepAngle - dStartAngle) <= 180) ? L"1" : L"0") + L' ' +
		           ConvertToWString(oNewRect.Right) + L' ' +
		           ConvertToWString(oNewRect.Bottom);

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

		AddClip();

		WriteNode(L"path" , arAttributes);
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
		AddClip();

		WriteNode(L"path", arAttributes);
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

		const IFont *pFont = m_pParser->GetFont();

		if (NULL == pFont)
			return;

		const std::wstring wsText = StringNormalization(wsString);

		if (wsText.empty())
			return;

		NodeAttributes arNodeAttributes;

		TXForm oTransform;
		oTransform.Copy(m_pParser->GetTransform());

		int nColor = m_pParser->GetTextColor();

		if (0 != nColor)
			arNodeAttributes.push_back({L"fill", CalculateColor(nColor, 255)});

		double dFontHeight = std::fabs(pFont->GetHeight());

		if (dFontHeight < 0.01)
			dFontHeight = 18;

		arNodeAttributes.push_back({L"font-size", ConvertToWString(dFontHeight)});

		std::wstring wsFontName = pFont->GetFaceName();

#ifndef BUILDING_WASM_MODULE
		if (!wsFontName.empty())
		{
			NSFonts::CFontSelectFormat oFormat;
			oFormat.wsName = new std::wstring(pFont->GetFaceName());

			NSFonts::CFontInfo *pFontInfo = m_pParser->GetFontManager()->GetFontInfoByParams(oFormat);

			if (NULL != pFontInfo && !StringEquals(wsFontName, pFontInfo->m_wsFontName))
				wsFontName = L"&apos;" + wsFontName + L"&apos;, &apos;" + pFontInfo->m_wsFontName + L"&apos;";
		}
#endif

		if (!wsFontName.empty())
			arNodeAttributes.push_back({L"font-family", wsFontName});

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

		AddTransform(arNodeAttributes);
		AddClip();

		std::wstring wsX, wsY;

		for (const TPointD& oPoint : arGlyphPos)
		{
			wsX += ConvertToWString(oPoint.X) + L' ';
			wsY += ConvertToWString(oPoint.Y) + L' ';
		}

		wsX.pop_back();
		wsY.pop_back();

		arNodeAttributes.push_back({L"x", wsX});
		arNodeAttributes.push_back({L"y", wsY});

		WriteNode(L"text", arNodeAttributes, wsText);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWELLIPSE(short shOgjectIndex, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.ToRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.Left   + oNewRect.Right)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.Top    + oNewRect.Bottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.Right  - oNewRect.Left)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.Bottom - oNewRect.Top)    / 2)}};
		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"ellipse", arAttributes);
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
		AddClip();

		WriteNode(L"polyline", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWPATH(short shOgjectIndex, unsigned int unPenId, const CPath *pPath)
	{
		if (NULL == m_pParser || NULL == pPath)
			return;

		const std::wstring wsValue = CreatePath(*pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path" , arAttributes);
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
			oTempRect = oRect.ToRectD();

			wsValue +=	L"M "  + ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Top) +
			            L" L " + ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Top) + L' ' +
			                     ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                     ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                     ConvertToWString(oTempRect.Left)	+ L',' + ConvertToWString(oTempRect.Top) + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddStroke(arAttributes);
		AddNoneFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_DRAWSTRING(short shOgjectIndex, unsigned int unBrushId, unsigned int unFormatID, const std::wstring &wsString, const TEmfPlusRectF &oRect)
	{
		WriteText(wsString, TPointD(oRect.dX, oRect.dY), oRect.ToRectL(), TPointD(m_pParser->GetTransform().M11, m_pParser->GetTransform().M22));
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLCLOSEDCURVE(unsigned int unBrushId, double dTension, const std::vector<TEmfPlusRectF> &arPoints)
	{

	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLELLIPSE(unsigned int unBrushId, const TEmfPlusRectF &oRect)
	{
		TRectD oNewRect = oRect.ToRectD();

		NodeAttributes arAttributes = {{L"cx", ConvertToWString((oNewRect.Left   + oNewRect.Right)  / 2)},
		                               {L"cy", ConvertToWString((oNewRect.Top    + oNewRect.Bottom) / 2)},
		                               {L"rx", ConvertToWString((oNewRect.Right  - oNewRect.Left)   / 2)},
		                               {L"ry", ConvertToWString((oNewRect.Bottom - oNewRect.Top)    / 2)}};

		AddFill(arAttributes);
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"ellipse", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLPATH(short shOgjectIndex, unsigned int unBrushId, const CEmfPlusPath *pPath)
	{
		if (NULL == m_pParser || NULL == pPath)
			return;

		std::wstring wsValue = CreatePath(*pPath);

		if (wsValue.empty())
			return;

		NodeAttributes arAttributes = {{L"d", wsValue}};

		TRectD oPathRect = pPath->GetBounds();

		AddFill(arAttributes, std::fabs(oPathRect.Right - oPathRect.Left), std::fabs(oPathRect.Bottom - oPathRect.Top));
		AddTransform(arAttributes);
		AddClip();

		WriteNode(L"path" , arAttributes);
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
		AddClip();

		WriteNode(L"polygon", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLRECTS(unsigned int unBrushId, const std::vector<TEmfPlusRectF> &arRects)
	{
		std::wstring wsValue;

		TRectD oTempRect;

		for (const TEmfPlusRectF& oRect : arRects)
		{
			oTempRect = oRect.ToRectD();

			m_pParser->GetTransform().Apply(oTempRect.Left,  oTempRect.Top);
			m_pParser->GetTransform().Apply(oTempRect.Right, oTempRect.Bottom);

			wsValue += L"M "  + ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Top)    +
			           L" L " + ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Top)    + L' ' +
			                    ConvertToWString(oTempRect.Right) + L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Bottom) + L' ' +
			                    ConvertToWString(oTempRect.Left)  + L',' + ConvertToWString(oTempRect.Top)    + L' ';
		}

		NodeAttributes arAttributes = {{L"d", wsValue}};

		AddFill(arAttributes);
		AddClip();

		WriteNode(L"path", arAttributes);
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_FILLREGION(short shOgjectIndex, unsigned int unBrushId)
	{
		
	}

	void CEmfInterpretatorSvg::HANDLE_EMFPLUS_RESTORE(unsigned int)
	{
		m_bUpdatedClip = false;
	}

	void CEmfInterpretatorSvg::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		CInterpretatorSvgBase::DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
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
	
	void CEmfInterpretatorSvg::PathClip(const CPath &oPath, int nClipMode, TXForm *pTransform)
	{
		CInterpretatorSvgBase::PathClip(oPath, nClipMode, pTransform);
	}

	TRectD CEmfInterpretatorSvg::TranslateRect(const TRectL &oRect) const
	{
		TRectD oNewRect(oRect.Left, oRect.Top, oRect.Right, oRect.Bottom);

		if (oNewRect.Right < oNewRect.Left)
		{
			double dTempValue = oNewRect.Left;
			oNewRect.Left    = oNewRect.Right;
			oNewRect.Right   = dTempValue;
		}

		if (oNewRect.Bottom < oNewRect.Top)
		{
			double dTempValue = oNewRect.Top;
			oNewRect.Top     = oNewRect.Bottom;
			oNewRect.Bottom  = dTempValue;
		}

		return oNewRect;
	}
}

