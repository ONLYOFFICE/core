#include "./stdafx.h"

#include "SpPr.h"
#include "PrstGeom.h"
#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{

		SpPr::SpPr()
		{
		}


		SpPr::~SpPr()
		{
		}
	

		SpPr::SpPr(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const SpPr& SpPr::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SpPr::fromXML(XmlUtils::CXmlNode& node)
		{
			m_namespace = XmlUtils::GetNamespace(node.GetName());

			Geometry.GetGeometryFrom(node);
			Fill.GetFillFrom(node);
			EffectList.GetEffectListFrom(node);

			node.ReadAttributeBase(L"bwMode", bwMode);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("xfrm") == strName)
						xfrm = oNode;
					else if (_T("ln") == strName)
						ln = oNode;
					else if (_T("scene3d") == strName)
						scene3d = oNode;
					else if (_T("sp3d") == strName)
						sp3d = oNode;
				}
			}

			FillParentPointersForChilds();
		}


		CString SpPr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("bwMode"), bwMode);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(xfrm);
			oValue.Write(Geometry);
			oValue.Write(Fill);
			oValue.WriteNullable(ln);
			oValue.Write(EffectList);
			oValue.WriteNullable(scene3d);
			oValue.WriteNullable(sp3d);

			return XmlUtils::CreateNode(m_namespace + _T(":spPr"), oAttr, oValue);
		}

		void SpPr::Merge(SpPr& spPr)const
		{
			if(xfrm.IsInit())
				xfrm->Merge(spPr.xfrm);
			if(Geometry.is_init())
				spPr.Geometry = Geometry;
			if(Fill.is_init())
				spPr.Fill = Fill;//.fromXML(Fill.toXML());
			if(ln.IsInit())
				ln->Merge(spPr.ln);
		}

		void SpPr::FillParentPointersForChilds()
		{
			if(xfrm.IsInit())
				xfrm->SetParentPointer(this);
			Geometry.SetParentPointer(this);
			Fill.SetParentPointer(this);
			if(ln.IsInit())
				ln->SetParentPointer(this);
			EffectList.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
			if(sp3d.IsInit())
				sp3d->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX