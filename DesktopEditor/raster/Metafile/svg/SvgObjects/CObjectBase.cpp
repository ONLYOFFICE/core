#include "CObjectBase.h"

#include "CDefs.h"

namespace SVG
{
	CSvgGraphicsObject::CSvgGraphicsObject(XmlUtils::CXmlNode &oNode, CSvgGraphicsObject *pParent)
	    : CSvgObject(oNode, pParent)
	{}

	CSvgGraphicsObject::~CSvgGraphicsObject()
	{}

	void CSvgGraphicsObject::SetStroke(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stroke"))
			m_oStroke.m_oColor.SetValue(mAttributes.at(L"stroke"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-width"))
			m_oStroke.m_oWidth.SetValue(mAttributes.at(L"stroke-width"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetFill(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"fill"))
			m_oFill.SetValue(mAttributes.at(L"fill"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetTransform(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"transform"))
			m_oTransform.SetMatrix(mAttributes.at(L"transform"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::SetClip(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"clip-path"))
			m_oClip.SetValue(mAttributes.at(L"clip-path"), ushLevel, bHardMode);
	}

	void CSvgGraphicsObject::StartPath(IRenderer *pRenderer, const CDefs *pDefs, bool bIsClip) const
	{
		(bIsClip) ? StartClipPath(pRenderer) : StartStandardPath(pRenderer, pDefs);
	}

	void CSvgGraphicsObject::StartStandardPath(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		ApplyClip(pRenderer, pDefs);

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();
	}

	void CSvgGraphicsObject::StartClipPath(IRenderer *pRenderer) const
	{
		pRenderer->BeginCommand(c_nClipType);
		pRenderer->BeginCommand(c_nPathType);
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
		pRenderer->EndCommand(c_nPathType);
		pRenderer->EndCommand(c_nClipType);
		pRenderer->PathCommandEnd();
	}

	void CSvgGraphicsObject::ApplyDefaultStroke(IRenderer *pRenderer, int &nTypePath) const
	{
		nTypePath += c_nStroke;
		pRenderer->put_PenSize(1);
		pRenderer->put_PenColor(0);
		pRenderer->put_PenAlpha(255);
	}

	void CSvgGraphicsObject::ApplyStroke(IRenderer *pRenderer, int &nTypePath, bool bUseDedault) const
	{
		if (!m_oStroke.m_oColor.Empty())
		{
			if (NSCSS::NSProperties::ColorType::ColorNone == m_oStroke.m_oColor.GetType())
				return;

			nTypePath += c_nStroke;
			pRenderer->put_PenColor(m_oStroke.m_oColor.ToInt());

			double dStrokeWidth = m_oStroke.m_oWidth.ToDouble(NSCSS::Pixel);

			if (0 != dStrokeWidth)
				pRenderer->put_PenSize(dStrokeWidth);
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
			pRenderer->put_BrushAlpha1(255);
			pRenderer->put_BrushType(c_BrushTypeSolid);
		}
		else if (NSCSS::NSProperties::ColorType::ColorUrl == m_oFill.GetType() && NULL != pDefs)
		{
			if (ApplyDef(pRenderer, pDefs, m_oFill.ToWString()))
				nTypePath += c_nWindingFillMode;
		}
		else if (bUseDedault)
			ApplyDefaultFill(pRenderer, nTypePath);
	}

	void CSvgGraphicsObject::ApplyClip(IRenderer *pRenderer, const CDefs *pDefs) const
	{
		if (NULL == pRenderer || NULL == pDefs || m_oClip.Empty() || NSCSS::NSProperties::ColorType::ColorUrl != m_oClip.GetType())
			return;

		CDefObject *pClipObject = pDefs->GetDef(m_oClip.ToWString());

		if (NULL == pClipObject)
			return;

		pClipObject->Apply(pRenderer, pDefs, GetBounds());
	}

	void CSvgGraphicsObject::ApplyTransform(IRenderer *pRenderer, Aggplus::CMatrix& oOldMatrix) const
	{
		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		oOldMatrix.SetElements(dM11, dM12, dM21, dM22, dRx, dRy);

		Aggplus::CMatrix oMatrix(oOldMatrix);
		Aggplus::CMatrix oNewMatrix(m_oTransform.GetMatrix().GetValue());

		oMatrix.Multiply(&oNewMatrix);

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
