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
