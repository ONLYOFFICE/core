#include "CWmfInterpretatorRender.h"

namespace MetaFile
{
	CWmfInterpretatorRender::CWmfInterpretatorRender(IOutputDevice* pIOutputDevice) :
		m_pMetaFileRenderer(static_cast<CMetaFileRenderer*>(pIOutputDevice)){}

	InterpretatorType CWmfInterpretatorRender::GetType() const
	{
		return InterpretatorType::Render;
	}

	void CWmfInterpretatorRender::Begin()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->Begin();
	}

	void CWmfInterpretatorRender::End()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->End();
	}

	void CWmfInterpretatorRender::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
	}

	void CWmfInterpretatorRender::DrawString(std::wstring &wsText, unsigned int unCharsCount, double dX, double dY, double *pDx,
											 int iGraphicsMode, double dXScale, double dYScale)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawString(wsText, unCharsCount, dX, dY, pDx, iGraphicsMode, dXScale, dYScale);
	}

	void CWmfInterpretatorRender::DrawDriverString(const std::wstring& wsString, const std::vector<TPointD>& arPoints)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawDriverString(wsString, arPoints);
	}

	void CWmfInterpretatorRender::StartPath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->StartPath();
	}

	void CWmfInterpretatorRender::MoveTo(double dX, double dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->MoveTo(dX, dY);
	}

	void CWmfInterpretatorRender::LineTo(double dX, double dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->LineTo(dX, dY);
	}

	void CWmfInterpretatorRender::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
	}

	void CWmfInterpretatorRender::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
	}

	void CWmfInterpretatorRender::ClosePath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ClosePath();
	}

	void CWmfInterpretatorRender::DrawPath(int nType)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->DrawPath(nType);
	}

	void CWmfInterpretatorRender::EndPath()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->EndPath();
	}

	void CWmfInterpretatorRender::ResetClip()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ResetClip();
	}

	void CWmfInterpretatorRender::IntersectClip(const TRectD& oClip)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->IntersectClip(oClip);
	}

	void CWmfInterpretatorRender::ExcludeClip(const TRectD &oClip, const TRectD &oBB)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->ExcludeClip(oClip, oBB);
	}

	void CWmfInterpretatorRender::PathClip(IPath *pPath, int nClipMode, TXForm *pTransform)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->PathClip(pPath, nClipMode, pTransform);
	}

	void CWmfInterpretatorRender::StartClipPath(unsigned int unMode, int nFillMode)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->StartClipPath(unMode, nFillMode);
	}

	void CWmfInterpretatorRender::EndClipPath(unsigned int unMode)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->EndClipPath(unMode);
	}

	void CWmfInterpretatorRender::UpdateDC()
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->UpdateDC();
	}

	void CWmfInterpretatorRender::SetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dX, double &dY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
	}

	void CWmfInterpretatorRender::GetTransform(double *pdM11, double *pdM12, double *pdM21, double *pdM22, double *pdX, double *pdY)
	{
		if (NULL != m_pMetaFileRenderer)
			m_pMetaFileRenderer->GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
	}

	CMetaFileRenderer *CWmfInterpretatorRender::GetRenderer() const
	{
		return m_pMetaFileRenderer;
	}
}
