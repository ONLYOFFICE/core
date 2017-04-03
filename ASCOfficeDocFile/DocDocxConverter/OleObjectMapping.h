/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
                m_pXmlWriter->WriteNodeBegin( L"o:OLEObject", TRUE );

				if ( ole->bLinked )
				{
					int relID = m_context->_docx->RegisterExternalOLEObject(_caller, ole->ClipboardFormat, ole->Link);

                    m_pXmlWriter->WriteAttribute( L"r:id", ( std::wstring( L"rId" ) + FormatUtils::IntToWideString( relID ) ));
                    m_pXmlWriter->WriteAttribute( L"Type", L"Link" );
                    m_pXmlWriter->WriteAttribute( L"UpdateMode", ole->UpdateMode);
				}
				else
				{
					int relID = -1;
					
					if (ole->isEmbedded)
						relID = m_context->_docx->RegisterPackage(_caller, ole->ClipboardFormat);
					else
						relID = m_context->_docx->RegisterOLEObject(_caller, ole->ClipboardFormat);

                    m_pXmlWriter->WriteAttribute( L"r:id", L"rId"+ FormatUtils::IntToWideString( relID ) );
                    m_pXmlWriter->WriteAttribute( L"Type", L"Embed" );

					copyEmbeddedObject( ole );
				}

                m_pXmlWriter->WriteAttribute( L"ProgID", ole->Program);
                m_pXmlWriter->WriteAttribute( L"ShapeID", _shapeId);
                m_pXmlWriter->WriteAttribute( L"DrawAspect", L"Content" );
                m_pXmlWriter->WriteAttribute( L"ObjectID", ole->ObjectId);
                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

                m_pXmlWriter->WriteNodeEnd( L"o:OLEObject" );
			}
		}

		static std::wstring GetTargetExt(const std::wstring& objectType)
		{
            std::wstring objectExt = L".bin";

            if ( objectType == L"Biff8" )
			{
                objectExt = L".xls";
			}
            else if ( objectType == L"MSWordDoc" )
			{
                objectExt = L".doc";
			}
            else if ( objectType == L"MSPresentation" )
			{
                objectExt = L".ppt";
			}
            else if ( objectType == L"MSWordDocx" )
			{
                objectExt = L".docx";
			}
            else if ( objectType == L"Equation" )
			{
                objectExt = L".xml";
			}
			return objectExt;
		}

		static std::wstring GetContentType(const std::wstring& objectType)
		{
			std::wstring objectContentType = OpenXmlContentTypes::OleObject;

            if ( objectType == L"Biff8" )
			{
				objectContentType = OpenXmlContentTypes::MSExcel;
			}
            else if ( objectType == L"MSWordDoc" )
			{
				objectContentType = OpenXmlContentTypes::MSWord;
			}
            else if ( objectType == L"MSPresentation" )
			{
				objectContentType = OpenXmlContentTypes::MSPowerpoint;
			}
            else if ( objectType == L"MSWordDocx" )
			{
				objectContentType = OpenXmlContentTypes::MSWordDocx;
			}
            else if ( objectType == L"Equation" )
			{
				objectContentType = OpenXmlContentTypes::Xml;
			}
			return objectContentType;
		}

	private:
		inline void copyEmbeddedObject( const OleObject* ole )
		{
			if ( ole != NULL )
			{
				std::wstring clsid;
                std::wstring exelChart = L"Excel.Chart";

				if ( std::search( ole->Program.begin(), ole->Program.end(), exelChart.begin(), exelChart.end() ) == ole->Program.end() )
				{
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
