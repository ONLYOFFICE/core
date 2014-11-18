#pragma once
#ifndef PPTX_LOGIC_TRANSITION_INCLUDE_H_
#define PPTX_LOGIC_TRANSITION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/TransitionSpeed.h"
#include "SndAc.h"
#include "TransitionBase.h"

namespace PPTX
{
	namespace Logic
	{

		class Transition : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Transition)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"advClick", advClick);
				node.ReadAttributeBase(L"advTm", advTm);
				node.ReadAttributeBase(L"p14:dur", dur);	
				node.ReadAttributeBase(L"spd", spd);

				base.GetTransitionTypeFrom(node);
				trSerialize.GetTransitionTypeFrom(node);
				sndAc		= node.ReadNode(_T("p:sndAc"));
				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("advClick"), advClick);
				oAttr.Write(_T("advTm"), advTm);
				oAttr.WriteLimitNullable(_T("spd"), spd);
				oAttr.Write(_T("dur"), dur);

				XmlUtils::CNodeValue oValue;
				oValue.Write(base);
				oValue.WriteNullable(sndAc);

				return XmlUtils::CreateNode(_T("p:transition"), oAttr, oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (_T("") != trSerialize.m_strNodeName)
				{
					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

					pWriter->WriteBool2(0, advClick);
					pWriter->WriteInt2(1, advTm);
					pWriter->WriteInt2(2, dur);
					pWriter->WriteLimit2(3, spd);

					pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

					pWriter->WriteRecord1(0, trSerialize);
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetLong() + 4;
				if (pReader->GetPos() == end)
					return;

				pReader->Skip(1); // attribute start
				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						advClick = pReader->GetBool();
					else if (1 == _at)
						advTm = pReader->GetLong();
					else if (2 == _at)
						dur = pReader->GetLong();
					else if (3 == _at)
					{
						spd = new Limit::TransitionSpeed();
						spd->SetBYTECode(pReader->GetUChar());
					}
				}

				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							trSerialize.fromPPTY(pReader);
							break;
						}						
						default:
						{
							pReader->SkipRecord();
							break;
						}
					}
				}

				pReader->Seek(end);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (_T("") == trSerialize.m_strNodeName)
					return;

				pWriter->WriteString(_T("<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\">\
<mc:Choice xmlns:p14=\"http://schemas.microsoft.com/office/powerpoint/2010/main\" Requires=\"p14\">"));

				pWriter->StartNode(_T("p:transition"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("spd"), spd);
				pWriter->WriteAttribute(_T("p14:dur"), dur);
				pWriter->WriteAttribute(_T("advClick"), advClick);
				pWriter->WriteAttribute(_T("advTm"), advTm);
				pWriter->EndAttributes();

				if (trSerialize.m_strNodeName != _T(""))
					trSerialize.toXmlWriter(pWriter);

				pWriter->EndNode(_T("p:transition"));

				pWriter->WriteString(_T("</mc:Choice><mc:Fallback>"));

				pWriter->StartNode(_T("p:transition"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("spd"), spd);
				pWriter->WriteAttribute(_T("advClick"), advClick);
				pWriter->WriteAttribute(_T("advTm"), advTm);
				pWriter->EndAttributes();

				if (trSerialize.m_strNodeName != _T(""))
					trSerialize.toXmlWriterOld(pWriter);

				pWriter->EndNode(_T("p:transition"));

				pWriter->WriteString(_T("</mc:Fallback></mc:AlternateContent>"));
			}

		public:
			/*nullable<*/TransitionBase/*>*/		base;
			nullable<SndAc>							sndAc;

			TransitionSerialize						trSerialize;

			nullable_bool advClick;
			nullable_int advTm;
			nullable_int dur;
			nullable_limit<Limit::TransitionSpeed> spd;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(base.is_init())
					base.SetParentPointer(this);
				if(sndAc.IsInit())
					sndAc->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				advTm.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TRANSITION_INCLUDE_H_