import os
import argparse
import platform
import uuid

langs = ['cpp', 'cs']
os_name = platform.system().lower()

def mkdir(dir):
    if not os.path.exists(dir):
        os.mkdir(dir)

def log(level, message):
    print('configure.py: ' + level + ': ' + message)

def getDefaultBuilderDir():
    dir = ''
    if os_name == 'windows':
        dir = 'C:\\Program Files\\ONLYOFFICE\\DocumentBuilder'
    elif os_name == 'linux':
        dir = '/opt/onlyoffice/documentbuilder'
    return dir

def getAllTests():
    tests = {}
    for lang in langs:
        tests[lang] = []
        test_dirs = os.listdir(lang)
        for test_dir in test_dirs:
            if not os.path.isdir(lang + '/' + test_dir):
                continue
            tests[lang].append(lang + '/' + test_dir)
    return tests

def printAvailableTests():
    all_tests = getAllTests()
    print('all')
    for lang in langs:
        print('-----')
        print(lang)
        tests = all_tests[lang]
        for test in tests:
            print(test)

class PrintTestsList(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        printAvailableTests()
        exit()

def getSelectedTests(tests):
    all_tests = getAllTests()
    # make set of all available tests
    tests_set = {'all'}
    for lang in langs:
        tests_set.add(lang)
        tests_set.update(all_tests[lang])
    # make dict with set of selected tests
    tests_selected = {lang: set() for lang in langs}
    # filter tests through only available ones
    for test in tests:
        if not test in tests_set:
            log('warning', 'wrong test "' + test + '". Call script with --list (or -l) to see all available tests')
            continue

        if test == 'all':
            for lang in langs:
                tests_selected[lang].update(all_tests[lang])
        elif not '/' in test:
            lang = test
            tests_selected[lang].update(all_tests[lang])
        else:
            lang = test.split('/')[0]
            tests_selected[lang].add(test)
    # delete empty tests
    for lang in langs:
        if not tests_selected[lang]:
            del tests_selected[lang]

    return tests_selected

def replacePlaceholders(template_file, output_file, replacements):
    content = ''
    # open and read template file
    with open(template_file, 'r') as file:
        content = file.read()
    # replace all placeholders with corresponding values
    for placeholder, replacement in replacements.items():
        content = content.replace(placeholder, replacement)
    # write result to output file
    with open(output_file, 'w') as file:
        file.write(content)

def genVSProjectsCPP(tests, builder_dir):
    for test in tests:
        test_dir = 'out/' + test
        mkdir(test_dir)
        test_name = test.split('/')[1]
        if os.path.exists(test_dir + '/' + test_name + '.vcxproj'):
            log('info', 'VS C++ project for sample "' + test + '" already exists. Skipping.')
            continue
        else:
            log('info', 'generating VS C++ project for sample "' + test + '"...')
        # .vcxproj
        project_guid = str(uuid.uuid4())
        replacements = {
            '[PROJECT_GUID]': project_guid,
            '[TEST_NAME]': test_name,
            '[BUILDER_DIR]': builder_dir,
            '[ROOT_DIR]': os.getcwd()
        }
        replacePlaceholders('configure/project_templates/cpp/template.vcxproj', test_dir + '/' + test_name + '.vcxproj', replacements)
        # .sln
        replacements = {
            '[SOLUTION_GUID]': str(uuid.uuid4()).upper(),
            '[TEST_NAME]': test_name,
            '[PROJECT_GUID]': project_guid.upper(),
            '[EXT_GLOBALS_GUID]': str(uuid.uuid4()).upper()
        }
        replacePlaceholders('configure/project_templates/cpp/template.sln', test_dir + '/' + test_name + '.sln', replacements)
        # .vcxproj.filters
        replacements = {
            '[GUID_SOURCE_FILES]': str(uuid.uuid4()).upper(),
            '[GUID_HEADER_FILES]': str(uuid.uuid4()).upper(),
            '[GUID_RESOURCE_FILES]': str(uuid.uuid4()).upper(),
            '[TEST_NAME]': test_name
        }
        replacePlaceholders('configure/project_templates/cpp/template.vcxproj.filters', test_dir + '/' + test_name + '.vcxproj.filters', replacements)
        # .vcxproj.user
        replacements = {
            '[BUILDER_DIR]': builder_dir
        }
        replacePlaceholders('configure/project_templates/cpp/template.vcxproj.user', test_dir + '/' + test_name + '.vcxproj.user', replacements)

def genVSProjectsCS(tests, builder_dir):
    for test in tests:
        test_dir = 'out/' + test
        mkdir(test_dir)
        test_name = test.split('/')[1]
        if os.path.exists(test_dir + '/' + test_name + '.csproj'):
            log('info', 'VS C# project for sample "' + test + '" already exists. Skipping.')
            continue
        else:
            log('info', 'generating VS C# project for sample "' + test + '"...')
        # .csproj
        project_guid = str(uuid.uuid4())
        replacements = {
            '[TEST_NAME]': test_name,
            '[BUILDER_DIR]': builder_dir,
        }
        replacePlaceholders('configure/project_templates/cs/template.csproj', test_dir + '/' + test_name + '.csproj', replacements)
        # .sln
        replacements = {
            '[SOLUTION_GUID]': str(uuid.uuid4()).upper(),
            '[TEST_NAME]': test_name,
            '[PROJECT_GUID]': project_guid.upper(),
            '[EXT_GLOBALS_GUID]': str(uuid.uuid4()).upper()
        }
        replacePlaceholders('configure/project_templates/cs/template.sln', test_dir + '/' + test_name + '.sln', replacements)

def genVSProjects(tests_selected, builder_dir):
    if os_name != 'windows':
        log('warning', 'generating Visual Studio projects is only available on Windows')
        return

    builder_dir = builder_dir.replace('/', '\\')
    for lang, tests in tests_selected.items():
        mkdir('out/' + lang)
        if lang == 'cpp':
            genVSProjectsCPP(tests, builder_dir)
        elif lang == 'cs':
            genVSProjectsCS(tests, builder_dir)

def genQtProjects(tests_selected, builder_dir):
    # only for C++ projects
    if 'cpp' not in tests_selected:
        return

    root_dir = os.getcwd()
    if os_name == 'windows':
        builder_dir = builder_dir.replace('\\', '/')
        root_dir = root_dir.replace('\\', '/')

    tests = tests_selected['cpp']
    mkdir('out/cpp')
    for test in tests:
        test_dir = 'out/' + test
        mkdir(test_dir)
        test_name = test.split('/')[1]
        if os.path.exists(test_dir + '/' + test_name + '.pro'):
            log('info', 'Qt project for sample "' + test + '" already exists. Skipping.')
            continue
        else:
            log('info', 'generating Qt C++ project for sample "' + test + '"...')
        # .pro
        replacements = {
            '[TEST_NAME]': test_name,
            '[BUILDER_DIR]': builder_dir,
            '[ROOT_DIR]': root_dir
        }
        replacePlaceholders('configure/project_templates/cpp/template.pro', test_dir + '/' + test_name + '.pro', replacements)

def genMakefile(tests_selected, builder_dir):
    # only for C++ projects
    if 'cpp' not in tests_selected:
        return

    if os_name == 'windows':
        log('warning', 'generating Makefile is not available on Windows')
        return

    # initialize variables
    compiler = ''
    lflags = ''
    env_lib_path = ''
    if os_name == 'linux':
        compiler = 'g++'
        lflags = '-Wl,--unresolved-symbols=ignore-in-shared-libs'
        env_lib_path = 'LD_LIBRARY_PATH'
    elif os_name == 'darwin':
        compiler = 'clang++'
        env_lib_path = 'DYLD_LIBRARY_PATH'
    root_dir = os.getcwd()
    tests = tests_selected['cpp']
    mkdir('out/cpp')
    for test in tests:
        test_dir = 'out/' + test
        mkdir(test_dir)
        test_name = test.split('/')[1]
        if os.path.exists(test_dir + '/Makefile'):
            log('info', 'Makefile for sample "' + test + '" already exists. Skipping.')
        else:
            log('info', 'generating Makefile for C++ sample "' + test + '"...')
            continue
        # Makefile
        replacements = {
            '[TEST_NAME]': test_name,
            '[BUILDER_DIR]': builder_dir,
            '[ROOT_DIR]': root_dir,
            '[COMPILER]': compiler,
            '[LFLAGS]': lflags,
            '[ENV_LIB_PATH]': env_lib_path
        }
        replacePlaceholders('configure/project_templates/cpp/Makefile', test_dir + '/Makefile', replacements)


if __name__ == '__main__':
    # go to root dir
    file_dir = os.path.dirname(os.path.realpath(__file__))
    os.chdir(file_dir + '/..')
    # initialize argument parser
    parser = argparse.ArgumentParser(description='Generate project files for Document Builder samples')
    parser.add_argument('--vs', action='store_true', help='create Visual Studio (.vcxproj and .csproj) project files')
    parser.add_argument('--qt', action='store_true', help='create Qt (.pro) project files')
    parser.add_argument('--make', action='store_true', help='create Makefile')
    parser.add_argument('-t', '--test', dest='tests', action='append', help='specifies tests to generate project files', required=True)
    parser.add_argument('-l', '--list', action=PrintTestsList, nargs=0, help='show list of available tests and exit')

    builder_dir = getDefaultBuilderDir()
    if builder_dir:
        parser.add_argument('--dir', action='store', help='specifies Document Builder directory (default: ' + builder_dir + ')', default=builder_dir)
    else:
        parser.add_argument('--dir', action='store', help='specifies Document Builder directory', required=True)

    args = parser.parse_args()

    # validate arguments
    if not os.path.exists(args.dir):
        log('error', 'Document Builder directory doesn\'t exist: ' + args.dir)
        exit(1)

    if not (args.vs or args.qt or args.make):
        parser.print_usage()
        log('error', 'at least one of --vs, --qt or --make must be provided')
        exit(1)

    # filter tests
    tests_selected = getSelectedTests(args.tests)

    # generate projects
    mkdir('out')
    # VS
    if args.vs:
        genVSProjects(tests_selected, args.dir)
    elif 'cs' in tests_selected:
        log('warning', 'generating C# projects only available ' + ('on Windows ' if os_name != 'windows' else '') + 'with --vs')
    # Qt
    if args.qt:
        genQtProjects(tests_selected, args.dir)
    # Makefile
    if args.make:
        genMakefile(tests_selected, args.dir)
