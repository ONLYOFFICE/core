#include "CObjectBase.h"

#include "CDefs.h"

namespace SVG
{
	CSvgGraphicsObject::CSvgGraphicsObject(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
		: CSvgObject(oNode, pParent)
	{
		m_oStyles.m_oStroke.m_oLineCap.SetMapping({std::make_pair(L"butt", Aggplus::LineCapFlat), std::make_pair(L"round", Aggplus::LineCapRound), std::make_pair(L"square", Aggplus::LineCapSquare)});
		m_oStyles.m_oStroke.m_oLineCap = Aggplus::LineCapFlat;

		m_oStyles.m_oStroke.m_oLineJoin.SetMapping({std::make_pair(L"arcs", Aggplus::LineJoinMiter), std::make_pair(L"bevel", Aggplus::LineJoinBevel), std::make_pair(L"miter", Aggplus::LineJoinMiter), std::make_pair(L"miter-clip", Aggplus::LineJoinMiterClipped), std::make_pair(L"round", Aggplus::LineJoinRound)});
		m_oStyles.m_oStroke.m_oLineCap = Aggplus::LineJoinMiter;
	}

	CSvgGraphicsObject::~CSvgGraphicsObject()
	{}

	void CSvgGraphicsObject::SetStroke(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stroke"))
			m_oStyles.m_oStroke.m_oColor.SetValue(mAttributes.at(L"stroke"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-width"))
			m_oStyles.m_oStroke.m_oWidth.SetValue(mAttributes.at(L"stroke-width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-dasharray"))
			m_oStyles.m_oStroke.m_arDash = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(mAttributes.at(L"stroke-dasharray"));

		if (mAttributes.end() != mAttributes.find(L"stroke-linecap"))
			m_oStyles.m_oStroke.m_oLineCap.SetValue(mAttributes.at(L"stroke-linecap"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-linejoin"))
			m_oStyles.m_oStroke.m_oLineJoin.SetValue(mAttributes.at(L"stroke-linejoin"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-opacity"))
			m_oStyles.m_oStroke.m_oColor.SetOpacity(mAttributes.at(L"stroke-opacity"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetFill(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"fill"))
			m_oStyles.m_oFill.SetValue(mAttributes.at(L"fill"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"fill-opacity"))
			m_oStyles.m_oFill.SetOpacity(mAttributes.at(L"fill-opacity"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetTransform(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"transform"))
			m_oStyles.m_oTransform.SetMatrix(mAttributes.at(L"transform"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetClip(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"clip-path"))
			m_oStyles.m_oClip.m_oHref.SetValue(mAttributes.at(L"clip-path"), ushLevel, bHardMode);
		if (mAttributes.end() != mAttributes.find(L"clip-rule"))
			m_oStyles.m_oClip.m_oRule.SetValue(mAttributes.at(L"clip-rule"), std::vector<std::wstring>{L"nonzero", L"evenodd"}, ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::StartPath(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		Apply(pRenderer, &m_oStyles.m_oClip, pDefs);
		(bIsClip) ? StartClipPath(pRenderer) : StartStandardPath(pRenderer);
	}

	void CSvgGraphicsObject::StartStandardPath(IRenderer *pRenderer) const
	{
		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();
	}

	void CSvgGraphicsObject::StartClipPath(IRenderer *pRenderer) const
	{
		pRenderer->BeginCommand(c_nClipType);
		pRenderer->PathCommandStart();
	}

	void CSvgGraphicsObject::EndPath(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip, const TSvgStyles* pOtherStyles) const
	{
		(bIsClip) ? EndClipPath(pRenderer) : EndStandardPath(pRenderer, pDefs, pOtherStyles);
	}

	void CSvgGraphicsObject::EndStandardPath(IRenderer *pRenderer, const CDefs *pDefs, const TSvgStyles *pOtherStyles) const
	{
		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);

		if (NULL == pOtherStyles)
			ApplyStyle(pRenderer, &m_oStyles, pDefs, nPathType, oOldMatrix);
		else
		{
			TSvgStyles oNewStyles(m_oStyles);
			oNewStyles += *pOtherStyles;
			ApplyStyle(pRenderer, &oNewStyles, pDefs, nPathType, oOldMatrix);
		}

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		pRenderer->SetTransform(oOldMatrix.sx(), oOldMatrix.shy(), oOldMatrix.shx(), oOldMatrix.sy(), oOldMatrix.tx(), oOldMatrix.ty());
	}

	void CSvgGraphicsObject::EndClipPath(IRenderer *pRenderer) const
	{
		pRenderer->EndCommand(c_nClipType);
		pRenderer->PathCommandEnd();
	}

	bool CSvgGraphicsObject::Apply(IRenderer *pRenderer, const TStroke *pStroke, bool bUseDefault) const
	{
		if (NULL == pRenderer || NULL == pStroke || NSCSS::NSProperties::ColorType::ColorNone == pStroke->m_oColor.GetType() || (!bUseDefault && ((pStroke->m_oWidth.Empty() || pStroke->m_oWidth.Zero()) && pStroke->m_oColor.Empty())))
		{
			pRenderer->put_PenSize(0);
			return false;
		}

		double dStrokeWidth = pStroke->m_oWidth.ToDouble(NSCSS::Pixel);

		if (0. == dStrokeWidth)
			dStrokeWidth = 1.;

		int nColor = (pStroke->m_oColor.Empty() || NSCSS::NSProperties::ColorType::ColorNone == pStroke->m_oColor.GetType()) ? 0 : pStroke->m_oColor.ToInt();

		pRenderer->put_PenSize(dStrokeWidth);
		pRenderer->put_PenColor(nColor);
		pRenderer->put_PenAlpha(255. * pStroke->m_oColor.GetOpacity());

		if (!pStroke->m_arDash.empty())
		{
			pRenderer->PenDashPattern((double*)pStroke->m_arDash.data(), pStroke->m_arDash.size());
			pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
		}
		else
			pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);

		if (!pStroke->m_oLineCap.Empty())
		{
			pRenderer->put_PenLineStartCap(pStroke->m_oLineCap.ToInt());
			pRenderer->put_PenLineEndCap(pStroke->m_oLineCap.ToInt());
		}

		if (!pStroke->m_oLineJoin.Empty())
			pRenderer->put_PenLineJoin(pStroke->m_oLineJoin.ToInt());

		return true;
	}

	bool CSvgGraphicsObject::Apply(IRenderer *pRenderer, const NSCSS::NSProperties::CColor *pFill, const CDefs *pDefs, bool bUseDefault) const
	{
		if (NULL == pRenderer || NULL == pFill || NSCSS::NSProperties::ColorType::ColorNone == pFill->GetType() || (!bUseDefault && pFill->Empty()))
		{
			pRenderer->put_BrushType(c_BrushTypeNoFill);
			return false;
		}
		else if (NSCSS::NSProperties::ColorType::ColorHEX == pFill->GetType() ||
				 NSCSS::NSProperties::ColorType::ColorRGB == pFill->GetType())
		{
			pRenderer->put_BrushColor1((pFill->Empty() && bUseDefault) ? 0 : pFill->ToInt());
			pRenderer->put_BrushType(c_BrushTypeSolid);
		}
		else if (NSCSS::NSProperties::ColorType::ColorUrl == pFill->GetType())
		{
			if (!ApplyDef(pRenderer, pDefs, pFill->ToWString()))
			{
				if (bUseDefault)
				{
					pRenderer->put_BrushColor1(0);
					pRenderer->put_BrushType(c_BrushTypeSolid);
				}
				else
					return false;
			}
		}
		else if (bUseDefault)
		{
			pRenderer->put_BrushColor1(0);
			pRenderer->put_BrushType(c_BrushTypeSolid);
		}

		pRenderer->put_BrushAlpha1(255. * pFill->GetOpacity());

		return true;
	}

	bool CSvgGraphicsObject::Apply(IRenderer *pRenderer, const NSCSS::NSProperties::CTransform *pTransform, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer || NULL == pTransform)
			return false;

		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		oOldMatrix.SetElements(dM11, dM12, dM21, dM22, dRx, dRy);

		Aggplus::CMatrix oMatrix = pTransform->GetMatrix().GetFinalValue(&oOldMatrix);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());

		return true;
	}

	bool CSvgGraphicsObject::Apply(IRenderer *pRenderer, const TClip *pClip, const CDefs *pDefs) const
	{
		if (NULL == pRenderer || NULL == pClip || NULL == pDefs)
			return false;

		pRenderer->BeginCommand(c_nResetClipType);
		pRenderer->EndCommand(c_nResetClipType);

		if (pClip->m_oRule == L"evenodd")
			pRenderer->put_ClipMode(c_nClipRegionTypeEvenOdd);
		else
			pRenderer->put_ClipMode(c_nClipRegionTypeWinding);

		if (pClip->m_oHref.Empty() || NSCSS::NSProperties::ColorType::ColorUrl != pClip->m_oHref.GetType())
			return true;

		CDefObject *pClipObject = pDefs->GetDef(pClip->m_oHref.ToWString());

		if (NULL == pClipObject)
			return false;

		return pClipObject->Apply(pRenderer, pDefs, GetBounds());
	}

	bool CSvgGraphicsObject::ApplyDef(IRenderer *pRenderer, const CDefs *pDefs, const std::wstring &wsUrl) const
	{
		if (NULL == pRenderer || NULL == pDefs || wsUrl.empty())
			return false;

		CDefObject *pDefObject = pDefs->GetDef(wsUrl);

		if (NULL == pDefObject)
			return false;

		return pDefObject->Apply(pRenderer, pDefs, GetBounds());
	}
}
