#pragma once
#ifndef ODT_CONTENT_VARIABLE_DECLS_INCLUDE_H_
#define ODT_CONTENT_VARIABLE_DECLS_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <vector>
#include "VariableDecl.h"


namespace Odt
{
	namespace Content
	{
		class VariableDecls : public WritingElement
		{
		public:
			VariableDecls();
			virtual ~VariableDecls();
			explicit VariableDecls(const XML::XNode& node);
			const VariableDecls& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			std::vector<VariableDecl> m_list;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_VARIABLE_DECLS_INCLUDE_H_