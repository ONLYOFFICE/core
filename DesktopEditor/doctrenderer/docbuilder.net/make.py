import os
import argparse
import shutil

solution_name = 'docbuilder.net.sln'

deploy_directory = 'deploy'
tmp_directory = deploy_directory + '/tmp'

help_platform = 'set a platform. win_64 or win_32. default: win_64'
help_docbuilder = 'set path to DocumentBuilder. default: C:/Program Files/ONLYOFFICE/DocumentBuilder'
help_msbuild = 'set path to MSBuild. default: C:/Program Files/Microsoft Visual Studio/2022/Community/Msbuild/Current/Bin'
help_out = 'set output path. default: out'

default_platform = 'win_64'
default_docbuilder = 'C:/Program Files/ONLYOFFICE/DocumentBuilder'
default_msbuild = 'C:/Program Files/Microsoft Visual Studio/2022/Community/Msbuild/Current/Bin'
default_out = 'out'

def createParser ():
    parser = argparse.ArgumentParser()

    # win_32 or win_64
    parser.add_argument('-platform', nargs='?', default=default_platform, help=help_platform)

    # set DocumentBuilder path
    parser.add_argument('-docbuilder', nargs='?', default=default_docbuilder, help=help_docbuilder)
    
    # set MSBuild path
    parser.add_argument('-msbuild', nargs='?', default=default_msbuild, help=help_msbuild)

    # out dir
    parser.add_argument('-out', nargs='?', default=default_out, help=help_out)
    return parser


if __name__ == "__main__":
    
    parser = createParser()
    args = parser.parse_args()

    # add msbuilder path into PATH
    os.environ["PATH"] += os.pathsep + args.msbuild

    # add docbuilder path into PATH
    os.environ["PATH"] += os.pathsep + args.docbuilder

    # create build command
    build_string =  'msbuild src/' + solution_name + ' '
    build_string += '-p:Configuration=Release '
    build_string += '-p:Platform=' + ('Win32 ' if  args.platform == 'win_32' else 'x64 ')
    
    # call build
    os.system(build_string)

    # delete intermediate folder
    shutil.rmtree(tmp_directory)

    deploy_platform_directory = deploy_directory + '/' + args.platform

    # replace files into out directory
    if args.out and args.out != deploy_directory:
        if os.path.exists(args.out):
            shutil.rmtree(args.out)

        os.mkdir(args.out)
        get_files = os.listdir(deploy_platform_directory)

        for g in get_files:
            os.replace(deploy_platform_directory + '/' + g, args.out + '/' + g)

        shutil.rmtree(deploy_directory)
    