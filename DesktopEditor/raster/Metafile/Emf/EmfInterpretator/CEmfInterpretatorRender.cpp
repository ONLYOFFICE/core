/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "CEmfInterpretatorRender.h"

namespace MetaFile
{
	CEmfInterpretatorRender::CEmfInterpretatorRender(IOutputDevice* pIOutputDevice) 
		: m_pMetaFileRenderer(static_cast<CMetaFileRenderer*>(pIOutputDevice)){}

	InterpretatorType CEmfInterpretatorRender::GetType() const
	{
		return InterpretatorType::Render;
	}
	
	void CEmfInterpretatorRender::CreateConditional(IMetaFileBase* pFile)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->CreateConditional(pFile);
	}

	void CEmfInterpretatorRender::ChangeConditional()
	{
			if (NULL != m_pMetaFileRenderer)
				m_pMetaFileRenderer->ChangeConditional();
	}

	void CEmfInterpretatorRender::Begin()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->Begin();
	}

	void CEmfInterpretatorRender::End()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->End();
	}

	void CEmfInterpretatorRender::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
	}

	void CEmfInterpretatorRender::DrawString(std::wstring &wsText, unsigned int unCharsCount, double dX, double dY, double *pDx,
											 int iGraphicsMode, double dXScale, double dYScale)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawString(wsText, unCharsCount, dX, dY, pDx, iGraphicsMode, dXScale, dYScale);
	}

	void CEmfInterpretatorRender::DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawDriverString(wsString, arPoints);
	}

	void CEmfInterpretatorRender::StartPath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->StartPath();
	}

	void CEmfInterpretatorRender::MoveTo(double dX, double dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->MoveTo(dX, dY);
	}

	void CEmfInterpretatorRender::LineTo(double dX, double dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->LineTo(dX, dY);
	}

	void CEmfInterpretatorRender::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
	}

	void CEmfInterpretatorRender::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
	}

	void CEmfInterpretatorRender::ClosePath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ClosePath();
	}

	void CEmfInterpretatorRender::DrawPath(int nType)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawPath(nType);
	}

	void CEmfInterpretatorRender::EndPath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->EndPath();
	}

	void CEmfInterpretatorRender::ResetClip()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ResetClip();
	}

	void CEmfInterpretatorRender::IntersectClip(const TRectD &oClip)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->IntersectClip(oClip);
	}

	void CEmfInterpretatorRender::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ExcludeClip(oClip, oBB);
	}

	void CEmfInterpretatorRender::PathClip(const CPath &oPath, int nClipMode, TXForm *pTransform)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->PathClip(oPath, nClipMode, pTransform);
	}

	void CEmfInterpretatorRender::StartClipPath(unsigned int unMode, int nFillMode)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->StartClipPath(unMode, nFillMode);
	}

	void CEmfInterpretatorRender::EndClipPath(unsigned int unMode)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->EndClipPath(unMode);
	}

	void CEmfInterpretatorRender::UpdateDC()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->UpdateDC();
	}

	void CEmfInterpretatorRender::SetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dX, double &dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
	}

	void CEmfInterpretatorRender::GetTransform(double *pdM11, double *pdM12, double *pdM21, double *pdM22, double *pdX, double *pdY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
	}

	CMetaFileRenderer *CEmfInterpretatorRender::GetRenderer() const
	{
		return m_pMetaFileRenderer;
	}
	
	double CEmfInterpretatorRender::GetScaleX() const
	{
		if (NULL == m_pMetaFileRenderer)
			return 1.;

		return m_pMetaFileRenderer->GetScaleX();
	}

	double CEmfInterpretatorRender::GetScaleY() const
	{
		if (NULL == m_pMetaFileRenderer)
			return 1.;

		return m_pMetaFileRenderer->GetScaleY();
	}

	void CEmfInterpretatorRender::HANDLE_EMR_RESTOREDC(const int &nIndexDC)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMR_SELECTCLIPPATH(const unsigned int &unRegionMode)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMR_EXCLUDECLIPRECT(const TRectL &oClip)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMR_EXTSELECTCLIPRGN(const unsigned int &unRgnDataSize, const unsigned int &unRegionMode, CDataStream &oDataStream)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMR_SETMETARGN()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMR_INTERSECTCLIPRECT(const TRectL &oClip)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_OFFSETCLIP(double, double)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_RESETCLIP()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_SETCLIPPATH(short, const CEmfPlusPath *)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_SETCLIPRECT(short, const TEmfPlusRectF &)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_SETCLIPREGION(short, short, const CEmfPlusRegion *)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_ENDOFFILE()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_GETDC()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
	
	void CEmfInterpretatorRender::HANDLE_EMFPLUS_RESTORE(unsigned int)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->NeedUpdateClip();
	}
}
