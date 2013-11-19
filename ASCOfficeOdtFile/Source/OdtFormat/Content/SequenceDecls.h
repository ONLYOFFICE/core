#pragma once
#ifndef ODT_CONTENT_SEQUENCE_DECLS_INCLUDE_H_
#define ODT_CONTENT_SEQUENCE_DECLS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "SequenceDecl.h"


namespace Odt
{
	namespace Content
	{
		class SequenceDecls : public WritingElement
	{
		public:
			SequenceDecls();
			virtual ~SequenceDecls();
			explicit SequenceDecls(const XML::XNode& node);
			const SequenceDecls& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<SequenceDecl> m_list;
		};
	}	// namespace Content
} // namespace Odt

#endif // ODT_CONTENT_SEQUENCE_DECLS_INCLUDE_H_