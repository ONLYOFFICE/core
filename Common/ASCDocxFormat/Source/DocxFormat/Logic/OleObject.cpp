
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OleObject.h"
#include "./../Folder.h"


namespace OOX
{
	namespace Logic
	{

		OleObject::OleObject()
		{
		}


		OleObject::~OleObject()
		{
		}
	

		OleObject::OleObject(const XML::XNode& node)
		{
			fromXML(node);
		}		


		const OleObject& OleObject::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void OleObject::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Type		= element.attribute("Type").value();
			ProgId		= element.attribute("ProgID").value();
			ShapeId		= element.attribute("ShapeID").value();
			DrawAspect  = element.attribute("DrawAspect").value();
			ObjectId	= element.attribute("ObjectID").value();
			rId			= element.attribute("id").value().ToWString();
			FieldCodes  = element.element("FieldCodes");
		}


		const XML::XNode OleObject::toXML() const
		{			
			return XML::XElement();
				//XML::XElement(ns.o + "OLEObject",
				//	XML::XAttribute("Type", Type) +
				//	XML::XAttribute("ProgID", ProgId) +
				//	XML::XAttribute("ShapeID", ShapeId) +
				//	XML::XAttribute("DrawAspect", DrawAspect) +
				//	XML::XAttribute("ObjectID", ObjectId) +
				//	XML::XAttribute(ns.r + "id", rId) +
				//	XML::Write(FieldCodes)
				//);
		}

	} // namespace Logic
} // namespace OOX