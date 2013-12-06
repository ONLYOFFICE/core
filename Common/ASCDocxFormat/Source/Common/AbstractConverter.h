#pragma once
#ifndef ABSTRACT_CONVERTER_INCLUDE_H_
#define ABSTRACT_CONVERTER_INCLUDE_H_

template<class Input, class Output>
class AbstractConverter 
{
public:
	AbstractConverter(const OOX::CPath& originPath)
	{
		m_origin.read(originPath);
	}

public:
	const bool isInputValid(const OOX::CPath& path) const
	{
		return m_input.isValid(path);
	}

	const bool isOriginValid(const OOX::CPath& path) const
	{
		return m_origin.isValid(path);
	}

	const bool isOutputValid(const OOX::CPath& path) const
	{
		return m_output.isValid(path);
	}

	void read(const OOX::CPath& path)
	{
		m_input.read(path);
	}

	void write(const OOX::CPath& path) const
	{
		m_output.write(path);
	}

protected:
	typedef AbstractConverter base;

protected:
	Input	m_input;
	Output	m_output;
	Output	m_origin;
};

#endif // ABSTRACT_CONVERTER_INCLUDE_H_