#pragma once
#ifndef PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H_
#define PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class CNvGraphicFramePr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CNvGraphicFramePr)

			CNvGraphicFramePr& operator=(const CNvGraphicFramePr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				noChangeAspect	= oSrc.noChangeAspect;
				noDrilldown		= oSrc.noDrilldown;
				noGrp			= oSrc.noGrp;
				noMove			= oSrc.noMove;
				noResize		= oSrc.noResize;
				noSelect		= oSrc.noSelect;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode oNode;			
				if (node.GetNode(_T("a:graphicFrameLocks"), oNode))
				{
					oNode.ReadAttributeBase(L"noChangeAspect", noChangeAspect);
					oNode.ReadAttributeBase(L"noDrilldown", noDrilldown);
					oNode.ReadAttributeBase(L"noGrp", noGrp);
					oNode.ReadAttributeBase(L"noMove", noMove);
					oNode.ReadAttributeBase(L"noResize", noResize);
					oNode.ReadAttributeBase(L"noSelect", noSelect);
				}
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("noChangeAspect"), noChangeAspect);
				oAttr.Write(_T("noDrilldown"), noDrilldown);
				oAttr.Write(_T("noGrp"), noGrp);
				oAttr.Write(_T("noMove"), noMove);
				oAttr.Write(_T("noResize"), noResize);
				oAttr.Write(_T("noSelect"), noSelect);

				if (_T("") == oAttr.m_strValue)
					return _T("<p:cNvGraphicFramePr/>");

				return _T("<p:cNvGraphicFramePr>") + XmlUtils::CreateNode(_T("a:graphicFrameLocks"), oAttr) + _T("</p:cNvGraphicFramePr>");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:cNvGraphicFramePr"));
				else
					pWriter->StartNode(_T("p:cNvGraphicFramePr"));
				
				pWriter->EndAttributes();
				
				pWriter->StartNode(_T("a:graphicFrameLocks"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("noChangeAspect"), noChangeAspect);
				pWriter->WriteAttribute(_T("noDrilldown"), noDrilldown);
				pWriter->WriteAttribute(_T("noGrp"), noGrp);
				pWriter->WriteAttribute(_T("noMove"), noMove);
				pWriter->WriteAttribute(_T("noResize"), noResize);
				pWriter->WriteAttribute(_T("noSelect"), noSelect);

				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:graphicFrameLocks"));

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:cNvGraphicFramePr"));
				else
					pWriter->EndNode(_T("p:cNvGraphicFramePr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBool2(0, noChangeAspect);
				pWriter->WriteBool2(1, noDrilldown);
				pWriter->WriteBool2(2, noGrp);
				pWriter->WriteBool2(3, noMove);
				pWriter->WriteBool2(4, noResize);
				pWriter->WriteBool2(5, noSelect);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
						{
							noChangeAspect = pReader->GetBool();
							break;
						}
						case 1:
						{
							noDrilldown = pReader->GetBool();
							break;
						}
						case 2:
						{
							noGrp = pReader->GetBool();
							break;
						}
						case 3:
						{
							noMove = pReader->GetBool();
							break;
						}
						case 4:
						{
							noResize = pReader->GetBool();
							break;
						}
						case 5:
						{
							noSelect = pReader->GetBool();
							break;
						}						
						default:
							break;
					}
				}		

				pReader->Seek(_end_rec);
			}

		public:
			nullable_bool	noChangeAspect;
			nullable_bool	noDrilldown;
			nullable_bool	noGrp;
			nullable_bool	noMove;
			nullable_bool	noResize;
			nullable_bool	noSelect;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CNVGRAPHICFRAMESPPR_INCLUDE_H