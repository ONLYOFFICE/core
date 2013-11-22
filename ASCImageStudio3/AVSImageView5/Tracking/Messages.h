#pragma once

namespace Messages
{
	const int c_nMessageNone			= 0;
	const int c_nMessageGetFrameImage	= 3101;
	const int c_nMessageGetSamplesAudio	= 3102;
	const int c_nMessageGetVideoInfo	= 3103;
	const int c_nMessageSetProgress		= 3104;
	const int c_nMessageAddChapter		= 3105;

	// универсальный класс для передачи сообщений между другими классами
	// при необходимости можно переделать/расширить типы передаваемых данных
	class CMessage
	{
	public:
		
		const static int MessageNone = 0;
		
		const static int MessageRefresh = 1101;
		const static int MessageHScroll = 2101;
		const static int MessageVScroll = 2201;
		const static int MessageMouseDown = 2301;
		const static int MessageMouseUp = 2302;
		const static int MessageMouseMove = 2303;
		
	protected:
		
		void* m_pReceiver;
		
		int m_nType;
		int m_nValues[4];
		double m_dValues[4];
		void* m_pValues[4];
		
	public:
	
		CMessage()
		{
			Clear();
		}
		CMessage(void* pReceiver, int nType)
		{
			Clear();
			Create(pReceiver, nType);
		}
		
		void Create(void* pReceiver, int nType)
		{
			m_pReceiver = pReceiver;
			m_nType = nType;
		}
		void Clear()
		{
			m_pReceiver = 0;

			m_nType = MessageNone;

			for (int index = 0; index < 4; ++index)
			{
				m_nValues[index] = 0;
				m_dValues[index] = 0;
				m_pValues[index] = 0;
			}
		}
		
		void* GetReceiver()
		{
			return m_pReceiver;
		}
		int GetType()
		{
			return m_nType;
		}
		int* GetIntegerValues()
		{
			return m_nValues;
		}
		double* GetDoubleValues()
		{
			return m_dValues;
		}
		void** GetPointerValues()
		{
			return m_pValues;
		}
	};
	
	// класс для приема сообщений
	class CMessageReceiver
	{
	public:

		virtual void ReceiveMessage(CMessage* pMessage) = 0;
	};
	
	// класс для сообщений, которые передаются напрямую в класс приемник: без использования RPC или Window сообщений
	class CDirectMessage : public CMessage
	{
	public:

		static BOOL Send(CMessage* pMessage)
		{
			if (!pMessage)
				return FALSE;

			CMessageReceiver* pReceiver = (CMessageReceiver*)(pMessage->GetReceiver());
			if (!pReceiver)
				return FALSE;

			pReceiver->ReceiveMessage(pMessage);

			return TRUE;
		}
		static BOOL Send(CMessageReceiver* pReceiver, int nType)
		{
			CMessage msg(pReceiver, nType);
			
			return Send(&msg);
		}
		static BOOL Send(CMessageReceiver* pReceiver, int nType, int nValue, int nValue1 = 0, int nValue2 = 0, int nValue3 = 0)
		{
			CMessage msg(pReceiver, nType);
			
			msg.GetIntegerValues()[0] = nValue;
			msg.GetIntegerValues()[1] = nValue1;
			msg.GetIntegerValues()[2] = nValue2;
			msg.GetIntegerValues()[3] = nValue3;

			return Send(&msg);
		}
		static BOOL Send(CMessageReceiver* pReceiver, int nType, void* pData)
		{
			CMessage msg(pReceiver, nType);
			
			msg.GetPointerValues()[0] = pData;

			return Send(&msg);
		}
	};
	
	// класс для сообщений, которые пересылаются через Window сообщения: используется SendMessage(hwnd)
	class CWindowMessage : public CMessage
	{
	public:
	
		static BOOL Send(CMessage* pMessage)
		{
			if (!pMessage)
				return FALSE;

			HWND hWnd = (HWND)(pMessage->GetReceiver());
			if (!hWnd)
				return FALSE;

			SendMessage(hWnd, WM_USER, 0, (LPARAM)pMessage);

			return TRUE;
		}
		static BOOL Send(HWND hWnd, int nType)
		{
			CMessage msg(hWnd, nType);
			
			return Send(&msg);
		}
		static BOOL Send(HWND hWnd, int nType, int nValue, int nValue1 = 0, int nValue2 = 0, int nValue3 = 0)
		{
			CMessage msg((void*)hWnd, nType);
			
			msg.GetIntegerValues()[0] = nValue;
			msg.GetIntegerValues()[1] = nValue1;
			msg.GetIntegerValues()[2] = nValue2;
			msg.GetIntegerValues()[3] = nValue3;

			return Send(&msg);
		}
		static BOOL Send(HWND hWnd, int nType, void* pData)
		{
			CMessage msg((void*)hWnd, nType);
			
			msg.GetPointerValues()[0] = pData;

			return Send(&msg);
		}
		
		static BOOL Receive(int nMessageID)
		{
			return (nMessageID == WM_USER);
		}
		static CWindowMessage* Get(LPARAM param)
		{
			return (CWindowMessage*)param;
		}
	};
}
