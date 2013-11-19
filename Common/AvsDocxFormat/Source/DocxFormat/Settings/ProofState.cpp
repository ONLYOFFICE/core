
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ProofState.h"


namespace OOX
{
	namespace Settings
	{

		ProofState::ProofState()
		{
		}


		ProofState::~ProofState()
		{
		}


		ProofState::ProofState(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ProofState& ProofState::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ProofState::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_spelling	=	element.attribute("spelling").value();
			m_grammar		=	element.attribute("grammar").value();
		}


		const XML::XNode ProofState::toXML() const
		{
			return
				XML::XElement(ns.w + "proofState", 
					XML::XAttribute(ns.w + "spelling", m_spelling) + 
					XML::XAttribute(ns.w + "grammar", m_grammar)
				);
		}

	} // namespace Settings
} // namespace OOX