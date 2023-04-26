#include "CObjectBase.h"

#include "CDefs.h"

namespace SVG
{
	CSvgGraphicsObject::CSvgGraphicsObject(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
	    : CSvgObject(oNode, pParent)
	{
		m_oStroke.m_oLineCap.SetMapping({std::make_pair(L"butt", Aggplus::LineCapFlat), std::make_pair(L"round", Aggplus::LineCapRound), std::make_pair(L"square", Aggplus::LineCapSquare)});
		m_oStroke.m_oLineCap = Aggplus::LineCapFlat;

		m_oStroke.m_oLineJoin.SetMapping({std::make_pair(L"arcs", Aggplus::LineJoinMiter), std::make_pair(L"bevel", Aggplus::LineJoinBevel), std::make_pair(L"miter", Aggplus::LineJoinMiter), std::make_pair(L"miter-clip", Aggplus::LineJoinMiterClipped), std::make_pair(L"round", Aggplus::LineJoinRound)});
		m_oStroke.m_oLineCap = Aggplus::LineJoinMiter;
	}

	CSvgGraphicsObject::~CSvgGraphicsObject()
	{}

	CSvgGraphicsObject *CSvgGraphicsObject::Copy() const
	{
		return NULL;
	}

	void CSvgGraphicsObject::SetStroke(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stroke"))
			m_oStroke.m_oColor.SetValue(mAttributes.at(L"stroke"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-width"))
			m_oStroke.m_oWidth.SetValue(mAttributes.at(L"stroke-width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-dasharray"))
			m_oStroke.m_arDash = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(mAttributes.at(L"stroke-dasharray"));

		if (mAttributes.end() != mAttributes.find(L"stroke-linecap"))
			m_oStroke.m_oLineCap.SetValue(mAttributes.at(L"stroke-linecap"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-linejoin"))
			m_oStroke.m_oLineJoin.SetValue(mAttributes.at(L"stroke-linejoin"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-opacity"))
			m_oStroke.m_oColor.SetOpacity(mAttributes.at(L"stroke-opacity"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetFill(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"fill"))
			m_oFill.SetValue(mAttributes.at(L"fill"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"fill-opacity"))
			m_oFill.SetOpacity(mAttributes.at(L"fill-opacity"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetTransform(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"transform"))
			m_oTransform.SetMatrix(mAttributes.at(L"transform"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetClip(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"clip-path"))
			m_oClip.m_oHref.SetValue(mAttributes.at(L"clip-path"), ushLevel, bHardMode);
		if (mAttributes.end() != mAttributes.find(L"clip-rule"))
			m_oClip.m_oRule.SetValue(mAttributes.at(L"clip-rule"), std::vector<std::wstring>{L"nonzero", L"evenodd"}, ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::StartPath(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		ApplyClip(pRenderer, pDefs);
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

	void CSvgGraphicsObject::EndPath(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		(bIsClip) ? EndClipPath(pRenderer) : EndStandardPath(pRenderer, pDefs);
	}

	void CSvgGraphicsObject::EndStandardPath(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		int nPathType = 0;
		Aggplus::CMatrix oOldMatrix(1., 0., 0., 1., 0, 0);
		ApplyStyle(pRenderer, pDefs, nPathType, oOldMatrix);

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

	void CSvgGraphicsObject::ApplyDefaultStroke(IRenderer *pRenderer, int &nTypePath) const
	{
		nTypePath += c_nStroke;
		pRenderer->put_PenSize(1);
		pRenderer->put_PenColor(0);
		pRenderer->put_PenAlpha(255);
		pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);
	}

	void CSvgGraphicsObject::ApplyStroke(IRenderer *pRenderer, int &nTypePath, bool bUseDedault) const
	{
		if (!m_oStroke.m_oColor.Empty())
		{
			if (NSCSS::NSProperties::ColorType::ColorNone == m_oStroke.m_oColor.GetType())
				return;

			nTypePath += c_nStroke;
			pRenderer->put_PenColor(m_oStroke.m_oColor.ToInt());
			pRenderer->put_PenAlpha(255. * m_oStroke.m_oColor.GetOpacity());

			double dStrokeWidth = m_oStroke.m_oWidth.ToDouble(NSCSS::Pixel);

			if (0 != dStrokeWidth)
				pRenderer->put_PenSize(dStrokeWidth);

			if (!m_oStroke.m_arDash.empty())
			{
				pRenderer->PenDashPattern((double*)m_oStroke.m_arDash.data(), m_oStroke.m_arDash.size());
				pRenderer->put_PenDashStyle(Aggplus::DashStyleCustom);
			}
			else
				pRenderer->put_PenDashStyle(Aggplus::DashStyleSolid);

			if (!m_oStroke.m_oLineCap.Empty())
			{
				pRenderer->put_PenLineStartCap(m_oStroke.m_oLineCap.ToInt());
				pRenderer->put_PenLineEndCap(m_oStroke.m_oLineCap.ToInt());
			}

			if (!m_oStroke.m_oLineJoin.Empty())
				pRenderer->put_PenLineJoin(m_oStroke.m_oLineJoin.ToInt());
		}
		else if (bUseDedault)
			ApplyDefaultStroke(pRenderer, nTypePath);
	}

	void CSvgGraphicsObject::ApplyDefaultFill(IRenderer *pRenderer, int &nTypePath) const
	{
		nTypePath += c_nWindingFillMode;
		pRenderer->put_BrushColor1(0);
		pRenderer->put_BrushAlpha1(255);
		pRenderer->put_BrushType(c_BrushTypeSolid);
	}

	void CSvgGraphicsObject::ApplyFill(IRenderer *pRenderer, const CDefs *pDefs, int &nTypePath, bool bUseDedault) const
	{
		if (NSCSS::NSProperties::ColorType::ColorNone == m_oFill.GetType())
			return;
		else if (NSCSS::NSProperties::ColorType::ColorHEX == m_oFill.GetType() ||
		         NSCSS::NSProperties::ColorType::ColorRGB == m_oFill.GetType())
		{
			nTypePath += c_nWindingFillMode;
			pRenderer->put_BrushColor1(m_oFill.ToInt());
			pRenderer->put_BrushAlpha1(255. * m_oFill.GetOpacity());
			pRenderer->put_BrushType(c_BrushTypeSolid);
		}
		else if (NSCSS::NSProperties::ColorType::ColorUrl == m_oFill.GetType() && NULL != pDefs)
		{
			if (ApplyDef(pRenderer, pDefs, m_oFill.ToWString()))
			{
				nTypePath += c_nWindingFillMode;
				pRenderer->put_BrushAlpha1(255. * m_oFill.GetOpacity());
			}
		}
		else if (bUseDedault)
			ApplyDefaultFill(pRenderer, nTypePath);
	}

	void CSvgGraphicsObject::ApplyClip(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		if (NULL == pRenderer || NULL == pDefs)
			return;

		pRenderer->BeginCommand(c_nResetClipType);
		pRenderer->EndCommand(c_nResetClipType);

		if (m_oClip.m_oRule == L"evenodd")
			pRenderer->put_ClipMode(c_nClipRegionTypeEvenOdd);
		else
			pRenderer->put_ClipMode(c_nClipRegionTypeWinding);

		if (m_oClip.m_oHref.Empty() || NSCSS::NSProperties::ColorType::ColorUrl != m_oClip.m_oHref.GetType())
			return;

		CDefObject *pClipObject = pDefs->GetDef(m_oClip.m_oHref.ToWString());

		if (NULL == pClipObject)
			return;

		pClipObject->Apply(pRenderer, pDefs, GetBounds());
	}

	void CSvgGraphicsObject::ApplyTransform(IRenderer *pRenderer, Aggplus::CMatrix& oOldMatrix) const
	{
		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		oOldMatrix.SetElements(dM11, dM12, dM21, dM22, dRx, dRy);

		Aggplus::CMatrix oMatrix = m_oTransform.GetMatrix().GetFinalValue(&oOldMatrix);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
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
