#pragma once
#ifndef PPTX_LOGIC_BULLET_INCLUDE_H_
#define PPTX_LOGIC_BULLET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "BuNone.h"
#include "BuChar.h"
#include "BuAutoNum.h"
#include "BuBlip.h"

namespace PPTX
{
	namespace Logic
	{
		class Bullet : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Bullet)

			Bullet& operator=(const Bullet& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				m_Bullet		= oSrc.m_Bullet;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString strName = XmlUtils::GetNameNoNS(node.GetName());
				
				if (strName == _T("buNone"))
					m_Bullet.reset(new Logic::BuNone(node));
				else if (strName == _T("buChar"))
					m_Bullet.reset(new Logic::BuChar(node));
				else if (strName == _T("buAutoNum"))
					m_Bullet.reset(new Logic::BuAutoNum(node));
				else if (strName == _T("buBlip"))
					m_Bullet.reset(new Logic::BuBlip(node));
				else m_Bullet.reset();
			}

			virtual void ReadBulletFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNode oNode;
				if (element.GetNode(_T("a:buNone"), oNode))
					m_Bullet.reset(new Logic::BuNone(oNode));
				else if (element.GetNode(_T("a:buChar"), oNode))
					m_Bullet.reset(new Logic::BuChar(oNode));
				else if (element.GetNode(_T("a:buAutoNum"), oNode))
					m_Bullet.reset(new Logic::BuAutoNum(oNode));
				else if (element.GetNode(_T("a:buBlip"), oNode))
					m_Bullet.reset(new Logic::BuBlip(oNode));
				else m_Bullet.reset();
			}

			virtual bool is_init()const{return (m_Bullet.IsInit());};
			virtual bool has_bullet()const{return ((is_init()) && (!is<BuNone>()));};
			
			template<class T> AVSINLINE const bool	is() const	{ return m_Bullet.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return m_Bullet.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return m_Bullet.as<T>(); }

            virtual CString toXML()const
			{
				if (m_Bullet.IsInit())
					return m_Bullet->toXML();
				return _T("");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (m_Bullet.is_init())
					m_Bullet->toXmlWriter(pWriter);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (m_Bullet.is_init())
					m_Bullet->toPPTY(pWriter);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;
				if (pReader->GetPos() == _end_rec)
					return;

				BYTE _type = pReader->GetUChar();

				switch (_type)
				{
				case BULLET_TYPE_BULLET_CHAR:
					{
						pReader->Skip(6);
						Logic::BuChar* p1 = new Logic::BuChar();
						p1->Char = pReader->GetString2();
						pReader->Skip(1);

						m_Bullet.reset(p1);
						break;
					}
				case BULLET_TYPE_BULLET_AUTONUM:
					{
						Logic::BuAutoNum* p2 = new Logic::BuAutoNum();
						pReader->Skip(5); // len + type + start attr

						while (true)
						{
							BYTE _at = pReader->GetUChar();
							if (_at == NSBinPptxRW::g_nodeAttributeEnd)
								break;

							switch (_at)
							{
								case 0:
								{
									p2->type.SetBYTECode(pReader->GetUChar());
									break;
								}
								case 1:
								{
									p2->startAt = pReader->GetLong();
									break;
								}
								default:
									break;
							}
						}

						m_Bullet.reset(p2);
						break;
					}
				case BULLET_TYPE_BULLET_BLIP:
					// TODO:
					break;
				default:
					m_Bullet.reset(new Logic::BuNone());
					break;
				}

				pReader->Seek(_end_rec);
			}

		//public:
		private:
			smart_ptr<WrapperWritingElement> m_Bullet;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual void SetParentPointer(const WrapperWritingElement* pParent)
			{
				if(is_init())
					m_Bullet->SetParentPointer(pParent);
			};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BULLET_INCLUDE_H
