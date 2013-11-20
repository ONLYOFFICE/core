#pragma once
#ifndef OOX_NUMBERING_FILE_INCLUDE_H_
#define OOX_NUMBERING_FILE_INCLUDE_H_

#include "File.h"
#include "property.h"
#include "nullable_property.h"
#include <vector>
#include <string>
#include "Logic/Align.h"
#include "Logic/ParagraphProperty.h"
#include "Logic/RunProperty.h"
#include "Logic/NumFormat.h"
#include "Limit/Suffix.h"


namespace OOX
{
	class Numbering : public OOX::File
	{
	public:
		class Level : public WritingElement
		{
		public:
			Level();
			virtual ~Level();
			explicit Level(const XML::XNode& node);
			const Level& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>									Ilvl;
			nullable_property<std::string>					Tplc;
			property<int>									Start;
			property<Logic::NumFormat>						NumFmt;
			nullable_property<std::string, Limit::Suffix>	Suffix;
			property<std::string>							Text;
			nullable_property<Logic::Align>					Align;
			nullable_property<Logic::ParagraphProperty>		ParagraphProperty;
			nullable_property<Logic::RunProperty>			RunProperty;
			nullable_property<int>							Tentative;
		};

		class AbstractNum : public WritingElement
		{
		public:
			AbstractNum();
			virtual ~AbstractNum();
			explicit AbstractNum(const XML::XNode& node);
			const AbstractNum& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const Level getLevel(const int numLevel) const;

		public:
			property<std::vector<Level> >	Levels;
			property<int>					Id;
			nullable_property<std::string>	Nsid;
			property<std::string>			MultiLevelType;
			nullable_property<std::string>	Tmpl;		
			nullable_property<std::string>	numStyleLink;
		};

		class LevelOverride : public WritingElement
		{
		public:
			LevelOverride();
			virtual ~LevelOverride();
			explicit LevelOverride(const XML::XNode& node);
			const LevelOverride& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
		    property<int> Ilvl;
			nullable_property<int> StartOverride;
			nullable_property<Level> Level;
		};


		class Num : public WritingElement
		{
		public:
			Num();
			virtual ~Num();
			explicit Num(const XML::XNode& node);
			const Num& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int> NumId;
			property<int> AbstractNumId;
			property<std::vector<LevelOverride> > LevelOverrides;
		};


	public:
		Numbering();
		Numbering(const boost::filesystem::wpath& filename);
		virtual ~Numbering();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		void clear();

	public:
		const int getAbstractNumId(const int numId) const;
		const AbstractNum getAbstractNum(const int abstractNumId) const;

	public:
		property<std::vector<AbstractNum> >	AbstractNums;
		property<std::vector<Num> >					Nums;
	};
} // namespace OOX

#endif // OOX_NUMBERING_FILE_INCLUDE_H_