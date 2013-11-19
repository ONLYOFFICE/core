#pragma once
#ifndef PPTX_LOGIC_ANIMSCALE_INCLUDE_H_
#define PPTX_LOGIC_ANIMSCALE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimScale : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimScale)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr = node.ReadNode(_T("p:cBhvr"));
				
				XmlUtils::CXmlNode oNodeBy;
				if (node.GetNode(_T("p:by"), oNodeBy))
				{
					oNodeBy.ReadAttributeBase(L"x", byX);
					oNodeBy.ReadAttributeBase(L"y", byY);
				}
				XmlUtils::CXmlNode oNodeFrom;
				if (node.GetNode(_T("p:from"), oNodeFrom))
				{
					oNodeFrom.ReadAttributeBase(L"x", fromX);
					oNodeFrom.ReadAttributeBase(L"y", fromY);
				}
				XmlUtils::CXmlNode oNodeTo;
				if (node.GetNode(_T("p:to"), oNodeTo))
				{
					oNodeTo.ReadAttributeBase(L"x", toX);
					oNodeTo.ReadAttributeBase(L"y", toY);
				}

				node.ReadAttributeBase(L"zoomContents", zoomContents);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("zoomContents"), zoomContents);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);

				if (byX.IsInit() || byY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), byX);
					oAttr1.Write(_T("y"), byY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:by"), oAttr1);
				}
				if (fromX.IsInit() || fromY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), fromX);
					oAttr1.Write(_T("y"), fromY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:from"), oAttr1);
				}
				if (toX.IsInit() || toY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), toX);
					oAttr1.Write(_T("y"), toY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:to"), oAttr1);
				}

				return XmlUtils::CreateNode(_T("p:animScale"), oAttr, oValue);
			}

		public:
			CBhvr				cBhvr;

			nullable_int		byX;
			nullable_int		byY;
			nullable_int		fromX;
			nullable_int		fromY;
			nullable_int		toX;
			nullable_int		toY;

			nullable_bool		zoomContents;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMSCALE_INCLUDE_H