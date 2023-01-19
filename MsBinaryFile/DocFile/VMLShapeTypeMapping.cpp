/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "VMLShapeTypeMapping.h"
#include "OfficeDrawing/Shapetypes/OvalType.h"
#include "OfficeDrawing/Shapetypes/WordArtText.h"
#include "../XlsFile/Format/Logic/Biff_structures/ODRAW/OfficeArtFOPTE.h"

namespace DocFileFormat
{
	VMLShapeTypeMapping::VMLShapeTypeMapping (XMLTools::CStringXmlWriter* pWriter, bool isInlineShape) : PropertiesMapping(pWriter), _lock(NULL), _isInlineShape(isInlineShape)
	{
        this->_lock = new XMLTools::XMLElement( L"o:lock");
        appendValueAttribute( this->_lock, L"v:ext", L"edit");
	}

	VMLShapeTypeMapping::~VMLShapeTypeMapping()
	{
		RELEASEOBJECT(_lock);
	}
}
namespace DocFileFormat
{
	void VMLShapeTypeMapping::Apply (IVisitable* visited)
	{
		ShapeType* pShape = static_cast<ShapeType*>( visited );

		if (pShape && ( typeid(*pShape) != typeid(OvalType)))
		{
            m_pXmlWriter->WriteNodeBegin( L"v:shapetype", true );

			// ID
            m_pXmlWriter->WriteAttribute( L"id", GenerateTypeId( pShape ));

			// Coordinate System
            m_pXmlWriter->WriteAttribute( L"coordsize", L"21600,21600");

			// Shape Code
            m_pXmlWriter->WriteAttribute( L"o:spt", FormatUtils::IntToWideString( pShape->GetTypeCode() ));

			// Adj
			if (pShape->AdjustmentValues.length())
                m_pXmlWriter->WriteAttribute( L"adj", pShape->AdjustmentValues);

			// Path
			if (!pShape->Path.empty())
                m_pXmlWriter->WriteAttribute( L"path", pShape->Path);
			else if (_isInlineShape)
                m_pXmlWriter->WriteAttribute( L"path", L"m@4@5l@4@11@9@11@9@5xe");


			//Default fill / stroke
			if ( !pShape->Filled )
			{
                m_pXmlWriter->WriteAttribute( L"filled" , L"f");
			}

			if ( !pShape->Stroked )
			{
                m_pXmlWriter->WriteAttribute( L"stroked", L"f");
			}

			if ( _isInlineShape )
			{
                m_pXmlWriter->WriteAttribute( L"o:preferrelative", L"t");
			}

            m_pXmlWriter->WriteNodeEnd( L"", true, false );//закрытие атрибутов

			//Textpath
			if (!pShape->Textpath.empty())
			{
                m_pXmlWriter->WriteNodeBegin( L"v:textpath", true );
				m_pXmlWriter->WriteString( pShape->Textpath );
                m_pXmlWriter->WriteNodeEnd( L"", true );
			}
			// Stroke
            m_pXmlWriter->WriteNodeBegin( L"v:stroke", true );
            m_pXmlWriter->WriteAttribute( L"joinstyle", FormatUtils::MapValueToWideString( pShape->Joins, &JoinStyleMap[0][0], 3, 6 ));
            m_pXmlWriter->WriteNodeEnd( L"", true );

			// Formulas
			if ((int)pShape->Formulas.size())
			{
                m_pXmlWriter->WriteNodeBegin( L"v:formulas");

				for ( std::list<std::wstring>::iterator iter = pShape->Formulas.begin(); iter != pShape->Formulas.end(); iter++ )
				{
                    m_pXmlWriter->WriteNodeBegin( L"v:f", true );
                    m_pXmlWriter->WriteAttribute( L"eqn", *iter );
                    m_pXmlWriter->WriteNodeEnd( L"", true );
				}

                m_pXmlWriter->WriteNodeEnd( L"v:formulas");
			}
			else if (_isInlineShape)
			{
                m_pXmlWriter->WriteString(L"<v:formulas><v:f eqn=\"if lineDrawn pixelLineWidth 0\"/>\
<v:f eqn=\"sum @0 1 0\"/><v:f eqn=\"sum 0 0 @1\"/>\
<v:f eqn=\"prod @2 1 2\"/><v:f eqn=\"prod @3 21600 pixelWidth\"/>\
<v:f eqn=\"prod @3 21600 pixelHeight\"/><v:f eqn=\"sum @0 0 1\"/>\
<v:f eqn=\"prod @6 1 2\"/><v:f eqn=\"prod @7 21600 pixelWidth\"/>\
<v:f eqn=\"sum @8 21600 0\"/><v:f eqn=\"prod @7 21600 pixelHeight\"/>\
<v:f eqn=\"sum @10 21600 0\"/></v:formulas>");
			}

			// Path
            m_pXmlWriter->WriteNodeBegin( L"v:path", true );
              
			m_pXmlWriter->WriteAttribute( L"o:extrusionok", L"f");

			if (_isInlineShape)
			{
                m_pXmlWriter->WriteAttribute( L"gradientshapeok", L"t" );
                m_pXmlWriter->WriteAttribute( L"o:connecttype", L"rect" );
			}
			else
			{
				if (pShape->ShapeConcentricFill)
                    m_pXmlWriter->WriteAttribute( L"gradientshapeok", L"t" );

				if (pShape->Limo.length())
                    m_pXmlWriter->WriteAttribute( L"limo", pShape->Limo );

				if (pShape->ConnectorLocations.length())
				{
                    m_pXmlWriter->WriteAttribute( L"o:connecttype", L"custom");
                    m_pXmlWriter->WriteAttribute( L"o:connectlocs", pShape->ConnectorLocations);
				}

				if (pShape->TextBoxRectangle.length())
                    m_pXmlWriter->WriteAttribute( L"textboxrect", pShape->TextBoxRectangle);

				if (pShape->ConnectorAngles.length())
                    m_pXmlWriter->WriteAttribute( L"o:connectangles", pShape->ConnectorAngles);
			}
			WordArtTextType* wordArt = dynamic_cast<WordArtTextType*>(pShape);
			if (wordArt)
			{
                m_pXmlWriter->WriteAttribute( L"textpathok", L"t" );
			}
			
            m_pXmlWriter->WriteNodeEnd( L"", true );

	//Lock ??? 
			ODRAW::ProtectionBooleanProperties *prot = dynamic_cast<ODRAW::ProtectionBooleanProperties*>(pShape->Lock.get());
			if ((prot) && (prot->fUsefLockAspectRatio))
			{
				appendValueAttribute( _lock, L"aspectratio", prot->fLockAspectRatio ? L"t" : L"f" );
			}
			else
			{
				appendValueAttribute( _lock, L"aspectratio", L"t");
			}

			if ( _lock->GetAttributeCount() > 0 )
			{
				m_pXmlWriter->WriteString( _lock->GetXMLString() );
			}

			// Handles
			if ( !pShape->Handles.empty() )
			{
                m_pXmlWriter->WriteNodeBegin( L"v:handles" );

				for ( std::list<Handle>::iterator iter = pShape->Handles.begin(); iter != pShape->Handles.end(); ++iter)
				{
                    m_pXmlWriter->WriteNodeBegin( L"v:h", true );

					if (iter->position.length())
                        m_pXmlWriter->WriteAttribute( L"position", iter->position);

					if (iter->switchHandle.length())
                        m_pXmlWriter->WriteAttribute( L"switch", iter->switchHandle);

					if (iter->xrange.length())
                        m_pXmlWriter->WriteAttribute( L"xrange", iter->xrange);

					if (iter->yrange.length())
                        m_pXmlWriter->WriteAttribute( L"yrange", iter->yrange);

					if (iter->polar.length())
                        m_pXmlWriter->WriteAttribute( L"polar", iter->polar);

					if (iter->radiusrange.length())
                        m_pXmlWriter->WriteAttribute( L"radiusrange", iter->radiusrange );

                    m_pXmlWriter->WriteNodeEnd( L"", true );
				}

                m_pXmlWriter->WriteNodeEnd( L"v:handles" );
			}

            m_pXmlWriter->WriteNodeEnd( L"v:shapetype" );
		}
	}

	std::wstring VMLShapeTypeMapping::GenerateTypeId (const ShapeType* pShape)
	{
		std::wstring type;

        type += std::wstring( L"_x0000_t" );
		type += FormatUtils::IntToWideString(pShape->GetTypeCode() );

		return type;
	}
}
