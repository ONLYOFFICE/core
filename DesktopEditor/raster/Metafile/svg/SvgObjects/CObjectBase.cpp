#include "CObjectBase.h"
#include "../CSvgFile.h"

namespace SVG
{
	TSvgStyles &TSvgStyles::operator+=(const TSvgStyles &oSvgStyles)
	{
		if (!oSvgStyles.m_oFill.Empty())
			m_oFill.SetValue(L'#' + oSvgStyles.m_oFill.ToWString(), 0, false);

		m_oFill.SetOpacity(std::to_wstring(oSvgStyles.m_oFill.GetOpacity()), 0, false);

		if (!oSvgStyles.m_oStroke.m_oColor.Empty())
			m_oStroke.m_oColor.SetValue(L'#' + oSvgStyles.m_oStroke.m_oColor.ToWString(), 0, false);

		m_oStroke.m_oWidth.SetValue(oSvgStyles.m_oStroke.m_oWidth.ToWString(), 0, false);

		if (m_oStroke.m_arDash.empty() && !oSvgStyles.m_oStroke.m_arDash.empty())
			m_oStroke.m_arDash = oSvgStyles.m_oStroke.m_arDash;

		if (m_oStroke.m_oLineCap.Empty() && !oSvgStyles.m_oStroke.m_oLineCap.Empty())
			m_oStroke.m_oLineCap = oSvgStyles.m_oStroke.m_oLineCap;

		if (m_oStroke.m_oLineJoin.Empty() && !oSvgStyles.m_oStroke.m_oLineJoin.Empty())
			m_oStroke.m_oLineJoin = oSvgStyles.m_oStroke.m_oLineJoin;

		return *this;
	}

	CObject::CObject(const NSCSS::CNode &oData)
		: m_oXmlNode(oData)
	{}

	CObject::CObject(XmlUtils::CXmlNode &oNode)
	{
		SetNodeData(oNode);
	}

	CObject::~CObject()
	{}

	void CObject::SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode)
	{
		if (wsStyles.empty())
			return;

		SetData(NSCSS::NS_STATIC_FUNCTIONS::GetRules(wsStyles), ushLevel, bHardMode);
	}

	void CObject::SetTransform(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"transform"))
			m_oTransformtaion.m_oTransform.SetMatrix(mAttributes.at(L"transform"), ushLevel, bHardMode);
		else
			m_oTransformtaion.m_oTransform.SetMatrix(L"", ushLevel, false);
	}

	void CObject::SetClip(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"clip-path"))
			m_oTransformtaion.m_oClip.m_oHref.SetValue(mAttributes.at(L"clip-path"), ushLevel, bHardMode);
		else
			m_oTransformtaion.m_oClip.m_oHref.SetValue(L"", ushLevel, false);

		if (mAttributes.end() != mAttributes.find(L"clip-rule"))
			m_oTransformtaion.m_oClip.m_oRule.SetValue(mAttributes.at(L"clip-rule"), std::vector<std::wstring>{L"nonzero", L"evenodd"}, ushLevel, bHardMode);
	}

	void CObject::SetMask(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"mask"))
			m_oTransformtaion.m_oMask.SetValue(mAttributes.at(L"mask"), ushLevel, bHardMode);
		else
			m_oTransformtaion.m_oMask.SetValue(L"", ushLevel, false);
	}

	void CObject::SetDisplay(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		std::wstring wsDisplay, wsVisibility;

		if (mAttributes.end() != mAttributes.find(L"display"))
			wsDisplay = mAttributes.at(L"display");

		if (mAttributes.end() != mAttributes.find(L"visibility"))
			wsVisibility = mAttributes.at(L"visibility");

		if (L"none" == wsDisplay || L"hidden" == wsVisibility || L"collapse" == wsVisibility)
			m_oTransformtaion.m_bDraw = false;
		else
			m_oTransformtaion.m_bDraw = true;
	}

	void CObject::SetOpacity(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"opacity"))
			m_oTransformtaion.m_oOpacity.SetValue(mAttributes.at(L"opacity"), ushLevel, bHardMode);
	}

	bool CObject::ApplyTransform(IRenderer *pRenderer, const NSCSS::NSProperties::CTransform *pTransform, Aggplus::CMatrix& oOldMatrix) const
	{
		if (NULL == pRenderer || NULL == pTransform)
			return false;

		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		oOldMatrix.SetElements(dM11, dM12, dM21, dM22, dRx, dRy);

		Aggplus::CMatrix oMatrix(oOldMatrix);

		pTransform->GetMatrix().ApplyTranform(oMatrix);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());

		return true;
	}

	bool CObject::ApplyClip(IRenderer *pRenderer, const TClip *pClip, const CSvgFile *pFile, const TBounds &oBounds) const
	{
		if (NULL == pRenderer || NULL == pClip || NULL == pFile)
			return false;

		if (pClip->m_oHref.Empty() || NSCSS::NSProperties::ColorType::ColorUrl != pClip->m_oHref.GetType())
			return true;

		if (pClip->m_oRule == L"evenodd")
			pRenderer->put_ClipMode(c_nClipRegionTypeEvenOdd);
		else
			pRenderer->put_ClipMode(c_nClipRegionTypeWinding);

		pRenderer->BeginCommand(c_nResetClipType);
		pRenderer->EndCommand(c_nResetClipType);

		return ApplyDef(pRenderer, pFile, pClip->m_oHref.ToWString(), oBounds);
	}

	bool CObject::ApplyMask(IRenderer *pRenderer, const NSCSS::NSProperties::CColor *pMask, const CSvgFile *pFile, const TBounds &oBounds) const
	{
		if (NULL == pRenderer || NULL == pMask || NULL == pFile || NSCSS::NSProperties::ColorType::ColorUrl != pMask->GetType())
			return false;

		return ApplyDef(pRenderer, pFile, pMask->ToWString(), oBounds);
	}
	
	bool CObject::ApplyDef(IRenderer *pRenderer, const CSvgFile *pFile, const std::wstring &wsUrl, const TBounds &oBounds) const
	{
		if (NULL == pRenderer || NULL == pFile || wsUrl.empty())
			return false;

		CAppliedObject *pDefObject = dynamic_cast<CAppliedObject*>(pFile->GetMarkedObject(wsUrl));

		if (NULL == pDefObject)
			return false;

		return pDefObject->Apply(pRenderer, pFile, oBounds);
	}

	void CObject::SetNodeData(XmlUtils::CXmlNode &oNode)
	{
		if (!oNode.IsValid())
			return;

		std::vector<std::wstring> arProperties, arValues;

		oNode.GetAllAttributes(arProperties, arValues);

		m_oXmlNode.m_wsName = oNode.GetName();

		for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
		{
			if (L"class" == arProperties[unIndex])
			{
				m_oXmlNode.m_wsClass = arValues[unIndex];
				std::transform(m_oXmlNode.m_wsClass.begin(), m_oXmlNode.m_wsClass.end(), m_oXmlNode.m_wsClass.begin(), std::towlower);
			}
			else if (L"id" == arProperties[unIndex])
				m_oXmlNode.m_wsId = arValues[unIndex];
			else if (L"style" == arProperties[unIndex])
				m_oXmlNode.m_wsStyle = arValues[unIndex];
			else
				m_oXmlNode.m_mAttributes.insert({arProperties[unIndex], arValues[unIndex]});
		}
	}

	std::wstring CObject::GetId() const
	{
		return m_oXmlNode.m_wsId;
	}

	std::vector<NSCSS::CNode> CObject::GetFullPath() const
	{
		return {m_oXmlNode};
	}

	CRenderedObject::CRenderedObject(const NSCSS::CNode &oData, CRenderedObject *pParent)
		: CObject(oData), m_pParent(pParent)
	{
		SetDefaultStyles();
	}

	CRenderedObject::CRenderedObject(XmlUtils::CXmlNode &oNode, CRenderedObject *pParent)
		: CObject(oNode), m_pParent(pParent)
	{
		SetDefaultStyles();
	}

	CRenderedObject::~CRenderedObject()
	{}

	ObjectType CRenderedObject::GetType() const
	{
		return RendererObject;
	}

	void CRenderedObject::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		SetTransform(mAttributes, ushLevel, bHardMode);
		SetDisplay  (mAttributes, ushLevel, bHardMode);
		SetOpacity  (mAttributes, ushLevel, bHardMode);
		SetClip     (mAttributes, ushLevel, bHardMode);
		SetMask     (mAttributes, ushLevel, bHardMode);
	}

	std::vector<NSCSS::CNode> CRenderedObject::GetFullPath() const
	{
		if (NULL == m_pParent)
			return {m_oXmlNode};

		std::vector<NSCSS::CNode> arObjects = m_pParent->GetFullPath();
		arObjects.push_back(m_oXmlNode);
		return arObjects;
	}

	void CRenderedObject::SetDefaultStyles()
	{
		m_oStyles.m_oStroke.m_oLineCap.SetMapping({std::make_pair(L"butt", Aggplus::LineCapFlat), std::make_pair(L"round", Aggplus::LineCapRound), std::make_pair(L"square", Aggplus::LineCapSquare)});
		m_oStyles.m_oStroke.m_oLineCap = Aggplus::LineCapFlat;

		m_oStyles.m_oStroke.m_oLineJoin.SetMapping({std::make_pair(L"arcs", Aggplus::LineJoinMiter), std::make_pair(L"bevel", Aggplus::LineJoinBevel), std::make_pair(L"miter", Aggplus::LineJoinMiter), std::make_pair(L"miter-clip", Aggplus::LineJoinMiterClipped), std::make_pair(L"round", Aggplus::LineJoinRound)});
		m_oStyles.m_oStroke.m_oLineCap = Aggplus::LineJoinMiter;

		m_oStyles.m_oStroke.m_oMiterlimit = 4.;
		
		m_oTransformtaion.m_oOpacity = 1.;
		m_oTransformtaion.m_bDraw = true;
	}

	void CRenderedObject::SetStroke(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stroke"))
			m_oStyles.m_oStroke.m_oColor.SetValue(mAttributes.at(L"stroke"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-width"))
			m_oStyles.m_oStroke.m_oWidth.SetValue(mAttributes.at(L"stroke-width"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-dasharray"))
			m_oStyles.m_oStroke.m_arDash = NSCSS::NS_STATIC_FUNCTIONS::ReadDoubleValues(mAttributes.at(L"stroke-dasharray"));

		if (mAttributes.end() != mAttributes.find(L"stroke-dashoffset"))
			m_oStyles.m_oStroke.m_oDashOffset.SetValue(mAttributes.at(L"stroke-dashoffset"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-linecap"))
			m_oStyles.m_oStroke.m_oLineCap.SetValue(mAttributes.at(L"stroke-linecap"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-linejoin"))
			m_oStyles.m_oStroke.m_oLineJoin.SetValue(mAttributes.at(L"stroke-linejoin"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-opacity"))
			m_oStyles.m_oStroke.m_oColor.SetOpacity(mAttributes.at(L"stroke-opacity"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-miterlimit"))
			m_oStyles.m_oStroke.m_oMiterlimit.SetValue(mAttributes.at(L"stroke-miterlimit"), ushLevel, bHardMode);
	}

	void CRenderedObject::SetFill(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"fill"))
			m_oStyles.m_oFill.SetValue(mAttributes.at(L"fill"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"fill-opacity"))
			m_oStyles.m_oFill.SetOpacity(mAttributes.at(L"fill-opacity"), ushLevel, bHardMode);
	}

	bool CRenderedObject::StartPath(IRenderer *pRenderer, const CSvgFile *pFile, Aggplus::CMatrix &oOldTransform, CommandeMode oMode) const
	{
		if (NULL == pRenderer || !m_oTransformtaion.m_bDraw || Equals(0., m_oTransformtaion.m_oOpacity.ToDouble()))
			return false;

		ApplyTransform(pRenderer, &m_oTransformtaion.m_oTransform, oOldTransform);
		ApplyClip(pRenderer, &m_oTransformtaion.m_oClip, pFile, GetBounds());
		ApplyMask(pRenderer, &m_oTransformtaion.m_oMask, pFile, GetBounds());

		if (CommandeModeClip == oMode)
			pRenderer->BeginCommand(c_nClipType);
		else if (1. != m_oTransformtaion.m_oOpacity.ToDouble())
		{
			pRenderer->BeginCommand(c_nLayerType);
			pRenderer->put_LayerOpacity(m_oTransformtaion.m_oOpacity.ToDouble());
		}

		pRenderer->BeginCommand(c_nPathType);
		pRenderer->PathCommandStart();

		return true;
	}

	void CRenderedObject::EndPath(IRenderer *pRenderer, const CSvgFile *pFile, const Aggplus::CMatrix &oOldTransform, CommandeMode oMode, const TSvgStyles* pOtherStyles) const
	{
		if (CommandeModeClip == oMode)
		{
			pRenderer->EndCommand(c_nClipType);
			pRenderer->EndCommand(c_nPathType);
			pRenderer->PathCommandEnd();
			pRenderer->SetTransform(oOldTransform.sx(), oOldTransform.shy(), oOldTransform.shx(), oOldTransform.sy(), oOldTransform.tx(), oOldTransform.ty());
			return;
		}
		
		int nPathType = 0;

		if (NULL == pOtherStyles)
			ApplyStyle(pRenderer, &m_oStyles, pFile, nPathType);
		else
		{
			TSvgStyles oNewStyles(m_oStyles);
			oNewStyles += *pOtherStyles;
			ApplyStyle(pRenderer, &oNewStyles, pFile, nPathType);
		}

		pRenderer->DrawPath(nPathType);
		pRenderer->EndCommand(c_nPathType);
		pRenderer->PathCommandEnd();

		if (!m_oTransformtaion.m_oClip.m_oHref.Empty())
		{
			pRenderer->BeginCommand(c_nResetClipType);
			pRenderer->EndCommand(c_nResetClipType);
		}

		if (!m_oTransformtaion.m_oMask.Empty())
		{
			pRenderer->BeginCommand(c_nResetMaskType);
			pRenderer->EndCommand(c_nResetMaskType);
		}
		
		if (1. != m_oTransformtaion.m_oOpacity.ToDouble())
			pRenderer->EndCommand(c_nLayerType);

		pRenderer->SetTransform(oOldTransform.sx(), oOldTransform.shy(), oOldTransform.shx(), oOldTransform.sy(), oOldTransform.tx(), oOldTransform.ty());
	}

	void CRenderedObject::ApplyStyle(IRenderer *pRenderer, const TSvgStyles *pStyles, const CSvgFile *pFile, int &nTypePath) const
	{}

	bool CRenderedObject::ApplyStroke(IRenderer *pRenderer, const TStroke *pStroke, bool bUseDefault) const
	{
		if (NULL == pRenderer || NULL == pStroke || NSCSS::NSProperties::ColorType::ColorNone == pStroke->m_oColor.GetType() || (!bUseDefault && ((pStroke->m_oWidth.Empty() || pStroke->m_oWidth.Zero()) && pStroke->m_oColor.Empty())))
		{
			pRenderer->put_PenSize(0);
			return false;
		}

		double dStrokeWidth = pStroke->m_oWidth.ToDouble(NSCSS::Pixel);

		if (Equals(0., dStrokeWidth))
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

		pRenderer->put_PenMiterLimit(pStroke->m_oMiterlimit.ToDouble());
		pRenderer->put_PenLineJoin(pStroke->m_oLineJoin.ToInt());

		return true;
	}

	bool CRenderedObject::ApplyFill(IRenderer *pRenderer, const NSCSS::NSProperties::CColor *pFill, const CSvgFile *pFile, bool bUseDefault) const
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
			if (!ApplyDef(pRenderer, pFile, pFill->ToWString(), GetBounds()))
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

	bool CRenderedObject::ApplyOpacity(IRenderer *pRenderer, const NSCSS::NSProperties::CDigit *pOpacity) const
	{
		if (NULL == pRenderer || NULL == pOpacity)
			return false;

		return false;
	}

	CAppliedObject::CAppliedObject(XmlUtils::CXmlNode &oNode)
		: CObject(oNode)
	{}

	CAppliedObject::~CAppliedObject()
	{}

	ObjectType CAppliedObject::GetType() const
	{
		return AppliedObject;
	}
}
