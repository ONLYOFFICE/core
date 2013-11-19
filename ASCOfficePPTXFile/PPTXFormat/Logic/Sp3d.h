#pragma once
#ifndef PPTX_LOGIC_SP3D_INCLUDE_H_
#define PPTX_LOGIC_SP3D_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/Material.h"
#include "Bevel.h"
#include "UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class Sp3d : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Sp3d)

			Sp3d& operator=(const Sp3d& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				contourW		= oSrc.contourW;
				extrusionH		= oSrc.extrusionH;
				prstMaterial	= oSrc.prstMaterial;
				z				= oSrc.z;

				bevelT			= oSrc.bevelT;
				bevelB			= oSrc.bevelB;
				extrusionClr	= oSrc.extrusionClr;
				contourClr		= oSrc.contourClr;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"contourW", contourW);
				node.ReadAttributeBase(L"extrusionH", extrusionH);
				node.ReadAttributeBase(L"prstMaterial", prstMaterial);
				node.ReadAttributeBase(L"z", z);

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("bevelT") == strName)
							bevelT = oNode;
						else if (_T("bevelB") == strName)
							bevelB = oNode;
						else if (_T("extrusionClr") == strName)
							extrusionClr.GetColorFrom(oNode);
						else if (_T("contourClr") == strName)
							contourClr.GetColorFrom(oNode);
					}
				}

				FillParentPointersForChilds();
			}


			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("contourW"), contourW);
				oAttr.Write(_T("extrusionH"), extrusionH);
				oAttr.WriteLimitNullable(_T("prstMaterial"), prstMaterial);
				oAttr.Write(_T("z"), z);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(bevelT);
				oValue.WriteNullable(bevelB);

				if (extrusionClr.is_init())
					oValue.Write(_T("a:extrusionClr"), extrusionClr);
				if (contourClr.is_init())
					oValue.Write(_T("a:contourClr"), contourClr);

				return XmlUtils::CreateNode(_T("a:sp3d"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:sp3d"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("contourW"), contourW);
				pWriter->WriteAttribute(_T("extrusionH"), extrusionH);
				pWriter->WriteAttribute(_T("prstMaterial"), prstMaterial);
				pWriter->WriteAttribute(_T("z"), z);
				pWriter->EndAttributes();
				
				pWriter->Write(bevelT);
				pWriter->Write(bevelB);

				if (extrusionClr.is_init())
				{
					pWriter->StartNode(_T("a:extrusionClr"));
					pWriter->EndAttributes();
					extrusionClr.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:extrusionClr"));					
				}
				if (contourClr.is_init())
				{
					pWriter->StartNode(_T("a:contourClr"));
					pWriter->EndAttributes();
					extrusionClr.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:contourClr"));	
				}

				pWriter->EndNode(_T("a:sp3d"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, contourW);
				pWriter->WriteInt2(1, extrusionH);
				pWriter->WriteLimit2(2, prstMaterial);
				pWriter->WriteInt2(3, z);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, bevelT);
				pWriter->WriteRecord2(1, bevelB);
				pWriter->WriteRecord1(2, extrusionClr);
				pWriter->WriteRecord1(3, contourClr);
			}

		public:
			nullable_int					contourW;
			nullable_int					extrusionH;
			nullable_limit<Limit::Material> prstMaterial;
			nullable_int					z;

			nullable<Bevel>					bevelT;
			nullable<Bevel>					bevelB;
			UniColor						extrusionClr;
			UniColor						contourClr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(bevelT.IsInit())
					bevelT->SetParentPointer(this);
				if(bevelB.IsInit())
					bevelB->SetParentPointer(this);
				extrusionClr.SetParentPointer(this);
				contourClr.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				contourW.normalize_positive();
				extrusionH.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SP3D_INCLUDE_H_