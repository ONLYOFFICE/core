
#include "VMLShapeTypeMapping.h"
#include "OfficeDrawing/Shapetypes/OvalType.h"

namespace DocFileFormat
{
	VMLShapeTypeMapping::VMLShapeTypeMapping (XmlUtils::CXmlWriter* pWriter, bool isBulletPicture) : PropertiesMapping(pWriter), _lock(NULL), _isBulletPicture(isBulletPicture)
	{
		this->_lock = new XMLTools::XMLElement<wchar_t>( _T( "o:lock" ) );
		appendValueAttribute( this->_lock, _T( "v:ext" ), _T( "edit" ) );
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
			m_pXmlWriter->WriteNodeBegin( _T("v:shapetype"), true );

			// ID
			m_pXmlWriter->WriteAttribute( _T("id"), GenerateTypeId( pShape ).c_str() );

			// Coordinate System
			m_pXmlWriter->WriteAttribute( _T("coordsize"), _T( "21600,21600" ) );

			// Shape Code
			m_pXmlWriter->WriteAttribute( _T("o:spt"), FormatUtils::IntToWideString( pShape->GetTypeCode() ).c_str() );

			// Adj
			if (pShape->AdjustmentValues.length())
				m_pXmlWriter->WriteAttribute( _T("adj"), pShape->AdjustmentValues.c_str() );

			// Path
			if (pShape->Path.length())
				m_pXmlWriter->WriteAttribute( _T("path"), pShape->Path.c_str() );

			//Default fill / stroke
			if ( !pShape->Filled )
			{
				m_pXmlWriter->WriteAttribute( _T( "filled" ), _T( "f" ) );
			}

			if ( !pShape->Stroked )
			{
				m_pXmlWriter->WriteAttribute( _T( "stroked" ), _T( "f" ) );
			}

			if ( _isBulletPicture )
			{
				m_pXmlWriter->WriteAttribute( _T( "o:preferrelative" ), _T( "t" ) );  
			}

			m_pXmlWriter->WriteNodeEnd( _T( "" ), true, false );

			// Stroke
			m_pXmlWriter->WriteNodeBegin( _T( "v:stroke" ), true );
			m_pXmlWriter->WriteAttribute( _T( "joinstyle" ), FormatUtils::MapValueToWideString( pShape->Joins, &JoinStyleMap[0][0], 3, 6 ).c_str() );
			m_pXmlWriter->WriteNodeEnd( _T( "" ), true );

			// Formulas
			if ((int)pShape->Formulas.size())
			{
				m_pXmlWriter->WriteNodeBegin( _T( "v:formulas" ) );

				for ( list<wstring>::iterator iter = pShape->Formulas.begin(); iter != pShape->Formulas.end(); iter++ )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "v:f" ), true );
					m_pXmlWriter->WriteAttribute( _T( "eqn" ), iter->c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), true );
				}

				m_pXmlWriter->WriteNodeEnd( _T( "v:formulas" ) );
			}

			// Path
			m_pXmlWriter->WriteNodeBegin( _T( "v:path" ), true );

			if (_isBulletPicture)
			{
				m_pXmlWriter->WriteAttribute( _T( "o:extrusionok" ), _T( "f" ) );
				m_pXmlWriter->WriteAttribute( _T( "gradientshapeok" ), _T( "t" ) );
				m_pXmlWriter->WriteAttribute( _T( "o:connecttype" ), _T( "rect" ) );
			}
			else
			{
				if (pShape->ShapeConcentricFill)
					m_pXmlWriter->WriteAttribute( _T( "gradientshapeok" ), _T( "t" ) );

				if (pShape->Limo.length())
					m_pXmlWriter->WriteAttribute( _T( "limo" ), pShape->Limo.c_str() );

				if (pShape->ConnectorLocations.length())
				{
					m_pXmlWriter->WriteAttribute( _T( "o:connecttype" ), _T("custom"));
					m_pXmlWriter->WriteAttribute( _T( "o:connectlocs" ), pShape->ConnectorLocations.c_str() );
				}

				if (pShape->TextBoxRectangle.length())
					m_pXmlWriter->WriteAttribute( _T( "textboxrect" ), pShape->TextBoxRectangle.c_str() );

				if (pShape->ConnectorAngles.length())
					m_pXmlWriter->WriteAttribute( _T( "o:connectangles" ), pShape->ConnectorAngles.c_str() );
			}

			m_pXmlWriter->WriteNodeEnd( _T( "" ), true );

			//Lock
			if ( ( pShape->Lock.fUsefLockAspectRatio ) && ( pShape->Lock.fLockAspectRatio ) )
			{
				appendValueAttribute( _lock, _T( "aspectratio" ), _T( "t" ) );
			}

			if ( _lock->GetAttributeCount() > 1 )
			{
				m_pXmlWriter->WriteString( _lock->GetXMLString().c_str() );
			}

			// Handles
			if ( !pShape->Handles.empty() )
			{
				m_pXmlWriter->WriteNodeBegin( _T( "v:handles" ) );

				for ( list<Handle>::iterator iter = pShape->Handles.begin(); iter != pShape->Handles.end(); ++iter)
				{
					m_pXmlWriter->WriteNodeBegin( _T( "v:h" ), true );

					if (iter->position.length())
						m_pXmlWriter->WriteAttribute( _T( "position" ), iter->position.c_str() );

					if (iter->switchHandle.length())
						m_pXmlWriter->WriteAttribute( _T( "switch" ), iter->switchHandle.c_str() );

					if (iter->xrange.length())
						m_pXmlWriter->WriteAttribute( _T( "xrange" ), iter->xrange.c_str() );

					if (iter->yrange.length())
						m_pXmlWriter->WriteAttribute( _T( "yrange" ), iter->yrange.c_str() );

					if (iter->polar.length())
						m_pXmlWriter->WriteAttribute( _T( "polar" ), iter->polar.c_str() );

					if (iter->radiusrange.length())
						m_pXmlWriter->WriteAttribute( _T( "radiusrange" ), iter->radiusrange.c_str() );

					m_pXmlWriter->WriteNodeEnd( _T( "" ), true );
				}

				m_pXmlWriter->WriteNodeEnd( _T( "v:handles" ) );
			}

			m_pXmlWriter->WriteNodeEnd( _T( "v:shapetype" ) );
		}
	}

	std::wstring VMLShapeTypeMapping::GenerateTypeId (const ShapeType* pShape)
	{
		wstring type;

		type += wstring( _T( "_x0000_t" ) );
		type += FormatUtils::IntToWideString(pShape->GetTypeCode() );

		return type;
	}
}
