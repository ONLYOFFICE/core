//#include "./stdafx.h"

#include "BuildNodeBase.h"
#include "BldP.h"
#include "BldDgm.h"
#include "BldGraphic.h"
#include "BldOleChart.h"

namespace PPTX
{
	namespace Logic
	{

		BuildNodeBase::BuildNodeBase()
		{
		}

		BuildNodeBase::~BuildNodeBase()
		{
		}

		BuildNodeBase::BuildNodeBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const BuildNodeBase& BuildNodeBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BuildNodeBase::fromXML(XmlUtils::CXmlNode& node)
		{
			CString name = XmlUtils::GetNameNoNS(node.GetName());

			if (name == _T("bldP"))
				m_node.reset(new Logic::BldP(node));
			else if (name == _T("bldDgm"))
				m_node.reset(new Logic::BldDgm(node));
			else if (name == _T("bldGraphic"))
				m_node.reset(new Logic::BldGraphic(node));
			else if (name == _T("bldOleChart"))
				m_node.reset(new Logic::BldOleChart(node));
			else m_node.reset();
		}

		void BuildNodeBase::GetBuildNodeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;
			
			if (element.GetNode(_T("p:bldP"), oNode))
				m_node.reset(new Logic::BldP(oNode));
			else if (element.GetNode(_T("p:bldDgm"), oNode))
				m_node.reset(new Logic::BldDgm(oNode));
			else if (element.GetNode(_T("p:bldGraphic"), oNode))
				m_node.reset(new Logic::BldGraphic(oNode));
			else if (element.GetNode(_T("p:bldOleChart"), oNode))
				m_node.reset(new Logic::BldOleChart(oNode));
			else m_node.reset();
		}

		CString BuildNodeBase::toXML() const
		{
			if (m_node.IsInit())
				return m_node->toXML();
			return _T("");
		}

	} // namespace Logic
} // namespace PPTX