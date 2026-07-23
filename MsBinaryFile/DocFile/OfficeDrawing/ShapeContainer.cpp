/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "ShapeContainer.h"

namespace DocFileFormat
{
	ShapeContainer::ShapeContainer():
	RegularContainer(), m_nIndex(0), m_nShapeType(0), m_bSkip(false), m_bBackground(false), m_bOLE(false), m_bOleInPicture(false)
	{
	}

	ShapeContainer::ShapeContainer( IBinaryReader* _reader, unsigned int size, unsigned int typeCode, unsigned int version, unsigned int instance ) :
	m_bSkip(false), m_bBackground(false), m_bOLE(false), m_nIndex(0), m_nShapeType(0), RegularContainer( _reader, size, typeCode, version, instance )
	{
		for ( size_t i = 0; i < this->Children.size(); ++i )
		{
			ClientAnchor *clientAnchor = dynamic_cast<ClientAnchor*>( this->Children[i] );
			//if ( (clientAnchor) && (clientAnchor->value == 0x80000000))
			//	m_bSkip = true; //О реорганизации территориальных органов ПФР с 01.11.2018.doc

			Shape* sh = dynamic_cast<Shape*>( this->Children[i] );
			if (sh)
			{
				m_bBackground	= sh->fBackground;
				m_bOLE			= sh->fOleShape;

				if (sh->shapeType)
				{
					m_nShapeType = sh->shapeType->GetTypeCode();
				}
				else
				{
					for ( size_t j = 0; j < this->Children.size(); ++j)
					{
						ShapeOptions* sh_options = dynamic_cast<ShapeOptions*>( this->Children[j] );
						if (sh_options)
						{
							if (sh_options->OptionsByID.end() != sh_options->OptionsByID.find(ODRAW::pib))
							{
								m_nShapeType = msosptPictureFrame;
							}
						}
					}
				}
			}

		}
	}

	ShapeContainer::~ShapeContainer()
	{
	}

	Record* ShapeContainer::NewObject( IBinaryReader* _reader, unsigned int bodySize, unsigned int typeCode, unsigned int version, unsigned int instance )
	{
		return new ShapeContainer( _reader, bodySize, typeCode, version, instance );
	}

	ODRAW::OfficeArtFOPTEPtr ShapeContainer::ExtractOption(const ODRAW::ePropertyId & prop) const
	{
		ODRAW::OfficeArtFOPTEPtr ret;

		for ( size_t i = 0; i < this->Children.size(); ++i )
		{
			ShapeOptions* opt = dynamic_cast<ShapeOptions*>( this->Children[i] );

			if ( opt == NULL ) continue;

			std::map<ODRAW::ePropertyId, ODRAW::OfficeArtFOPTEPtr>::iterator pFind = opt->OptionsByID.find(prop);
			if (pFind != opt->OptionsByID.end())
			{
				ret = pFind->second;
			}
		}
		return ret;
	}

	std::vector<ODRAW::OfficeArtFOPTEPtr> ShapeContainer::ExtractOptions() const
	{
		std::vector<ODRAW::OfficeArtFOPTEPtr> ret;

		//build the list of all option entries of this shape
		for ( size_t i = 0; i < this->Children.size(); ++i )
		{
			ShapeOptions* opt = dynamic_cast<ShapeOptions*>( this->Children[i] );

			if ( opt == NULL ) continue;

			for ( size_t i = 0; i < opt->Options.size(); i++)
			{
				ret.push_back( opt->Options[i]);
			}
		}

		return ret;
	}
}
