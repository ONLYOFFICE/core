#ifndef _BUILD_TEMPORARY_CS_H_
#define _BUILD_TEMPORARY_CS_H_

namespace NSCriticalSection
{
	class CRITICAL_SECTION_NATIVE
	{
	public:
		CRITICAL_SECTION_NATIVE() {};
		virtual ~CRITICAL_SECTION_NATIVE() {};

		virtual void Enter() = 0;
		virtual void Leave() = 0;
	};

	class CRITICAL_SECTION
	{
	private:
		CRITICAL_SECTION_NATIVE* m_pCS;

	public:
		CRITICAL_SECTION();
		~CRITICAL_SECTION();

		void InitializeCriticalSection();
		void DeleteCriticalSection();

		void Enter();
		void Leave();
	};
}

class CTemporaryCS
{
public:
	CTemporaryCS(NSCriticalSection::CRITICAL_SECTION* cs);
	~CTemporaryCS();
	void LeaveCS();
	void EnterCS(NSCriticalSection::CRITICAL_SECTION* cs);
protected:
	NSCriticalSection::CRITICAL_SECTION* m_cs;
};

#endif // _BUILD_TEMPORARY_CS_H_