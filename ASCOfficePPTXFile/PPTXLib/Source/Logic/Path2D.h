#pragma once
#ifndef PPTX_LOGIC_PATH2D_INCLUDE_H_
#define PPTX_LOGIC_PATH2D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "nullable.h"
#include "./../Limit/PathFillMode.h"
#include "UniPath2D.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{

		class Path2D : public WrapperWritingElement
		{
		public:
			Path2D();
			virtual ~Path2D();
			explicit Path2D(const XML::XNode& node);
			const Path2D& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<bool> extrusionOk;
			nullable_property<std::string, Limit::PathFillMode> fill;
			nullable_property<int> h;
			nullable_property<bool> stroke; 
			nullable_property<int> w;

			property<std::list<UniPath2D> > Paths;
		protected:
			virtual void FillParentPointersForChilds();
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATH2D_INCLUDE_H_