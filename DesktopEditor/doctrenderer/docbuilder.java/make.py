import os
import argparse
import platform
import subprocess

# NOTE: In JDK 8 and earlier, `javac` does not create the directories specified in the -d option if they do not already exist
#       So we need to create them manually
def makedirs(dir):
    if not os.path.exists(dir):
        os.makedirs(dir)
    return

def is_javac_available(javac):
    try:
        process = subprocess.Popen([javac, '-version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        ret = process.wait()
        return ret == 0
    except Exception:
        return False

# return all files with extension `ext` in directory `dir` as string
def getFilesInDir(dir, ext):
    files = [];
    for file in os.listdir(dir):
        if file.endswith(ext):
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
        if not os.path.exists(javac):
            print('Error: Cannot find: ' + javac)
            exit()
        jar = os.path.join(java_home, 'bin', 'jar' + ext)
    else:
        print('Warning: environment variable JAVA_HOME wasn\'t set. Default Java compiler will be used (if any).')

    if not is_javac_available(javac):
        print('Error: javac is not available')
        exit()

    # BUILD
    classes_dir = file_dir + '/build/classes'
    makedirs(classes_dir + '/docbuilder/utils')
    headers_dir = file_dir + '/src/jni'
    # build all Java classes
    subprocess.call([javac, '-d', classes_dir] + java_files, cwd=os.getcwd(), stderr=subprocess.STDOUT)

    # PACKING TO JAR
    if not args.no_jar:
        os.chdir(classes_dir)
        class_files = getFilesInDir('docbuilder', '.class')
        class_files += getFilesInDir('docbuilder/utils', '.class')
        makedirs('../libs')
        subprocess.call([jar, '-cvf', '../libs/docbuilder.jar'] + class_files, cwd=os.getcwd(), stderr=subprocess.STDOUT)
