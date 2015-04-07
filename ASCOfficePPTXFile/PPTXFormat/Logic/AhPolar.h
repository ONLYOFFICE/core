#pragma once
#ifndef PPTX_LOGIC_AHPOLAR_INCLUDE_H_
#define PPTX_LOGIC_AHPOLAR_INCLUDE_H_

#include "Ah.h"

namespace PPTX
{
	namespace Logic
	{

		class AhPolar : public Ah
		{
		public:
			PPTX_LOGIC_BASE(AhPolar)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oPos = node.ReadNode(_T("a:pos"));

				x	= oPos.GetAttributeBase(L"x");
				y	= oPos.GetAttributeBase(L"y");

				node.ReadAttributeBase(L"gdRefAng", gdRefAng);
				node.ReadAttributeBase(L"gdRefR", gdRefR);
				node.ReadAttributeBase(L"maxAng", maxAng);
				node.ReadAttributeBase(L"maxR", maxR);
				node.ReadAttributeBase(L"minAng", minAng);
				node.ReadAttributeBase(L"minR", minR);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("gdRefR"), gdRefR);
				oAttr1.Write(_T("minR"), minR);
				oAttr1.Write(_T("maxR"), maxR);
				oAttr1.Write(_T("gdRefAng"), gdRefAng);
				oAttr1.Write(_T("minAng"), minAng);
				oAttr1.Write(_T("maxAng"), maxAng);

				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("x"), x);
				oAttr2.Write(_T("y"), y);

				return XmlUtils::CreateNode(_T("a:ahPolar"), oAttr1, XmlUtils::CreateNode(_T("a:pos"), oAttr2));
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:ahPolar"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("gdRefR"), gdRefR);
				pWriter->WriteAttribute(_T("minR"), minR);
				pWriter->WriteAttribute(_T("maxR"), maxR);
				pWriter->WriteAttribute(_T("gdRefAng"), gdRefAng);
				pWriter->WriteAttribute(_T("minAng"), minAng);
				pWriter->WriteAttribute(_T("maxAng"), maxAng);
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pos"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pos"));

				pWriter->EndNode(_T("a:ahPolar"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_AH_POLAR);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x);
				pWriter->WriteString1(1, y);

				pWriter->WriteString2(2, gdRefAng);
				pWriter->WriteString2(3, gdRefR);
				pWriter->WriteString2(4, maxAng);
				pWriter->WriteString2(5, maxR);
				pWriter->WriteString2(6, minAng);
				pWriter->WriteString2(7, minR);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			CString				x;
			CString				y;

			nullable_string gdRefAng;
			nullable_string gdRefR;
			nullable_string maxAng;
			nullable_string maxR;
			nullable_string minAng;
			nullable_string minR;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			CString GetODString()const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("gdRefR"), gdRefR);
				oAttr1.Write(_T("minR"), minR);
				oAttr1.Write(_T("maxR"), maxR);
				oAttr1.Write(_T("gdRefAng"), gdRefAng);
				oAttr1.Write(_T("minAng"), minAng);
				oAttr1.Write(_T("maxAng"), maxAng);

				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("x"), x);
				oAttr2.Write(_T("y"), y);

				return XmlUtils::CreateNode(_T("ahPolar"), oAttr1, XmlUtils::CreateNode(_T("pos"), oAttr2));
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHPOLAR_INCLUDE_H_