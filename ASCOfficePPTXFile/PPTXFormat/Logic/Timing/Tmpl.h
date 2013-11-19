#pragma once
#ifndef PPTX_LOGIC_TMPL_INCLUDE_H_
#define PPTX_LOGIC_TMPL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TnLst.h"

namespace PPTX
{
	namespace Logic
	{
		class Tmpl : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Tmpl)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				tnLst	= node.ReadNode(_T("p:tnLst"));
				node.ReadAttributeBase(L"lvl", lvl);
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("lvl"), lvl);

				return XmlUtils::CreateNode(_T("p:tmpl"), oAttr, tnLst.toXML());
			}
		public:
			TnLst			tnLst;
			nullable_int	lvl;
		protected:
			virtual void FillParentPointersForChilds()
			{
				tnLst.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				lvl.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TMPL_INCLUDE_H_