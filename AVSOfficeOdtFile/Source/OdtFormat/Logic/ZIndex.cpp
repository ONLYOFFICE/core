
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ZIndex.h"
#include "property.h"


namespace Odt
{
	namespace Logic
	{

		ZIndex::ZIndex()
		{
		}


		ZIndex::ZIndex(const Common::ZIndex& zIndex)
		{
			fromBase(zIndex); //TODO исправить, т.к. у Odt zIndex > 0
		}


		ZIndex::ZIndex(const XML::XElement& element)
		{			
			Index = element.attribute("z-index").value();
		}


		const XML::XNode ZIndex::toXML(const Namespaces& ns) const
		{			
			return 
				XML::XContainer(
					XML::XAttribute(ns.svg + "z-index", Index)	// TODO, это не рабочая конструкция
					);
		}


		const ZIndex& ZIndex::operator = (const Common::ZIndex& zIndex)
		{
			fromBase(zIndex);
			return *this;
		}

	} // namespace Logic
} // namespace Odt