#pragma once
#ifndef UTILITY_UTILITY_INCLUDE_H_
#define UTILITY_UTILITY_INCLUDE_H_

#include "DateTime.h"
#include "Parse.h"
#include "ToString.h"
#include "Unit.h"
#include "Encoding.h"
#include "TxtFile.h"
#include <boost/foreach.hpp>
#include "UniversalUnit.h"
#include "FileNameUtility.h"


template<typename Out, typename In>
static const std::vector<Out> transform(const std::vector<In>& lines, const Out(*func)(const In&))
{
	std::vector<Out> result;
	BOOST_FOREACH(const In& line, lines)
	{
		result.push_back(func(line));
	}
	return result;
}


template<typename Out, typename In>
static const std::list<Out> transform(const std::list<In>& lines, const Out(*func)(const In&))
{
	std::list<Out> result;
	BOOST_FOREACH(const In& line, lines)
	{
		result.push_back(func(line));
	}
	return result;
}


template<typename Out, typename In, typename In2>
static const std::list<Out> transform2(const std::list<In>& lines, const int codepage, const Out(*func)(const In&, const In2 codePage))
{
	std::list<Out> result;
	BOOST_FOREACH(const In& line, lines)
	{
		result.push_back(func(line, codepage));
	}
	return result;
}
#endif // UTILITY_UTILITY_INCLUDE_H_