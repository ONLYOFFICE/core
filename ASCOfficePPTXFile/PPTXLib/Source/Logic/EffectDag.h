#pragma once
#ifndef PPTX_LOGIC_EFFECTDAG_INCLUDE_H_
#define PPTX_LOGIC_EFFECTDAG_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "UniEffect.h"
#include "./../Limit/EffectContainerType.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{

		class EffectDag : public WrapperWritingElement
		{
		public:
			EffectDag();
			virtual ~EffectDag();
			explicit EffectDag(const XML::XNode& node);
			const EffectDag& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::list<UniEffect> > Effects;
			nullable_property<std::string> name;
			nullable_property<std::string, Limit::EffectContainerType> type;
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_EFFECTDAG_INCLUDE_H_