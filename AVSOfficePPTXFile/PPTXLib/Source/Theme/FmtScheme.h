#pragma once
#ifndef PPTX_THEME_FMTSCHEME_INCLUDE_H_
#define PPTX_THEME_FMTSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "./../Logic/UniFill.h"
#include "./../Logic/Ln.h"
#include "./../Logic/EffectStyle.h"

namespace PPTX
{
	namespace nsTheme
	{
		class FmtScheme : public WrapperWritingElement
		{
		public:
			FmtScheme();
			virtual ~FmtScheme();			
			explicit FmtScheme(const XML::XNode& node);
			const FmtScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			void GetLineStyle(int number, Logic::Ln& lnStyle)const;
			void GetFillStyle(int number, Logic::UniFill& fillStyle)const;

		public:
			nullable_property<std::string> name;
			property<std::list<Logic::UniFill> > fillStyleLst;
			property<std::list<Logic::Ln> > lnStyleLst;
			property<std::list<Logic::EffectStyle> > effectStyleLst;
			property<std::list<Logic::UniFill> > bgFillStyleLst;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_FMTSCHEME_INCLUDE_H