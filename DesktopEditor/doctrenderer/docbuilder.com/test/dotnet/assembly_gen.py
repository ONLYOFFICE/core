import os
import argparse

default_platform = "win_64"
default_vcvars = "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build"

help_platform = "win_64 or win_32"
help_vcvars = "vcvars files directory"

def createParser ():
    parser = argparse.ArgumentParser()

    # win_32 or win_64
    parser.add_argument('-platform', nargs='?', default=default_platform, help=help_platform)

    # set vcvars path
    parser.add_argument('-vcvars', nargs='?', default=default_vcvars, help=help_vcvars)
   
    return parser

if __name__ == "__main__":

    parser = createParser()
    args = parser.parse_args()
    
    dll_path = "../../deploy/" + args.platform + "/docbuilder.com.dll"
    out_path = "../../deploy/" + args.platform + "/onlyofficedocbuilder.dll"
    
    # call build
    os.system("\"" + args.vcvars + "/vcvars64.bat\" && " +
    "tlbimp " + 
    dll_path + " /out=" +
    out_path)    
    