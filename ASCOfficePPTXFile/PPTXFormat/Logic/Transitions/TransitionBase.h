#pragma once
#ifndef PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_
#define PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

#include <list>

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
				//проверка на самоприсваивание
				if (this == &src) {
					return *this;
				}
				m_strNodeName = src.m_strNodeName;
				m_strAttributesNames = src.m_strAttributesNames;
				m_strAttributesValues = src.m_strAttributesValues;
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
					BYTE _at = pReader->GetUChar_TypeNode();
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
							m_strAttributesNames.push_back(pReader->GetString2());
							break;
						}
						case 2:
						{
							m_strAttributesValues.push_back(pReader->GetString2());
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
				
				
				std::list<CString>::const_iterator pos1 = m_strAttributesNames.begin();
				while (pos1 != m_strAttributesNames.end())
				{
					const CString& s = *pos1;
					pWriter->WriteString1(1, s);
					pos1++;
				}

				std::list<CString>::const_iterator pos2 = m_strAttributesValues.begin();
				while (pos2 != m_strAttributesValues.end())
				{
					const CString& s = *pos2;;
					pWriter->WriteString1(2, s);
					pos2++;
				}

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_strNodeName);				
				
				pWriter->StartAttributes();

				//и нафига тогда листы?? - примем аксиомно что размеры одинаковые
				//size_t c1 = m_strAttributesNames.GetCount();
				//size_t c2 = m_strAttributesValues.GetCount();

				//if (c1 == c2)
				{
					std::list<CString>::const_iterator pos1 = m_strAttributesNames.begin();
					std::list<CString>::const_iterator pos2 = m_strAttributesValues.begin();

					while (pos1 != m_strAttributesNames.end() && pos2 != m_strAttributesValues.end())
					{
						const CString& s1 = *pos1;
						const CString& s2 = *pos2;

						pWriter->WriteAttribute(s1, s2);
						
						pos1++;
						pos2++;
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

				//size_t c1 = m_strAttributesNames.GetCount();
				//size_t c2 = m_strAttributesValues.GetCount();

				//if (c1 == c2)
				{
					std::list<CString>::const_iterator pos1 = m_strAttributesNames.begin();
					std::list<CString>::const_iterator pos2 = m_strAttributesValues.begin();

					while (pos1 != m_strAttributesNames.end() && pos2 != m_strAttributesValues.end())
					{
						const CString& s1 = *pos1;
						const CString& s2 = *pos2;

						pWriter->WriteAttribute(s1, s2);

						pos1++;
						pos2++;
					}
				}				
				
				pWriter->EndAttributes();

				pWriter->EndNode(name);
			}

		public:
			CString				m_strNodeName;
			std::list<CString>	m_strAttributesNames;
			std::list<CString>	m_strAttributesValues;

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TRANSITIONBASE_INCLUDE_H
