#pragma once
#ifndef OOX_LOGIC_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_INCLUDE_H_

#include "TextItemBase.h"
#include <string>
#include <vector>
#include "ParagraphItem.h"
#include "ParagraphProperty.h"
#include "RunProperty.h"
#include "property.h"
#include "nullable_property.h"
#include "Run.h"
#include "Shape.h"
#include "OLEObject.h"
#include "./../RId.h"
#include "Drawing.h"

namespace OOX
{
	namespace Logic
	{
		class Paragraph : public TextItemBase
		{
		public:
			Paragraph();
			virtual ~Paragraph();
			explicit Paragraph(const XML::XNode& node);
			explicit Paragraph(const RId& rId, const OOX::CPath& imagePath, const long width, const long height);
			explicit Paragraph(const RId& rId, const OOX::CPath& filename, const long xEmu, const std::string& hRelativeFrom, const long yEmu, const std::string& vRelativeFrom, const long widthEmu, const long heightEmu);
			const Paragraph& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			// TODO попробовать убрать все Add
			void Add(const Run& run);
			void AddRun(const Run& run);
			void AddText(const std::string& text);
			void AddText(const std::string& text, const nullable__<Logic::RunProperty>& property);
			void AddTab();
			void AddTab(const nullable__<Logic::RunProperty>& property);
			void AddLineBreak();
			void AddBreak(const std::string& type);			
			void AddSpace(const size_t count);
			void AddSpace(const size_t count, const nullable__<Logic::RunProperty>& property);
			void AddHyperlink(const RId& rId, const std::string& text);
			void AddHyperlink(const std::string& nameHref, const std::string& text);
			void AddDrawing(const Drawing& drawing);

			void AddBookmarkStart(const std::string& name);
			void AddBookmarkEnd(const std::string& name);

		public:
			void setRunProperty(const RunProperty& property);	

		public:
			const bool isInList() const;
			const int GetListNum() const;
			const int GetLevel() const;
		
		public:
			nullable_property<ParagraphProperty>		Property;
			property<std::vector<ParagraphItem> >		Items;
			property<int> CountInList;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_INCLUDE_H_
