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
		void AppendDimensionToStyle( wstring* style, const PictureDescriptor* pict, bool twistDimensions ) const;
		void AppendDimensionToStyle( wstring* style, const Spa* pSpa, bool twistDimensions ) const;
		void AppendDimensionToStyle( wstring* style, const ChildAnchor* anchor, bool twistDimensions ) const;
		void appendStyleProperty( wstring* b, const wstring& propName, const wstring& propValue ) const;
		std::wstring getTextboxAnchor( unsigned int anchor ) const;
		std::wstring mapVerticalPosition( PositionVertical vPos ) const;
		std::wstring mapVerticalPositionRelative( PositionVerticalRelative vRel ) const;
		std::wstring mapHorizontalPosition( PositionHorizontal hPos ) const;
		std::wstring mapHorizontalPositionRelative( PositionHorizontalRelative hRel ) const;
		void AppendOptionsToStyle( wstring* style, const list<OptionEntry>& options ) const;
		std::wstring buildStyle( const Shape* shape, const ChildAnchor* anchor, const list<OptionEntry>& options, int zIndex ) const;
		std::wstring getLineStyle( unsigned int p ) const;
		std::wstring getArrowStyle( unsigned int op ) const;
		std::wstring getArrowLength( unsigned int op ) const;
		std::wstring getArrowWidth( unsigned int op ) const;
		std::wstring getFillMethod( unsigned int p ) const;
		std::wstring getFillColorString( const unsigned char* p, unsigned int size ) const;
		/// Returns the OpenXML fill type of a fill effect
		std::wstring getFillType( unsigned int p ) const;
		std::wstring getShadowType( unsigned int p ) const;
		/// Returns the OpenXML wrap type of the shape
		std::wstring getWrapType(const Spa* pSpa) const;
		std::wstring ParsePath (const list<OptionEntry>& options) const;

		void WriteBeginShapeNode (const Shape* pShape);
		void WriteEndShapeNode (const Shape* pShape);
		std::wstring GetShapeID (const Shape* pShape) const;

		std::wstring GetLineFrom (const ChildAnchor* pAnchor) const;
		std::wstring GetLineTo (const ChildAnchor* pAnchor) const;

		std::vector<CString> GetTextRectangles(const OptionEntry& inscribe) const;

	private: 
		bool						m_bBullet;
		Spa*						m_pSpa;
		IMapping*					m_pCaller;
		BlipStoreContainer*			m_pBlipStore;
		int							m_nImageId;
		std::wstring				m_textPathStyle;
		ConversionContext*			m_ctx;
		PictureDescriptor*			m_pict;
		XMLTools::XMLElement<wchar_t> m_fill; 
		XMLTools::XMLElement<wchar_t>	m_stroke;
		XMLTools::XMLElement<wchar_t>	m_shadow; 
		XMLTools::XMLElement<wchar_t>	m_imagedata;
		XMLTools::XMLElement<wchar_t>	m_3dstyle; 
		XMLTools::XMLElement<wchar_t>	m_textpath;
	};
}
