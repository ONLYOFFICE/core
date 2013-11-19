#ifndef PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "Ph.h"
#include "UniMedia.h"

namespace PPTX
{
	namespace Logic
	{
		class NvPr : public WrapperWritingElement
		{
		public:
			NvPr();
			virtual ~NvPr();			
			explicit NvPr(const XML::XNode& node);
			const NvPr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			// Attributes
			nullable_property<bool> isPhoto;
			nullable_property<bool> userDrawn;

			//Childs
			nullable_property<Ph> ph;
			UniMedia media;
			//custDataLst
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_NV_PROPERTIES_INCLUDE_H