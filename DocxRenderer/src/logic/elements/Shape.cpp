/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "Shape.h"

#include <limits.h>

#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	UINT CShape::m_gRelativeHeight = c_iStandartRelativeHeight;

	unsigned int ClampUIntSign(const double& value)
	{
		if (value < 0)
			return 0;
		if (value > 0x7FFFFFFF)
			return 0x7FFFFFFF;
		return (unsigned int)value;
	}

	CShape::CShape()
	{
		m_nRelativeHeight = m_gRelativeHeight;
		m_gRelativeHeight += c_iStandartRelativeHeight;
	}

	CShape::CShape(std::shared_ptr<CImageInfo> pInfo, const std::wstring& strDstMedia) :
	    m_strDstMedia(strDstMedia), m_pImageInfo(pInfo)
	{
		m_nRelativeHeight = m_gRelativeHeight;
		m_gRelativeHeight += c_iStandartRelativeHeight;
	}

	CShape::~CShape()
	{
		Clear();
	}

	void CShape::Clear()
	{
		m_arOutputObjects.clear();
		m_oVector.Clear();
	}

	UINT CShape::GenerateShapeId()
	{
		static UINT iId = 0;
		iId++;
		return iId;
	}

	void CShape::ResetRelativeHeight()
	{
		m_gRelativeHeight = c_iStandartRelativeHeight;
	}

	void CShape::SetVector(CVectorGraphics&& oVector)
	{
		m_oVector = std::move(oVector);
		auto arData = m_oVector.GetData();

		m_dLeft = m_oVector.GetLeft();
		m_dTop = m_oVector.GetTop();
		m_dWidth = m_oVector.GetRight() - m_dLeft;
		m_dHeight = m_oVector.GetBottom() - m_dTop;

		size_t nPeacks = 0;
		size_t nCurves = 0;

		for(auto& path_command : arData)
			switch (path_command.type)
			{
			case CVectorGraphics::ePathCommandType::pctMove:
				nPeacks++;
				break;

			case CVectorGraphics::ePathCommandType::pctLine:
				nPeacks++;
				break;

			case CVectorGraphics::ePathCommandType::pctCurve:
				nCurves++;
				break;

			case CVectorGraphics::ePathCommandType::pctClose:
			default:
				break;
			}

		DetermineGraphicsType(m_dWidth, m_dHeight, nPeacks, nCurves);

		if (m_dWidth < 0.0001)
			m_dWidth = 0.0001;
		if (m_dHeight < 0.0001)
			m_dHeight = 0.0001;

		m_dBot = m_dTop + m_dHeight;
		m_dRight = m_dLeft + m_dWidth;
	}

	void CShape::CalcNoRotVector()
	{
		m_oNoRotVector = m_oVector;
		m_oNoRotVector.Rotate(-m_dRotation);
	}

	bool CShape::TryMergeShape(std::shared_ptr<CShape>& pShape)
	{
		// can try to select dynamically, for example depending on size
		double dHorNearby = 30;
		double dVerNearby = 30;

		if (
		        // only for shapes
		        (pShape->m_eGraphicsType == eGraphicsType::gtComplicatedFigure ||
		         pShape->m_eGraphicsType == eGraphicsType::gtRectangle) &&

		        (this->m_eGraphicsType == eGraphicsType::gtComplicatedFigure ||
		         this->m_eGraphicsType == eGraphicsType::gtRectangle) &&

		        // everything matches
		        pShape->m_eType == this->m_eType &&
		        pShape->m_oPen.IsEqual(&m_oPen) &&
		        pShape->m_oBrush.IsEqual(&m_oBrush) &&
		        pShape->m_bIsNoFill == m_bIsNoFill &&
		        pShape->m_bIsNoStroke == m_bIsNoStroke &&

		        // not an image
		        pShape->m_pImageInfo == nullptr &&
		        this->m_pImageInfo == nullptr &&

		        // close to each other horizontally
		        (fabs(pShape->m_dRight - this->m_dLeft) < dHorNearby ||
		         fabs(pShape->m_dLeft - this->m_dRight) < dHorNearby ||

		         // also consider when inside each other
		         fabs(pShape->m_dRight - this->m_dRight) < dHorNearby ||
		         fabs(pShape->m_dLeft - this->m_dLeft) < dHorNearby) &&

		        // close to each other vertically
		        (fabs(pShape->m_dBot - this->m_dTop) < dVerNearby ||
		         fabs(pShape->m_dTop - this->m_dBot) < dVerNearby ||

		         // inside each other
		         fabs(pShape->m_dBot - this->m_dBot) < dVerNearby ||
		         fabs(pShape->m_dTop - this->m_dTop) < dVerNearby))
		{
			RecalcWithNewItem(pShape.get());
			m_oVector.Join(std::move(pShape->m_oVector));
			pShape = nullptr;

			this->m_eGraphicsType = eGraphicsType::gtComplicatedFigure;
			this->m_eLineType = eLineType::ltUnknown;
			this->m_eSimpleLineType = eSimpleLineType::sltUnknown;
			return true;
		}
		return false;
	}

	std::wstring CShape::PathToWString() const
	{
		auto& vector = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector : m_oVector;
		auto& data = vector.GetData();

		if (data.empty())
			return m_strDstMedia;

		double left = vector.GetLeft();
		double right = vector.GetRight();
		double top = vector.GetTop();
		double bot = vector.GetBottom();

		double width = right - left;
		double height = bot - top;

		NSStringUtils::CStringBuilder oWriter;

		oWriter.WriteString(L"<a:path w=\"");
		oWriter.AddUInt(static_cast<unsigned int>(width * c_dMMToEMU));
		oWriter.WriteString(L"\" h=\"");
		oWriter.AddUInt(static_cast<unsigned int>(height * c_dMMToEMU));
		oWriter.WriteString(L"\">");

		for(auto& path_command : data)
		{
			switch (path_command.type)
			{
			case CVectorGraphics::ePathCommandType::pctMove:
				oWriter.WriteString(L"<a:moveTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctLine:
				oWriter.WriteString(L"<a:lnTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctCurve:
				oWriter.WriteString(L"<a:cubicBezTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctClose:
			default:
				break;
			}

			for(auto& point : path_command.points)
			{
				LONG lX = static_cast<LONG>((point.x - left) * c_dMMToEMU);
				LONG lY = static_cast<LONG>((point.y - top) * c_dMMToEMU);

				oWriter.WriteString(L"<a:pt x=\"");
				oWriter.AddInt(static_cast<int>(lX));
				oWriter.WriteString(L"\" y=\"");
				oWriter.AddInt(static_cast<int>(lY));
				oWriter.WriteString(L"\"/>");
			}

			switch (path_command.type)
			{
			case CVectorGraphics::ePathCommandType::pctMove:
				oWriter.WriteString(L"</a:moveTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctLine:
				oWriter.WriteString(L"</a:lnTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctCurve:
				oWriter.WriteString(L"</a:cubicBezTo>");
				break;

			case CVectorGraphics::ePathCommandType::pctClose:
				oWriter.WriteString(L"<a:close/>");
				break;
			default:
				break;
			}
		}
		oWriter.WriteString(L"</a:path>");
		std::wstring strPath = oWriter.GetData();
		oWriter.ClearNoAttack();
		return strPath;
	}

	void CShape::DetermineGraphicsType(double dWidth, double dHeight,size_t nPeacks, size_t nCurves) noexcept
	{
		if ((m_bIsNoStroke && m_bIsNoFill) || (m_oBrush.Color1 == c_iWhiteColor && m_oPen.Color == c_iWhiteColor))
			m_eGraphicsType = eGraphicsType::gtNoGraphics;
		else if ((nPeacks == 5 || nPeacks == 2) && !nCurves) //1 move + 4 Peacks or 2 Peacks
		{
			m_eGraphicsType = eGraphicsType::gtRectangle;
			if (dHeight < 0.7)
			{
				if (dWidth > 2.0) // long dash - 2.8mm for times new roman
					m_eSimpleLineType = eSimpleLineType::sltHLongDash;
				else if (dWidth > 0.7) // minimum dash - 0.75mm for dotDotDash
					m_eSimpleLineType = eSimpleLineType::sltHDash;
				else
					m_eSimpleLineType = eSimpleLineType::sltHDot; // maximum dot - 0.5mm
			}
			else if (dWidth < 0.7)
			{
				if (dHeight > 2.0) // long dash - 2.8mm for times new roman
					m_eSimpleLineType = eSimpleLineType::sltVLongDash;
				else if (dHeight > 0.7) // minimum dash - 0.75mm for dotDotDash
					m_eSimpleLineType = eSimpleLineType::sltVDash;
				else
					m_eSimpleLineType = eSimpleLineType::sltVDot; // maximum dot - 0.5mm
			}
		}
		else if (nCurves > 0 &&  nPeacks <= 1) // 1 move
		{
			m_eGraphicsType = eGraphicsType::gtCurve;
			if (dHeight < dWidth)
				m_eSimpleLineType = eSimpleLineType::sltHWave;
			else
				m_eSimpleLineType = eSimpleLineType::sltVWave;
		}
		else if (nCurves > 0 &&  nPeacks > 1)
			m_eGraphicsType = eGraphicsType::gtComplicatedFigure;
	}

	bool CShape::IsItFitLine() const noexcept
	{
		return (m_eGraphicsType == eGraphicsType::gtRectangle && (
		            m_eSimpleLineType == eSimpleLineType::sltHDot ||
		            m_eSimpleLineType == eSimpleLineType::sltHDash ||
		            m_eSimpleLineType == eSimpleLineType::sltHLongDash)) ||
		        (m_eGraphicsType == eGraphicsType::gtCurve &&  m_eSimpleLineType == eSimpleLineType::sltHWave);
	}

	bool CShape::IsCorrelated(std::shared_ptr<const CShape> pShape) const noexcept
	{
		return m_eGraphicsType == pShape->m_eGraphicsType;
	}

	bool CShape::IsPeak() const noexcept
	{
		return m_eSimpleLineType == eSimpleLineType::sltHDot || m_eSimpleLineType == eSimpleLineType::sltVDot;
	}

	bool CShape::IsSide() const noexcept
	{
		return m_eSimpleLineType == eSimpleLineType::sltHLongDash || m_eSimpleLineType == eSimpleLineType::sltVLongDash;
	}
	bool CShape::IsOoxmlValid() const noexcept
	{
		bool is_offset = m_dLeft > c_dST_PositionOffsetMin && m_dTop > c_dST_PositionOffsetMin;
		bool is_width = m_dWidth > c_dST_PositiveCoordinatetMin && m_dWidth < c_dST_PositiveCoordinatetMax;
		bool is_height = m_dHeight > c_dST_PositiveCoordinatetMin && m_dHeight < c_dST_PositiveCoordinatetMax;
		return is_offset && is_width && is_height;
	}

	void CShape::CheckLineType(std::shared_ptr<CShape>& pFirstShape)
	{
		if (!pFirstShape)
			return;

		if (pFirstShape->m_eLineType == eLineType::ltUnknown && pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHLongDash)
			pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;

		else if (pFirstShape->m_eLineType == eLineType::ltUnknown && pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHWave)
			pFirstShape->m_eLineType = pFirstShape->m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
	}
	void CShape::CheckLineType(std::shared_ptr<CShape>& pFirstShape, std::shared_ptr<CShape>& pSecondShape, bool bIsLast)
	{
		if (!pFirstShape || !pSecondShape)
			return;

		if (!pFirstShape->IsItFitLine() || !pSecondShape->IsItFitLine() || !pFirstShape->IsCorrelated(pSecondShape) ||
		        fabs(pFirstShape->m_dHeight - pSecondShape->m_dHeight) > c_dGRAPHICS_ERROR_IN_LINES_MM)
		{
			return; // line must be the same height
		}

		auto set_line_type = [] (std::shared_ptr<CShape>& master, std::shared_ptr<CShape>& slave, eLineType type) {
			master->m_eLineType = type;
			master->RecalcWithNewItem(slave.get());
			master->m_oVector.Join(std::move(slave->m_oVector));
			slave = nullptr;
		};

		// check for double line
		if (pFirstShape->m_eLineType == eLineType::ltDouble)
		{
			if (pFirstShape->m_dTop < pSecondShape->m_dTop)
				set_line_type(pFirstShape, pSecondShape, eLineType::ltDouble);
			else
				set_line_type(pSecondShape, pFirstShape, eLineType::ltDouble);

			return;
		}
		if (pFirstShape->m_eLineType == eLineType::ltWavyDouble)
		{
			if (pFirstShape->m_dTop < pSecondShape->m_dTop)
				set_line_type(pFirstShape, pSecondShape, eLineType::ltWavyDouble);
			else
				set_line_type(pSecondShape, pFirstShape, eLineType::ltWavyDouble);

			return;
		}

		else if (fabs(pFirstShape->m_dTop - pSecondShape->m_dTop) < c_dGRAPHICS_ERROR_IN_LINES_MM * 5 &&
		         fabs(pFirstShape->m_dWidth - pSecondShape->m_dWidth) < c_dGRAPHICS_ERROR_IN_LINES_MM &&
		         fabs(pFirstShape->m_dLeft - pSecondShape->m_dLeft) < c_dGRAPHICS_ERROR_IN_LINES_MM)
		{
			// first definition condition
			if (pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHLongDash && pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHLongDash)
			{
				if (pFirstShape->m_dTop < pSecondShape->m_dTop)
					set_line_type(pFirstShape, pSecondShape, eLineType::ltDouble);
				else
					set_line_type(pSecondShape, pFirstShape, eLineType::ltDouble);
			}
			else if (pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHWave && pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHWave)
			{
				if (pFirstShape->m_dTop < pSecondShape->m_dTop)
					set_line_type(pFirstShape, pSecondShape, eLineType::ltWavyDouble);
				else
					set_line_type(pSecondShape, pFirstShape, eLineType::ltWavyDouble);
			}
			return;
		}
		else if (fabs(pFirstShape->m_dTop - pSecondShape->m_dTop) > c_dGRAPHICS_ERROR_IN_LINES_MM)
			return; // everything must be on one line

		// now we assume that graphics are on one line
		// distance between objects on one line should be small
		if (fabs(pFirstShape->m_dLeft + pFirstShape->m_dWidth - pSecondShape->m_dLeft) > c_dGRAPHICS_ERROR_IN_LINES_MM * 5)
		{
			if (pFirstShape->m_eLineType == eLineType::ltUnknown && pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHLongDash)
				pFirstShape->m_eLineType = pFirstShape-> m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;

			else if (pFirstShape->m_eLineType == eLineType::ltUnknown && pFirstShape->m_eSimpleLineType == eSimpleLineType::sltHWave)
				pFirstShape->m_eLineType = pFirstShape->m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;

			return;
		}

		if (bIsLast)
		{
			// if we have only 2 shapes in line, need to specifically determine type
			if (pFirstShape->m_eLineType == eLineType::ltUnknown)
			{
				switch (pFirstShape->m_eSimpleLineType)
				{
				case eSimpleLineType::sltHDot:
					if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDot)
						pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDottedHeavy : eLineType::ltDotted;

					break;

				case eSimpleLineType::sltHDash:
					if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDash)
						pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashedHeavy : eLineType::ltDash;

					else if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDot)
						pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashDotHeavy : eLineType::ltDotDash;

					break;

				case eSimpleLineType::sltHLongDash:
					if (fabs(pFirstShape->m_dLeft + pFirstShape->m_dWidth - pSecondShape->m_dLeft) < 0.7)
						pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;

					else
						pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashLongHeavy : eLineType::ltDashLong;

					break;

				case eSimpleLineType::sltHWave:
					if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHWave)
						pFirstShape->m_eLineType = pFirstShape->m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;

					break;
				default:
					break;
				}
			}

			pFirstShape->RecalcWithNewItem(pSecondShape.get());
			pFirstShape->m_oVector.Join(std::move(pSecondShape->m_oVector));
			pSecondShape = nullptr;
			return;
		}

		bool passed = false;
		switch (pFirstShape->m_eSimpleLineType)
		{
		case eSimpleLineType::sltHDot:
			if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDot)
			{
				if ((pFirstShape->m_eLineType == eLineType::ltUnknown || pFirstShape->m_eLineType == eLineType::ltDotted ||
				     pFirstShape->m_eLineType == eLineType::ltDottedHeavy) && pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDottedHeavy : eLineType::ltDotted;
					passed = true;
				}
				else if ((pFirstShape->m_eLineType == eLineType::ltDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotHeavy ||
				          pFirstShape->m_eLineType == eLineType::ltDotDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotDotHeavy) &&
				         pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashDotDotHeavy : eLineType::ltDotDotDash;
					pFirstShape->m_eSimpleLineType = eSimpleLineType::sltHDot;
					passed = true;
				}
			}
			else if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDash)
			{
				if ((pFirstShape->m_eLineType == eLineType::ltDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotHeavy) &&
				        pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eSimpleLineType = eSimpleLineType::sltHDash;
					passed = true;
				}
				else if ((pFirstShape->m_eLineType == eLineType::ltDotDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotDotHeavy) &&
				         pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eSimpleLineType = eSimpleLineType::sltHDash;
					passed = true;
				}
			}
			break;

		case eSimpleLineType::sltHDash:
			if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDash)
			{
				if ((pFirstShape->m_eLineType == eLineType::ltUnknown || pFirstShape->m_eLineType == eLineType::ltDash ||
				     pFirstShape->m_eLineType == eLineType::ltDashedHeavy) && pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashedHeavy : eLineType::ltDash;
					passed = true;
				}
				else if ((pFirstShape->m_eLineType == eLineType::ltDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotHeavy) &&
				         pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					passed = true;
				}
			}
			else if (pSecondShape->m_eSimpleLineType == eSimpleLineType::sltHDot)
			{
				if ((pFirstShape->m_eLineType == eLineType::ltUnknown || pFirstShape->m_eLineType == eLineType::ltDotDash ||
				     pFirstShape->m_eLineType == eLineType::ltDashDotHeavy) && pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashDotHeavy : eLineType::ltDotDash;
					pFirstShape->m_eSimpleLineType = eSimpleLineType::sltHDot;
					passed = true;
				}
				else if ((pFirstShape->m_eLineType == eLineType::ltDotDotDash || pFirstShape->m_eLineType == eLineType::ltDashDotDotHeavy) &&
				         pSecondShape->m_eLineType == eLineType::ltUnknown)
				{
					pFirstShape->m_eSimpleLineType = eSimpleLineType::sltHDot;
					passed = true;
				}
			}
			break;

		case eSimpleLineType::sltHLongDash:
			if (fabs(pFirstShape->m_dLeft +pFirstShape->m_dWidth - pSecondShape->m_dLeft) < 0.7 ||
			        pFirstShape->m_eLineType == eLineType::ltThick || pFirstShape->m_eLineType == eLineType::ltSingle)
			{
				pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltThick : eLineType::ltSingle;
				passed = true;
			}
			else if ((pFirstShape->m_eLineType == eLineType::ltUnknown || pFirstShape->m_eLineType == eLineType::ltDashLong ||
			          pFirstShape->m_eLineType == eLineType::ltDashLongHeavy) && pSecondShape->m_eLineType == eLineType::ltUnknown)
			{
				pFirstShape->m_eLineType = pFirstShape->m_dHeight > 0.3 ? eLineType::ltDashLongHeavy : eLineType::ltDashLong;
				passed = true;
			}
			break;

		case eSimpleLineType::sltHWave:
			if ((pFirstShape->m_eLineType == eLineType::ltUnknown || pFirstShape->m_eLineType == eLineType::ltWave ||
			     pFirstShape->m_eLineType == eLineType::ltWavyHeavy || pFirstShape->m_eLineType == eLineType::ltWavyDouble) &&
			        pSecondShape->m_eLineType == eLineType::ltUnknown)
			{
				pFirstShape->m_eLineType = pFirstShape->m_oPen.Size > 0.3 ? eLineType::ltWavyHeavy : eLineType::ltWave;
				passed = true;
			}
			break;
		default:
			break;
		}

		if (passed)
		{
			pFirstShape->RecalcWithNewItem(pSecondShape.get());
			pFirstShape->m_oVector.Join(std::move(pSecondShape->m_oVector));
			pSecondShape = nullptr;
		}
	}

	void CShape::ToXml(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<w:r>");
		oWriter.WriteString(L"<w:rPr><w:noProof/></w:rPr>");
		oWriter.WriteString(L"<w:drawing>");
		BuildGeneralProperties(oWriter);
		oWriter.WriteString(L"</w:drawing>");
		oWriter.WriteString(L"</w:r>");
	}

	void CShape::BuildGeneralProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<wp:anchor");
		oWriter.WriteString(L" distT=\"0\""); // Specifies the minimum distance to maintain between the edge
		oWriter.WriteString(L" distB=\"0\""); // of this drawing object and any subsequent text in document when
		oWriter.WriteString(L" distL=\"0\""); // this graphic object is displayed in document content.
		oWriter.WriteString(L" distR=\"0\"");
		oWriter.WriteString(L" simplePos=\"0\""); // true/1 Specifies that this object should be positioned using positioning info in simplePos child element
		oWriter.WriteString(L" relativeHeight=\""); // Specifies relative Z-ordering of all DrawingML objects in this document.
		oWriter.AddUInt(m_nRelativeHeight);
		oWriter.WriteString(L"\"");
		oWriter.WriteString(L" behindDoc=\""); // behind text - 1, in front of text - 0
		oWriter.AddUInt(static_cast<UINT>(m_bIsBehindDoc));
		oWriter.WriteString(L"\"");
		oWriter.WriteString(L" locked=\"0\""); // true/1 Specifies that anchor location for this object should not be changed at runtime when app edits document content.
		oWriter.WriteString(L" layoutInCell=\"0\""); // object will be positioned as specified, but table will be resized and/or moved in document as needed to accommodate object.
		oWriter.WriteString(L" allowOverlap=\"1\""); // allowed to overlap content of another object
		oWriter.WriteString(L" hidden=\"0\""); // Specifies whether this floating DrawingML object will be displayed.
		oWriter.WriteString(L">");

		oWriter.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");

		double left = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetLeft() : m_dLeft;
		double right = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetRight() : m_dRight;
		double top = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetTop() : m_dTop;
		double bot = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetBottom() : m_dBot;

		double width = right - left;
		double height = bot - top;

		oWriter.WriteString(L"<wp:positionH relativeFrom=\"page\">");
		oWriter.WriteString(L"<wp:posOffset>");
		oWriter.AddInt64(static_cast<int64_t>(left * c_dMMToEMU));
		oWriter.WriteString(L"</wp:posOffset>");
		oWriter.WriteString(L"</wp:positionH>");

		oWriter.WriteString(L"<wp:positionV relativeFrom=\"page\">");
		oWriter.WriteString(L"<wp:posOffset>");
		oWriter.AddInt64(static_cast<int64_t>(top * c_dMMToEMU));
		oWriter.WriteString(L"</wp:posOffset>");
		oWriter.WriteString(L"</wp:positionV>");

		// shape boundary end coordinates
		oWriter.WriteString(L"<wp:extent");
		oWriter.WriteString(L" cx=\"");
		oWriter.AddUInt(static_cast<unsigned int>(width * c_dMMToEMU));
		oWriter.WriteString(L"\" cy=\"");
		oWriter.AddUInt(static_cast<unsigned int>(height * c_dMMToEMU));
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>"); // This element specifies additional distance to be added to each edge of image to compensate for any drawing effects applied to DrawingML object
		oWriter.WriteString(L"<wp:wrapNone/>");

		oWriter.WriteString(L"<wp:docPr id=\"");
		oWriter.AddUInt(GenerateShapeId());
		switch (m_eType)
		{
		case eShapeType::stTextBox:
			oWriter.WriteString(L"\" name=\"Text Box ");
			break;
		case eShapeType::stVectorGraphics:
			oWriter.WriteString(L"\" name=\"Freeform: Shape ");
			break;
		case eShapeType::stVectorTexture:
			oWriter.WriteString(L"\" name=\"Freeform: Texture ");
			break;
		case eShapeType::stCanvas:
			oWriter.WriteString(L"\" name=\"Canvas ");
			break;
		case eShapeType::stGroup:
			oWriter.WriteString(L"\" name=\"Group ");
			break;
		default:
			oWriter.WriteString(L"\" name=\"Shape ");
			break;
		}
		oWriter.AddUInt(m_nShapeId);
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"<wp:cNvGraphicFramePr/>");
		BuildSpecificProperties(oWriter);
		oWriter.WriteString(L"</wp:anchor>");
	}

	void CShape::BuildSpecificProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");

		switch (m_eType)
		{
		case eShapeType::stVectorTexture:
			BuildPictureProperties(oWriter);
			break;
		case eShapeType::stCanvas:
			BuildCanvasProperties(oWriter);
			break;
		case eShapeType::stGroup:
			BuildGroupProperties(oWriter);
			break;
		case eShapeType::stTextBox:
		case eShapeType::stVectorGraphics:
		default:
			BuildShapeProperties(oWriter);
			break;
		}

		oWriter.WriteString(L"</a:graphic>");
	}

	void CShape::BuildShapeProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");

		oWriter.WriteString(L"<wps:wsp>");

		oWriter.WriteString(L"<wps:cNvSpPr/>"); //non-visual shape properties. http://officeopenxml.com/drwSp-nvSpPr.php

		oWriter.WriteString(L"<wps:spPr>"); //shape properties. http://officeopenxml.com/drwSp-SpPr.php

		// does not work
		//oWriter.WriteString(L"<wps:style/>"); //shape styles. http://officeopenxml.com/drwSp-styles.php
		BuildForm(oWriter);
		BuildGraphicProperties(oWriter);

		oWriter.WriteString(L"</wps:spPr>");

		BuildTextBox(oWriter);

		oWriter.WriteString(L"</wps:wsp>");

		oWriter.WriteString(L"</a:graphicData>");
	}
	void CShape::BuildBlipFill(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<a:blip r:embed=\"rId");
		oWriter.AddUInt(c_iStartingIdForImages + m_pImageInfo->m_nId);
		oWriter.WriteString(L"\">");
		oWriter.WriteString(L"<a:alphaModFix/>");
		oWriter.WriteString(L"</a:blip>");
		if (m_oBrush.Image != NULL)
		{
			oWriter.WriteString(L"<a:tile sx=\"100%\" sy=\"100%\" flip=\"none\" algn=\"tl\"/>");
		}
		else
		{
			double left = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetLeft() : m_dLeft;
			double right = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetRight() : m_dRight;
			double top = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetTop() : m_dTop;
			double bot = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetBottom() : m_dBot;

			double height = bot - top;
			double width = right - left;

			// coeff
			double offset_left = (right - m_dImageLeft) / width - 1;
			double offset_right = (m_dImageRight - left) / width - 1;
			double offset_top = (bot - m_dImageTop) / height - 1;
			double offset_bot = (m_dImageBot - top) / height - 1;

			// percentage
			offset_left *= 100;
			offset_right *= 100;
			offset_top *= 100;
			offset_bot *= 100;

			// ooxml (percentage * 1000)
			oWriter.WriteString(L"<a:srcRect/>");
			oWriter.WriteString(L"<a:stretch>");
			oWriter.WriteString(L"<a:fillRect ");
			oWriter.WriteString(L"l=\"");
			oWriter.AddInt(static_cast<int>(-offset_left * 1000));
			oWriter.WriteString(L"\" ");
			oWriter.WriteString(L"r=\"");
			oWriter.AddInt(static_cast<int>(-offset_right * 1000));
			oWriter.WriteString(L"\" ");
			oWriter.WriteString(L"t=\"");
			oWriter.AddInt(static_cast<int>(-offset_top * 1000));
			oWriter.WriteString(L"\" ");
			oWriter.WriteString(L"b=\"");
			oWriter.AddInt(static_cast<int>(-offset_bot * 1000));
			oWriter.WriteString(L"\"");
			oWriter.WriteString(L"/>");
			oWriter.WriteString(L"</a:stretch>");
		}
	}

	void CShape::BuildPictureProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");

		oWriter.WriteString(L"<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
		oWriter.WriteString(L"<pic:nvPicPr>");
		oWriter.WriteString(L"<pic:cNvPr id=\"");
		oWriter.AddUInt(m_pImageInfo->m_nId);
		oWriter.WriteString(L"\" name=\"Picture ");
		oWriter.AddUInt(m_pImageInfo->m_nId);
		oWriter.WriteString(L"\"");
		oWriter.WriteString(L"/>");
		oWriter.WriteString(L"<pic:cNvPicPr preferRelativeResize=\"0\">");
		oWriter.WriteString(L"<a:picLocks noChangeArrowheads=\"1\"/>");
		oWriter.WriteString(L"</pic:cNvPicPr>");
		oWriter.WriteString(L"</pic:nvPicPr>");
		oWriter.WriteString(L"<pic:blipFill>");
		BuildBlipFill(oWriter);
		oWriter.WriteString(L"</pic:blipFill>");
		oWriter.WriteString(L"<pic:spPr bwMode=\"auto\">");

		BuildForm(oWriter);
		BuildGraphicProperties(oWriter);

		oWriter.WriteString(L"</pic:spPr>");
		oWriter.WriteString(L"</pic:pic>");

		oWriter.WriteString(L"</a:graphicData>");
	}

	void CShape::BuildGroupProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\">");
		oWriter.WriteString(L"<wpg:wgp>");
		oWriter.WriteString(L"<wpg:cNvGrpSpPr/>");
		oWriter.WriteString(L"<wpg:grpSpPr>");
		BuildForm(oWriter);
		BuildGraphicProperties(oWriter);
		oWriter.WriteString(L"</wpg:grpSpPr>");

		// todo add any number of elements in group
		BuildPictureProperties(oWriter);
		BuildShapeProperties(oWriter);

		oWriter.WriteString(L"</wps:wsp>");
		oWriter.WriteString(L"</wpg:wgp>");
		oWriter.WriteString(L"</a:graphicData>");
	}

	void CShape::BuildCanvasProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		// todo add implementation
		oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\">");
	}

	void CShape::BuildGraphicProperties(NSStringUtils::CStringBuilder &oWriter) const
	{
		std::wstring strPath = PathToWString();

		// If just text without graphics
		if (strPath.empty())
		{
			oWriter.WriteString(L"<a:prstGeom prst=\"rect\">");
			oWriter.WriteString(L"<a:avLst/>");
			oWriter.WriteString(L"</a:prstGeom>");
		}
		else
		{
			// Draw complex path
			oWriter.WriteString(L"<a:custGeom>");
			oWriter.WriteString(L"<a:avLst/>");
			oWriter.WriteString(L"<a:gdLst/>");
			oWriter.WriteString(L"<a:ahLst/>");
			oWriter.WriteString(L"<a:cxnLst/>");
			oWriter.WriteString(L"<a:rect l=\"l\" t=\"t\" r=\"r\" b=\"b\"/>");
			oWriter.WriteString(L"<a:pathLst>");
			oWriter.WriteString(strPath);
			oWriter.WriteString(L"</a:pathLst>");
			oWriter.WriteString(L"</a:custGeom>");
		}

		if (m_bIsNoFill)
		{
			oWriter.WriteString(L"<a:noFill/>");
		}
		else if (m_eType != CShape::eShapeType::stVectorTexture)
		{
			// Has fill
			oWriter.WriteString(L"<a:solidFill>");
			oWriter.WriteString(L"<a:srgbClr val=\"");
			oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oBrush.Color1)));
			if (0xFF != m_oBrush.Alpha1)
			{
				oWriter.WriteString(L"\"><a:alpha val=\"");
				oWriter.AddInt(static_cast<int>(m_oBrush.Alpha1 / 255.0 * 100.0 * 1000));
				oWriter.WriteString(L"\"/></a:srgbClr>");
			}
			else
			{
				oWriter.WriteString(L"\"/>");
			}
			oWriter.WriteString(L"</a:solidFill>");
		}

		if (m_bIsNoStroke)
		{
			oWriter.WriteString(L"<a:ln>"); // w - width default is 0.75pt = 9525
			oWriter.WriteString(L"<a:noFill/>");
			oWriter.WriteString(L"</a:ln>");
		}
		else
		{
			oWriter.WriteString(L"<a:ln w=\"");
			// todo Some m_oPen.Size values come multiplied by 10
			oWriter.AddInt(static_cast<int>(m_oPen.Size * c_dMMToEMU)); // note can write in mm
			oWriter.WriteString(L"\">");

			oWriter.WriteString(L"<a:solidFill>");

			oWriter.WriteString(L"<a:srgbClr val=\"");
			oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oPen.Color))); // note can use words like "black" instead of color
			if (0xFF != m_oPen.Alpha)
			{
				oWriter.WriteString(L"\"><a:alpha val=\"");
				oWriter.AddInt(static_cast<int>(m_oPen.Alpha / 255.0 * 100.0));
				oWriter.WriteString(L"%\"/></a:srgbClr>");
			}
			else
			{
				oWriter.WriteString(L"\"/>");
			}

			oWriter.WriteString(L"</a:solidFill>");
			if (m_oPen.DashStyle == Aggplus::DashStyleCustom)
			{
				// dash pattern
				std::vector<double> dash_pattern(m_oPen.Count);
				long count = m_oPen.Count;
				m_oPen.GetDashPattern(dash_pattern.data(), count);

				oWriter.WriteString(L"<a:custDash>");
				for (size_t i = 0; i < count; i+= 2)
				{
					double to_percentage = 100.0 * 1000.0 / m_oPen.Size;
					oWriter.WriteString(L"<a:ds d=\"");
					oWriter.AddUInt(ClampUIntSign(dash_pattern[i] * to_percentage));
					oWriter.WriteString(L"\" ");
					oWriter.WriteString(L"sp=\"");
					oWriter.AddUInt(ClampUIntSign(dash_pattern[i + 1] * to_percentage));
					oWriter.WriteString(L"\" />");
				}
				oWriter.WriteString(L"</a:custDash>");
			}
			// Aggplus::DashStyleSolid
			oWriter.WriteString(L"</a:ln>");
		}
	}

	void CShape::BuildForm(NSStringUtils::CStringBuilder &oWriter, const bool& bIsLT) const
	{
		double left = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetLeft() : m_dLeft;
		double right = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetRight() : m_dRight;
		double top = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetTop() : m_dTop;
		double bot = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetBottom() : m_dBot;

		double height = bot - top;
		double width = right - left;

		// responsible for rectangular shape frame dimensions
		oWriter.WriteString(L"<a:xfrm");
		if (fabs(m_dRotation) > c_dMIN_ROTATION)
		{
			oWriter.WriteString(L" rot=\"");
			oWriter.AddInt(static_cast<int>(m_dRotation * c_dDegreeToAngle));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L">");

		if (!bIsLT)
		{
			oWriter.WriteString(L"<a:off x=\"0\" y=\"0\"/>");
		}
		else
		{
			oWriter.WriteString(L"<a:off x=\"");
			oWriter.AddInt64(static_cast<int64_t>(left * c_dMMToEMU));
			oWriter.WriteString(L"\" y=\"");
			oWriter.AddInt64(static_cast<int64_t>(top * c_dMMToEMU));
			oWriter.WriteString(L"\"/>");
		}

		oWriter.WriteString(L"<a:ext");
		oWriter.WriteString(L" cx=\"");
		oWriter.AddUInt(static_cast<unsigned int>(width * c_dMMToEMU));
		oWriter.WriteString(L"\" cy=\"");
		oWriter.AddUInt(static_cast<unsigned int>(height * c_dMMToEMU));
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"</a:xfrm>");
	}

	void CShape::BuildTextBoxParams(NSStringUtils::CStringBuilder &oWriter) const
	{
		oWriter.WriteString(L" rot=\"0\""); //Defines the rotation applied to text within the bounding box.
		oWriter.WriteString(L" spcFirstLastPara=\"0\""); //Whether user-specified paragraph spacing before and after should be respected.
		oWriter.WriteString(L" vertOverflow=\"overflow\""); //Whether text can overflow the bounding box vertically
		oWriter.WriteString(L" horzOverflow=\"overflow\""); //Whether text can overflow the bounding box horizontally.
		oWriter.WriteString(L" vert=\"horz\"");
		//oWriter.WriteString(L" wrap=\"none\""); //Shape boundary by text width
		oWriter.WriteString(L" wrap=\"square\""); //Defines wrap parameters to be used for this text body.
		//How much the text boundary is indented from the shape boundary
		oWriter.WriteString(L" lIns=\"0\""); //left   default 0.25cm = 91440
		oWriter.WriteString(L" tIns=\"0\""); //top    default 0.13cm = 45720
		oWriter.WriteString(L" rIns=\"0\""); //right  default 0.25cm
		oWriter.WriteString(L" bIns=\"0\""); //bottom default 0.13cm
		oWriter.WriteString(L" numCol=\"1\""); //Defines the number of text columns in the bounding rectangle.
		oWriter.WriteString(L" spcCol=\"0\""); //Defines space between text columns in the text area (only if numCol >1)
		oWriter.WriteString(L" rtlCol=\"0\""); //Whether columns are used right-to-left (true) or left-to-right (false).
		oWriter.WriteString(L" fromWordArt=\"0\""); //true/1 Text in this text box is transformed text from a WordArt object.
		oWriter.WriteString(L" anchor=\"t\""); //Vertical text alignment in shape (t - top, b - bottom, ctr - middle) default top
		oWriter.WriteString(L" anchorCtr=\"0\""); //true/1 Defines text box centering.
		oWriter.WriteString(L" forceAA=\"0\""); //true/1 Forces text to render anti-aliased regardless of font size.
		oWriter.WriteString(L" compatLnSpc=\"1\""); //Line spacing for this text is determined in a simplified way using the font scene.
		oWriter.WriteString(L">");

		oWriter.WriteString(L"<a:prstTxWarp prst=\"textNoShape\">");
		oWriter.WriteString(L"<a:avLst/>");
		oWriter.WriteString(L"</a:prstTxWarp>");
		oWriter.WriteString(L"<a:noAutofit/>");
	}

	void CShape::BuildTextBox(NSStringUtils::CStringBuilder &oWriter) const
	{
		if (m_eType == eShapeType::stTextBox && !m_arOutputObjects.empty())
		{
			oWriter.WriteString(L"<wps:txbx>"); //text within the shape. http://officeopenxml.com/drwSp-text.php
			oWriter.WriteString(L"<w:txbxContent>");
			for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
			{
				auto pObj = m_arOutputObjects[i];
				pObj->ToXml(oWriter);
			}
			oWriter.WriteString(L"</w:txbxContent>");
			oWriter.WriteString(L"</wps:txbx>");

			oWriter.WriteString(L"<wps:bodyPr"); //Defines the text body properties inside the shape
			BuildTextBoxParams(oWriter);
			oWriter.WriteString(L"</wps:bodyPr>");
		}
		else
		{
			oWriter.WriteString(L"<wps:bodyPr/>");
		}
	}
	void CShape::ToXmlPptx(NSStringUtils::CStringBuilder &oWriter) const
	{
		if (m_eType == eShapeType::stVectorTexture)
		{
			oWriter.WriteString(L"<p:pic>");
			oWriter.WriteString(L"<p:nvPicPr>");
			oWriter.WriteString(L"<p:cNvPr id=\"");
			oWriter.AddUInt(m_pImageInfo->m_nId);
			oWriter.WriteString(L"\" name=\"Picture ");
			oWriter.AddUInt(m_pImageInfo->m_nId);
			oWriter.WriteString(L"\"/>");
			oWriter.WriteString(L"<p:cNvPicPr><a:picLocks noChangeAspect=\"1\"/></p:cNvPicPr>");
			oWriter.WriteString(L"<p:nvPr/>");
			oWriter.WriteString(L"</p:nvPicPr>");

			oWriter.WriteString(L"<p:blipFill>");
			BuildBlipFill(oWriter);
			oWriter.WriteString(L"</p:blipFill>");

			oWriter.WriteString(L"<p:spPr>");
			BuildForm(oWriter, true);
			BuildGraphicProperties(oWriter);
			oWriter.WriteString(L"</p:spPr>");
			oWriter.WriteString(L"</p:pic>");
			return;
		}

		oWriter.WriteString(L"<p:sp>");
		oWriter.WriteString(L"<p:spPr>");

		BuildForm(oWriter, true);
		BuildGraphicProperties(oWriter);
		oWriter.WriteString(L"</p:spPr>");

		if (m_eType == eShapeType::stTextBox && !m_arOutputObjects.empty())
		{
			oWriter.WriteString(L"<a:txBody>");
			oWriter.WriteString(L"<a:bodyPr");
			BuildTextBoxParams(oWriter);
			oWriter.WriteString(L"</a:bodyPr>");

			for (const auto& obj : m_arOutputObjects)
				obj->ToXmlPptx(oWriter);

			oWriter.WriteString(L"</a:txBody>");
		}
		oWriter.WriteString(L"</p:sp>");
	}
	void CShape::ToBin(NSWasm::CData& oWriter) const
	{
		auto& vector = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector : m_oVector;
		auto& data = vector.GetData();

		double left = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetLeft() : m_dLeft;
		double right = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetRight() : m_dRight;
		double top = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetTop() : m_dTop;
		double bot = fabs(m_dRotation) > c_dMIN_ROTATION ? m_oNoRotVector.GetBottom() : m_dBot;

		double height = bot - top;
		double width = right - left;

		// WriteUniColor
		auto WriteUniColor = [&oWriter] (long color, long alpha) {
			BYTE r = reinterpret_cast<BYTE*>(&color)[0];
			BYTE g = reinterpret_cast<BYTE*>(&color)[1];
			BYTE b = reinterpret_cast<BYTE*>(&color)[2];

			oWriter.StartRecord(1); // COLOR_TYPE_SRGB
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(0); oWriter.WriteBYTE(r);
			oWriter.WriteBYTE(1); oWriter.WriteBYTE(g);
			oWriter.WriteBYTE(2); oWriter.WriteBYTE(b);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			// WriteMods (alpha)
			oWriter.StartRecord(0);
			oWriter.AddInt(1);
			oWriter.StartRecord(1);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(L"alpha");
			oWriter.WriteBYTE(1); oWriter.AddInt(alpha * 100000 / 255);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();
			oWriter.EndRecord();
			oWriter.EndRecord();
		};

		auto WriteUniFill = [&oWriter, &WriteUniColor] (long color, long alpha) {
			oWriter.StartRecord(3); // FILL_TYPE_SOLID
			oWriter.StartRecord(0);
			WriteUniColor(color, alpha);
			oWriter.EndRecord();
			oWriter.EndRecord();
		};

		auto write_spPr = [this, &oWriter, &vector, &data, &WriteUniFill, &top, &left, &height, &width] () {
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

			// WriteRecord WriteXfrm
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(0); oWriter.AddSInt(static_cast<int>(left * c_dMMToEMU));
			oWriter.WriteBYTE(1); oWriter.AddSInt(static_cast<int>(top * c_dMMToEMU));
			oWriter.WriteBYTE(2); oWriter.AddSInt(static_cast<int>(width * c_dMMToEMU));
			oWriter.WriteBYTE(3); oWriter.AddSInt(static_cast<int>(height * c_dMMToEMU));

			if (fabs(m_dRotation) > c_dMIN_ROTATION)
			{
				int degree = static_cast<int>(m_dRotation * c_dDegreeToAngle);
				oWriter.WriteBYTE(10); oWriter.AddSInt(degree);
			}
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();
			// end of WriteRecord WriteXfrm

			// WriteRecord WriteGeometry
			oWriter.StartRecord(1);
			if (vector.IsEmpty())
			{
				oWriter.StartRecord(1);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(L"rect"); // default rect for text
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.EndRecord();
			}
			else
			{
				oWriter.StartRecord(2);

				// WritePathLst
				oWriter.StartRecord(4);
				oWriter.AddInt(1);
				oWriter.StartRecord(1);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(2); oWriter.AddInt(height * c_dMMToEMU); // pathH
				oWriter.WriteBYTE(4); oWriter.AddInt(width * c_dMMToEMU); // pathW
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.StartRecord(0);
				oWriter.AddInt(static_cast<unsigned int>(data.size()));

				auto write_coords = [&oWriter, &left, &top] (const CVectorGraphics::CPathCommand& command) {
					BYTE byte_count = 0;
					oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
					for (const auto& point : command.points)
					{
						int x_coord = static_cast<int>((point.x - left) * c_dMMToEMU);
						int y_coord = static_cast<int>((point.y - top) * c_dMMToEMU);
						oWriter.WriteBYTE(byte_count++);
						oWriter.WriteStringUtf16(std::to_wstring(x_coord));
						oWriter.WriteBYTE(byte_count++);
						oWriter.WriteStringUtf16(std::to_wstring(y_coord));
					}
					oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				};

				for (const auto& command : data)
				{
					oWriter.StartRecord(0);
					switch (command.type)
					{
					case CVectorGraphics::ePathCommandType::pctMove:
						oWriter.StartRecord(1);
						write_coords(command);
						oWriter.EndRecord();
						break;
					case CVectorGraphics::ePathCommandType::pctLine:
						oWriter.StartRecord(2);
						write_coords(command);
						oWriter.EndRecord();
						break;
					case CVectorGraphics::ePathCommandType::pctCurve:
						oWriter.StartRecord(4);
						write_coords(command);
						oWriter.EndRecord();
						break;
					case CVectorGraphics::ePathCommandType::pctClose:
						oWriter.StartRecord(3);
						oWriter.EndRecord();
						break;
					default:
						break;
					}
					oWriter.EndRecord();
				}
				oWriter.EndRecord();
				oWriter.EndRecord();
				oWriter.EndRecord();
				// end of WritePathLst

				// WriteRecord WriteTextRect
				oWriter.StartRecord(5);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(L"l");
				oWriter.WriteBYTE(1); oWriter.WriteStringUtf16(L"t");
				oWriter.WriteBYTE(2); oWriter.WriteStringUtf16(L"r");
				oWriter.WriteBYTE(3); oWriter.WriteStringUtf16(L"b");
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.EndRecord();
				// end of WriteRecord WriteTextRect

				oWriter.EndRecord();
			}
			oWriter.EndRecord();
			// end of WriteRecord WriteGeometry

			// WriteRecord WriteUniFill
			if (!m_bIsNoFill && m_eType != CShape::eShapeType::stVectorTexture)
			{
				oWriter.StartRecord(2);
				WriteUniFill(m_oBrush.Color1, m_oBrush.Alpha1);
				oWriter.EndRecord();
			}

			if (!m_bIsNoStroke)
			{
				// WriteRecord WriteLn
				oWriter.StartRecord(3);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(3); oWriter.AddInt(static_cast<unsigned int>(m_oPen.Size * c_dMMToEMU)); // ln w
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

				oWriter.StartRecord(0);
				WriteUniFill(m_oPen.Color, m_oPen.Alpha);
				oWriter.EndRecord();

				oWriter.EndRecord();
			}
		};

		if (m_eType == eShapeType::stVectorTexture)
		{
			oWriter.StartRecord(2);

			// WriteRecord WriteUniNvPr
			oWriter.StartRecord(0);

			// WriteRecord Write_cNvPr
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(0); oWriter.AddInt(m_pImageInfo->m_nId);
			std::wstring name = L"Picture " + std::to_wstring(m_pImageInfo->m_nId);
			oWriter.WriteBYTE(1); oWriter.WriteStringUtf16(name);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();

			// WriteRecord WritePicCNvPr
			oWriter.StartRecord(1);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(3); oWriter.WriteBool(true); // noChangeAspect
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();

			oWriter.EndRecord();
			// end of WriteRecord WriteUniNvPr

			// WriteRecord WriteUniFill (blip)
			oWriter.StartRecord(1);
			oWriter.StartRecord(1); // FILL_TYPE_BLIP
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

			// WriteBlip
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.StartRecord(2);
			oWriter.AddInt(0); // effects
			oWriter.EndRecord();

			std::wstring rId = L"rId" + std::to_wstring(c_iStartingIdForImages + m_pImageInfo->m_nId);
			oWriter.WriteBYTE(10); oWriter.WriteStringUtf16(rId); // embed

			oWriter.EndRecord();
			// end of WriteBlip

			if (m_oBrush.Image == NULL)
			{
				oWriter.StartRecord(1);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);

				// coeff
				double src_offset_left = 1 - (width / (right - m_dImageLeft));
				double src_offset_right = 1 - (width / (m_dImageRight - left));
				double src_offset_top = 1 - (height / (bot - m_dImageTop));
				double src_offset_bot = 1 - (height / (m_dImageBot - top));

				// percentage
				src_offset_left *= 100;
				src_offset_right *= 100;
				src_offset_top *= 100;
				src_offset_bot *= 100;

				std::wstring l = std::to_wstring(static_cast<int>(src_offset_left * 1000));
				std::wstring t = std::to_wstring(static_cast<int>(src_offset_top * 1000));
				std::wstring r = std::to_wstring(static_cast<int>(src_offset_right * 1000));
				std::wstring b = std::to_wstring(static_cast<int>(src_offset_bot * 1000));

				oWriter.WriteBYTE(0); oWriter.WriteStringUtf16(l);
				oWriter.WriteBYTE(1); oWriter.WriteStringUtf16(t);
				oWriter.WriteBYTE(2); oWriter.WriteStringUtf16(r);
				oWriter.WriteBYTE(3); oWriter.WriteStringUtf16(b);

				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.EndRecord();

				oWriter.StartRecord(3);
				oWriter.EndRecord();
			}
			else // tile
			{
				oWriter.StartRecord(2);
				oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
				oWriter.WriteBYTE(0); oWriter.AddInt(100 * 1000);
				oWriter.WriteBYTE(1); oWriter.AddInt(100 * 1000);
				oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
				oWriter.EndRecord();
			}

			oWriter.EndRecord();
			oWriter.EndRecord();
			// end of WriteRecord WriteUniFill (blip)

			// WriteRecord WriteSpPr
			oWriter.StartRecord(2); write_spPr(); oWriter.EndRecord();

			oWriter.EndRecord();
			return;
		}

		oWriter.StartRecord(1);
		oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
		oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

		// WriteRecord WriteSpPr
		oWriter.StartRecord(1); write_spPr(); oWriter.EndRecord();

		if (m_eType == eShapeType::stTextBox && !m_arOutputObjects.empty())
		{
			// WriteRecord WriteTxBody
			oWriter.StartRecord(3);

			// WriteRecord WriteBodyPr
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(1); oWriter.WriteBYTE(4);      // anchor
			oWriter.WriteBYTE(2); oWriter.WriteBool(false);  // anchorCtr
			oWriter.WriteBYTE(3); oWriter.AddInt(0);         // bIns
			oWriter.WriteBYTE(4); oWriter.WriteBool(true);   // compatLnSpc
			oWriter.WriteBYTE(5); oWriter.WriteBool(false);  // forceAA
			oWriter.WriteBYTE(6); oWriter.WriteBool(false);  // fromWordArt
			oWriter.WriteBYTE(7); oWriter.WriteBYTE(1);      // horzOverflow
			oWriter.WriteBYTE(8); oWriter.AddInt(0);         // lIns
			oWriter.WriteBYTE(9); oWriter.AddInt(1);         // numCol
			oWriter.WriteBYTE(10); oWriter.AddInt(0);        // rIns
			oWriter.WriteBYTE(11); oWriter.AddInt(0);        // rot
			oWriter.WriteBYTE(12); oWriter.WriteBool(false); // rtlCol
			oWriter.WriteBYTE(13); oWriter.AddInt(0);        // spcCol
			oWriter.WriteBYTE(14); oWriter.WriteBool(false); // spcFirstLastPara
			oWriter.WriteBYTE(15); oWriter.AddInt(0);        // tIns
			// 16 is upright param
			oWriter.WriteBYTE(17); oWriter.WriteBYTE(1);     // vert
			oWriter.WriteBYTE(18); oWriter.WriteBYTE(1);     // vertOverflow
			oWriter.WriteBYTE(19); oWriter.WriteBYTE(1);     // vert

			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			// todo WritePrstTxWarp
			oWriter.EndRecord();
			// end of WriteRecord WriteBodyPr

			// WriteRecordArray WriteParagraph
			oWriter.StartRecord(2);
			unsigned int len = static_cast<unsigned int>(m_arOutputObjects.size());
			oWriter.AddInt(len);
			for (const auto& o : m_arOutputObjects)
			{
				oWriter.StartRecord(0);
				o->ToBin(oWriter);
				oWriter.EndRecord();
			}

			oWriter.EndRecord();
			// end of WriteRecordArray WriteParagraph

			oWriter.EndRecord();
			// end of WriteRecord WriteTxBody
		}
		oWriter.EndRecord();
	}
}; // namespace NSDocxRenderer
