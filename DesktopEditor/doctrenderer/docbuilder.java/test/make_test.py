import os
import argparse

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

    if args.builder_dir:
        builder_dir = args.builder_dir
        os.system('java -cp ../build/libs/docbuilder.jar' + os.pathsep + 'build/classes -Djava.library.path=\"' + builder_dir + '\" Program')
    else:
        makedirs('build/classes')
        os.system('javac -d build/classes -cp ../build/libs/docbuilder.jar ' + java_file)
        print('Program was built successfully')
        print('Run it with:    java -cp \"../build/libs/docbuilder.jar' + os.pathsep + 'build/classes\" -D\"java.library.path\"=\"path_to_docbuilder\" Program')
        print('Or just run:    python make_test.py --run \"path_to_docbuilder\"')
