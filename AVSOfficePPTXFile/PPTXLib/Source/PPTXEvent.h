#pragma once
#ifndef PPTX_EVENT_INCLUDE_H_
#define PPTX_EVENT_INCLUDE_H_

namespace PPTX
{
	class IPPTXEvent
	{
	public:
		IPPTXEvent()
		{
			percent = 0;
		}
		virtual bool Progress(long ID, long Percent) = 0;
		virtual long GetPercent()
		{
			return percent;
		}
		virtual void AddPercent(long addition)
		{
			percent += addition;
		}
	protected:
		long percent;
	};
}
#endif // PPTX_EVENT_INCLUDE_H_