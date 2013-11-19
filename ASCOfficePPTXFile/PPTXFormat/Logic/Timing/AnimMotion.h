#pragma once
#ifndef PPTX_LOGIC_ANIMMOTION_INCLUDE_H_
#define PPTX_LOGIC_ANIMMOTION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "./../../Limit/TLOrigin.h"
#include "./../../Limit/TLPathEditMode.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimMotion : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimMotion)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr = node.ReadNode(_T("p:cBhvr"));
				
				XmlUtils::CXmlNode oNodeBy;
				if (node.GetNode(_T("p:by"), oNodeBy))
				{
					oNodeBy.ReadAttributeBase(L"x", byX);
					oNodeBy.ReadAttributeBase(L"y", byY);
				}
				XmlUtils::CXmlNode oNodeFrom;
				if (node.GetNode(_T("p:from"), oNodeFrom))
				{
					oNodeFrom.ReadAttributeBase(L"x", fromX);
					oNodeFrom.ReadAttributeBase(L"y", fromY);
				}
				XmlUtils::CXmlNode oNodeTo;
				if (node.GetNode(_T("p:to"), oNodeTo))
				{
					oNodeTo.ReadAttributeBase(L"x", toX);
					oNodeTo.ReadAttributeBase(L"y", toY);
				}
				XmlUtils::CXmlNode oNodeCtr;
				if (node.GetNode(_T("p:rCtr"), oNodeCtr))
				{
					oNodeCtr.ReadAttributeBase(L"x", rCtrX);
					oNodeCtr.ReadAttributeBase(L"y", rCtrY);
				}

				node.ReadAttributeBase(L"path", path);
				node.ReadAttributeBase(L"ptsTypes", ptsTypes);
				node.ReadAttributeBase(L"rAng", rAng);
				node.ReadAttributeBase(L"origin", origin);
				node.ReadAttributeBase(L"pathEditMode", pathEditMode);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("origin"), origin);
				oAttr.Write(_T("path"), path);
				oAttr.WriteLimitNullable(_T("pathEditMode"), pathEditMode);
				oAttr.Write(_T("rAng"), rAng);
				oAttr.Write(_T("ptsTypes"), ptsTypes);

				XmlUtils::CNodeValue oValue;
				
				if (byX.IsInit() || byY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), byX);
					oAttr1.Write(_T("y"), byY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:by"), oAttr1);
				}
				if (fromY.IsInit() || fromY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), fromX);
					oAttr1.Write(_T("y"), fromY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:from"), oAttr1);
				}
				if (toX.IsInit() || toY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), toX);
					oAttr1.Write(_T("y"), toY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:to"), oAttr1);
				}
				if (rCtrX.IsInit() || rCtrY.IsInit())
				{
					XmlUtils::CAttribute oAttr1;
					oAttr1.Write(_T("x"), rCtrX);
					oAttr1.Write(_T("y"), rCtrY);

					oValue.m_strValue += XmlUtils::CreateNode(_T("p:rCtr"), oAttr1);
				}
				oValue.Write(cBhvr);

				return XmlUtils::CreateNode(_T("p:animMotion"), oAttr, oValue);
			}

		public:
			CBhvr					cBhvr;

			nullable_int			byX;
			nullable_int			byY;
			nullable_int			fromX;
			nullable_int			fromY;
			nullable_int			toX;
			nullable_int			toY;
			nullable_int			rCtrX;
			nullable_int			rCtrY;
//Attributes
			nullable_limit<Limit::TLOrigin>			origin; //ST_TLAnimateMotionBehaviorOrigin
			nullable_string							path;	//M = move to, L = line to, C = curve to, Z=close loop, E=end 
															//UPPERCASE = absolute coords, lowercase = relative coords 
															//Thus total allowed set = {M,L,C,Z,E,m,l,c,z,e) 
															//W3C XML Schema string 
			nullable_limit<Limit::TLPathEditMode>	pathEditMode;	//ST_TLAnimateMotionPathEditMode
			nullable_string							ptsTypes;		//A = Auto, F = Corner, T = Straight, S = Smooth 
																	//UPPERCASE = Straight Line follows point, lowercase = curve follows point. 
																	//Thus, the total allowed set = {A,F,T,S,a,f,t,s} 
																	//W3C XML Schema string 
			nullable_int			rAng;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMMOTION_INCLUDE_H