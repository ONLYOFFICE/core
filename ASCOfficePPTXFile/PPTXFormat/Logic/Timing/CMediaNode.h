#pragma once
#ifndef PPTX_LOGIC_CMEDIANODE_INCLUDE_H_
#define PPTX_LOGIC_CMEDIANODE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CTn.h"
#include "TgtEl.h"

namespace PPTX
{
	namespace Logic
	{
		class CMediaNode : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(CMediaNode)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cTn			= node.ReadNode(_T("p:cTn"));
				tgtEl		= node.ReadNode(_T("p:tgtEl"));

				node.ReadAttributeBase(L"mute", mute);
				node.ReadAttributeBase(L"numSld", numSld);
				node.ReadAttributeBase(L"showWhenStopped", showWhenStopped);
				node.ReadAttributeBase(L"vol", vol);

				Normalize();

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("mute"), mute);
				oAttr.Write(_T("numSld"), numSld);
				oAttr.Write(_T("showWhenStopped"), showWhenStopped);
				oAttr.Write(_T("vol"), vol);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cTn);
				oValue.Write(tgtEl);

				return XmlUtils::CreateNode(_T("p:cMediaNode"), oAttr, oValue);
			}

		public:
			CTn						cTn;
			TgtEl					tgtEl;

			nullable_bool			mute;
			nullable_int			numSld;
			nullable_bool			showWhenStopped;
			nullable_int			vol;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cTn.SetParentPointer(this);
				tgtEl.SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				numSld.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CMEDIANODE_INCLUDE_H