#pragma once
#ifndef PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H_
#define PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "NvGraphicFramePr.h"
#include "Xfrm.h"
#include "ShapeProperties.h"
//#include "DocxFormat/RId.h"
#include "Table/Table.h"
#include "SmartArt.h"
#include "Pic.h"


namespace PPTX
{
	namespace Logic
	{
		class GraphicFrame : public WrapperWritingElement
		{
		public:
			GraphicFrame();
			virtual ~GraphicFrame();			
			explicit GraphicFrame(XmlUtils::CXmlNode& node);
			const GraphicFrame& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual CString toXML() const;
			virtual void GetRect(Aggplus::RECT& pRect)const;

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
	

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
		public:
			NvGraphicFramePr		nvGraphicFramePr;
			Xfrm					xfrm;
			//property<Graphic> grpahic;
			//nullable_property<PPTX::RId> dm;
			//nullable_property<PPTX::RId> lo;
			//nullable_property<PPTX::RId> qs;
			//nullable_property<PPTX::RId> cs;
			nullable_string			spid;
            nullable<Table>			table;
			nullable<SmartArt>		smartArt;
			nullable<ChartRec>		chartRec;
			nullable<Pic>			pic;

			CString		 GetVmlXmlBySpid(CString & rels)	const;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GRAPHICFRAME_INCLUDE_H
