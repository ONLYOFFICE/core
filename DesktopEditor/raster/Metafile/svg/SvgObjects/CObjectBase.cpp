#include "CObjectBase.h"

#include "CDefs.h"

namespace SVG
{
	CObjectBase::CObjectBase(CObjectBase *pParent)
	    : m_pParent(pParent)
	{}

	CObjectBase::~CObjectBase()
	{}

	std::vector<NSCSS::CNode> CObjectBase::GetFullPath() const
	{
		    if (NULL == m_pParent)
				return {m_oXmlNode};

			std::vector<NSCSS::CNode> arObjects = m_pParent->GetFullPath();
			arObjects.push_back({m_oXmlNode});
			return arObjects;
	}

	void CObjectBase::SetData(const std::wstring wsStyles, unsigned short ushLevel, bool bHardMode)
	{
		if (wsStyles.empty())
			return;

		const std::vector<std::wstring> arWords = NSCSS::NS_STATIC_FUNCTIONS::GetWordsWithSigns(wsStyles, L":;");

		if (arWords.empty())
			return;

		std::wstring wsProperty, wsValue;

		std::map<std::wstring, std::wstring> mAttributes;

		for (std::vector<std::wstring>::const_iterator iWord = arWords.begin(); iWord != arWords.end(); ++iWord)
		{
			if ((*iWord).back() == L':')
			{
				wsProperty = *iWord;
				wsProperty.pop_back();
			}
			else
			{
				wsValue += *iWord;

				if (L' ' == wsValue.front())
					wsValue.erase(0, 1);

				if (!wsValue.empty() && ((*iWord).back() == L';' || iWord == (arWords.end() - 1)))
				{
					if (wsValue.back() == L';')
						wsValue.pop_back();

					std::transform(wsProperty.begin(), wsProperty.end(), wsProperty.begin(), tolower);
					std::transform(wsValue.begin(), wsValue.end(), wsValue.begin(), tolower);
					mAttributes[wsProperty] = wsValue;
					wsProperty.clear();
					wsValue.clear();
				}
			}
		}

		if (!mAttributes.empty())
			SetData(mAttributes, ushLevel, bHardMode);
	}

	std::wstring CObjectBase::GetId() const
	{
		return m_oXmlNode.m_sId;
	}

	void CObjectBase::SaveNodeData(XmlUtils::CXmlNode &oNode)
	{
		std::vector<std::wstring> arProperties, arValues;

		oNode.GetAllAttributes(arProperties, arValues);

		NSCSS::CNode oXmlNode;
		m_oXmlNode.m_sName  = oNode.GetName();

		for (unsigned int unIndex = 0; unIndex < arProperties.size(); ++unIndex)
		{
			if (L"class" == arProperties[unIndex])
				m_oXmlNode.m_sClass = arValues[unIndex];
			else if (L"id" == arProperties[unIndex])
				m_oXmlNode.m_sId = arValues[unIndex];
			else if (L"style" == arProperties[unIndex])
				m_oXmlNode.m_sStyle = arValues[unIndex];
			else
				m_oXmlNode.m_mAttrs.insert({arProperties[unIndex], arValues[unIndex]});
		}
	}

	void CObjectBase::SetStroke(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"stroke"))
			m_oStroke.m_oColor.SetValue(mAttributes.at(L"stroke"), ushLevel, bHardMode);

		if (mAttributes.end() != mAttributes.find(L"stroke-width"))
			m_oStroke.m_oWidth.SetValue(mAttributes.at(L"stroke-width"), ushLevel, bHardMode);
	}

	void CObjectBase::SetFill(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"fill"))
			m_oFill.SetValue(mAttributes.at(L"fill"), ushLevel, bHardMode);
	}

	void CObjectBase::SetTransform(const std::map<std::wstring, std::wstring> &mAttributes, unsigned short ushLevel, bool bHardMode)
	{
		if (mAttributes.end() != mAttributes.find(L"transform"))
			m_oTransform.SetMatrix(mAttributes.at(L"transform"), ushLevel, bHardMode);
	}

	void CObjectBase::ApplyDefaultStroke(IRenderer *pRenderer, int &nTypePath) const
	{
		nTypePath += c_nStroke;
		pRenderer->put_PenSize(1);
		pRenderer->put_PenColor(0);
	}

	void CObjectBase::ApplyStroke(IRenderer *pRenderer, int &nTypePath, bool bUseDedault) const
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

	void CObjectBase::ApplyDefaultFill(IRenderer *pRenderer, int &nTypePath) const
	{
		nTypePath += c_nWindingFillMode;
		pRenderer->put_BrushColor1(0);
	}

	void CObjectBase::ApplyFill(IRenderer *pRenderer, CDefs *pDefs, int &nTypePath, bool bUseDedault) const
	{
		if (NSCSS::NSProperties::ColorType::ColorNone == m_oFill.GetType())
			return;
		else if (NSCSS::NSProperties::ColorType::ColorHEX == m_oFill.GetType() ||
		         NSCSS::NSProperties::ColorType::ColorRGB == m_oFill.GetType())
		{
			nTypePath += c_nWindingFillMode;
			pRenderer->put_BrushColor1(m_oFill.ToInt());
		}
		else if (NSCSS::NSProperties::ColorType::ColorUrl == m_oFill.GetType() && NULL != pDefs)
			ApplyFillObject(pRenderer, pDefs->GetDef(m_oFill.ToWString()), pDefs);
		else if (bUseDedault)
			ApplyDefaultFill(pRenderer, nTypePath);
	}

	void CObjectBase::ApplyTransform(IRenderer *pRenderer, Aggplus::CMatrix& oOldMatrix) const
	{
		double dM11, dM12, dM21, dM22, dRx, dRy;

		pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dRx, &dRy);

		oOldMatrix.SetElements(dM11, dM12, dM21, dM22, dRx, dRy);

		Aggplus::CMatrix oMatrix(oOldMatrix);
		Aggplus::CMatrix oNewMatrix(m_oTransform.GetMatrix().GetValue());

		oMatrix.Multiply(&oNewMatrix);

		pRenderer->SetTransform(oMatrix.sx(), oMatrix.shy(), oMatrix.shx(), oMatrix.sy(), oMatrix.tx(), oMatrix.ty());
	}
}
