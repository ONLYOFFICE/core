#pragma once
#ifndef OOX_NUMBERING_FILE_INCLUDE_H_
#define OOX_NUMBERING_FILE_INCLUDE_H_

#include <vector>
#include <string>

#include "File.h"
#include "Logic/Align.h"
#include "Logic/ParagraphProperty.h"
#include "Logic/RunProperty.h"
#include "Logic/NumFormat.h"

#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX
{
	class Numbering : public OOX::File
	{
	public:
		class Level
		{
		public:
			Level();
			virtual ~Level();
			
			Level(XmlUtils::CXmlNode& oNode);
			const Level& operator =(XmlUtils::CXmlNode& oNode);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			NSCommon::nullable<int>							Ilvl;
			NSCommon::nullable<std::wstring>				Tplc;
			NSCommon::nullable<int>							Start;
			NSCommon::nullable<Logic::NumFormat>			NumFmt;
			NSCommon::nullable<std::wstring>				Suffix;
			NSCommon::nullable<std::wstring>				Text;
			NSCommon::nullable<Logic::Align>				Align;
			NSCommon::nullable<Logic::ParagraphProperty>	ParagraphProperty;
			NSCommon::nullable<Logic::RunProperty>			RunProperty;
			NSCommon::nullable<int>							Tentative;
		};

		class LevelOverride 
		{
		public:
			LevelOverride();
			virtual ~LevelOverride();
			
			LevelOverride(XmlUtils::CXmlNode& oNode);
			const LevelOverride& operator =(XmlUtils::CXmlNode& oNode);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
		    NSCommon::nullable<int>				Ilvl;
			NSCommon::nullable<int>				StartOverride;
			NSCommon::nullable<Level>			Level;
		};

		class AbstractNum
		{
		public:
			AbstractNum();
			virtual ~AbstractNum();
			
			AbstractNum(XmlUtils::CXmlNode& oNode);
			const AbstractNum& operator =(XmlUtils::CXmlNode& oNode);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			const Level getLevel(const int numLevel) const;

		public:
			std::vector<Level>					Levels;
			
			NSCommon::nullable<int>				Id;
			NSCommon::nullable<std::wstring>	Nsid;
			NSCommon::nullable<std::wstring>	MultiLevelType;
			NSCommon::nullable<std::wstring>	Tmpl;		
			NSCommon::nullable<std::wstring>	numStyleLink;
		};

		class Num 
		{
		public:
			Num();
			virtual ~Num();
			
			Num(XmlUtils::CXmlNode& oNode);
			const Num& operator =(XmlUtils::CXmlNode& oNode);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode);

		public:
			NSCommon::nullable<int>		NumId;
			NSCommon::nullable<int>		AbstractNumId;

			std::vector<LevelOverride>	LevelOverrides;
		};

	public:
		Numbering();
		Numbering(const OOX::CPath& filename);
		virtual ~Numbering();

	public:
		virtual void read(const OOX::CPath& filename);
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const OOX::CPath DefaultDirectory() const;
		virtual const OOX::CPath DefaultFileName() const;

	public:
		void clear();

	public:

		std::vector<AbstractNum>	AbstractNums;
		std::vector<Num>			Nums;
	};
} // namespace OOX

#endif // OOX_NUMBERING_FILE_INCLUDE_H_