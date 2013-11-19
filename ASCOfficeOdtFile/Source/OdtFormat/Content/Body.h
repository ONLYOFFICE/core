#pragma once
#ifndef ODT_CONTENT_BODY_INCLUDE_H_
#define ODT_CONTENT_BODY_INCLUDE_H_

#include "./../WritingElement.h"
#include "Text.h"
#include <vector>
#include "property.h"
#include "Forms.h"
#include "SequenceDecls.h"
#include "VariableDecls.h"
#include "nullable.h"
#include "Frame.h"


namespace Odt
{
	namespace Content
	{
		class Body : public WritingElement
		{
		public:
			Body();
			virtual ~Body();
			explicit Body(const XML::XNode& node);
			const Body& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasList() const;
			const bool hasNote() const;

		public:
			property<bool>								UseSoftPageBreaks;
			property<std::vector<Text> >	Items;
			property<std::vector<Frame> >	Frames;

		private:
			nullable<Forms>					m_forms;
			SequenceDecls						m_sequenceDecls;
			nullable<VariableDecls>	m_variableDecls;

		private:
			bool	m_hasList;
			bool	m_hasNote;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_BODY_INCLUDE_H_