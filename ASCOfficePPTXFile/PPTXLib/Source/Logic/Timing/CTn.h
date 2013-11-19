#pragma once
#ifndef PPTX_LOGIC_CTN_INCLUDE_H_
#define PPTX_LOGIC_CTN_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include "CondLst.h"
#include "Cond.h"
#include "TnLst.h"
#include "Iterate.h"
#include "./../../Limit/TLRestart.h"
#include "./../../Limit/TLNodeType.h"
#include "./../../Limit/TLNodeFillType.h"
#include "./../../Limit/TLPresetClass.h"
#include "./../../Limit/TLMasterRelation.h"
#include "./../../Limit/TLSyncBehavior.h"

namespace PPTX
{
	namespace Logic
	{
		class CTn : public WrapperWritingElement
		{
		public:
			CTn();
			virtual ~CTn();			
			explicit CTn(const XML::XNode& node);
			const CTn& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			//Childs
			nullable_property<CondLst> stCondLst;
			nullable_property<CondLst> endCondLst;
			nullable_property<Cond> endSync;
			nullable_property<Iterate> iterate;
			nullable_property<TnLst> childTnLst;
			nullable_property<TnLst> subTnLst;


			//Attributes
			nullable_property<int> accel;
			nullable_property<bool> afterEffect;
			nullable_property<bool> autoRev;
			nullable_property<int> bldLvl;
			nullable_property<int> decel;
			nullable_property<bool> display;
			nullable_property<std::string> dur;	//unsigned int/"indefinite"
			nullable_property<std::string> evtFilter;
			nullable_property<std::string, Limit::TLNodeFillType> fill;
			nullable_property<int, setter::only_positive<int> > grpId;
			nullable_property<int, setter::only_positive<int> > id;
			nullable_property<std::string, Limit::TLMasterRelation> masterRel;
			nullable_property<bool> nodePh;
			nullable_property<std::string, Limit::TLNodeType> nodeType;
			nullable_property<std::string, Limit::TLPresetClass> presetClass;
			nullable_property<int> presetID;
			nullable_property<int> presetSubtype;
			nullable_property<std::string> repeatCount;
			nullable_property<std::string> repeatDur;
			nullable_property<std::string, Limit::TLRestart> restart;
			nullable_property<int> spd;
			nullable_property<std::string, Limit::TLSyncBehavior> syncBehavior;
			nullable_property<std::string> tmFilter;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CTN_INCLUDE_H