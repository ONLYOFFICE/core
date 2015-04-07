#pragma once
#ifndef UTILITY_UTILITY_INCLUDE_H_
#define UTILITY_UTILITY_INCLUDE_H_

#include "DateTime.h"
#include "Unit.h"
#include "Encoding.h"

#ifdef _HTML_FILE

#include "TxtFile.h"
#include <string>
#include <vector>
#include <list>
#include <boost/foreach.hpp>

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

#endif // _HTML_FILE

#endif // UTILITY_UTILITY_INCLUDE_H_