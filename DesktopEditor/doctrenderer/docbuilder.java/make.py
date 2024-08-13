import os
import argparse
import re
import platform
import subprocess

# NOTE: In JDK 8 and earlier, `javac` does not create the directories specified in the -d option if they do not already exist
#       So we need to create them manually
def makedirs(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)
    return

# return all files with extension `ext` in directory `dir` as string
def getFilesInDir(dir, ext):
    files = [];
    for file in os.listdir(dir):
        if file.endswith(ext):
            # for non-windows systems '$'-symbol in file names should be escaped
            if platform.system().lower() != 'windows':
                file = re.sub(r'\$', r'\\$', file)
            files.append(os.path.join(dir, file))

    return files

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Build Java wrapper for docbuilder library')
    parser.add_argument('-H', '--headers', action='store_true', help='Generate C++ JNI header files')
    parser.add_argument('-n', '--no-jar', dest='no_jar', action='store_true', help='Build only classes without JAR archive')
    args = parser.parse_args()

    file_dir = os.path.dirname(os.path.realpath(__file__));
    os.chdir(file_dir + '/src/java')

    java_files = getFilesInDir('docbuilder', '.java')
    java_files += getFilesInDir('docbuilder/utils', '.java')

    # INITIALIZE JAVA TOOLS
    javac = 'javac'
    jar = 'jar'
    ext = '.exe' if platform.system().lower() == 'windows' else ''
    java_home = os.environ.get('JAVA_HOME')
    if java_home:
        javac = os.path.join(java_home, 'bin', 'javac' + ext)
        jar = os.path.join(java_home, 'bin', 'jar' + ext)

    if not os.path.exists(javac):
        print('Error: Cannot find: ' + javac)
        exit()

    # BUILD
    classes_dir = file_dir + '/build/classes'
    makedirs(classes_dir + '/docbuilder/utils')
    headers_dir = file_dir + '/src/jni'
    # build all Java classes
    subprocess.call([javac, '-d', classes_dir] + java_files, cwd=os.getcwd())

    # PACKING TO JAR
    if not args.no_jar:
        os.chdir(classes_dir)
        class_files = getFilesInDir('docbuilder', '.class')
        class_files += getFilesInDir('docbuilder/utils', '.class')
        makedirs('../libs')
        subprocess.call([jar, '-cvf', '../libs/docbuilder.jar'] + class_files, cwd=os.getcwd())
