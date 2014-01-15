
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BorderValue.h"


namespace OOX
{
	namespace Limit
	{
		BorderValueSet::BorderValueSet()
		{
			_list.insert("single");
			_list.insert("none");
			_list.insert("nil");
			_list.insert("candyCorn");
			_list.insert("dashed");
			_list.insert("dotDash");
			_list.insert("dotDotDash");			
			_list.insert("double");
			_list.insert("triple");
			_list.insert("thinThickSmallGap");
			_list.insert("thickThinSmallGap");
			_list.insert("thinThickThinSmallGap");
			_list.insert("thinThickMediumGap");
			_list.insert("thickThinMediumGap");
			_list.insert("thinThickThinMediumGap");
			_list.insert("thinThickLargeGap");
			_list.insert("thickThinLargeGap");
			_list.insert("thinThickThinLargeGap");
			_list.insert("wave");
			_list.insert("doubleWave");
			_list.insert("dashDotStroked");
			_list.insert("threeDEmboss");
			_list.insert("threeDEngrave");
			_list.insert("outset");
			_list.insert("inset");
		}

		BorderValue::BorderValue()
		{
			//add("single");
			//add("none");
			//add("nil");
			//add("candyCorn");
			//add("dashed");
			//add("dotDash");
			//add("dotDotDash");			
			//add("double");
			//add("triple");
			//add("thinThickSmallGap");
			//add("thickThinSmallGap");
			//add("thinThickThinSmallGap");
			//add("thinThickMediumGap");
			//add("thickThinMediumGap");
			//add("thinThickThinMediumGap");
			//add("thinThickLargeGap");
			//add("thickThinLargeGap");
			//add("thinThickThinLargeGap");
			//add("wave");
			//add("doubleWave");
			//add("dashDotStroked");
			//add("threeDEmboss");
			//add("threeDEngrave");
			//add("outset");
			//add("inset");
		}
		BorderValueSet BorderValue::_set;

		const std::string BorderValue::no_find() const
		{
			return "none";
		}

		void BorderValue::operator()(std::string& _value, Parameter value)
		{
			if (_set._list.find(value) != _set._list.end())
				_value = value;
			else
				_value = no_find();
		}

	} // namespace Limit
} // namespace OOX