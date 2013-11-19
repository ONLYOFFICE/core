#pragma once
#ifndef PPTX_LOGIC_PATH_INCLUDE_H_
#define PPTX_LOGIC_PATH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "Rect.h"
#include "./../Limit/FillPath.h"

namespace PPTX
{
	namespace Logic
	{

		class Path : public WrapperWritingElement
		{
		public:
			Path();
			virtual ~Path();
			explicit Path(const XML::XNode& node);
			const Path& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<Rect> rect;
			nullable_property<std::string, Limit::FillPath> path;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATH_INCLUDE_H_