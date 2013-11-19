#pragma once
#ifndef PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_
#define PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "EmbeddedFontDataId.h"
#include "./../Logic/TextFont.h"

namespace PPTX
{
	namespace nsPresentation
	{
		class EmbeddedFont : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(EmbeddedFont)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Bold		= node.ReadNode(_T("p:bold"));
				BoldItalic	= node.ReadNode(_T("p:boldItalic"));
				Italic		= node.ReadNode(_T("p:italic"));
				Regular		= node.ReadNode(_T("p:regular"));
				Font		= node.ReadNode(_T("p:font"));

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(Bold);
				oValue.WriteNullable(BoldItalic);
				oValue.WriteNullable(Italic);
				oValue.WriteNullable(Regular);
				oValue.Write(Font);

				return XmlUtils::CreateNode(_T("p:embeddedFont"), oValue);
			}
		public:
			nullable<EmbeddedFontDataId> Bold;
			nullable<EmbeddedFontDataId> BoldItalic;
			nullable<EmbeddedFontDataId> Italic;
			nullable<EmbeddedFontDataId> Regular;
			Logic::TextFont Font;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(Bold.is_init())
					Bold->SetParentPointer(this);
				if(BoldItalic.is_init())
					BoldItalic->SetParentPointer(this);
				if(Italic.is_init())
					Italic->SetParentPointer(this);
				if(Regular.is_init())
					Regular->SetParentPointer(this);
				Font.SetParentPointer(this);
			}
		};
	} // namespace nsPresentation
} // namespace PPTX

#endif // PPTX_PRESENTATION_EMBEDDEDFONT_INCLUDE_H_