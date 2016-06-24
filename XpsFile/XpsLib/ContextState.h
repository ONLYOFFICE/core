/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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