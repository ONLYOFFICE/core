#pragma once
#ifndef OOX_STYLES_INCLUDE_H_
#define OOX_STYLES_INCLUDE_H_

#include "File.h"
#include "property.h"
#include "nullable_property.h"
#include "WritingElement.h"
#include "Logic/TableProperty.h"
#include "Logic/TableCellProperties.h"
#include "Logic/RunProperty.h"
#include "Logic/ParagraphProperty.h"
#include "Logic/TableStyle.h"
#include <vector>

namespace OOX
{
	class Styles : public OOX::File
	{
	public:
		class Style : public WritingElement
		{
		public:
			Style();
			virtual ~Style();
			explicit Style(const XML::XNode& node);
			const Style& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>							name;
			nullable_property<Logic::RunProperty>			RunProperty;
			nullable_property<Logic::ParagraphProperty>		ParagraphProperty;
			nullable_property<Logic::TableProperty>			tblPr;
			nullable_property<Logic::TableCellProperties>	tcPr;
			property<Logic::TableStyle>						tableStyles;
			property<std::string>							StyleId;
			nullable_property<std::string>					BasedOn;
			nullable_property<std::string>					Next;			
			property<std::string>							Type;
			nullable_property<int>							Default;
			nullable_property<int>							CustomStyle;			
			nullable_property<std::string>					Link;
			nullable_property<int>							UiPriority;
			property<bool>									QFormat;			
			property<bool>									SemiHidden;
			property<bool>									UnhideWhenUsed;	
			property<bool>									AutoRedefine;
		};

		class DocDefaults : public WritingElement
		{
		public:
			DocDefaults();
			virtual ~DocDefaults();
			explicit DocDefaults(const XML::XNode& node);
			const DocDefaults& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<Logic::ParagraphProperty>	ParagraphProperty;
			property<Logic::RunProperty>		RunProperty;
		};

		class LsdException : public WritingElement
		{
		public:
			LsdException();
			virtual ~LsdException();
			explicit LsdException(const XML::XNode& node);
			const LsdException& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>	Name;
			nullable_property<bool>	SemiHidden;
			nullable_property<int>	UiPriority;
			nullable_property<bool>	UnhideWhenUsed;
			nullable_property<bool>	QFormat;
		};

		class LattentStyles : public WritingElement
		{
		public:
			LattentStyles();
			virtual ~LattentStyles();
			explicit LattentStyles(const XML::XNode& node);
			const LattentStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<bool>							DefLockedState;
			property<int>							DefUIPriority;
			property<bool>							DefSemiHidden;
			property<bool>							DefUnhideWhenUsed;
			property<bool>							DefQFormat;
			property<int>							Count;
			property<std::vector<LsdException> >	LsdExceptions;
		};

	public:

		Styles();
		Styles(const OOX::CPath& filename);
		virtual ~Styles();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:
		const OOX::Styles::Style GetStyleById( const std::string& StyleId ) const;
		const OOX::Styles::Style GetDefaultStyle( const std::string& Type ) const;
		const OOX::Styles::Style Styles::GetStyleWithTypeAndName (const std::string& Type, const std::string& Name) const;

	public:
		property<std::vector<Style> >		Named;
		property<DocDefaults>				Default;
		nullable_property<LattentStyles>	Lattent;
	};
} // namespace OOX

#endif // OOX_STYLES_INCLUDE_H_