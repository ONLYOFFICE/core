/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include "ConversionContext.h"
#include "PictureDescriptor.h"
#include "OleObject.h"

#include "AbstractOpenXmlMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class OleObjectMapping: public AbstractOpenXmlMapping, public IMapping
	{
	public:
		OleObjectMapping(XMLTools::CStringXmlWriter* writer, ConversionContext* context, PictureDescriptor* pict, IMapping* caller, const std::wstring& shapeId) 
			:
		AbstractOpenXmlMapping(writer), m_context(NULL), _pict(NULL), _caller(NULL), _shapeId(shapeId)
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
				if (ole->isEmbedded)
				{
					if (ole->isEquation)	ole->ClipboardFormat	= L"Equation";
					else					ole->ClipboardFormat	= L"MSWordDocx";

					ole->Program			= L"Word.Document";
				}
				m_pXmlWriter->WriteNodeBegin( _T( "o:OLEObject" ), TRUE );

				//type
				if ( ole->bLinked )
				{
					int relID = m_context->_docx->RegisterExternalOLEObject(_caller, ole->ClipboardFormat, ole->Link);

					m_pXmlWriter->WriteAttribute( _T( "r:id" ), ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( relID ) ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "Type" ), _T( "Link" ) );
					m_pXmlWriter->WriteAttribute( _T( "UpdateMode" ), ole->UpdateMode.c_str() );
				}
				else
				{
					int relID = -1;
					
					if (ole->isEmbedded)
						relID = m_context->_docx->RegisterPackage(_caller, ole->ClipboardFormat);
					else
						relID = m_context->_docx->RegisterOLEObject(_caller, ole->ClipboardFormat);

					m_pXmlWriter->WriteAttribute( _T( "r:id" ), ( std::wstring( _T( "rId" ) ) + FormatUtils::IntToWideString( relID ) ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "Type" ), _T( "Embed" ) );

					//copy the object
					copyEmbeddedObject( ole );
				}

				//ProgID
				m_pXmlWriter->WriteAttribute( _T( "ProgID" ), ole->Program.c_str() );

				//ShapeId
				m_pXmlWriter->WriteAttribute( _T( "ShapeID" ), _shapeId.c_str() );

				//DrawAspect
				m_pXmlWriter->WriteAttribute( _T( "DrawAspect" ), _T( "Content" ) );

				//ObjectID
				m_pXmlWriter->WriteAttribute( _T( "ObjectID" ), ole->ObjectId.c_str() );

				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				m_pXmlWriter->WriteNodeEnd( _T( "o:OLEObject" ) );
			}
		}

		static std::wstring GetTargetExt(const std::wstring& objectType)
		{
			std::wstring objectExt = _T( ".bin" );

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
			else if ( objectType == _T( "MSWordDocx" ) )
			{
				objectExt = _T( ".docx" );
			}
			else if ( objectType == _T( "Equation" ) ) 
			{
				objectExt = _T( ".xml" );
			}
			return objectExt;
		}

		static std::wstring GetContentType(const std::wstring& objectType)
		{
			std::wstring objectContentType = OpenXmlContentTypes::OleObject;

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
			else if ( objectType == _T( "MSWordDocx" ) )
			{
				objectContentType = OpenXmlContentTypes::MSWordDocx;
			}
			else if ( objectType == _T( "Equation" ) )
			{
				objectContentType = OpenXmlContentTypes::Xml;
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

				std::wstring clsid;
				std::wstring exelChart = _T( "Excel.Chart" );

				if ( std::search( ole->Program.begin(), ole->Program.end(), exelChart.begin(), exelChart.end() ) == ole->Program.end() )
				{//??
					clsid = ole->ClassId;
				}
				OleObjectFileStructure object_descr(OleObjectMapping::GetTargetExt( ole->ClipboardFormat ), ole->ObjectId, clsid); 

				if (ole->isEquation || ole->isEmbedded)
				{
					object_descr.data = ole->emeddedData;
				}
				m_context->_docx->OleObjectsList.push_back(object_descr);
			}
		}

	private:  
		ConversionContext*	m_context;

		PictureDescriptor*	_pict;
		std::wstring		_shapeId;
		IMapping*			_caller;
	};
}
