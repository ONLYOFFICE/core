#pragma once

#include "PictureDescriptor.h"
#include "AbstractOpenXmlMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class OleObjectMapping: public AbstractOpenXmlMapping, public IMapping
	{
	public:
		OleObjectMapping(XmlUtils::CXmlWriter* writer, ConversionContext* context, PictureDescriptor* pict, IMapping* caller, const wstring& shapeId) : AbstractOpenXmlMapping(writer), m_context(NULL), _pict(NULL), _caller(NULL), _shapeId(shapeId)
		{
			m_context	=	context;
			_pict		=	pict;
			_caller		=	caller;
		}

		virtual void Apply(IVisitable* visited)
		{
			OleObject* ole = static_cast<OleObject*>(visited);

			if ( ole != NULL )
			{
				m_pXmlWriter->WriteNodeBegin( _T( "o:OLEObject" ), TRUE );

				//type
				if ( ole->fLinked )
				{
					int relID = m_context->_docx->RegisterExternalOLEObject(_caller, ole->ClipboardFormat, ole->Link);

					m_pXmlWriter->WriteAttribute( _T( "r:id" ), ( wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( relID ) ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "Type" ), _T( "Link" ) );
					m_pXmlWriter->WriteAttribute( _T( "UpdateMode" ), ole->UpdateMode.c_str() );
				}
				else
				{
					int relID = m_context->_docx->RegisterOLEObject(_caller, ole->ClipboardFormat);

					m_pXmlWriter->WriteAttribute( _T( "r:id" ), ( wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( relID ) ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "Type" ), _T( "Embed" ) );

					//copy the object
					copyEmbeddedObject( ole );
				}

				//ProgID
				m_pXmlWriter->WriteAttribute( _T( "ProgID" ), ole->Program.c_str() );

				//ShapeId
				m_pXmlWriter->WriteAttribute( _T( "ShapeID" ), this->_shapeId.c_str() );

				//DrawAspect
				m_pXmlWriter->WriteAttribute( _T( "DrawAspect" ), _T( "Content" ) );

				//ObjectID
				m_pXmlWriter->WriteAttribute( _T( "ObjectID" ), ole->ObjectId.c_str() );

				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				m_pXmlWriter->WriteNodeEnd( _T( "o:OLEObject" ) );
			}
		}

		static wstring GetTargetExt(const wstring& objectType)
		{
			wstring objectExt = _T( ".bin" );

			if ( objectType == _T( "Biff8" ) )
			{
				objectExt = _T( ".xls" );
			}
			else if ( objectType == _T( "MSWordDoc" ) )
			{
				objectExt = _T( ".doc" );
			}
			else if ( objectType == _T( "MSPresentation" ) )
			{
				objectExt = _T( ".ppt" );
			}

			return objectExt;
		}

		static wstring GetContentType(const wstring& objectType)
		{
			wstring objectContentType = OpenXmlContentTypes::OleObject;

			if ( objectType == _T( "Biff8" ) )
			{
				objectContentType = OpenXmlContentTypes::MSExcel;
			}
			else if ( objectType == _T( "MSWordDoc" ) )
			{
				objectContentType = OpenXmlContentTypes::MSWord;
			}
			else if ( objectType == _T( "MSPresentation" ) )
			{
				objectContentType = OpenXmlContentTypes::MSPowerpoint;
			}

			return objectContentType;
		}

	private:
		// Writes the embedded OLE object from the ObjectPool of the binary file to the OpenXml Package.

		inline void copyEmbeddedObject( const OleObject* ole )
		{
			if ( ole != NULL )
			{
				//!!!TODO: There is issue with some Office OLE Objects. Word can't open *.xls object (Excel.Chart) with set CLSID and
				//some Power Point Presentations, and Word Documents. Open Office CAN start this objects!!!

				CLSID clsid = GUID_NULL;

				wstring exelChart = _T( "Excel.Chart" );

				if ( search( ole->Program.begin(), ole->Program.end(), exelChart.begin(), exelChart.end() ) == ole->Program.end() )
				{
					clsid = ole->ClassId;
				}

				m_context->_docx->OleObjectsList.push_back( OleObjectFileStructure( OleObjectMapping::GetTargetExt( ole->ClipboardFormat ), ole->ObjectId, clsid ) );
			}
		}

	private:  
		ConversionContext*	m_context;
		PictureDescriptor* _pict;
		wstring _shapeId;

		IMapping* _caller;
	};
}