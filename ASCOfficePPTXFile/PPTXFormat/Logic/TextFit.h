#pragma once
#ifndef PPTX_LOGIC_TEXTFIT_INCLUDE_H_
#define PPTX_LOGIC_TEXTFIT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class TextFit : public WrapperWritingElement
		{
		public:
			enum eFit {FitEmpty = 0, FitNo = 1, FitSpAuto = 2, FitNormAuto = 3};

			PPTX_LOGIC_BASE(TextFit)

			virtual bool is_init(){return (type != FitEmpty);};

		public:
			void GetTextFitFrom(XmlUtils::CXmlNode& element)
			{
				type = FitEmpty;

				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:noAutofit"), oNode))
					type = FitNo;
				else if (element.GetNode(_T("a:spAutoFit"), oNode))
					type = FitSpAuto;
				else if (element.GetNode(_T("a:normAutofit"), oNode))
				{
					type = FitNormAuto;

					nullable_string sFontScale;
					nullable_string sLnSpcRed;

					oNode.ReadAttributeBase(L"fontScale", sFontScale);
					oNode.ReadAttributeBase(L"lnSpcReduction", sLnSpcRed);

					if (sFontScale.is_init())
					{
						int nFound = sFontScale->ReverseFind(TCHAR('%'));
						if (nFound < 0)
							fontScale = *sFontScale;
						else
						{
							CString sRet = sFontScale->Mid(0, nFound);
							double dRet = XmlUtils::GetDouble(sRet);
							int val = (int)(dRet * 1000);
							fontScale = val;
						}
					}

					if (sLnSpcRed.is_init())
					{
						int nFound = sLnSpcRed->ReverseFind(TCHAR('%'));
						if (nFound < 0)
							lnSpcReduction = *sLnSpcRed;
						else
						{
							CString sRet = sLnSpcRed->Mid(0, nFound);
							double dRet = XmlUtils::GetDouble(sRet);
							int val = (int)(dRet * 1000);
							lnSpcReduction = val;
						}
					}
				}
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				type = FitEmpty;

				CString strName = XmlUtils::GetNameNoNS(node.GetName());

				if (_T("a:noAutofit") == strName)
					type = FitNo;
				else if (_T("a:spAutoFit") == strName)
					type = FitSpAuto;
				else if (_T("a:normAutofit") == strName)
				{
					type = FitNormAuto;

					nullable_string sFontScale;
					nullable_string sLnSpcRed;

					node.ReadAttributeBase(L"fontScale", sFontScale);
					node.ReadAttributeBase(L"lnSpcReduction", sLnSpcRed);

					if (sFontScale.is_init())
					{
						int nFound = sFontScale->ReverseFind(TCHAR('%'));
						if (nFound < 0)
							fontScale = *sFontScale;
						else
						{
							CString sRet = sFontScale->Mid(0, nFound);
							double dRet = XmlUtils::GetDouble(sRet);
							int val = (int)(dRet * 1000);
							fontScale = val;
						}
					}

					if (sLnSpcRed.is_init())
					{
						int nFound = sLnSpcRed->ReverseFind(TCHAR('%'));
						if (nFound < 0)
							lnSpcReduction = *sLnSpcRed;
						else
						{
							CString sRet = sLnSpcRed->Mid(0, nFound);
							double dRet = XmlUtils::GetDouble(sRet);
							int val = (int)(dRet * 1000);
							lnSpcReduction = val;
						}
					}
				}
			}
			virtual CString toXML() const
			{
				if (type == FitNo)
					return _T("<a:noAutofit/>");

				if (type == FitSpAuto)
					return _T("<a:spAutoFit/>");

				if (type == FitNormAuto)
				{
					XmlUtils::CAttribute oAttr;
					oAttr.Write(_T("fontScale"), fontScale);
					oAttr.Write(_T("lnSpcReduction"), lnSpcReduction);

					return XmlUtils::CreateNode(_T("a:normAutofit"), oAttr);
				}

				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (type == FitNo)
				{
					pWriter->WriteString(_T("<a:noAutofit/>"));
					return;
				}

				if (type == FitSpAuto)
				{
					pWriter->WriteString(_T("<a:spAutoFit/>"));
					return;
				}

				if (type == FitNormAuto)
				{
					pWriter->StartNode(_T("a:normAutofit"));

					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("fontScale"), fontScale);
					pWriter->WriteAttribute(_T("lnSpcReduction"), lnSpcReduction);
					pWriter->EndAttributes();

					pWriter->EndNode(_T("a:normAutofit"));
				}				
			}

			void Merge(TextFit& fit)const
			{
				if(type != FitEmpty)
				{
					if((type == FitNo) || (type == FitSpAuto))
					{
						fit.type = type;
						fit.fontScale.reset();
						fit.lnSpcReduction.reset();
					}
					else if(type == FitNormAuto)
					{
						fit.type = type;
						if(fontScale.is_init())
							fit.fontScale = *fontScale;
						if(lnSpcReduction.is_init())
							fit.lnSpcReduction = *lnSpcReduction;
					}
				}
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, type);
				pWriter->WriteInt2(1, fontScale);
				pWriter->WriteInt2(2, lnSpcReduction);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 0:
					{
						type = (eFit)pReader->GetLong();
						break;
					}
					case 1:
					{
						fontScale = pReader->GetLong();
						break;
					}
					case 2:
					{
						lnSpcReduction = pReader->GetLong();
						break;
					}
					default:
						break;
					}
				}

				pReader->Seek(_end_rec);
			}
		public:
			eFit			type;
			nullable_int	fontScale;
			nullable_int	lnSpcReduction;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTFIT_INCLUDE_H