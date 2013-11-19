#pragma once
#ifndef ODT_CONTENT_SEQUENCE_DECL_INCLUDE_H_
#define ODT_CONTENT_SEQUENCE_DECL_INCLUDE_H_

#include <string>
#include "nullable.h"
#include "./../WritingElement.h"


namespace Odt
{
	namespace Content
	{
		class SequenceDecl : public WritingElement
		{
		public:
			SequenceDecl();
			virtual ~SequenceDecl();
			explicit SequenceDecl(const XML::XNode& node);
			const SequenceDecl& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			int											m_level;
			nullable<std::string>		m_separation;
			std::string							m_name;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_SEQUENCE_DECL_INCLUDE_H_