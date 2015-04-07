#pragma once
#ifndef PPTX_LOGIC_ANIMCLR_INCLUDE_H_
#define PPTX_LOGIC_ANIMCLR_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "./../UniColor.h"
#include "./../../Limit/TLColorSpace.h"
#include "./../../Limit/TLColorDirection.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimClr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimClr)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"clrSpc", clrSpc);
				node.ReadAttributeBase(L"dir", dir);

				cBhvr	= node.ReadNode(_T("cBhvr"));

				XmlUtils::CXmlNode oNodeFrom;
				if (node.GetNode(_T("p:from"), oNodeFrom))
					from.GetColorFrom(oNodeFrom);

				XmlUtils::CXmlNode oNodeTo;
				if (node.GetNode(_T("p:to"), oNodeTo))
					to.GetColorFrom(oNodeTo);

				XmlUtils::CXmlNode oNodeBy;
				if (node.GetNode(_T("p:by"), oNodeBy))
				{
					XmlUtils::CXmlNode oRGB;
					XmlUtils::CXmlNode oHSL;
					if (oNodeBy.GetNode(_T("p:rgb"), oRGB))
					{
						oRGB.ReadAttributeBase(L"r", byR);
						oRGB.ReadAttributeBase(L"g", byG);
						oRGB.ReadAttributeBase(L"b", byB);
					}
					else if (oNodeBy.GetNode(_T("p:hsl"), oRGB))
					{
						oHSL.ReadAttributeBase(L"h", byH);
						oHSL.ReadAttributeBase(L"s", byS);
						oHSL.ReadAttributeBase(L"l", byL);
					}
				}

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("clrSpc"), clrSpc);
				oAttr.WriteLimitNullable(_T("dir"), dir);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);

				if (byR.IsInit() && byG.IsInit() && byB.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("r"), byR);
					oAttr1.Write(_T("g"), byG);
					oAttr1.Write(_T("b"), byB);

					oValue.m_strValue += (_T("<p:by>") + XmlUtils::CreateNode(_T("p:rgb"), oAttr1) + _T("</p:by>"));
				}
				else if (byH.IsInit() && byS.IsInit() && byL.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("h"), byH);
					oAttr1.Write(_T("s"), byS);
					oAttr1.Write(_T("l"), byL);

					oValue.m_strValue += (_T("<p:by>") + XmlUtils::CreateNode(_T("p:hsl"), oAttr1) + _T("</p:by>"));
				}

				if (from.is_init())
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:from"), from.toXML());
				if (to.is_init())
					oValue.m_strValue += XmlUtils::CreateNode(_T("p:to"), to.toXML());

				return XmlUtils::CreateNode(_T("p:animClr"), oAttr, oValue);
			}

		public:
			CBhvr			cBhvr;

			nullable_int	byR;
			nullable_int	byG;
			nullable_int	byB;
			nullable_int	byH;
			nullable_int	byS;
			nullable_int	byL;
			UniColor		from;
			UniColor		to;
//Attributes
			nullable_limit<Limit::TLColorSpace>		clrSpc; //ST_TLAnimateColorSpace 
			nullable_limit<Limit::TLColorDirection> dir;	//ST_TLAnimateColorDirection 
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				from.SetParentPointer(this);
				to.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMCLR_INCLUDE_H