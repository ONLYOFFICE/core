#pragma once
#ifndef PPTX_LOGIC_PRSTTXWARP_INCLUDE_H_
#define PPTX_LOGIC_PRSTTXWARP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "./../Limit/TextShapeType.h"
#include "Gd.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{
		class PrstTxWarp : public WrapperWritingElement
		{
		public:
			PrstTxWarp();
			virtual ~PrstTxWarp();			
			explicit PrstTxWarp(const XML::XNode& node);
			const PrstTxWarp& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::TextShapeType> prst;
			property<std::list<Gd> > avLst;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTTXWARP_INCLUDE_H