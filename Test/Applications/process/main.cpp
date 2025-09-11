#include "../../../../desktop-sdk/ChromiumBasedEditors/lib/src/cefwrapper/external_process.h"

class CProcessRunnerCallbackWork : public NSProcesses::CProcessRunnerCallback
{
public:
	CProcessRunnerCallbackWork(){}
	virtual ~CProcessRunnerCallbackWork(){}

	virtual void process_callback(const int& id, const NSProcesses::StreamType& type, const std::string& message)
	{
		std::string type_out = "stop";
		switch (type)
		{
		case NSProcesses::StreamType::StdOut:
			type_out = "stdout";
			break;
		case NSProcesses::StreamType::StdErr:
			type_out = "stderr";
			break;
		default:
			break;
		}

		std::cout << "[" << id << ", " << type_out << "] " << message << std::endl;
	}
};

int main()
{
	CProcessRunnerCallbackWork callback;
	NSProcesses::CProcessManager manager(&callback);

	manager.Start("ping -c 4 google.com", {});

	std::this_thread::sleep_for(std::chrono::seconds(5));

	manager.StopAll();

	std::cout << "Hello World!" << std::endl;
	return 0;
}
