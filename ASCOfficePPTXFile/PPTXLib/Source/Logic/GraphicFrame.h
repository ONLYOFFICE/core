#pragma once
#ifndef PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H_
#define PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "NvGraphicFramePr.h"
#include "Xfrm.h"
#include "ShapeProperties.h"
//#include "DocxFormat/RId.h"
#include "Table/Table.h"


namespace PPTX
{
	namespace Logic
	{
		class GraphicFrame : public WrapperWritingElement
		{
		public:
			GraphicFrame();
			virtual ~GraphicFrame();			
			explicit GraphicFrame(const XML::XNode& node);
			const GraphicFrame& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual void GetRect(RECT& pRect)const;

		public:
			property<NvGraphicFramePr> nvGraphicFramePr;
			property<Xfrm> xfrm;
			//property<Graphic> grpahic;
			//nullable_property<OOX::RId> dm;
			//nullable_property<OOX::RId> lo;
			//nullable_property<OOX::RId> qs;
			//nullable_property<OOX::RId> cs;
			nullable_property<std::string> spid;
			nullable_property<Table> Table;

			const boost::filesystem::wpath GetPathBySpid()const;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H