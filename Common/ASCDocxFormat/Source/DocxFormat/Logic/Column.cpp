
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Column.h"


namespace OOX
{
	namespace Logic
	{

		Column::Column()
		{
		}


		Column::~Column()
		{
		}
	

		Column::Column(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Column& Column::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Column::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			//if(element.attribute("w").exist())
			//{
				int width = element.attribute("w").value().ToInt();
				Width = UniversalUnit(width, UniversalUnit::Dx);
			//}
			if(element.attribute("space").exist())
			{
				int space = element.attribute("space").value().ToInt();
				Space = UniversalUnit(space, UniversalUnit::Dx);
			}
		}


		const XML::XNode Column::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX