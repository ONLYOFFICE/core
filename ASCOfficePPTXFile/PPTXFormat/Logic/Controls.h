#pragma once
#ifndef PPTX_LOGIC_ACTIVEX_CONTROL_INCLUDE_H_
#define PPTX_LOGIC_ACTIVEX_CONTROL_INCLUDE_H_

#include "SpTreeElem.h"

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"
#include "Pic.h"

namespace PPTX
{
	namespace Logic
	{
		//19.3.2.1 control (Embedded Control)
		class Control : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Control)

		public:
			void AddObjectTo (const std::vector<SpTreeElem> *spTreeElements, NSShapeImageGen::CImageManager* pImageManager) const ;
			CString GetVmlXmlBySpid(CString spid, CString & rels) const ;
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const ;

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name	= node.GetAttribute(_T("name"));
				spid	= node.GetAttribute(_T("spid"));
				
				node.ReadAttributeBase(L"rId"		, rId);
				node.ReadAttributeBase(L"width"		, width);
				node.ReadAttributeBase(L"height"	, height);
				node.ReadAttributeBase(L"showAsIcon", showAsIcon);				

				pic = node.ReadNode(_T("p:pic"));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"),		name);
				
				if (rId.IsInit())	oAttr.Write(_T("rId"),		rId->ToString());
				if (width.IsInit())	oAttr.Write(_T("width"),	width->ToString());
				if (height.IsInit())oAttr.Write(_T("height"),	height->ToString());

				oAttr.Write(_T("spid"),		spid);

				return XmlUtils::CreateNode(_T("p:control"), oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:control"));

				pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("name")		, name);
					pWriter->WriteAttribute(_T("showAsIcon"), showAsIcon);
					pWriter->WriteAttribute(_T("spid")		, spid);
					
					if (rId.IsInit())  pWriter->WriteAttribute(_T("rId"), rId->ToString());
					if (width.IsInit())pWriter->WriteAttribute(_T("width"), width->ToString());
					if (height.IsInit())pWriter->WriteAttribute(_T("height"), height->ToString());
				pWriter->EndAttributes();

				pWriter->Write(pic);

				pWriter->EndNode(_T("p:control"));
			}

			nullable_string									name;
			nullable_string									spid;
			nullable_bool									showAsIcon;
			nullable<SimpleTypes::CPositiveCoordinate<0>>	height;
			nullable<SimpleTypes::CPositiveCoordinate<0>>	width;
			nullable<PPTX::RId>								rId;

			nullable<PPTX::Logic::Pic>						pic;
/// todooo
//extLst (Extension List) §19.2.1.12
		protected:			

			virtual void FillParentPointersForChilds()
			{
			}
		};
		
		// 19.3.1.15 controls (List of controls)
		class Controls : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Controls)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				arrControls.clear();

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (strName == _T("control"))
						{
							Control elem(oNode);
							arrControls.push_back(elem);
						}
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;

				oValue.WriteArray(arrControls);				

				return XmlUtils::CreateNode(_T("p:controls"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:controls"));

				for (size_t i = 0; i < arrControls.size(); ++i)
					arrControls[i].toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:controls"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const 
			{
				for (int i = 0; i <arrControls.size(); i++)
					arrControls[i].toPPTY(pWriter);
			}	
			void AddObjectsTo (const std::vector<PPTX::Logic::SpTreeElem> *spTreeElements, NSShapeImageGen::CImageManager* pImageManager) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
			}

		public:
			std::vector<Control>	arrControls;

		protected:
			virtual void FillParentPointersForChilds()
			{
				for (int i = 0; i <arrControls.size(); i++)
					arrControls[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ACTIVEX_CONTROLS_INCLUDE_H_