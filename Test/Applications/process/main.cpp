//#include "../../../../desktop-sdk/ChromiumBasedEditors/lib/src/cefwrapper/external_process.h"
#include "../../../../desktop-sdk/ChromiumBasedEditors/lib/src/cefwrapper/external_process_with_childs.h"

#include <iostream>

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
		case NSProcesses::StreamType::Stop:
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

	//manager.Start("ping -c 4 google.com", {});
	//manager.Start("calc", {});

	manager.Start("docker run -i --rm -e GITHUB_PERSONAL_ACCESS_TOKEN ghcr.io/github/github-mcp-server", {
		{"GITHUB_PERSONAL_ACCESS_TOKEN", "token"}
	});

	std::this_thread::sleep_for(std::chrono::seconds(15));

	manager.StopAll();

	std::cout << "Hello World!" << std::endl;
	return 0;
}
