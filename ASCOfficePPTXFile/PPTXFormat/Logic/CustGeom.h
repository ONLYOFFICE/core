#pragma once
#ifndef PPTX_LOGIC_CUSTGEOM_INCLUDE_H_
#define PPTX_LOGIC_CUSTGEOM_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Gd.h"
#include "Rect.h"
#include "Path2D.h"
#include "AhBase.h"
#include "Cxn.h"

namespace PPTX
{
	namespace Logic
	{
		class CustGeom : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CustGeom)

			CustGeom& operator=(const CustGeom& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				avLst = oSrc.avLst;
				gdLst = oSrc.gdLst;
				ahLst = oSrc.ahLst;
				cxnLst = oSrc.cxnLst;
				pathLst = oSrc.pathLst;

				rect = oSrc.rect;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:avLst"), oNode))
					oNode.LoadArray(_T("a:gd"), avLst);
				if (node.GetNode(_T("a:gdLst"), oNode))
					oNode.LoadArray(_T("a:gd"), gdLst);
				
				if (node.GetNode(_T("a:ahLst"), oNode))
				{
					oNode.LoadArray(_T("a:ahPolar"), ahLst);
					oNode.LoadArray(_T("a:ahXY"), ahLst);
				}

				if (node.GetNode(_T("a:cxnLst"), oNode))
					oNode.LoadArray(_T("a:cxn"), cxnLst);

				if (node.GetNode(_T("a:pathLst"), oNode))
					oNode.LoadArray(_T("a:path"), pathLst);

				rect = node.ReadNodeNoNS(_T("rect"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(_T("a:avLst"), avLst);
				oValue.WriteArray(_T("a:gdLst"), gdLst);
				oValue.WriteArray(_T("a:ahLst"), ahLst);
				oValue.WriteArray(_T("a:cxnLst"), cxnLst);
				oValue.WriteNullable(rect);
				oValue.WriteArray(_T("a:pathLst"), pathLst);
				
				return XmlUtils::CreateNode(_T("a:custGeom"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:custGeom"));
				pWriter->EndAttributes();

				if (avLst.size() == 0)
					pWriter->WriteString(_T("<a:avLst/>"));
				else
					pWriter->WriteArray(_T("a:avLst"), avLst);
				
				if (gdLst.size() == 0)
					pWriter->WriteString(_T("<a:gdLst/>"));
				else
					pWriter->WriteArray(_T("a:gdLst"), gdLst);

				if (ahLst.size() == 0)
					pWriter->WriteString(_T("<a:ahLst/>"));
				else
					pWriter->WriteArray(_T("a:ahLst"), ahLst);

				if (cxnLst.size() == 0)
					pWriter->WriteString(_T("<a:cxnLst/>"));
				else
					pWriter->WriteArray(_T("a:cxnLst"), cxnLst);		
		
				if (rect.is_init())
					pWriter->Write(rect);
				else
					pWriter->WriteString(_T("<a:rect l=\"0\" t=\"0\" r=\"r\" b=\"b\"/>"));
				if (pathLst.size() == 0)
					pWriter->WriteString(_T("<a:pathLst/>"));
				else
					pWriter->WriteArray(_T("a:pathLst"), pathLst);
				
				pWriter->EndNode(_T("a:custGeom"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_CUSTOM);

				pWriter->WriteRecordArray(0, 1, avLst);
				pWriter->WriteRecordArray(1, 1, gdLst);
				pWriter->WriteRecordArray(2, 1, ahLst);
				pWriter->WriteRecordArray(3, 1, cxnLst);
				pWriter->WriteRecordArray(4, 1, pathLst);
				pWriter->WriteRecord2(5, rect);

				pWriter->EndRecord();
			}

		public:
			std::vector<Gd>			avLst;
			std::vector<Gd>			gdLst;
			std::vector<AhBase>		ahLst;
			std::vector<Cxn>		cxnLst;
			nullable<Rect>			rect;
			std::vector<Path2D>		pathLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = 0;

				count = avLst.size();
				for (size_t i = 0; i < count; ++i)
					avLst[i].SetParentPointer(this);			

				count = gdLst.size();
				for (size_t i = 0; i < count; ++i)
					gdLst[i].SetParentPointer(this);

				count = ahLst.size();
				for (size_t i = 0; i < count; ++i)
					ahLst[i].SetParentPointer(this);

				count = cxnLst.size();
				for (size_t i = 0; i < count; ++i)
					cxnLst[i].SetParentPointer(this);

				count = pathLst.size();
				for (size_t i = 0; i < count; ++i)
					pathLst[i].SetParentPointer(this);

				if (rect.IsInit())
					rect->SetParentPointer(this);
			}
		public:
			virtual CString GetODString() const
			{
				CString strXml = _T("");

				size_t nCount  = 0;
				
				strXml += _T("<avLst>");
				nCount = avLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += avLst[i].GetODString();
				strXml += _T("</avLst>");

				strXml += _T("<gdLst>");
				nCount = gdLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += gdLst[i].GetODString();
				strXml += _T("</gdLst>");

				strXml += _T("<ahLst>");
				nCount = ahLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += ahLst[i].GetODString();
				strXml += _T("</ahLst>");

				strXml += _T("<cxnLst>");
				nCount = cxnLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += cxnLst[i].GetODString();
				strXml += _T("</cxnLst>");

				strXml += _T("<pathLst>");
				nCount = pathLst.size();
				for (size_t i = 0; i < nCount; ++i)
					strXml += pathLst[i].GetODString();
				strXml += _T("</pathLst>");

				if (rect.IsInit())
					strXml += rect->GetODString();
				
				return XmlUtils::CreateNode(_T("ooxml-shape"), strXml);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CUSTGEOM_INCLUDE_H