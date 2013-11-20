#ifndef OOX_LOGIC_DOCUMENT_GRID_INCLUDE_H_
#define OOX_LOGIC_DOCUMENT_GRID_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable_property.h"
#include "./../Limit/DocGridType.h"


namespace OOX
{
	namespace Logic
	{
		class DocumentGrid : public WritingElement
		{
		public:
			DocumentGrid();
			virtual ~DocumentGrid();
			explicit DocumentGrid(const XML::XNode& node);
			const DocumentGrid& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<int>	LinePitch;
			nullable_property<int>	CharSpace;
			nullable_property<std::string, Limit::DocGridType>	Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_DOCUMENT_GRID_INCLUDE_H_