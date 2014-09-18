//#include "./stdafx.h"

#include "TimeNodeBase.h"
#include "Seq.h"
#include "Par.h"
#include "Audio.h"
#include "Video.h"
#include "Excl.h"
#include "Anim.h"
#include "AnimClr.h"
#include "AnimEffect.h"
#include "AnimMotion.h"
#include "AnimRot.h"
#include "AnimScale.h"
#include "Cmd.h"
#include "Set.h"

namespace PPTX
{
	namespace Logic
	{

		TimeNodeBase::TimeNodeBase()
		{
		}

		TimeNodeBase::~TimeNodeBase()
		{
		}

		TimeNodeBase::TimeNodeBase(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const TimeNodeBase& TimeNodeBase::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TimeNodeBase::fromXML(XmlUtils::CXmlNode& node)
		{
			CString name = XmlUtils::GetNameNoNS(node.GetName());

			if(name == _T("seq"))
				m_node.reset(new Logic::Seq(node));
			else if(name == _T("par"))
				m_node.reset(new Logic::Par(node));
			else if(name == _T("audio"))
				m_node.reset(new Logic::Audio(node));
			else if(name == _T("video"))
				m_node.reset(new Logic::Video(node));
			else if(name == _T("excl"))
				m_node.reset(new Logic::Excl(node));
			else if(name == _T("anim"))
				m_node.reset(new Logic::Anim(node));
			else if(name == _T("animClr"))
				m_node.reset(new Logic::AnimClr(node));
			else if(name == _T("animEffect"))
				m_node.reset(new Logic::AnimEffect(node));
			else if(name == _T("animMotion"))
				m_node.reset(new Logic::AnimMotion(node));
			else if(name == _T("animRot"))
				m_node.reset(new Logic::AnimRot(node));
			else if(name == _T("animScale"))
				m_node.reset(new Logic::AnimScale(node));
			else if(name == _T("cmd"))
				m_node.reset(new Logic::Cmd(node));
			else if(name == _T("set"))
				m_node.reset(new Logic::Set(node));
			else m_node.reset();
		}

		void TimeNodeBase::GetTimeNodeFrom(XmlUtils::CXmlNode& element)
		{
			XmlUtils::CXmlNode oNode;			
			if(element.GetNode(_T("p:seq"), oNode))
				m_node.reset(new Logic::Seq(oNode));
			else if(element.GetNode(_T("p:par"), oNode))
				m_node.reset(new Logic::Par(oNode));
			else if(element.GetNode(_T("p:audio"), oNode))
				m_node.reset(new Logic::Audio(oNode));
			else if(element.GetNode(_T("p:video"), oNode))
				m_node.reset(new Logic::Video(oNode));
			else if(element.GetNode(_T("p:excl"), oNode))
				m_node.reset(new Logic::Excl(oNode));
			else if(element.GetNode(_T("p:anim"), oNode))
				m_node.reset(new Logic::Anim(oNode));
			else if(element.GetNode(_T("p:animClr"), oNode))
				m_node.reset(new Logic::AnimClr(oNode));
			else if(element.GetNode(_T("p:animEffect"), oNode))
				m_node.reset(new Logic::AnimEffect(oNode));
			else if(element.GetNode(_T("p:animMotion"), oNode))
				m_node.reset(new Logic::AnimMotion(oNode));
			else if(element.GetNode(_T("p:animRot"), oNode))
				m_node.reset(new Logic::AnimRot(oNode));
			else if(element.GetNode(_T("p:animScale"), oNode))
				m_node.reset(new Logic::AnimScale(oNode));
			else if(element.GetNode(_T("p:cmd"), oNode))
				m_node.reset(new Logic::Cmd(oNode));
			else if(element.GetNode(_T("p:set"), oNode))
				m_node.reset(new Logic::Set(oNode));
			else m_node.reset();
		}

		CString TimeNodeBase::toXML() const
		{
			if (m_node.IsInit())
				return m_node->toXML();
			return _T("");
		}

	} // namespace Logic
} // namespace PPTX