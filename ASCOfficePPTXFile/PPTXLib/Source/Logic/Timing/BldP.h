#pragma once
#ifndef PPTX_LOGIC_BLDP_INCLUDE_H_
#define PPTX_LOGIC_BLDP_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "./../../Limit/ParaBuildType.h"
#include "TmplLst.h"

namespace PPTX
{
	namespace Logic
	{
		class BldP : public WrapperWritingElement
		{
		public:
			BldP();
			virtual ~BldP();			
			explicit BldP(const XML::XNode& node);
			const BldP& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TmplLst> tmplLst;

			property<std::string> spid;
			property<int, setter::only_positive<int> > grpId;
			nullable_property<bool> uiExpand;
			nullable_property<std::string, Limit::ParaBuildType> build;
			nullable_property<int, setter::only_positive<int> > bldLvl;
			nullable_property<bool> animBg;
			nullable_property<bool> autoUpdateAnimBg;
			nullable_property<bool> rev;
			nullable_property<std::string> advAuto;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLDP_INCLUDE_H