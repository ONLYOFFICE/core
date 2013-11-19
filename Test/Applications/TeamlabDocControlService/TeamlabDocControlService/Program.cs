using System.Collections.Generic;
using System.ServiceProcess;
using System.Text;

namespace TeamlabDocControlService
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            #if (!DEBUG)

                ServiceBase[] ServicesToRun;

                // More than one user Service may run within the same process. To add
                // another service to this process, change the following line to
                // create a second service object. For example,
                //
                //   ServicesToRun = new ServiceBase[] {new Service1(), new MySecondUserService()};
                //
                ServicesToRun = new ServiceBase[] { new TeamlabDocService() };

                ServiceBase.Run(ServicesToRun);
             #else
                // Debug code: this allows the process to run as a non-service.
                // It will kick off the service start point, but never kill it.
                // Shut down the debugger to exit
                TeamlabDocService service = new TeamlabDocService();
                service.debugStart();
                // Put a breakpoint on the following line to always catch
                // your service when it has finished its work
                System.Threading.Thread.Sleep(System.Threading.Timeout.Infinite);
        #endif 
        }
        
    }
}