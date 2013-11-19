#pragma once
#ifndef PPTX_LOGIC_AHXY_INCLUDE_H_
#define PPTX_LOGIC_AHXY_INCLUDE_H_

#include "Ah.h"

namespace PPTX
{
	namespace Logic
	{

		class AhXY : public Ah
		{
		public:
			PPTX_LOGIC_BASE(AhXY)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oPos = node.ReadNode(_T("a:pos"));

				x	= oPos.ReadAttributeBase(L"x");
				y	= oPos.ReadAttributeBase(L"y");

				node.ReadAttributeBase(L"gdRefX", gdRefX);
				node.ReadAttributeBase(L"gdRefY", gdRefY);
				node.ReadAttributeBase(L"maxX", maxX);
				node.ReadAttributeBase(L"maxY", maxY);
				node.ReadAttributeBase(L"minX", minX);
				node.ReadAttributeBase(L"minY", minY);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("gdRefX"), gdRefX);
				oAttr1.Write(_T("minX"), minX);
				oAttr1.Write(_T("maxX"), maxX);
				oAttr1.Write(_T("gdRefY"), gdRefY);
				oAttr1.Write(_T("minY"), minY);
				oAttr1.Write(_T("maxY"), maxY);

				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("x"), x);
				oAttr2.Write(_T("y"), y);

				return XmlUtils::CreateNode(_T("a:ahXY"), oAttr1, XmlUtils::CreateNode(_T("a:pos"), oAttr2));
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:ahXY"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("gdRefX"), gdRefX);
				pWriter->WriteAttribute(_T("minX"), minX);
				pWriter->WriteAttribute(_T("maxX"), maxX);
				pWriter->WriteAttribute(_T("gdRefY"), gdRefY);
				pWriter->WriteAttribute(_T("minY"), minY);
				pWriter->WriteAttribute(_T("maxY"), maxY);
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:pos"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("x"), x);
				pWriter->WriteAttribute(_T("y"), y);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:pos"));

				pWriter->EndNode(_T("a:ahXY"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(GEOMETRY_TYPE_AH_XY);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, x);
				pWriter->WriteString1(1, y);

				pWriter->WriteString2(2, gdRefX);
				pWriter->WriteString2(3, gdRefY);
				pWriter->WriteString2(4, maxX);
				pWriter->WriteString2(5, maxY);
				pWriter->WriteString2(6, minX);
				pWriter->WriteString2(7, minY);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			CString								x;
			CString								y;

			nullable_string					gdRefX;
			nullable_string					gdRefY;
			nullable_string					maxX;
			nullable_string					maxY;
			nullable_string					minX;
			nullable_string					minY;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			
			CString GetODString()const
			{
				XmlUtils::CAttribute oAttr1;
				oAttr1.Write(_T("gdRefX"), gdRefX);
				oAttr1.Write(_T("minX"), minX);
				oAttr1.Write(_T("maxX"), maxX);
				oAttr1.Write(_T("gdRefY"), gdRefY);
				oAttr1.Write(_T("minY"), minY);
				oAttr1.Write(_T("maxY"), maxY);

				XmlUtils::CAttribute oAttr2;
				oAttr2.Write(_T("x"), x);
				oAttr2.Write(_T("y"), y);

				return XmlUtils::CreateNode(_T("ahXY"), oAttr1, XmlUtils::CreateNode(_T("pos"), oAttr2));
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_AHXY_INCLUDE_H_