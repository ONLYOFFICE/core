#pragma once
#ifndef PPTX_THEME_CLRSCHEME_INCLUDE_H_
#define PPTX_THEME_CLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include <map>
#include "./../Logic/UniColor.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ClrScheme : public WrapperWritingElement
		{
		public:
			ClrScheme();
			virtual ~ClrScheme();			
			explicit ClrScheme(const XML::XNode& node);
			const ClrScheme& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual DWORD GetRGBAFromScheme(const std::string& str)const;
			virtual DWORD GetARGBFromScheme(const std::string& str)const;
			virtual DWORD GetBGRAFromScheme(const std::string& str)const;
			virtual DWORD GetABGRFromScheme(const std::string& str)const;

		public:
			property<std::string> name;
			property<std::map<std::string, Logic::UniColor> > Scheme;
		protected:
			virtual void FillParentPointersForChilds();
			//Logic::UniColor dk1; //Dark 1 
			//Logic::UniColor lt1; //Light 1 
			//Logic::UniColor dk2; //Dark 2 
			//Logic::UniColor lt2; //Light 2 
			//Logic::UniColor accent1; //Accent 1 
			//Logic::UniColor accent2; //Accent 2 
			//Logic::UniColor accent3; //Accent 3 
			//Logic::UniColor accent4; //Accent 4 
			//Logic::UniColor accent5; //Accent 5 
			//Logic::UniColor accent6; //Accent 6 
			//Logic::UniColor hlink; //Hyperlink 
			//Logic::UniColor folHlink; //Followed Hyperlink 
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_CLRSCHEME_INCLUDE_H