
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Drawing.h"
#include "Inline.h"


namespace OOX
{
	namespace Logic
	{

		Drawing::Drawing() : haveAnchor(false)
		{
		}


		Drawing::~Drawing()
		{
		}


		Drawing::Drawing(const RId& rId, const boost::filesystem::wpath& filename, const long width, const long height)
		{
			haveAnchor	=	false;

			Inline->DistT = 0;
			Inline->DistB = 0;
			Inline->DistL = 0;
			Inline->DistR = 0;

			Inline->Extent = Extent();
			
			int newH;
			int newW;
			int dpi = 96;
			double heightPage	=	25.7;//in cm
			double widthPage	=	16.5;//in cm			
			double maximum		=	std::max(width / (widthPage / 2.54 * dpi), height / (heightPage / 2.54 * dpi));

			if (maximum < 1)
			{				
				newW	=	(int) ( width  * 2.54 * 72 * 100.0 * 1000.0 / 20.0 / 96.0);					//	from px to sx 
				newH	=	(int) ( height * 2.54 * 72 * 100.0 * 1000.0 / 20.0 / 96.0);					//	from px to sx 
			}
			else
			{				
				newW	=	(int) ( width  / maximum * 2.54 * 72.0 * 100.0 * 1000.0 / 20.0 / 96.0);		//	from px to sx
				newH	=	(int) ( height / maximum * 2.54 * 72.0 * 100.0 * 1000.0 / 20.0 / 96.0);		//	from px to sx
			}

			Inline->Extent->Size->Width			=	newW;
			Inline->Extent->Size->Height		=	newH;

			Inline->EffectExtent->Left			=	19050;
			Inline->EffectExtent->Top			=	0;
			Inline->EffectExtent->Right			=	3175;
			Inline->EffectExtent->Bottom		=	0;

			Inline->DocPr						=	DocPr();
			Inline->DocPr->Id					=	1;
			Inline->DocPr->Name					=	"Image 0";
			Inline->DocPr->Descr				=	Encoding::unicode2utf8(filename.filename());

			Inline->NoChangeAspect				=	1;
			Inline->Graphic->Uri				=	"http://schemas.openxmlformats.org/drawingml/2006/picture";

			Inline->Graphic->Pic->Id			=	0;
			Inline->Graphic->Pic->Name			=	Encoding::unicode2utf8(filename.filename());
			Inline->Graphic->Pic->rId			=	rId;

			Inline->Graphic->Pic->Off.init();
			Inline->Graphic->Pic->Off->X		=	0;
			Inline->Graphic->Pic->Off->Y		=	0;

			Inline->Graphic->Pic->Ext.init();
			Inline->Graphic->Pic->Ext->Width	=	newW;
			Inline->Graphic->Pic->Ext->Height	=	newH;
			Inline->Graphic->Pic->Prst			=	"rect";
		}

		Drawing::Drawing(const RId& rId, const boost::filesystem::wpath& filename, const long xEmu, const std::string& hRelativeFrom, const long yEmu, const std::string& vRelativeFrom, const long widthEmu, const long heightEmu)
		{
			Inline->Name = "anchor";

			Inline->DistT = 0;
			Inline->DistB = 0;
			Inline->DistL = 0;
			Inline->DistR = 0;

			Inline->positionH = xEmu;
			Inline->positionHRelativeFrom = hRelativeFrom;
			Inline->positionV = yEmu;
			Inline->positionVRelativeFrom = vRelativeFrom;

			DrawingWrap wrap;
			wrap.Type = "wrapTopAndBottom";
			wrap.TextWrap = "bothSides";
			Inline->Wrap = wrap;

			Point2D simplePos;
			simplePos.name = "simplePos";
			simplePos.x = 0;
			simplePos.y = 0;

			Inline->SimplePos = false;
			Inline->RelativeHeight = 0;
			Inline->BehindDoc = false;
			Inline->Locked = false;
			Inline->LayoutInCell = true;
			Inline->AllowOverlap = true;

			Inline->SimplePosPoint = simplePos;

			Inline->Extent = Extent();
			
			Inline->Extent->Size->Width  = widthEmu;
			Inline->Extent->Size->Height = heightEmu;

			Inline->EffectExtent->Left = 19050;
			Inline->EffectExtent->Top = 0;
			Inline->EffectExtent->Right = 3175;
			Inline->EffectExtent->Bottom = 0;

			Inline->DocPr = DocPr();
			Inline->DocPr->Id = 1;
			Inline->DocPr->Name = "Image 0";
			Inline->DocPr->Descr = Encoding::unicode2utf8(filename.filename());

			Inline->NoChangeAspect = 1;
			Inline->Graphic->Uri = "http://schemas.openxmlformats.org/drawingml/2006/picture";

			Inline->Graphic->Pic->Id = 0;
			Inline->Graphic->Pic->Name = Encoding::unicode2utf8(filename.filename());
			Inline->Graphic->Pic->rId = rId;

			Inline->Graphic->Pic->Off.init();
			Inline->Graphic->Pic->Off->X = 0;
			Inline->Graphic->Pic->Off->Y = 0;

			Inline->Graphic->Pic->Ext.init();
			Inline->Graphic->Pic->Ext->Width = widthEmu;
			Inline->Graphic->Pic->Ext->Height = heightEmu;
			Inline->Graphic->Pic->Prst = "rect";
		}

		Drawing::Drawing(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Drawing& Drawing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Drawing::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element.element("inline").exist())
			{
				Inline		=	element.element("inline");
				haveAnchor	=	false;
			}
			else if (element.element("anchor").exist())
			{
				Inline		=	element.element("anchor");
				haveAnchor	=	true;
			}
		}

		const XML::XNode Drawing::toXML() const
		{
			return XML::XElement(ns.w + "drawing", XML::Write(Inline));
		}

		const std::string Drawing::toTxt() const
		{
			return "[image]";
		}

	} // namespace Logic
} // namespace OOX