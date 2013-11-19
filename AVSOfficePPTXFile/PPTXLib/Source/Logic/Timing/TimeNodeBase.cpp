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

		TimeNodeBase::TimeNodeBase(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TimeNodeBase& TimeNodeBase::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TimeNodeBase::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string name = element.XName->Name;

			if(name == "seq")
				m_node.reset(new Logic::Seq(node));
			else if(name == "par")
				m_node.reset(new Logic::Par(node));
			else if(name == "audio")
				m_node.reset(new Logic::Audio(node));
			else if(name == "video")
				m_node.reset(new Logic::Video(node));
			else if(name == "excl")
				m_node.reset(new Logic::Excl(node));
			else if(name == "anim")
				m_node.reset(new Logic::Anim(node));
			else if(name == "animClr")
				m_node.reset(new Logic::AnimClr(node));
			else if(name == "animEffect")
				m_node.reset(new Logic::AnimEffect(node));
			else if(name == "animMotion")
				m_node.reset(new Logic::AnimMotion(node));
			else if(name == "animRot")
				m_node.reset(new Logic::AnimRot(node));
			else if(name == "animScale")
				m_node.reset(new Logic::AnimScale(node));
			else if(name == "cmd")
				m_node.reset(new Logic::Cmd(node));
			else if(name == "set")
				m_node.reset(new Logic::Set(node));
			else m_node.reset();
		}

		void TimeNodeBase::GetTimeNodeFrom(const XML::XElement& element)
		{
			if(element.element("seq").exist())
				m_node.reset(new Logic::Seq(element.element("seq")));
			else if(element.element("par").exist())
				m_node.reset(new Logic::Par(element.element("par")));
			else if(element.element("audio").exist())
				m_node.reset(new Logic::Audio(element.element("audio")));
			else if(element.element("video").exist())
				m_node.reset(new Logic::Video(element.element("video")));
			else if(element.element("excl").exist())
				m_node.reset(new Logic::Excl(element.element("excl")));
			else if(element.element("anim").exist())
				m_node.reset(new Logic::Anim(element.element("anim")));
			else if(element.element("animClr").exist())
				m_node.reset(new Logic::AnimClr(element.element("animClr")));
			else if(element.element("animEffect").exist())
				m_node.reset(new Logic::AnimEffect(element.element("animEffect")));
			else if(element.element("animMotion").exist())
				m_node.reset(new Logic::AnimMotion(element.element("animMotion")));
			else if(element.element("animRot").exist())
				m_node.reset(new Logic::AnimRot(element.element("animRot")));
			else if(element.element("animScale").exist())
				m_node.reset(new Logic::AnimScale(element.element("animScale")));
			else if(element.element("cmd").exist())
				m_node.reset(new Logic::Cmd(element.element("cmd")));
			else if(element.element("set").exist())
				m_node.reset(new Logic::Set(element.element("set")));
			else m_node.reset();
		}

		const XML::XNode TimeNodeBase::toXML() const
		{
			return XML::Write(m_node);
		}

	} // namespace Logic
} // namespace PPTX