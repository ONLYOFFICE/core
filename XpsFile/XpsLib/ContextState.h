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
#ifndef _XPS_XPSLIB_CONTEXTSTATE_H
#define _XPS_XPSLIB_CONTEXTSTATE_H

#include "Utils.h"

#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#include <list>
#include <map>

namespace XPS
{
	class CBrush;
	class CStaticResource;

	class CContextState
	{
	private:

		struct TStaticRecource
		{
			TStaticRecource(CStaticResource* resource, bool own)
			{
				pResource = resource;
				bOwn      = own;
			}

			CStaticResource* pResource;
			bool             bOwn;
		};

	public:

		CContextState(IRenderer* pRenderer);
		~CContextState();

		void    PushOpacity(const double& dOpacity);
		void    PopOpacity();
		double  GetCurrentOpacity();
		void    PushClip(const CWString& wsClip);
		void    PopClip();
		void    PushTransform(const double arrTransform[6]);
		void    PopTransform();
		double  NormalizeTransform();
		void    PushResource(CStaticResource* pResource, bool bOwn);
		void    PopResource();
			    
		void    GetPathGeometry(const CWString& wsKey, CWString& wsPathData, CWString& wsPathTransform);
		CBrush* GetBrush(const CWString& wsKey);
		void    GetTransform(const CWString& wsKey, CWString& wsTransform);
		Aggplus::CMatrix GetCurrentTransform()
		{
			return m_oCurrentTransform;
		}

	private:

		void SetClipToRenderer(const CWString& wsClip);
		void SetTransformToRenderer();

	private:

		Aggplus::CMatrix            m_oCurrentTransform;
		std::list<Aggplus::CMatrix> m_lTransformStack;
		std::vector<CWString>       m_vClipStack;
		IRenderer*                  m_pRenderer;
		std::vector<double>         m_vOpacity;
		double                      m_dCurOpacity;
		std::vector<TStaticRecource>m_vResourcesStack;
	};
}

#endif //_XPS_XPSLIB_CONTEXTSTATE_H