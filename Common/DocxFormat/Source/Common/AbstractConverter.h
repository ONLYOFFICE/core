#pragma once
#ifndef ABSTRACT_CONVERTER_INCLUDE_H_
#define ABSTRACT_CONVERTER_INCLUDE_H_

#include <boost/utility.hpp>
#include <boost/filesystem.hpp>


template<class Input, class Output>
class AbstractConverter : private boost::noncopyable
{
public:
	AbstractConverter(const boost::filesystem::wpath& originPath)
	{
		m_origin.read(originPath);
	}

public:
	const bool isInputValid(const boost::filesystem::wpath& path) const
	{
		return m_input.isValid(path);
	}

	const bool isOriginValid(const boost::filesystem::wpath& path) const
	{
		return m_origin.isValid(path);
	}

	const bool isOutputValid(const boost::filesystem::wpath& path) const
	{
		return m_output.isValid(path);
	}

	void read(const boost::filesystem::wpath& path)
	{
		m_input.read(path);
	}

	void write(const boost::filesystem::wpath& path) const
	{
		m_output.write(path);
	}

protected:
	typedef AbstractConverter base;

protected:
	Input		m_input;
	Output	m_output;
	Output	m_origin;
};

#endif // ABSTRACT_CONVERTER_INCLUDE_H_