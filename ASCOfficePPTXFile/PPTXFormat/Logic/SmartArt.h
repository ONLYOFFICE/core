#pragma once
#ifndef PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_
#define PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_

#include "./SpTree.h"

namespace PPTX
{
	namespace Logic
	{
		class SmartArt : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SmartArt)

			SmartArt& operator=(const SmartArt& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				diag = oSrc.diag;
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"r:dm", id_data);
				FillParentPointersForChilds();
			}


			virtual CString toXML() const
			{
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{				
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (diag.is_init())
				{
					smart_ptr<PPTX::FileContainer> old = pWriter->m_pCommonRels;
					pWriter->m_pCommonRels = m_oCommonRels;
					diag->toPPTY(pWriter);
					pWriter->m_pCommonRels = old;
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->SkipRecord();
			}

		public:

			nullable<PPTX::RId> id_data;

			nullable<PPTX::Logic::SpTree> diag;
			smart_ptr<FileContainer> m_oCommonRels;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(diag.IsInit())
					diag->SetParentPointer(this);
			}

		public:
			void LoadDrawing(NSBinPptxRW::CBinaryFileWriter* pWriter);
		};

		class ChartRec : public WrapperWritingElement
		{
		public:
			ChartRec()	
			{
				m_pData = NULL;
				m_lChartNumber = 0;
			}
			virtual ~ChartRec() 
			{
				RELEASEARRAY(m_pData);
			}
			explicit ChartRec(XmlUtils::CXmlNode& node)	
			{ 
				fromXML(node); 
			}
			const ChartRec& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}
			ChartRec(const ChartRec& oSrc) 
			{ 
				*this = oSrc; 
			}

			ChartRec& operator=(const ChartRec& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_pData = NULL;

				node.ReadAttributeBase(L"r:id", id_data);
				FillParentPointersForChilds();
			}


			virtual CString toXML() const
			{
				return _T("");
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		public:

			nullable<PPTX::RId> id_data;

			LONG m_lChartNumber;
			LPSAFEARRAY m_pData;
		protected:
			virtual void FillParentPointersForChilds()
			{				
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SLIDE_SMARTART_INCLUDE_H_
