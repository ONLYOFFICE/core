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
        builder_dir = args.builder_dir
        os.system('java -cp ../build/classes' + os.pathsep + 'build/classes -Djava.library.path=\"' + builder_dir + '\" Program')
    else:
        os.system('javac -d build/classes -cp ../build/classes ' + java_file)
        print('Program was built successfully')
        print('Run it with:    java -cp \"../build/classes' + os.pathsep + 'build/classes\" -D\"java.library.path\"=\"path_to_docbuilder\" Program')
        print('Or just run:    python make_test.py --run \"path_to_docbuilder\"')
