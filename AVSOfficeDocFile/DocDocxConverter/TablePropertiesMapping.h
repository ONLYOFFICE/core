#pragma once

#include "..\Common\XmlTools.h"
#include "BorderCode.h"
#include "PropertiesMapping.h"
#include "TablePropertyExceptions.h"
#include "SprmTDefTable.h"
#include "StyleSheet.h"
#include "StyleSheetMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	static const WCHAR WidthType[4][5] =
	{
		_T( "nil" ),
		_T( "auto" ),
		_T( "pct" ),
		_T( "dxa" )
	};

	struct FtsWWidth_Indent
	{
	public:
		FtsWWidth_Indent (byte* buffer) : ftsWidth(0), wWidth(0)
		{
			if (buffer)
			{
				ftsWidth	=	buffer[0];

				if (3 == ftsWidth)			//	ftsDxa
					wWidth	=	FormatUtils::BytesToInt16 (++buffer, 0, 2);
			}
		}

		unsigned char	ftsWidth;
		short			wWidth;
	};

	class TablePropertiesMapping: public PropertiesMapping, public IMapping
	{
	public:
		TablePropertiesMapping( XmlUtils::CXmlWriter* writer, StyleSheet* styles, vector<short>* grid, bool isTableStyleNeeded = true );
		virtual ~TablePropertiesMapping();
		void Apply( IVisitable* visited );

	private: 

		XMLTools::XMLElement<WCHAR>* _tblPr;
		XMLTools::XMLElement<WCHAR>* _tblGrid;
		XMLTools::XMLElement<WCHAR>* _tblBorders;
		StyleSheet* _styles;
		vector<short>* _grid;
		BorderCode* brcLeft;
		BorderCode* brcTop;
		BorderCode* brcBottom;
		BorderCode* brcRight;
		BorderCode* brcHorz;
		BorderCode* brcVert;

		bool _isTableStyleNeeded;
	};
}