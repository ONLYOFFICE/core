#pragma once

#include "../../../../Common/DocxFormat/Source/XML/stringcommon.h"

#include "../Common/Encoding.h"

template<typename Out, typename In>
static const std::vector<Out> _transform(const std::vector<In>& lines, const Out(*func)(const In&))
{
	std::vector<Out> result;
    for (typename std::vector<In>::const_iterator iter = lines.begin(); iter != lines.end(); ++iter)
	{
		result.push_back(func(*iter));
	}

	return result;
}

template<typename Out, typename In>
static const std::list<Out> _transform(const std::list<In>& lines, const Out(*func)(const In&))
{
	std::list<Out> result;
    for (typename std::list<In>::const_iterator iter = lines.begin(); iter != lines.end(); ++iter)
	{
		result.push_back(func(*iter));
	}
	return result;
}

template<typename Out, typename In, typename In2>
static const std::list<Out> _transform2(const std::list<In>& lines, const int codepage, const Out(*func)(const In&, const In2 codePage))
{
    std::list<Out> result;
    for (typename std::list<In>::const_iterator iter = lines.begin(); iter != lines.end(); ++iter)
	{
		result.push_back(func(*iter, codepage));
	}
	return result;
}

