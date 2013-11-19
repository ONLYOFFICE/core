#pragma once
#ifndef PPTX_LOGIC_TXBODY_INCLUDE_H_
#define PPTX_LOGIC_TXBODY_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "Paragraph.h"

namespace PPTX
{
	namespace Logic
	{
		class TxBody : public WrapperWritingElement
		{
		public:
			TxBody()	
			{
				m_ns = _T("p");
			}
			virtual ~TxBody() {}
			explicit TxBody(XmlUtils::CXmlNode& node)	{ fromXML(node); }
			const TxBody& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}
			TxBody(const TxBody& oSrc) { *this = oSrc; }

			TxBody& operator=(const TxBody& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				bodyPr			= oSrc.bodyPr;
				lstStyle		= oSrc.lstStyle;
				Paragrs.Copy(oSrc.Paragrs);

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Paragrs.RemoveAll();

				bodyPr		= node.ReadNode(_T("a:bodyPr"));
				lstStyle	= node.ReadNode(_T("a:lstStyle"));

				node.LoadArray(_T("a:p"), Paragrs);

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(bodyPr);
				oValue.WriteNullable(lstStyle);
				oValue.WriteArray(Paragrs);

				return XmlUtils::CreateNode(_T("p:txBody"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_ns + _T(":txBody"));
				pWriter->EndAttributes();

				bodyPr.m_namespace = _T("a");
				bodyPr.toXmlWriter(pWriter);

				if (lstStyle.is_init())
					lstStyle->m_name = _T("a:lstStyle");
				pWriter->Write(lstStyle);
				
				size_t nCount = Paragrs.GetCount();
				for (size_t i = 0; i < nCount; ++i)
					Paragrs[i].toXmlWriter(pWriter);
				
				pWriter->EndNode(m_ns + _T(":txBody"));
			}

			void toXmlWriterExcel(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				/*
				pWriter->StartNode(_T("c:rich"));
				pWriter->EndAttributes();
				*/

				bodyPr.m_namespace = _T("a");
				bodyPr.toXmlWriter(pWriter);

				if (lstStyle.is_init())
					lstStyle->m_name = _T("a:lstStyle");
				pWriter->Write(lstStyle);
				
				size_t nCount = Paragrs.GetCount();
				for (size_t i = 0; i < nCount; ++i)
					Paragrs[i].toXmlWriter(pWriter);
				
				/*
				pWriter->EndNode(_T("c:rich"));
				*/
			}

			CString GetText()const
			{
				CString result = _T("");
				size_t count = Paragrs.GetCount();

				for (size_t i = 0; i < count; ++i)
					result += Paragrs[i].GetText();
				return result;
			}

			void Merge(nullable<TxBody>& txBody)const
			{
				if(!txBody.is_init())
					txBody = new TxBody();
				bodyPr.Merge(txBody->bodyPr);
				if(lstStyle.is_init())
					lstStyle->Merge(txBody->lstStyle);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, bodyPr);
				pWriter->WriteRecord2(1, lstStyle);
				pWriter->WriteRecordArray(2, 0, Paragrs);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							bodyPr.fromPPTY(pReader);
							break;
						}
						case 1:
						{
							lstStyle = new Logic::TextListStyle();
							lstStyle->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							pReader->Skip(4);
							ULONG _c = pReader->GetULong();
							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								Paragrs.Add();
								Paragrs[Paragrs.GetCount() - 1].fromPPTY(pReader);								
							}
							break;
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
			BodyPr					bodyPr;
			nullable<TextListStyle> lstStyle;
			CAtlArray<Paragraph>	Paragrs;

			CString m_ns;
		protected:
			virtual void FillParentPointersForChilds()
			{
				bodyPr.SetParentPointer(this);
				if(lstStyle.is_init())
					lstStyle->SetParentPointer(this);
				
				size_t count = Paragrs.GetCount();
				for (size_t i = 0; i < count; ++i)
					Paragrs[i].SetParentPointer(this);
			}

		public:
			bool IsOneLineParagraphs() const
			{
				if (!bodyPr.wrap.is_init())
					return false;
				return (bodyPr.wrap->get() == _T("none"));
			}

			CString GetDocxTxBoxContent(NSBinPptxRW::CBinaryFileWriter* pWriter);
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXBODY_INCLUDE_H