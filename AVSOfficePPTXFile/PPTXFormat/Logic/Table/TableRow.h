#pragma once
#ifndef PPTX_LOGIC_TABLEROW_INCLUDE_H_
#define PPTX_LOGIC_TABLEROW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "TableCell.h"

namespace PPTX
{
	namespace Logic
	{
		class TableRow : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(TableRow)

			TableRow& operator=(const TableRow& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Height = oSrc.Height;
				Cells.Copy(oSrc.Cells);

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Height = node.ReadAttributeInt(L"h");
				node.LoadArray(_T("a:tc"), Cells);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("h"), Height);

				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(Cells);

				return XmlUtils::CreateNode(_T("a:tr"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tr"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"h", Height);
				pWriter->EndAttributes();

				size_t len = Cells.GetCount();
				for (size_t i = 0; i < len; ++i)
					Cells[i].toXmlWriter(pWriter);

				pWriter->EndNode(_T("a:tr"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, Height);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecordArray(0, 1, Cells);
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
							Height = pReader->GetLong();
							break;
						}
						default:
							break;
					}
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							pReader->Skip(4);
							LONG len = pReader->GetLong();
							for (LONG i = 0; i < len; ++i)
							{
								pReader->Skip(1);
								Cells.Add();
								Cells[i].fromPPTY(pReader);
							}
							break;
						}
						default:
							break;
					}
				}				

				pReader->Seek(_end_rec);
			}
			
		public:
			int							Height;
			CAtlArray<TableCell>		Cells;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = Cells.GetCount();
				for (size_t i = 0; i < count; ++i)
					Cells[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLEROW_INCLUDE_H_