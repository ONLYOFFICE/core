#pragma once
#ifndef DOCX_2_ODT_PRIVATE_CONVERTER_PROPERTIES_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_CONVERTER_PROPERTIES_INCLUDE_H_
#include "property.h"
#include "nullable_property.h"
#include <string>
#include "UniversalUnit.h"
#include "Logic/RunProperty.h"

namespace Docx2Odt
{
	namespace Private
	{
		class ConverterProperties
		{
		public:
			ConverterProperties();

		public:
			void setDestStyleIsContent(const bool flag);
			const bool getDestStyleIsContent() const;

			//для буквицы
			property<int> Lines;
			property<UniversalUnit> Distance;
			property<std::string> Bukvitza;
			nullable_property<OOX::Logic::RunProperty> RunPropertyBukvitza;
			property<bool> hasBukvitza;

		private:
			bool	addStyleToContent;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_CONVERTER_PROPERTIES_INCLUDE_H_