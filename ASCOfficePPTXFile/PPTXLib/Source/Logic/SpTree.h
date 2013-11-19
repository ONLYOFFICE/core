#pragma once
#ifndef PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_
#define PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Logic/NvGrpSpPr.h"
#include "./../Logic/GrpSpPr.h"
#include "SpTreeElem.h"
#include <list>
#include <string>
#include "ShapeProperties.h"

namespace PPTX
{
	namespace Logic
	{

		class SpTree : public WrapperWritingElement
		{
		public:
			SpTree();
			virtual ~SpTree();
			explicit SpTree(const XML::XNode& node);
			const SpTree& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void NormalizeRect(RECT& rect)const;
		public:
			property<Logic::NvGrpSpPr> nvGrpSpPr;
			property<Logic::GrpSpPr> grpSpPr;
			property<std::list<SpTreeElem> > SpTreeElems;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_