#pragma once
#ifndef PPTX_LOGIC_BODYPR_INCLUDE_H_
#define PPTX_LOGIC_BODYPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "./../Limit/TextAnchor.h"
#include "./../Limit/HorzOverflow.h"
#include "./../Limit/TextVerticalType.h"
#include "./../Limit/VertOverflow.h"
#include "./../Limit/TextWrap.h"
#include "Scene3d.h"
#include "Sp3d.h"
#include "PrstTxWarp.h"
#include "TextFit.h"

namespace PPTX
{
	namespace Logic
	{
		class BodyPr : public WrapperWritingElement
		{
		public:
			BodyPr();
			virtual ~BodyPr();			
			explicit BodyPr(const XML::XNode& node);
			const BodyPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(BodyPr& bodyPr)const;

		public:
			nullable_property<PrstTxWarp> prstTxWarp;
			TextFit Fit;
			nullable_property<Scene3d> scene3d;
			nullable_property<int> flatTx;
			nullable_property<Sp3d> sp3d;

			// Attributes
			nullable_property<std::string, Limit::TextAnchor> anchor;
			nullable_property<bool> anchorCtr;
			nullable_property<int> bIns;
			nullable_property<bool> compatLnSpc;
			nullable_property<bool> forceAA;
			nullable_property<bool> fromWordArt;
			nullable_property<std::string, Limit::HorzOverflow> horzOverflow;
			nullable_property<int> lIns;
			nullable_property<int, setter::between<int, 1, 16> > numCol;
			nullable_property<int> rIns;
			nullable_property<int> rot;
			nullable_property<bool> rtlCol;
			nullable_property<int, setter::only_positive<int> > spcCol;
			nullable_property<bool> spcFirstLastPara;
			nullable_property<int> tIns;
			nullable_property<bool> upright;
			nullable_property<std::string, Limit::TextVerticalType> vert;
			nullable_property<std::string, Limit::VertOverflow> vertOverflow;
			nullable_property<std::string, Limit::TextWrap> wrap;
		//private:
		public:
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BODYPR_INCLUDE_H