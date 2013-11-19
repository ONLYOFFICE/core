#pragma once
#ifndef PPTX_LOGIC_PRSTCLR_INCLUDE_H_
#define PPTX_LOGIC_PRSTCLR_INCLUDE_H_

#include "property.h"
#include "./../../Limit/PrstClrVal.h"
#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class PrstClr : public ColorBase
		{
		public:
			PrstClr();
			virtual ~PrstClr();			
			explicit PrstClr(const XML::XNode& node);
			const PrstClr& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual DWORD GetRGBA(DWORD RGBA);
			virtual DWORD GetARGB(DWORD ARGB);
			virtual DWORD GetBGRA(DWORD BGRA);
			virtual DWORD GetABGR(DWORD ABGR);
		private:
			void FillRGBFromVal();
		private:
			property<std::string, Limit::PrstClrVal> val;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PRSTCLR_INCLUDE_H