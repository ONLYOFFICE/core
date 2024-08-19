import os
import argparse
import platform
import subprocess
import re

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

def get_jdk_version(javac):
    try:
        process = subprocess.Popen([javac, '-version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        output = stdout if stdout else stderr
        if not output:
            return 0

        javac_version_str = output.decode('utf-8').strip()
        match = re.search('(\d+)(?:\.(\d+))?', javac_version_str)
        if not match:
            return 0

        major_version = int(match.group(1))
        minor_version = int(match.group(2)) if match.group(2) else 0

        # for JDK 9 and earlier command `javac -version` would give '1.x.xx'
        if major_version == 1:
            return minor_version
        return major_version

    except Exception:
        return 0

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

    # INITIALIZE JDK TOOLS
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

    # CHECK JDK VERSION
    jdk_version = get_jdk_version(javac)
    if jdk_version < 8:
        print('Error: javac version is not supported')
        exit()

    release_flags = []
    if jdk_version > 8:
        release_flags = ['--release', '8']

    # BUILD
    classes_dir = file_dir + '/build/classes'
    makedirs(classes_dir + '/docbuilder/utils')
    headers_dir = file_dir + '/src/jni'
    # build all Java classes
    subprocess.call([javac, '-d', classes_dir] + release_flags + java_files, cwd=os.getcwd(), stderr=subprocess.STDOUT)

    # PACKING TO JAR
    if not args.no_jar:
        os.chdir(classes_dir)
        class_files = getFilesInDir('docbuilder', '.class')
        class_files += getFilesInDir('docbuilder/utils', '.class')
        makedirs('../libs')
        subprocess.call([jar, '-cvf', '../libs/docbuilder.jar'] + class_files, cwd=os.getcwd(), stderr=subprocess.STDOUT)
