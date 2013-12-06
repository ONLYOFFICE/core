
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Shape.h"
#include "ShapeType.h"
#include "Rect.h"
#include "Oval.h"
#include "Roundrect.h"
#include "Line.h"

#include "Group.h"

namespace OOX
{
	namespace Logic
	{
		GroupItem::GroupItem()
		{

		}		

		GroupItem::GroupItem(const Oval& elem) : IItemable<WritingElement>(new Oval(elem))
		{	

		}

		GroupItem::GroupItem(const Rect& elem) : IItemable<WritingElement>(new Rect(elem)) 
		{	
		}


		GroupItem::GroupItem(const Roundrect& elem) : IItemable<WritingElement>(new Roundrect(elem)) 
		{

		}

		GroupItem::GroupItem(const ShapeType& elem) : IItemable<WritingElement>(new ShapeType(elem)) 
		{

		}

		GroupItem::GroupItem(const Shape& elem) : IItemable<WritingElement>(new Shape(elem)) 
		{

		}
	
		GroupItem::GroupItem(const Line& elem) : IItemable<WritingElement>(new Line(elem)) 
		{

		}

		GroupItem::GroupItem(const Group& elem) : IItemable<WritingElement>(new Group(elem)) 
		{

		}

		GroupItem::~GroupItem()
		{

		}

		GroupItem::GroupItem(const XML::XNode& node)
		{
			fromXML(node);
		}

		const GroupItem& GroupItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void GroupItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.XName == "oval")
				m_item.reset(new Oval(element));
			if (element.XName == "rect")
				m_item.reset (new Rect(element));
			if (element.XName == "roundrect")
				m_item.reset (new Roundrect(element));
			if (element.XName == "shapetype")
				m_item.reset (new ShapeType(element));
			if (element.XName == "shape")
				m_item.reset (new Shape(element));
			if (element.XName == "line")
				m_item.reset (new Line(element));
			if (element.XName == "group")
				m_item.reset (new Group(element));
		}

		const XML::XNode GroupItem::toXML() const
		{
		return XML::XElement();
		}
	}

	namespace Logic
	{
		Group::Group()
		{

		}

		Group::~Group()
		{

		}	

		Group::Group(const XML::XNode& node)
		{
			fromXML(node);
		}

		void Group::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			id			=	element.attribute("id").value();
			spid		=	element.attribute("spid").value();
			style		=	element.attribute("style").value();	
			coordorigin	=	element.attribute("coordorigin").value();
			coordsize	=	element.attribute("coordsize").value();

			for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			{
				if (i->isElement())
				{
					const XML::XElement grelement(*i);

					//std::string strName = grelement.XName;
					//if (element.XName == "group")
					{

					}
					//else
					{
						items->push_back(GroupItem(*i));
					}
				}
			}
		}

		const XML::XNode Group::toXML() const
		{			
			return XML::XElement();
		}
	} 
}