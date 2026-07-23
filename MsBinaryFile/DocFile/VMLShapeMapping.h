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
#pragma once

#include "PropertiesMapping.h"
#include "IMapping.h"

#include "ConversionContext.h"
#include "Spa.h"
#include "PictureDescriptor.h"
#include "VMLShapeTypeMapping.h"
#include "TwipsValue.h"
#include "EmuValue.h"
#include "FixedPointNumber.h"

#include "OfficeDrawing/BlipStoreContainer.h"
#include "OfficeDrawing/GroupShapeRecord.h"
#include "OfficeDrawing/ChildAnchor.h"
#include "OfficeDrawing/ClientAnchor.h"
#include "OfficeDrawing/Shapetypes/LineType.h"
#include "OfficeDrawing/MetafilePictBlip.h"
#include "OfficeDrawing/BitmapBlip.h"

namespace DocFileFormat
{
	class VMLShapeMapping : public PropertiesMapping, public IMapping
	{
	public:

		VMLShapeMapping (ConversionContext* ctx, XMLTools::CStringXmlWriter* writer, Spa* pSpa, PictureDescriptor* pict,  IMapping* caller, bool bullet = false, bool in_group = false);
		virtual ~VMLShapeMapping();
		virtual void Apply(IVisitable* visited);

        static std::wstring mapVerticalPosition (PositionVertical vPos );
        static std::wstring mapVerticalPositionRelative	(int vRel );
        static std::wstring mapHorizontalPosition (PositionHorizontal hPos );
        static std::wstring mapHorizontalPositionRelative (int hRel );
		static std::wstring mapWrapText (int val_);

		bool			m_inGroup;
		bool			m_isBullete;
		bool			m_isPictureBroken;
		std::wstring	m_shapeId;
	private:

		void ApplyPrimitives	(DrawingPrimitives	* primitives );
		size_t  ApplyPrimitive	(DrawingPrimitives	* primitives, size_t index);

		void WritePrimitiveProps(DrawingPrimitive	* primitive, bool root);

		void WriteGroup(const GroupContainer* pContainer);
		void WriteShape (const ShapeContainer* pContainer);

		std::wstring GenShapeId(const Shape* pShape) const;
		bool copyPicture( BlipStoreEntry* bse );
		std::wstring GetTargetExt( Global::BlipType _type ) const;
		
		void AppendDimensionToStyle	( std::wstring& style, const PictureDescriptor* pict, bool twistDimensions ) const;
		void AppendDimensionToStyle	( std::wstring& style, const Spa* pSpa, bool twistDimensions ) const;
		void AppendDimensionToStyle	( std::wstring& style, const ChildAnchor* anchor, bool twistDimensions ) const;
		void appendStyleProperty	( std::wstring& style, const std::wstring& propName, const std::wstring& propValue ) const;
		void appendStylePropertyFirst( std::wstring& style, const std::wstring& propName, const std::wstring& propValue ) const;
		
		std::wstring getTextboxAnchor( unsigned int anchor ) const;
				
		std::wstring buildStyle		( const Shape* shape, const ChildAnchor* anchor, const std::vector<ODRAW::OfficeArtFOPTEPtr>& options, int zIndex, bool &twistDimensions) const;
		void AppendOptionsToStyle	( std::wstring& style, const std::vector<ODRAW::OfficeArtFOPTEPtr>& options, int zIndex ) const;
		
		int UpdateFromGuides(const int val) const;

		std::wstring getLineStyle	( unsigned int p ) const;
		std::wstring getArrowStyle	( unsigned int op ) const;
		std::wstring getArrowLength	( unsigned int op ) const;
		std::wstring getArrowWidth	( unsigned int op ) const;
		std::wstring getFillMethod	( unsigned int p ) const;
		std::wstring getFillColorString( const ODRAW::OfficeArtFOPTEPtr& pOpt ) const;

		std::wstring getFillType	( unsigned int p ) const;
		std::wstring getShadowType	( unsigned int p ) const;

		std::wstring getWrapType	(const Spa* pSpa) const;

		void WriteBeginShapeNode	(const Shape* pShape);
		void WriteEndShapeNode		(const Shape* pShape);
		std::wstring GetShapeID		(const Shape* pShape) const;

		std::wstring GetLineFrom	(const ChildAnchor* pAnchor, bool twistDimensions) const;
		std::wstring GetLineTo		(const ChildAnchor* pAnchor, bool twistDimensions) const;

		std::wstring				GetWrapCoords		( const ODRAW::OfficeArtFOPTEPtr& pOpt ) const;
		std::vector<std::wstring>	GetTextRectangles	( const ODRAW::OfficeArtFOPTEPtr& pOpt ) const;
		std::wstring				GetConnectAngles	( const ODRAW::OfficeArtFOPTEPtr& pOpt ) const;
		std::wstring				GetConnectLocs		( const ODRAW::OfficeArtFOPTEPtr& pOpt ) const;
		void						GetGuides			( const ODRAW::OfficeArtFOPTEPtr& pOpt );

		int								m_nAdjValues[8];
		std::vector<ODRAW::MSOSG>		m_arrGuides;
		
		mutable bool					m_isInlineShape;
		Spa*							m_pSpa;
		IMapping*						m_pCaller;
		BlipStoreContainer*				m_pBlipStore;
		int								m_nImageId;
		std::wstring					m_textPathStyle;
		ConversionContext*				m_context;
		PictureDescriptor*				m_pict;

        XMLTools::XMLElement        m_fill;
        XMLTools::XMLElement        m_stroke;
        XMLTools::XMLElement        m_shadow;
        XMLTools::XMLElement        m_imagedata;
        XMLTools::XMLElement        m_3dstyle;
        XMLTools::XMLElement        m_textpath;
	};
}
