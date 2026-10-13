//go:build ignore

// Preserve the original macOS PID/event delivery while starting cloud sync.
#include <mach-o/dyld.h>
#include <libgen.h>
#include <limits.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char **environ;
int main(int argc,char **argv){
 char path[PATH_MAX],resolved[PATH_MAX],agent[PATH_MAX],native[PATH_MAX];uint32_t size=sizeof(path);
 if(_NSGetExecutablePath(path,&size)||!realpath(path,resolved))return 1;
 char *folder=dirname(resolved);
 snprintf(agent,sizeof(agent),"%s/MIDIACloud",folder);snprintf(native,sizeof(native),"%s/ONLYOFFICE WPS Native",folder);
 char *args[]={agent,"--serve",NULL};pid_t pid;
 posix_spawn(&pid,agent,NULL,NULL,args,environ);
 argv[0]=native;execv(native,argv);perror("ONLYOFFICE WPS");return 1;
}
