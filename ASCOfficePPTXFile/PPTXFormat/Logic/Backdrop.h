#pragma once
#ifndef PPTX_LOGIC_BACKDROP_INCLUDE_H_
#define PPTX_LOGIC_BACKDROP_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Backdrop : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Backdrop)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNodeA = node.ReadNode(_T("a:anchor"));
				XmlUtils::CXmlNode oNodeN = node.ReadNode(_T("a:norm"));
				XmlUtils::CXmlNode oNodeU = node.ReadNode(_T("a:up"));
				
				anchorX = oNodeA.ReadAttributeInt(L"x");
				anchorY = oNodeA.ReadAttributeInt(L"y");
				anchorZ = oNodeA.ReadAttributeInt(L"z");

				normX = oNodeN.ReadAttributeInt(L"dx");
				normY = oNodeN.ReadAttributeInt(L"dy");
				normZ = oNodeN.ReadAttributeInt(L"dz");

				upX = oNodeU.ReadAttributeInt(L"dx");
				upY = oNodeU.ReadAttributeInt(L"dy");
				upZ = oNodeU.ReadAttributeInt(L"dz");
			}

			virtual CString toXML() const
			{
				CString str1 = _T("");
				str1.Format(_T("<a:anchor x=\"%d\" y=\"%d\" z=\"%d\" />"), anchorX, anchorY, anchorZ);

				CString str2 = _T("");
				str1.Format(_T("<a:norm dx=\"%d\" dy=\"%d\" dz=\"%d\" />"), normX, normY, normZ);

				CString str3 = _T("");
				str1.Format(_T("<a:up dx=\"%d\" dy=\"%d\" dz=\"%d\" />"), upX, upY, upZ);

				return _T("<a:backdrop>") + str1 + str2 + str3 + _T("</a:backdrop>");
			}
		public:
			int anchorX;
			int anchorY;
			int anchorZ;

			int normX;
			int normY;
			int normZ;

			int upX;
			int upY;
			int upZ;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BACKDROP_INCLUDE_H_