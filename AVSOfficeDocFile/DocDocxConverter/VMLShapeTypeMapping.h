#pragma once

#include "PropertiesMapping.h"
#include "OfficeDrawing\ShapeType.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class VMLShapeTypeMapping: public PropertiesMapping, public IMapping
	{
	private: 
		XMLTools::XMLElement<WCHAR> *_lock;
		bool _isBulletPicture;

	public:
		VMLShapeTypeMapping(XmlUtils::CXmlWriter* writer, bool isBulletPicture = false );
		virtual ~VMLShapeTypeMapping();
		virtual void Apply( IVisitable* visited );
		/// Returns the id of the referenced type
		static wstring GenerateTypeId (const ShapeType* pShape);
	};
}