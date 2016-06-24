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
	bool ParseEmbeddedEquation( const std::string & xmlString, std::wstring & newXmlString );

	class VMLPictureMapping: public PropertiesMapping, public IMapping
	{
	public:
		VMLPictureMapping( ConversionContext* ctx, XmlUtils::CXmlWriter* writer, bool olePreview, IMapping* caller, bool isBulletPicture = false );
		virtual ~VMLPictureMapping();
		virtual void Apply( IVisitable* visited );
		std::wstring GetShapeId() const;

	private:
		/// Writes a border element
		void writePictureBorder (const wchar_t* name, const BorderCode* brc);
		void appendStyleProperty( wstring* b, const wstring& propName, const wstring& propValue ) const;

	protected:
		/// Copies the picture from the binary stream to the zip archive 
		/// and creates the relationships for the image.
		bool CopyPicture (BlipStoreEntry* oBlipEntry);

	public:

		static std::wstring GetTargetExt (Global::BlipType nType);
		static std::wstring GetContentType (Global::BlipType nType);

		bool							m_isEquation;
		bool							m_isEmbedded;
		std::string						m_embeddedData;
		std::wstring					m_equationXml;
	private:
		
		ConversionContext*				m_ctx;
		IMapping*						m_caller;
		int								m_nImageId;
		std::wstring					m_ShapeId;
		
		bool							m_isOlePreview;
		
		bool							m_isBulletPicture;
		XMLTools::XMLElement<wchar_t>*	m_imageData;
		
	};
}
