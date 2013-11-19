#pragma once
#ifndef PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_
#define PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/NvGrpSpPr.h"
#include "./../Logic/GrpSpPr.h"
#include "SpTreeElem.h"
#include "ShapeProperties.h"
#include "ShapeTextProperties.h"

namespace PPTX
{
	namespace Logic
	{

		class SpTree : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SpTree)

			SpTree& operator=(const SpTree& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				nvGrpSpPr	= oSrc.nvGrpSpPr;
				grpSpPr		= oSrc.grpSpPr;
				SpTreeElems.Copy(oSrc.SpTreeElems);

				m_name = oSrc.m_name;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();
				
				nvGrpSpPr	= node.ReadNodeNoNS(_T("nvGrpSpPr"));
				grpSpPr		= node.ReadNodeNoNS(_T("grpSpPr"));

				SpTreeElems.RemoveAll();

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("cNvPr") == strName)
						{
							nvGrpSpPr.cNvPr = oNode;
						}
						else if (_T("cNvGrpSpPr") == strName)
						{
							nvGrpSpPr.cNvGrpSpPr = oNode;
						}
						else
						{
							SpTreeElem elem(oNode);
							if (elem.is_init())
								SpTreeElems.Add(elem);
						}
					}
				}

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(nvGrpSpPr);
				oValue.Write(grpSpPr);
				oValue.WriteArray(SpTreeElems);

				return XmlUtils::CreateNode(m_name, oValue);
			}

			void toXmlWriterVML(NSBinPptxRW::CXmlWriter* pWriter, smart_ptr<PPTX::WrapperFile>& oTheme, smart_ptr<PPTX::WrapperWritingElement>& oClrMap);

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					if (pWriter->m_lGroupIndex == 0)
						pWriter->StartNode(_T("wpg:wgp"));
					else
						pWriter->StartNode(_T("wpg:grpSp"));
				}
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->StartNode(_T("xdr:grpSp"));
				else
					pWriter->StartNode(m_name);

				pWriter->EndAttributes();

				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					//nvGrpSpPr.cNvPr.toXmlWriter2(_T("wpg"), pWriter);
					nvGrpSpPr.cNvGrpSpPr.toXmlWriter2(_T("wpg"), pWriter);
				}
				else
					nvGrpSpPr.toXmlWriter(pWriter);
				
				grpSpPr.toXmlWriter(pWriter);
				
				pWriter->m_lGroupIndex++;

				size_t nCount = SpTreeElems.GetCount();
				for (size_t i = 0; i < nCount; ++i)
					SpTreeElems[i].toXmlWriter(pWriter);

				pWriter->m_lGroupIndex--;
				
				if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX)
				{
					if (pWriter->m_lGroupIndex == 0)
						pWriter->EndNode(_T("wpg:wgp"));
					else
						pWriter->EndNode(_T("wpg:grpSp"));
				}
				else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)
					pWriter->EndNode(_T("xdr:grpSp"));
				else
					pWriter->EndNode(m_name);
			}

			void NormalizeRect(RECT& rect)const
			{
				if(grpSpPr.xfrm.IsInit())
				{
					if( (grpSpPr.xfrm->chExtX.get_value_or(0) != 0) && (grpSpPr.xfrm->chExtY.get_value_or(0) != 0) )
					{
						double ScaleX = grpSpPr.xfrm->extX.get_value_or(0)/( double(grpSpPr.xfrm->chExtX.get()) );
						double ScaleY = grpSpPr.xfrm->extY.get_value_or(0)/( double(grpSpPr.xfrm->chExtY.get()) );
						double RectWidth = ScaleX * (rect.right - rect.left);
						double RectHeight = ScaleY * (rect.bottom - rect.top);
						rect.left	= (LONG)((rect.left - grpSpPr.xfrm->chOffX.get()) * ScaleX + grpSpPr.xfrm->offX.get());
						rect.top	= (LONG)((rect.top - grpSpPr.xfrm->chOffY.get()) * ScaleY + grpSpPr.xfrm->offY.get());
						rect.right	= (LONG)(rect.left + RectWidth);
						rect.bottom = (LONG)(rect.top + RectHeight);
					}
				}
				if(parentIs<Logic::SpTree>())
					parentAs<Logic::SpTree>().NormalizeRect(rect);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(SPTREE_TYPE_SPTREE);

				pWriter->WriteRecord1(0, nvGrpSpPr);
				pWriter->WriteRecord1(1, grpSpPr);
				pWriter->WriteRecordArray(2, 0, SpTreeElems);

				pWriter->EndRecord();
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(5); // type SPTREE + len

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							nvGrpSpPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							grpSpPr.fromPPTY(pReader);
							break;
						}
						case 2:
						{
							pReader->Skip(4); // len
							ULONG _c = pReader->GetULong();

							ULONG last = 0;
							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(5); // type (0) + len
								SpTreeElems.Add();
								SpTreeElems[last].fromPPTY(pReader);
								
								if (!SpTreeElems[last].is_init())
								{
									SpTreeElems.RemoveAt(last);									
								}
								else
								{
									++last;
								}
							}
						}
						default:
						{
							break;
						}
					}				
				}
				pReader->Seek(_end_rec);
			}

		public:
			Logic::NvGrpSpPr		nvGrpSpPr;
			Logic::GrpSpPr			grpSpPr;
			CAtlArray<SpTreeElem>	SpTreeElems;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				nvGrpSpPr.SetParentPointer(this);
				grpSpPr.SetParentPointer(this);

				size_t count = SpTreeElems.GetCount();
				for (size_t i = 0; i < count; ++i)
					SpTreeElems[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_SLIDES_SLIDE_SHAPETREE_INCLUDE_H_