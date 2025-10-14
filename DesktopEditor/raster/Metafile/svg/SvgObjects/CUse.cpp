#include "CUse.h"
#include "../CSvgFile.h"

namespace SVG
{
	CUse::CUse(CSvgReader& oReader, CRenderedObject *pParent)
		: CRenderedObject(oReader, pParent)
	{}

	CUse::~CUse()
	{}

	void CUse::SetAttribute(const std::string& sName, CSvgReader& oReader)
	{
		if ("x" == sName)
			m_oX.SetValue(oReader.GetDouble());
		else if ("y" == sName)
			m_oY.SetValue(oReader.GetDouble());
		else if ("width" == sName)
			m_oWidth.SetValue(oReader.GetDouble());
		else if ("height" == sName)
			m_oHeight.SetValue(oReader.GetDouble());
		else if ("href" == sName || "xlink:href" == sName)
			m_wsHref = oReader.GetText();
		else
			CRenderedObject::SetAttribute(sName, oReader);
	}

	void CUse::SetData(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		CRenderedObject::SetData(mAttributes, ushLevel, bHardMode);

		SetStroke(mAttributes, ushLevel, bHardMode);
		SetFill(mAttributes, ushLevel, bHardMode);
	}

	bool CUse::Draw(IRenderer *pRenderer, const CSvgFile *pFile, CommandeMode oMode, const TSvgStyles* pOtherStyles, const CRenderedObject* pContexObject) const
	{
		if (NULL == pRenderer || !m_oTransformation.m_bDraw)
			return false;

		Aggplus::CMatrix oOldTransform;

		if (!StartPath(pRenderer, pFile, oOldTransform))
			return false;

		double dM11, dM12, dM21, dM22, dDx, dDy;
		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

		Aggplus::CMatrix oNewTransform(dM11, dM12, dM21, dM22, dDx, dDy);
		oNewTransform.Translate(m_oX.ToDouble(NSCSS::Pixel), m_oY.ToDouble(NSCSS::Pixel));

		pRenderer->SetTransform(oNewTransform.sx(), oNewTransform.shy(), oNewTransform.shx(), oNewTransform.sy(), oNewTransform.tx(), oNewTransform.ty());

		const CRenderedObject *pFoundObj = dynamic_cast<CRenderedObject*>(pFile->GetMarkedObject(m_wsHref));

		bool bResult = false;

		if (NULL != pFoundObj && this != pFoundObj)
		{
			if (NULL != pOtherStyles)
			{
				TSvgStyles oNewStyles(m_oStyles);
				oNewStyles += *pOtherStyles;
				bResult = pFoundObj->Draw(pRenderer, pFile, oMode, &oNewStyles, this);
			}
			else
				bResult = pFoundObj->Draw(pRenderer, pFile, oMode, &m_oStyles, this);
		}

		EndPath(pRenderer, pFile, oOldTransform);

		return bResult;
	}

	TBounds CUse::GetBounds() const
	{
		return TBounds{0., 0., 0., 0.};
	}
}
