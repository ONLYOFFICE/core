/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "ContextState.h"
#include "StaticResources.h"
#include <math.h>

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

namespace XPS
{
	CContextState::CContextState(IRenderer* pRenderer) : m_oCurrentTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0), m_pRenderer(pRenderer), m_dCurOpacity(1.0)
	{
		m_lTransformStack.push_back(m_oCurrentTransform);
		m_vOpacity.push_back(m_dCurOpacity);
	}
	CContextState::~CContextState()
	{
		m_vClipStack.clear();
		m_lTransformStack.clear();
		m_vOpacity.clear();

		while (m_vResourcesStack.size())
			PopResource();
	}	
	void    CContextState::PushOpacity(const double& dOpacity)
	{
		m_dCurOpacity *= dOpacity;
		m_vOpacity.push_back(m_dCurOpacity);
	}
	void    CContextState::PopOpacity()
	{
		m_vOpacity.pop_back();
		if (m_vOpacity.size())
			m_dCurOpacity = m_vOpacity.at(m_vOpacity.size() - 1);
		else
			m_dCurOpacity = 1;
	}
	double  CContextState::GetCurrentOpacity()
	{
		return m_dCurOpacity;
	}
	void    CContextState::PushTransform(const double arrTransform[6])
	{
		Aggplus::CMatrix oTransform(arrTransform[0], arrTransform[1], arrTransform[2], arrTransform[3], arrTransform[4], arrTransform[5]);
		m_oCurrentTransform.Multiply(&oTransform);
		m_lTransformStack.push_back(m_oCurrentTransform);
		SetTransformToRenderer();
	}
	void    CContextState::PopTransform()
	{
		m_lTransformStack.pop_back();
		m_oCurrentTransform = m_lTransformStack.back();
		SetTransformToRenderer();
	}
	double  CContextState::NormalizeTransform()
	{
        double elems[6];
        m_oCurrentTransform.GetElements(elems);

        double dDet = sqrt(elems[0] * elems[3] - elems[1] * elems[2]);

        elems[0] /= dDet;
        elems[1] /= dDet;
        elems[2] /= dDet;
        elems[3] /= dDet;

		SetTransformToRenderer();

		return dDet;
	}
	void    CContextState::PushClip(const CWString& wsClip)
	{
		m_vClipStack.push_back(wsClip);
		SetClipToRenderer(wsClip);
	}
	void    CContextState::PopClip()
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
	void    CContextState::SetTransformToRenderer()
	{
		if (m_pRenderer)
		{
            m_pRenderer->SetTransform(m_oCurrentTransform.sx(), m_oCurrentTransform.shy(), m_oCurrentTransform.shx(), m_oCurrentTransform.sy(),
                                      xpsUnitToMM(m_oCurrentTransform.tx()), xpsUnitToMM(m_oCurrentTransform.ty()));
		}
	}	
	void    CContextState::SetClipToRenderer(const CWString& wsClip)
	{
		if (!wsClip.empty() && m_pRenderer)
		{
			m_pRenderer->PathCommandStart();
			m_pRenderer->BeginCommand(c_nClipType);
			m_pRenderer->BeginCommand(c_nPathType);
			bool bWinding = VmlToRenderer(wsClip, m_pRenderer);
			m_pRenderer->put_ClipMode(bWinding ? c_nClipRegionTypeWinding : c_nClipRegionTypeEvenOdd);
			m_pRenderer->EndCommand(c_nPathType);
			m_pRenderer->EndCommand(c_nClipType);
			m_pRenderer->PathCommandEnd();
		}
	}
	void    CContextState::PushResource(CStaticResource* pResource, bool bOwn)
	{
		m_vResourcesStack.push_back(TStaticRecource(pResource, bOwn));
	}
	void    CContextState::PopResource()
	{
		if (m_vResourcesStack.size())
		{
			TStaticRecource& oPair = m_vResourcesStack.at(m_vResourcesStack.size() - 1);
			if (oPair.pResource && oPair.bOwn)
				delete oPair.pResource;

			m_vResourcesStack.pop_back();
		}
	}
	void    CContextState::GetPathGeometry(const CWString& _wsKey, CWString& wsPathData, CWString& wsPathTransform)
	{
		if (_wsKey.size() < 17)
			return;

		CWString wsKey((wchar_t*)(_wsKey.c_str() + 16), false, _wsKey.size() - 17);
		CStaticResource* pResource;
		for (int nIndex = m_vResourcesStack.size() - 1; nIndex >= 0; nIndex--)
		{
			pResource = m_vResourcesStack.at(nIndex).pResource;
			if (pResource->GetFigure(wsKey, wsPathData))
				return;
		}
	}
	CBrush* CContextState::GetBrush(const CWString& _wsKey)
	{
		if (_wsKey.size() < 17)
			return NULL;

		CWString wsKey((wchar_t*)(_wsKey.c_str() + 16), false, _wsKey.size() - 17);
		CBrush* pBrush = NULL;
		CStaticResource* pResource;

		for (int nIndex = m_vResourcesStack.size() - 1; nIndex >= 0; nIndex--)
		{
			pResource = m_vResourcesStack.at(nIndex).pResource;
			pBrush = pResource->GetBrush(wsKey);
			if (pBrush)
				return pBrush;
		}

		return NULL;
	}
	void    CContextState::GetTransform(const CWString& _wsKey, CWString& wsTransform)
	{
		if (_wsKey.size() < 17)
			return;

		CWString wsKey((wchar_t*)(_wsKey.c_str() + 16), false, _wsKey.size() - 17);
		CStaticResource* pResource;
		for (int nIndex = m_vResourcesStack.size() - 1; nIndex >= 0; nIndex--)
		{
			pResource = m_vResourcesStack.at(nIndex).pResource;
			if (pResource->GetTransform(wsKey, wsTransform))
				return;
		}
	}
}
