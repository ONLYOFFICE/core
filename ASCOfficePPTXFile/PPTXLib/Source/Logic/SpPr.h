#pragma once
#ifndef PPTX_LOGIC_SPPR_INCLUDE_H_
#define PPTX_LOGIC_SPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "./../Limit/BWMode.h"
#include "Xfrm.h"
#include "UniFill.h"
#include "Ln.h"
#include "EffectProperties.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "Geometry.h"

namespace PPTX
{
	namespace Logic
	{
		class SpPr : public WrapperWritingElement
		{
		public:
			SpPr();
			virtual ~SpPr();			
			explicit SpPr(const XML::XNode& node);
			const SpPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(SpPr& spPr)const;

		public:
			nullable_property<Xfrm> xfrm;
			Logic::Geometry Geometry;
			UniFill Fill;
			nullable_property<Ln> ln;
			EffectProperties EffectList;
			nullable_property<Scene3d> scene3d;
			nullable_property<Sp3d> sp3d;

			nullable_property<std::string, Limit::BWMode> bwMode;
		//private:
		public:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SPPR_INCLUDE_H