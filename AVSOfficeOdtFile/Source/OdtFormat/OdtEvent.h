#pragma once
#ifndef ODT_EVENT_INCLUDE_H_
#define ODT_EVENT_INCLUDE_H_

namespace Odt
{
	class IOdtEvent
	{
	public:
		IOdtEvent()
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
#endif // ODT_EVENT_INCLUDE_H_