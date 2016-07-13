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
#include "IMapping.h"

#include "ConversionContext.h"
#include "Spa.h"
#include "PictureDescriptor.h"
#include "VMLShapeTypeMapping.h"
#include "TwipsValue.h"
#include "EmuValue.h"
#include "MemoryStream.h"
#include "RGBColor.h"
#include "FixedPointNumber.h"

#include "OfficeDrawing/BlipStoreContainer.h"
#include "OfficeDrawing/GroupShapeRecord.h"
#include "OfficeDrawing/ChildAnchor.h"
#include "OfficeDrawing/ClientAnchor.h"
#include "OfficeDrawing/Shapetypes/LineType.h"
#include "OfficeDrawing/PathParser.h"
#include "OfficeDrawing/MetafilePictBlip.h"
#include "OfficeDrawing/BitmapBlip.h"

namespace DocFileFormat
{
	class VMLShapeMapping : public PropertiesMapping, public IMapping
	{
	public:

		VMLShapeMapping (ConversionContext* ctx ,XmlUtils::CXmlWriter* writer, Spa* pSpa, PictureDescriptor* pict,  IMapping* caller, bool bullet = false);
		virtual ~VMLShapeMapping();
		virtual void Apply(IVisitable* visited);

	private:

		void ApplyPrimitives	(DrawingPrimitives	* primitives );
		int  ApplyPrimitive		(DrawingPrimitives	* primitives, int index);

		void WritePrimitiveProps(DrawingPrimitive	* primitive, bool root);

		// Converts a group of shapes
		void WriteGroup(const GroupContainer* pContainer);
		// Converts a single shape
		void WriteShape (const ShapeContainer* pContainer);

		/// Generates a string id for the given shape
		std::wstring GenShapeId(const Shape* pShape) const;
		/// Build the VML wrapcoords string for a given pWrapPolygonVertices
		std::wstring getWrapCoords( const OptionEntry& pWrapPolygonVertices ) const;
		/// Copies the picture from the binary stream to the zip archive 
		/// and creates the relationships for the image.
		bool copyPicture( const BlipStoreEntry* bse );
		std::wstring GetTargetExt( Global::BlipType _type ) const;
		
		void AppendDimensionToStyle	( std::wstring* style, const PictureDescriptor* pict, bool twistDimensions ) const;
		void AppendDimensionToStyle	( std::wstring* style, const Spa* pSpa, bool twistDimensions ) const;
		void AppendDimensionToStyle	( std::wstring* style, const ChildAnchor* anchor, bool twistDimensions ) const;
		void appendStyleProperty	( std::wstring* b, const std::wstring& propName, const std::wstring& propValue ) const;
		
		std::wstring getTextboxAnchor( unsigned int anchor ) const;
		
		std::wstring mapVerticalPosition		( PositionVertical vPos ) const;
		std::wstring mapVerticalPositionRelative( int vRel ) const;
		std::wstring mapHorizontalPosition		( PositionHorizontal hPos ) const;
		std::wstring mapHorizontalPositionRelative( int hRel ) const;
		
		void AppendOptionsToStyle( std::wstring* style, const std::list<OptionEntry>& options ) const;
		
		std::wstring buildStyle		( const Shape* shape, const ChildAnchor* anchor, const std::list<OptionEntry>& options, int zIndex ) const;
		std::wstring getLineStyle	( unsigned int p ) const;
		std::wstring getArrowStyle	( unsigned int op ) const;
		std::wstring getArrowLength	( unsigned int op ) const;
		std::wstring getArrowWidth	( unsigned int op ) const;
		std::wstring getFillMethod	( unsigned int p ) const;
		std::wstring getFillColorString( const unsigned char* p, unsigned int size ) const;
		/// Returns the OpenXML fill type of a fill effect
		std::wstring getFillType	( unsigned int p ) const;
		std::wstring getShadowType	( unsigned int p ) const;
		/// Returns the OpenXML wrap type of the shape
		std::wstring getWrapType	(const Spa* pSpa) const;
		std::wstring ParsePath		(const std::list<OptionEntry>& options) const;

		void WriteBeginShapeNode	(const Shape* pShape);
		void WriteEndShapeNode		(const Shape* pShape);
		std::wstring GetShapeID		(const Shape* pShape) const;

		std::wstring GetLineFrom	(const ChildAnchor* pAnchor) const;
		std::wstring GetLineTo		(const ChildAnchor* pAnchor) const;

		std::vector<CString> GetTextRectangles(const OptionEntry& inscribe) const;

	private: 
		bool							m_bBullet;
		Spa*							m_pSpa;
		IMapping*						m_pCaller;
		BlipStoreContainer*				m_pBlipStore;
		int								m_nImageId;
		std::wstring					m_textPathStyle;
		ConversionContext*				m_ctx;
		PictureDescriptor*				m_pict;

		XMLTools::XMLElement<wchar_t>	m_fill; 
		XMLTools::XMLElement<wchar_t>	m_stroke;
		XMLTools::XMLElement<wchar_t>	m_shadow; 
		XMLTools::XMLElement<wchar_t>	m_imagedata;
		XMLTools::XMLElement<wchar_t>	m_3dstyle; 
		XMLTools::XMLElement<wchar_t>	m_textpath;
	};
}
