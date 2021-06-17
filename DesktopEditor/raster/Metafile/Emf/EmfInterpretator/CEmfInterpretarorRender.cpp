#include "CEmfInterpretarorRender.h"

namespace MetaFile
{
    CEmfInterpretarorRender::CEmfInterpretarorRender()
    {}

    void CEmfInterpretarorRender::SetOutputDevice(IOutputDevice *pIOutputDevice)
    {
        m_pMetaFileRenderer = static_cast<CMetaFileRenderer*>(pIOutputDevice);
    }

    void CEmfInterpretarorRender::Begin()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->Begin();
    }

    void CEmfInterpretarorRender::End()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->End();
    }

    void CEmfInterpretarorRender::DrawBitmap(double dX, double dY, double dW, double dH, BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->DrawBitmap(dX, dY, dW, dH, pBuffer, unWidth, unHeight);
    }

    void CEmfInterpretarorRender::DrawString(std::wstring &wsText, unsigned int unCharsCount, double dX, double dY, double *pDx, int iGraphicsMode)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->DrawString(wsText, unCharsCount, dX, dY, pDx, iGraphicsMode);
    }

    void CEmfInterpretarorRender::StartPath()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->StartPath();
    }

    void CEmfInterpretarorRender::MoveTo(double dX, double dY)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->MoveTo(dX, dY);
    }

    void CEmfInterpretarorRender::LineTo(double dX, double dY)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->LineTo(dX, dY);
    }

    void CEmfInterpretarorRender::CurveTo(double dX1, double dY1, double dX2, double dY2, double dXe, double dYe)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
    }

    void CEmfInterpretarorRender::ArcTo(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->ArcTo(dLeft, dTop, dRight, dBottom, dStartAngle, dSweepAngle);
    }

    void CEmfInterpretarorRender::ClosePath()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->ClosePath();
    }

    void CEmfInterpretarorRender::DrawPath(int nType)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->DrawPath(nType);
    }

    void CEmfInterpretarorRender::EndPath()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->EndPath();
    }

    void CEmfInterpretarorRender::ResetClip()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->ResetClip();
    }

    void CEmfInterpretarorRender::IntersectClip(double dLeft, double dTop, double dRight, double dBottom)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->IntersectClip(dLeft, dTop, dRight, dBottom);
    }

    void CEmfInterpretarorRender::StartClipPath(unsigned int unMode, int nFillMode)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->StartClipPath(unMode, nFillMode);
    }

    void CEmfInterpretarorRender::EndClipPath(unsigned int unMode)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->EndClipPath(unMode);
    }

    void CEmfInterpretarorRender::UpdateDC()
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->UpdateDC();
    }

    void CEmfInterpretarorRender::SetTransform(double &dM11, double &dM12, double &dM21, double &dM22, double &dX, double &dY)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
    }

    void CEmfInterpretarorRender::GetTransform(double *pdM11, double *pdM12, double *pdM21, double *pdM22, double *pdX, double *pdY)
    {
        if (NULL != m_pMetaFileRenderer)
            m_pMetaFileRenderer->GetTransform(pdM11, pdM12, pdM21, pdM22, pdX, pdY);
    }
}
