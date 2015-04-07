#pragma once

#include "PropertiesMapping.h"
#include "TablePropertyExceptions.h"
#include "SprmTDefTable.h"
#include "BorderCode.h"
#include "IMapping.h"

namespace DocFileFormat
{
	typedef enum _VerticalCellAlignment
	{
		vca_top,
		vca_center,
		vca_bottom
	} VerticalCellAlignment;

	static const wchar_t VerticalCellAlignmentMap[3][7] =
	{
		_T( "top" ),
		_T( "center" ),
		_T( "bottom" )
	};

	class TableCellPropertiesMapping : public PropertiesMapping, public IMapping
	{
	public:	

		virtual ~TableCellPropertiesMapping();
		TableCellPropertiesMapping (XmlUtils::CXmlWriter* pWriter, const vector<short>* tableGrid, int gridIndex, int cellIndex);
		virtual void Apply( IVisitable* visited );
	
		inline int GetGridSpan() const
		{
			return _gridSpan;
		}

	private:

		void apppendCellShading (unsigned char* sprmArg, int size, int cellIndex);
		bool IsTableBordersDefined (const list<SinglePropertyModifier>* grpprl) const;
		bool IsTableCellWidthDefined (const list<SinglePropertyModifier>* grpprl) const;
	
	private: 
	
		int _gridIndex;
		int _cellIndex;
		XMLTools::XMLElement<wchar_t>* _tcPr;
		XMLTools::XMLElement<wchar_t>* _tcMar;
		XMLTools::XMLElement<wchar_t>* _tcBorders;
		const vector<short>* _grid;
		vector<short> _tGrid;

		short _width;
		Global::CellWidthType _ftsWidth;
		TC80 _tcDef;

		BorderCode* _brcTop;
		BorderCode* _brcLeft;
		BorderCode* _brcRight;
		BorderCode* _brcBottom;

		/// The grind span of this cell
		int _gridSpan;
	};
}