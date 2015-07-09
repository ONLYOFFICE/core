#include "ContextState.h"

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	CContextState::CContextState(IRenderer* pRenderer) : m_oCurrentTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0), m_pRenderer(pRenderer)
	{
		m_lTransformStack.push_back(m_oCurrentTransform);
	}
	CContextState::~CContextState()
	{
		m_vClipStack.clear();
		m_lTransformStack.clear();
	}	
	void CContextState::PushTransform(const double arrTransform[6])
	{
		Aggplus::CMatrix oTransform(arrTransform[0], arrTransform[1], arrTransform[2], arrTransform[3], arrTransform[4], arrTransform[5]);
		m_oCurrentTransform.Multiply(&oTransform);
		m_lTransformStack.push_back(m_oCurrentTransform);
		SetTransformToRenderer();
	}
	void CContextState::PopTransform()
	{
		m_lTransformStack.pop_back();
		m_oCurrentTransform = m_lTransformStack.back();
		SetTransformToRenderer();
	}
	double CContextState::NormalizeTransform()
	{
		agg::trans_affine& oMatrix = m_oCurrentTransform.m_agg_mtx;
		double dDet = sqrt(oMatrix.sx * oMatrix.sy - oMatrix.shx * oMatrix.shy);

		oMatrix.sx  /= dDet;
		oMatrix.shx /= dDet;
		oMatrix.sy  /= dDet;
		oMatrix.shy /= dDet;

		SetTransformToRenderer();

		return dDet;
	}
	void CContextState::PushClip(const CWString& wsClip)
	{
		m_vClipStack.push_back(wsClip);
		SetClipToRenderer(wsClip);
	}
	void CContextState::PopClip()
	{
		m_vClipStack.pop_back();
		if (m_pRenderer)
		{
			m_pRenderer->BeginCommand(c_nResetClipType);
			m_pRenderer->EndCommand(c_nResetClipType);

			for (int nIndex = 0, nCount = m_vClipStack.size(); nIndex < nCount; nIndex++)
			{
				CWString wsClip = m_vClipStack.at(nIndex);
				SetClipToRenderer(wsClip);
			}
		}
	}
	void CContextState::SetTransformToRenderer()
	{
		if (m_pRenderer)
		{
			m_pRenderer->SetTransform(m_oCurrentTransform.m_agg_mtx.sx, m_oCurrentTransform.m_agg_mtx.shy,
									  m_oCurrentTransform.m_agg_mtx.shx, m_oCurrentTransform.m_agg_mtx.sy,
									  xpsUnitToMM(m_oCurrentTransform.m_agg_mtx.tx), xpsUnitToMM(m_oCurrentTransform.m_agg_mtx.ty));
		}
	}	
	void CContextState::SetClipToRenderer(const CWString& wsClip)
	{
		if (!wsClip.empty() && m_pRenderer)
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			bool bWinding = VmlToRenderer(wsClip.c_str(), m_pRenderer);
			m_pRenderer->put_ClipMode(bWinding ? c_nClipRegionTypeWinding : c_nClipRegionTypeEvenOdd);
			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();
		}
	}
}