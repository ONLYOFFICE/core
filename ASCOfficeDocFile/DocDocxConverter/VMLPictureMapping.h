#pragma once

#include "PropertiesMapping.h"
#include "PictureDescriptor.h"
#include "TwipsValue.h"
#include "ConversionContext.h"
#include "OfficeDrawing\Shapetypes\PictureFrameType.h"
#include "OfficeDrawing\Shape.h"
#include "OfficeDrawing\MetafilePictBlip.h"
#include "OfficeDrawing\BitmapBlip.h"
#include "VMLShapeTypeMapping.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class VMLPictureMapping: public PropertiesMapping, public IMapping
	{
	public:
		VMLPictureMapping( ConversionContext* ctx, XmlUtils::CXmlWriter* writer, bool olePreview, IMapping* caller, bool isBulletPicture = false );
		virtual ~VMLPictureMapping();
		virtual void Apply( IVisitable* visited );
		std::wstring GetShapeId() const;

	private:
		/// Writes a border element
		void writePictureBorder (const WCHAR* name, const BorderCode& brc);

	protected:
		/// Copies the picture from the binary stream to the zip archive 
		/// and creates the relationships for the image.
		bool CopyPicture (BlipStoreEntry* oBlipEntry);

	public:

		static std::wstring GetTargetExt (Global::BlipType nType);
		static std::wstring GetContentType (Global::BlipType nType);

	private:

		int				m_nImageId;
		std::wstring	m_ShapeId;

		bool _olePreview;
		bool _isBulletPicture;
		XMLTools::XMLElement<WCHAR> *_imageData;
		ConversionContext* _ctx;

		IMapping* _caller;
	};
}