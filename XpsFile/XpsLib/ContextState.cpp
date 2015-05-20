#include "ContextState.h"

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	CContextState::CContextState() : m_oCurrentTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
	{
		m_lTransformStack.push_back(m_oCurrentTransform);
	}
	CContextState::~CContextState()
	{
	}
	void CContextState::AddFigure(const std::wstring& wsKey, const std::wstring& wsValue)
	{
		m_mFigures.insert(std::pair<std::wstring, std::wstring>(wsKey, wsValue));
	}
	std::wstring CContextState::GetFigure(const std::wstring& wsKey)
	{
		std::map<std::wstring, std::wstring>::iterator oIter = m_mFigures.find(wsKey);
		if (oIter != m_mFigures.end())
			return oIter->second;

		return L"";
	}
	void CContextState::PushTransform(const double arrTransform[6])
	{
		Aggplus::CMatrix oTransform(arrTransform[0], arrTransform[1], arrTransform[2], arrTransform[3], arrTransform[4], arrTransform[5]);
		m_oCurrentTransform.Multiply(&oTransform);
		m_lTransformStack.push_back(m_oCurrentTransform);
	}
	void CContextState::PopTransform()
	{
		m_lTransformStack.pop_back();
		m_oCurrentTransform = m_lTransformStack.back();
	}
	double CContextState::NormalizeTransform()
	{
		agg::trans_affine& oMatrix = m_oCurrentTransform.m_agg_mtx;
		double dDet = sqrt(oMatrix.sx * oMatrix.sy - oMatrix.shx * oMatrix.shy);

		oMatrix.sx  /= dDet;
		oMatrix.shx /= dDet;
		oMatrix.sy  /= dDet;
		oMatrix.shy /= dDet;

		return dDet;
	}
	void CContextState::SetTransformToRenderer(IRenderer* pRenderer)
	{
		pRenderer->SetTransform(m_oCurrentTransform.m_agg_mtx.sx, m_oCurrentTransform.m_agg_mtx.shy,
								m_oCurrentTransform.m_agg_mtx.shx, m_oCurrentTransform.m_agg_mtx.sy,
								xpsUnitToMM(m_oCurrentTransform.m_agg_mtx.tx), xpsUnitToMM(m_oCurrentTransform.m_agg_mtx.ty));

	}
}