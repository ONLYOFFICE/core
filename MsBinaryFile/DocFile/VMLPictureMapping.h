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
#pragma once

#include "PropertiesMapping.h"
#include "PictureDescriptor.h"
#include "TwipsValue.h"
#include "ConversionContext.h"
#include "OfficeDrawing/Shapetypes/PictureFrameType.h"
#include "OfficeDrawing/Shape.h"
#include "OfficeDrawing/MetafilePictBlip.h"
#include "OfficeDrawing/BitmapBlip.h"
#include "VMLShapeTypeMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class OleObject;


	class VMLPictureMapping: public PropertiesMapping, public IMapping
	{
	public:
		VMLPictureMapping( ConversionContext* ctx, XMLTools::CStringXmlWriter* writer, bool olePreview, IMapping* caller, bool isInlinePicture = false, bool inGroup = false);
		virtual ~VMLPictureMapping();
		virtual void Apply( IVisitable* visited );
	private:
		/// Writes a border element
		void writePictureBorder (const std::wstring & name, const BorderCode* brc);
		void appendStyleProperty( std::wstring& style, const std::wstring& propName, const std::wstring& propValue ) const;
		bool ParseEmbeddedEquation( const std::string & xmlString, std::wstring & newXmlString );
		bool ParseEmbeddedBlob(const std::string & xmlString, std::wstring & newXmlString);
		std::wstring GetShapeID(const Shape* pShape) const;
	protected:
		/// Copies the picture from the binary stream to the zip archive 
		/// and creates the relationships for the image.
		bool CopyPicture (PictureDescriptor* pict);

	public:

		static std::wstring GetTargetExt (Global::BlipType nType);
		static std::wstring GetContentType (Global::BlipType nType);

		bool							m_isBlob;
		bool							m_isBullete;
		bool							m_isEquation;
		bool							m_isEmbedded;
		bool							m_isPictureBroken;
		std::string						m_embeddedData;
		std::wstring					m_equationXml;
		std::wstring					m_blobXml;
		
		std::wstring					m_shapeId;
	private:
		
		ConversionContext*				m_context;
		IMapping*						m_caller;
		int								m_nImageId;
		
		bool							m_isOlePreview;		
		bool							m_isInlinePicture;
		bool							m_inGroup;

        XMLTools::XMLElement*           m_imageData;
		
	};
}
