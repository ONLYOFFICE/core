#pragma once
#ifndef PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_
#define PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class TransitionBase : public WrapperWritingElement
		{
		public:
			TransitionBase();
			virtual ~TransitionBase();			
			explicit TransitionBase(XmlUtils::CXmlNode& node);
			const TransitionBase& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void GetTransitionTypeFrom(XmlUtils::CXmlNode& element);
			virtual bool is_init()const{return (base.IsInit());};
			
			template<class T> const bool is() const { return base.is<T>(); }
			template<class T> T& as()				{ return base.as<T>(); }
			template<class T> const T& as() const	{ return base.as<T>(); }

			virtual CString toXML() const;
		//public:
		private:
			smart_ptr<WrapperWritingElement> base;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					base->SetParentPointer(pParent);
			}
		};

		class TransitionSerialize : public WrapperWritingElement
		{
		public:
			TransitionSerialize()
			{
				m_strNodeName = _T("");
			}
			virtual ~TransitionSerialize()
			{
			}
			explicit TransitionSerialize(XmlUtils::CXmlNode& node);			
			const TransitionSerialize& operator =(XmlUtils::CXmlNode& node);

			TransitionSerialize& operator =(const TransitionSerialize& src)
			{
				// not impl
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void GetTransitionTypeFrom(XmlUtils::CXmlNode& element);

			virtual CString toXML() const;

			// PPTY format serialise
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
							m_strNodeName = pReader->GetString2();
							break;
						}
						case 1:
						{
							m_strAttributesNames.AddTail(pReader->GetString2());
							break;
						}
						case 2:
						{
							m_strAttributesValues.AddTail(pReader->GetString2());
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				pWriter->WriteString1(0, m_strNodeName);
				
				POSITION pos = m_strAttributesNames.GetHeadPosition();
				while (NULL != pos)
				{
					const CString& s = m_strAttributesNames.GetNext(pos);
					pWriter->WriteString1(1, s);
				}

				pos = m_strAttributesValues.GetHeadPosition();
				while (NULL != pos)
				{
					const CString& s = m_strAttributesValues.GetNext(pos);
					pWriter->WriteString1(2, s);
				}

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_strNodeName);				
				
				pWriter->StartAttributes();

				size_t c1 = m_strAttributesNames.GetCount();
				size_t c2 = m_strAttributesValues.GetCount();

				if (c1 == c2)
				{
					POSITION pos1 = m_strAttributesNames.GetHeadPosition();
					POSITION pos2 = m_strAttributesValues.GetHeadPosition();

					while (pos1 != NULL && pos2 != NULL)
					{
						const CString& s1 = m_strAttributesNames.GetNext(pos1);
						const CString& s2 = m_strAttributesValues.GetNext(pos2);

						pWriter->WriteAttribute(s1, s2);
					}
				}				
				
				pWriter->EndAttributes();

				pWriter->EndNode(m_strNodeName);
			}

			virtual void toXmlWriterOld(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString name = m_strNodeName;
				if ((name != _T("p:random")) &&
					(name != _T("p:circle")) &&
					(name != _T("p:dissolve")) &&
					(name != _T("p:diamond")) &&
					(name != _T("p:newsflash")) &&
					(name != _T("p:plus")) &&
					(name != _T("p:wedge")) &&
					(name != _T("p:blinds")) &&
					(name != _T("p:checker")) &&
					(name != _T("p:comb")) &&
					(name != _T("p:randomBar")) &&
					(name != _T("p:cover")) &&
					(name != _T("p:pull")) &&
					(name != _T("p:cut")) &&
					(name != _T("p:fade")) &&
					(name != _T("p:push")) &&
					(name != _T("p:wipe")) &&
					(name != _T("p:strips")) &&
					(name != _T("p:wheel")) &&
					(name != _T("p:split")) &&
					(name != _T("p:zoom")))
				{
					pWriter->WriteString(_T("<p:fade/>"));
					return;
				}

				pWriter->StartNode(name);				
				
				pWriter->StartAttributes();

				size_t c1 = m_strAttributesNames.GetCount();
				size_t c2 = m_strAttributesValues.GetCount();

				if (c1 == c2)
				{
					POSITION pos1 = m_strAttributesNames.GetHeadPosition();
					POSITION pos2 = m_strAttributesValues.GetHeadPosition();

					while (pos1 != NULL && pos2 != NULL)
					{
						const CString& s1 = m_strAttributesNames.GetNext(pos1);
						const CString& s2 = m_strAttributesValues.GetNext(pos2);

						pWriter->WriteAttribute(s1, s2);
					}
				}				
				
				pWriter->EndAttributes();

				pWriter->EndNode(name);
			}

		public:
			CString				m_strNodeName;
			CAtlList<CString>	m_strAttributesNames;
			CAtlList<CString>	m_strAttributesValues;

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H