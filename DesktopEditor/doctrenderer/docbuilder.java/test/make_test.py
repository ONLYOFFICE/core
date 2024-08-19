import os
import argparse
import subprocess
import platform

# NOTE: In JDK 8 and earlier, `javac` does not create the directories specified in the -d option if they do not already exist
#       So we need to create them manually
def makedirs(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)
    return

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build and run the test example for docbuilder Java wrapper')
    parser.add_argument('-r', '--run', dest='builder_dir', metavar='<directory>', help='Run the test example with specified docbuilder directory')
    args = parser.parse_args()

    file_dir = os.path.dirname(os.path.realpath(__file__));
    os.chdir(file_dir)

    java_file = 'Program.java'

    # INITIALIZE JAVA TOOLS
    javac = 'javac'
    java = 'java'
    ext = '.exe' if platform.system().lower() == 'windows' else ''
    java_home = os.environ.get('JAVA_HOME')
    if java_home:
        javac = os.path.join(java_home, 'bin', 'javac' + ext)
        if not os.path.exists(javac):
            print('Error: Cannot find: ' + javac)
            exit()
        java = os.path.join(java_home, 'bin', 'java' + ext)
    else:
        print('Warning: environment variable JAVA_HOME wasn\'t set. Default Java compiler will be used (if any).')

    if args.builder_dir:
        builder_dir = args.builder_dir
        subprocess.call([java, '-cp', os.path.join(builder_dir, 'docbuilder.jar') + os.pathsep + 'build/classes', 'Program'], cwd=os.getcwd(), stderr=subprocess.STDOUT)
    else:
        makedirs('build/classes')
        subprocess.call([javac, '-d', 'build/classes', '-cp', '../build/libs/docbuilder.jar', java_file], cwd=os.getcwd(), stderr=subprocess.STDOUT)
        print('Program was built successfully')
        print('Run it with:    java -cp \"/path/to/docbuilder/docbuilder.jar' + os.pathsep + 'build/classes\" Program')
        print('Or just run:    python make_test.py --run \"/path/to/docbuilder\"')
