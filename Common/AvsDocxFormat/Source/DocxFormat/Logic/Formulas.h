#pragma once
#ifndef OOX_LOGIC_FORMULAS_INCLUDE_H_
#define OOX_LOGIC_FORMULAS_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "property.h"
#include "F.h"

namespace OOX
{
	namespace Logic
	{
		class Formulas : public WritingElement
		{
		public:
			Formulas();
			virtual ~Formulas();			
			explicit Formulas(const XML::XNode& node);
			const Formulas& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<F> > Fs;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_FORMULAS_INCLUDE_H_