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
#ifndef _XPS_XPSLIB_STATICRESOURCES_H
#define _XPS_XPSLIB_STATICRESOURCES_H

#include "Utils.h"
#include "../../OfficeUtils/src/ZipFolder.h"
#include <map>

namespace XmlUtils
{
    class CXmlLiteReader;
}
namespace XPS
{
    class CWString;
}
class IRenderer;

namespace XPS
{
	class CBrush;
	class CStaticResource
	{
	public:
		CStaticResource(const std::string& wsPath);
		CStaticResource(XmlUtils::CXmlLiteReader& oReader);
		~CStaticResource();
		bool    GetFigure(CWString& wsKey, CWString& wsFigure);
		CBrush* GetBrush(CWString& wsKey);
		bool    GetTransform(CWString& wsKey, CWString& wsTransform);

	private:

		void Parse(XmlUtils::CXmlLiteReader& oReader);
		void AddFigure(const CWString& wsKey, const CWString& wsValue);
		void AddBrush(const CWString& wsKey, CBrush* pBrush);
		void AddTransform(const CWString& wsKey, const CWString& wsValue);

	private:

		std::map<CWString, CWString> m_mFigures;
		std::map<CWString, CBrush*>  m_mBrushes;
		std::map<CWString, CWString> m_mTransforms;
	};

	class CBrush
	{
	public:
		CBrush(){}
		virtual ~CBrush(){}
		virtual bool SetToRenderer(IRenderer* pRenderer) { return false; }
		virtual bool IsImageBrush() { return false; }	
	};
	class CSolidBrush : public CBrush
	{
	public:
		CSolidBrush(int nBgr, int nAlpha) : m_nBgr(nBgr), m_nAlpha(nAlpha){}
		bool SetToRenderer(IRenderer* pRenderer);

	private:

		int m_nBgr;
		int m_nAlpha;
	};
	class CImageBrush : public CBrush
	{
	public:
		CImageBrush(const wchar_t* wsPath)
		{
			m_wsPath.create(wsPath, true);
			m_wsRoot = NULL;
		}
		bool SetToRenderer(IRenderer* pRenderer);
		bool IsImageBrush() { return true; }
		void SetPaths(IFolder* wsRoot, const wchar_t* wsPage);
	private:

		CWString m_wsPath;
		IFolder* m_wsRoot;
		CWString m_wsPage;
	};
	class CGradientBrush : public CBrush
	{
	public:
		CGradientBrush()
		{
			m_pColors    = NULL;
			m_pPositions = NULL;
			m_lCount     = 0;
		}
		virtual ~CGradientBrush()
		{
			RELEASEARRAYOBJECTS(m_pColors);
			RELEASEARRAYOBJECTS(m_pPositions);
		}
		void SetGradientStops(LONG* pColors, double* pPositions, LONG lCount)
		{
			m_pColors    = pColors;
			m_pPositions = pPositions;
			m_lCount     = lCount;
		}

	protected:

		LONG*   m_pColors;
		double* m_pPositions;
		LONG    m_lCount;
	};
	class CLinearGradientBrush : public CGradientBrush
	{
	public:
		CLinearGradientBrush(const double& dX0, const double& dY0, const double& dX1, const double& dY1) : m_dX0(dX0), m_dY0(dY0), m_dX1(dX1), m_dY1(dY1)
		{
		}
		bool SetToRenderer(IRenderer* pRenderer);

	private:

		double m_dX0;
		double m_dY0;
		double m_dX1;
		double m_dY1;
	};
	class CRadialGradientBrush : public CGradientBrush
	{
	public:
		CRadialGradientBrush(const double& dXcenter, const double& dYcenter, const double& dXorigin, const double& dYorigin, const double& dRadX, const double& dRadY) :
			m_dXc(dXcenter), m_dYc(dYcenter), m_dXo(dXorigin), m_dYo(dYorigin), m_dRadX(dRadX), m_dRadY(dRadY)
		{

		}
		bool SetToRenderer(IRenderer* pRenderer);
	private:

		double m_dXc;
		double m_dYc;
		double m_dXo;
		double m_dYo;
		double m_dRadX;
		double m_dRadY;
	};
	CBrush* ReadBrushNode(XmlUtils::CXmlLiteReader& oReader, const double& dOpacity = 1.0, CWString* pwsKey = NULL);
	CBrush* ReadBrush(XmlUtils::CXmlLiteReader& oReader, const double& dOpacity = 1.0, CWString* pwsKey = NULL);
	CBrush* ReadBrush(const wchar_t* wsString, const double& dOpacity = 1.0);
}

#endif //_XPS_XPSLIB_STATICRESOURCES_H
