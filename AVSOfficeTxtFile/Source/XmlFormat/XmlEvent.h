#pragma once
#ifndef XML_FORMAT_EVENT_INCLUDE_H_
#define XML_FORMAT_EVENT_INCLUDE_H_

namespace XmlFormat
{
	class IXmlEvent
	{
	public:
		IXmlEvent()
		{
			m_lPercent = 0;
		}
		virtual bool Progress(long ID, long Percent) = 0;
		virtual long GetPercent()
		{
			return m_lPercent;
		}
		virtual void AddPercent(long addition)
		{
			m_lPercent += addition;
		}
	protected:
		long m_lPercent;
	};
}
#endif // XML_FORMAT_EVENT_INCLUDE_H_