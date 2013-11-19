#pragma once
#ifndef PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_
#define PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextAlign.h"
#include "./../Limit/FontAlign.h"
#include "property.h"
#include "nullable.h"
#include "TextSpacing.h"
#include "RunProperties.h"
#include "Bullets/BulletColor.h"
#include "Bullets/BulletTypeface.h"
#include "Bullets/Bullet.h"
#include "Bullets/BulletSize.h"
#include "Tab.h"

namespace PPTX
{
	namespace Logic
	{

		class TextParagraphPr : public WrapperWritingElement
		{
		public:
			TextParagraphPr();
			virtual ~TextParagraphPr();
			explicit TextParagraphPr(const XML::XNode& node);
			const TextParagraphPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<TextParagraphPr>& props)const;
		public:
			//Childs
			nullable_property<TextSpacing> lnSpc;
			nullable_property<TextSpacing> spcAft;
			nullable_property<TextSpacing> spcBef;

			BulletColor buColor;
			BulletSize buSize;
			BulletTypeface buTypeface;
			Bullet ParagraphBullet;

			property<std::list<Tab> > tabLst;// (Tab List)
			nullable_property<RunProperties> defRPr;

			// Attribs
			nullable_property<std::string, Limit::TextAlign> algn;	// (Alignment)
			nullable_property<int> defTabSz;						// (Default Tab Size)
			nullable_property<bool> eaLnBrk;						// (East Asian Line Break)
			nullable_property<std::string, Limit::FontAlign> fontAlgn;// (Font Alignment)
			nullable_property<bool> hangingPunct;					// (Hanging Punctuation)
			nullable_property<int, setter::between<int, -51206400, 51206400> > indent;// (Indent)
			nullable_property<bool> latinLnBrk;						// (Latin Line Break)
			nullable_property<int, setter::between<int, 0, 8> > lvl;// (Level)
			nullable_property<int, setter::between<int, 0, 51206400> > marL;// (Left Margin)
			nullable_property<int, setter::between<int, 0, 51206400> > marR;// (Right Margin)
			nullable_property<bool> rtl;							// (Right To Left)
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_