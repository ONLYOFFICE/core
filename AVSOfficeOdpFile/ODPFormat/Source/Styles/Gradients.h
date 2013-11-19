#pragma once
#ifndef ODP_STYLES_GRADIENTS_INCLUDE_H_
#define ODP_STYLES_GRADIENTS_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "Gradient.h"

namespace Odp
{
	namespace Styles
	{
		class Gradients : public Odt::WritingVector<Gradient>
		{
		public:
			Gradients();
			virtual ~Gradients();
			explicit Gradients(const XML::XNode& node);
			const Gradients& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const Gradient& operator[](const std::string& name) const;
			Gradient& operator[](const std::string& name);
			const Gradient& find(const std::string& name) const;
			Gradient& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_GRADIENTS_INCLUDE_H_