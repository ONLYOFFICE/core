import os
import argparse

# NOTE: In JDK 8 and earlier, `javac` does not create the directories specified in the -d option if they do not already exist
#       So we need to create them manually
def makedirs(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)
    return

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build Java wrapper for docbuilder library')
    parser.add_argument('-H', '--headers', action='store_true', help='Generate C++ JNI header files')
    parser.add_argument('-n', '--no-jar', dest='no_jar', action='store_true', help='Build only classes without JAR archive')
    args = parser.parse_args()

    file_dir = os.path.dirname(os.path.realpath(__file__));
    os.chdir(file_dir + '/src/java')

    java_files = ''
    for file in os.listdir('docbuilder'):
        if file.endswith('.java'):
            java_files += ' docbuilder/' + file

    java_utils_file = ' docbuilder/utils/NativeLibraryLoader.java'
    java_files += java_utils_file

    # BUILD
    classes_dir = file_dir + '/build/classes'
    makedirs(classes_dir + '/docbuilder/utils')
    headers_dir = file_dir + '/src/jni'
    # build all Java classes
    os.system('javac -d ' + classes_dir + (' -h ' + headers_dir if args.headers else '') + java_files)

    # PACKING TO JAR
    if not args.no_jar:
        os.chdir(classes_dir)
        class_files = ''
        for file in os.listdir('docbuilder'):
            if file.endswith('.class'):
                class_files += ' docbuilder/' + file
        class_utils_file = ' docbuilder/utils/NativeLibraryLoader.class'
        class_files += class_utils_file
        makedirs('../libs')
        os.system('jar -cvf ../libs/docbuilder.jar ' + class_files)
