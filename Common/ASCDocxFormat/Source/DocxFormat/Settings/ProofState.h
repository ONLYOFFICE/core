#pragma once
#ifndef OOX_SETTINGS_PROOF_STATE_INCLUDE_H_
#define OOX_SETTINGS_PROOF_STATE_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable.h"


namespace OOX
{
	namespace Settings
	{
		class ProofState : public WritingElement
		{
		public:
			ProofState();
			virtual ~ProofState();
			explicit ProofState(const XML::XNode& node);
			const ProofState& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			nullable__<std::string>	m_spelling;
			std::string						m_grammar;
		};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTINGS_PROOF_STATE_INCLUDE_H_