#pragma once
#ifndef PPTX_LOGIC_SCHEMECLR_INCLUDE_H_
#define PPTX_LOGIC_SCHEMECLR_INCLUDE_H_

#include "property.h"
#include "./../../Limit/SchemeClrVal.h"
#include "./../../Theme/ClrScheme.h"
#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class SchemeClr : public ColorBase
		{
		public:
			SchemeClr();
			virtual ~SchemeClr();			
			explicit SchemeClr(const XML::XNode& node);
			const SchemeClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual DWORD GetRGBA(DWORD RGBA);
			virtual DWORD GetARGB(DWORD ARGB);
			virtual DWORD GetBGRA(DWORD BGRA);
			virtual DWORD GetABGR(DWORD ABGR);
		public:
			property<std::string, Limit::SchemeClrVal> val;
		private:
			void FillRGBFromVal(DWORD rgb);

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SCHEMECLR_INCLUDE_H