#pragma once
#ifndef PPTX_LOGIC_CLOSE_INCLUDE_H_
#define PPTX_LOGIC_CLOSE_INCLUDE_H_

#include "PathBase.h"
#include "property.h"
#include "nullable.h"

namespace PPTX
{
	namespace Logic
	{

		class Close : public PathBase
		{
		public:
			Close();
			virtual ~Close();
			explicit Close(const XML::XNode& node);
			const Close& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			//std::string GetODString()const;
			void GetODString(std::list<std::string>& strList)const;
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLOSE_INCLUDE_H_