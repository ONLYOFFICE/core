#pragma once
#ifndef TXT_XML_EVENT_INCLUDE_H_
#define TXT_XML_EVENT_INCLUDE_H_

namespace TxtXml
{
	class ITxtXmlEvent
	{
	public:
		ITxtXmlEvent()
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
#endif // TXT_XML_EVENT_INCLUDE_H_