#pragma once
#ifndef PPTX_LOGIC_SYSCLR_INCLUDE_H_
#define PPTX_LOGIC_SYSCLR_INCLUDE_H_

#include "property.h"
#include "ColorBase.h"
#include "./../../Limit/SysClrVal.h"

namespace PPTX
{
	namespace Logic
	{
		class SysClr : public ColorBase
		{
		public:
			SysClr();
			virtual ~SysClr();			
			explicit SysClr(const XML::XNode& node);
			const SysClr& operator =(const XML::XNode& node);

			virtual DWORD GetRGBA(DWORD RGBA);
			virtual DWORD GetARGB(DWORD ARGB);
			virtual DWORD GetBGRA(DWORD BGRA);
			virtual DWORD GetABGR(DWORD ABGR);
		private:
			void FillRGBFromVal();

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			property<std::string, Limit::SysClrVal> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SYSCLR_INCLUDE_H