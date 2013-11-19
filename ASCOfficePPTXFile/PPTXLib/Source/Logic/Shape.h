#pragma once
#ifndef PPTX_LOGIC_SHAPE_INCLUDE_H_
#define PPTX_LOGIC_SHAPE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include <string>
#include "NvSpPr.h"
#include "SpPr.h"
#include "ShapeStyle.h"
#include "TxBody.h"
#include "ShapeProperties.h"
#include "UniFill.h"
#include "Ln.h"

#include "Encoding.h"

namespace PPTX
{
	namespace Logic
	{
		class Shape : public WrapperWritingElement
		{
		public:
			Shape();
			virtual ~Shape();			
			explicit Shape(const XML::XNode& node);
			const Shape& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			std::string GetText()const{if(txBody.is_init()) return txBody->GetText(); return ""; };
			std::wstring GetWText()const{if(txBody.is_init()) return txBody->GetWText(); return L""; };

			void GetShapeFullDescription(Shape& shape, int level = 0)const;
			void GetRect(RECT& pRect)const;
			DWORD GetFill(UniFill& fill)const;
			DWORD GetLine(Ln& line)const;

			void FillShapeProperties(ShapeProperties& props)const;

		private:
			void FillLevelUp()const;
			mutable Shape const * levelUp;
			void Merge(Shape& shape)const;
		public:
			void SetLevelUpElement(const Shape& p)const{levelUp = &p;};

		public:
			property<NvSpPr> nvSpPr;
			property<SpPr> spPr;
			nullable_property<ShapeStyle> style;
			nullable_property<TxBody> txBody;
 
			// Attributes
			nullable_property<bool> attrUseBgFill;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SHAPE_INCLUDE_H