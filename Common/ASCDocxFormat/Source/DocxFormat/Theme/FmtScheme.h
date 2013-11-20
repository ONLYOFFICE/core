#pragma once
#ifndef OOX_THEME_FMT_SCHEME_INCLUDE_H_
#define OOX_THEME_FMT_SCHEME_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "FillStyleLst.h"
#include "LnStyleLst.h"
#include "EffectStyleLst.h"
#include "BgFillStyleLst.h"


namespace OOX
{
	namespace Theme
	{
		class FmtScheme : public WritingElement
		{
		public:
			FmtScheme();
			virtual ~FmtScheme();
			explicit FmtScheme(const XML::XNode& node);
			const FmtScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<std::string>    name;
			nullable_property<FillStyleLst>	  fillStyleLst;
			nullable_property<LnStyleLst>	  lnStyleLst;
			nullable_property<EffectStyleLst> effectStyleLst;
			nullable_property<BgFillStyleLst> bgFillStyleLst;
		};
	} // namespace Theme
} // namespace OOX

#endif // OOX_THEME_FMT_SCHEME_INCLUDE_H_
