import os
import argparse
import platform

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build and run the test example for docbuilder Java wrapper')
    parser.add_argument('-r', '--run', dest='builder_dir', metavar='<directory>', help='Run the test example with specified docbuilder directory')
    args = parser.parse_args()

    file_dir = os.path.dirname(os.path.realpath(__file__));
    os.chdir(file_dir)

    java_file = 'Program.java'

    if args.builder_dir:
        os_name = platform.system().lower()
        builder_dir = args.builder_dir
        if 'windows' == os_name:
            # to load dependent libraries
            os.environ['PATH'] = builder_dir + os.pathsep + os.environ['PATH']
        os.system('java -cp ../build/classes' + os.pathsep + 'build/classes -Djava.library.path=\"' + builder_dir + '\" Program')
    else:
        os.system('javac -d build/classes -cp ../build/classes ' + java_file)
        print('Program was built successfully')
        print('Run it with:    java -cp \"../build/classes' + os.pathsep + 'build/classes\" -D\"java.library.path\"=\"path_to_docbuilder\" Program')
        os_name = platform.system().lower()
        if 'windows' == os_name:
            print('NOTE: You may need to add \"path_to_docbuilder\" to your PATH to load dependent libraries')
        print('Or just run:    python make_test.py --run \"path_to_docbuilder\"')
