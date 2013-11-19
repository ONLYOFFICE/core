#pragma once
#ifndef PPTX_LOGIC_REFLECTION_INCLUDE_H_
#define PPTX_LOGIC_REFLECTION_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/RectAlign.h"

namespace PPTX
{
	namespace Logic
	{

		class Reflection : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Reflection)

			Reflection& operator=(const Reflection& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				algn	= oSrc.algn;
				blurRad	= oSrc.blurRad;
				stA		= oSrc.stA;
				endA	= oSrc.endA;
				stPos	= oSrc.stPos;
				endPos	= oSrc.endPos;
				dir		= oSrc.dir;
				fadeDir	= oSrc.fadeDir;
				dist	= oSrc.dist;
				kx		= oSrc.kx;
				ky		= oSrc.ky;
				rotWithShape = oSrc.rotWithShape;
				sx		= oSrc.sx;
				sy		= oSrc.sy;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"algn", algn);
				node.ReadAttributeBase(L"blurRad", blurRad);
				node.ReadAttributeBase(L"dir", dir);
				node.ReadAttributeBase(L"dist", dist);
				node.ReadAttributeBase(L"kx", kx);
				node.ReadAttributeBase(L"ky", ky);
				node.ReadAttributeBase(L"rotWithShape", rotWithShape);
				node.ReadAttributeBase(L"sx", sx);
				node.ReadAttributeBase(L"sy", sy);
				node.ReadAttributeBase(L"stA", stA);
				node.ReadAttributeBase(L"endA", endA);
				node.ReadAttributeBase(L"stPos", stPos);
				node.ReadAttributeBase(L"endPos", endPos);
				node.ReadAttributeBase(L"fadeDir", fadeDir);

				Normalize();
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("blurRad"), blurRad);
				oAttr.Write(_T("stA"), stA);
				oAttr.Write(_T("stPos"), stPos);
				oAttr.Write(_T("endA"), endA);
				oAttr.Write(_T("endPos"), endPos);
				oAttr.Write(_T("dist"), dist);
				oAttr.Write(_T("dir"), dir);
				oAttr.Write(_T("fadeDir"), fadeDir);
				oAttr.Write(_T("sx"), sx);
				oAttr.Write(_T("sy"), sy);
				oAttr.Write(_T("kx"), kx);
				oAttr.Write(_T("ky"), ky);
				oAttr.WriteLimitNullable(_T("algn"), algn);
				oAttr.Write(_T("rotWithShape"), rotWithShape);

				return XmlUtils::CreateNode(_T("a:reflection"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_REFLECTION);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, algn);
				pWriter->WriteInt2(1, blurRad);
				pWriter->WriteInt2(2, stA);
				pWriter->WriteInt2(3, endA);
				pWriter->WriteInt2(4, stPos);
				pWriter->WriteInt2(5, endPos);
				pWriter->WriteInt2(6, dir);
				pWriter->WriteInt2(7, fadeDir);
				pWriter->WriteInt2(8, dist);
				pWriter->WriteInt2(9, kx);
				pWriter->WriteInt2(10, ky);
				pWriter->WriteInt2(11, sx);
				pWriter->WriteInt2(12, sy);
				pWriter->WriteBool2(13, rotWithShape);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}

		public:
			nullable_limit<Limit::RectAlign>	algn;
			nullable_int						blurRad;
			nullable_int						stA;
			nullable_int						endA;
			nullable_int						stPos;
			nullable_int						endPos;
			nullable_int						dir;
			nullable_int						fadeDir;
			nullable_int						dist;
			nullable_int						kx;
			nullable_int						ky;
			nullable_bool						rotWithShape;
			nullable_int						sx;
			nullable_int						sy;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				blurRad.normalize_positive();
				stA.normalize_positive();
				endA.normalize_positive();
				stPos.normalize_positive();
				endPos.normalize_positive();
				dist.normalize_positive();

				dir.normalize(0, 2100000);
				fadeDir.normalize(0, 2100000);

				kx.normalize(-5400000, 5400000);
				ky.normalize(-5400000, 5400000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_REFLECTION_INCLUDE_H_