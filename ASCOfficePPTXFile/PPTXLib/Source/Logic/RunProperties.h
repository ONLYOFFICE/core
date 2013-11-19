#pragma once
#ifndef PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "./../Limit/TextCaps.h"
#include "./../Limit/TextStrike.h"
#include "./../Limit/TextUnderline.h"
#include "Ln.h"
#include "UniFill.h"
#include "TextFont.h"
#include "EffectProperties.h"
#include "Hyperlink.h"

namespace PPTX
{
	namespace Logic
	{
		class RunProperties : public WrapperWritingElement
		{
		public:
			RunProperties();
			virtual ~RunProperties();			
			explicit RunProperties(const XML::XNode& node);
			const RunProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<RunProperties>& props)const;

		public:
			//Childs
			nullable_property<Ln> ln;
			UniFill Fill;
			EffectProperties EffectList;
			//highlight (Highlight Color)  §21.1.2.3.4 
			//uLn (Underline Stroke)  §21.1.2.3.14 
			//uLnTx (Underline Follows Text)  §21.1.2.3.15 
			//uFill (Underline Fill)  §21.1.2.3.12 
			//uFillTx (Underline Fill Properties Follow Text)  §21.1.2.3.13 
			nullable_property<TextFont> latin;
			nullable_property<TextFont> ea;
			nullable_property<TextFont> cs;
			nullable_property<TextFont> sym;
			nullable_property<Hyperlink> hlinkClick;
			nullable_property<Hyperlink> hlinkMouseOver;
			//rtl (Right to Left Run)  §21.1.2.2.8 

			// Attributes
			nullable_property<std::string> altLang;
			nullable_property<bool> b;
			nullable_property<int> baseline;
			nullable_property<std::string> bmk;
			nullable_property<std::string, Limit::TextCaps> cap;
			nullable_property<bool> dirty;
			nullable_property<bool> err;
			nullable_property<bool> i;
			nullable_property<int, setter::between<int, 0, 400000> > kern;
			nullable_property<bool> kumimoji;
			nullable_property<std::string> lang;
			nullable_property<bool> noProof;
			nullable_property<bool> normalizeH;
			nullable_property<bool> smtClean;
			nullable_property<int, setter::only_positive<int> > smtId;
			nullable_property<int, setter::between<int, -4000, 4000> > spc;
			nullable_property<std::string, Limit::TextStrike> strike;
			nullable_property<int, setter::between<int, 10, 400000> > sz;
			nullable_property<std::string, Limit::TextUnderline> u;
		//private:
		public:
			nullable_property<XML::XNamespace> m_namespace;
			property<std::string> m_name;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H