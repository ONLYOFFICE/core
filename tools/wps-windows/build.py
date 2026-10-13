"""Build the native WPS photo patch and a per-user Windows x64 installer."""
from pathlib import Path
import hashlib, json, os, re, shutil, subprocess, sys, tarfile, urllib.request, zipfile

ROOT = Path(__file__).resolve().parents[2]
WORK = Path(os.environ.get('WPS_BUILD_ROOT', 'D:/wps-build'))
CACHE = Path(os.environ.get('WPS_CACHE', 'D:/wps-cache'))
LIB = ROOT / 'build/lib/win_64'
QT = Path(os.environ.get('QMAKE', 'D:/Qt/5.15.2/msvc2019_64/bin/qmake.exe'))
DLL_NAMES = ['graphics','kernel','UnicodeConverter','kernel_network','Fb2File','PdfFile','HtmlFile2','EpubFile','XpsFile','OFDFile','DjVuFile','doctrenderer','DocxRenderer','IWorkFile','HWPFile','StarMathConverter','ooxmlsignature']


def native_cache_key():
    records = subprocess.check_output(['git', 'ls-tree', '-r', '-z', 'HEAD'], cwd=ROOT).split(b'\0')
    tree = b'\0'.join(record for record in records if record and not
        record.rsplit(b'\t', 1)[1].startswith((b'.github/', b'tools/', b'OOXML/test/')))
    return hashlib.sha256(tree + Path(__file__).read_bytes() +
        os.environ.get('ImageVersion', '').encode()).hexdigest()


def run(args, cwd=ROOT, capture=False):
    print('RUN', subprocess.list2cmdline([str(a) for a in args]), flush=True)
    return subprocess.run([str(a) for a in args], cwd=cwd, check=True, text=True, capture_output=capture)


def download(url, name):
    target = CACHE / name
    if not target.exists():
        print('DOWNLOAD', url, flush=True)
        temporary = target.with_suffix(target.suffix + '.part')
        urllib.request.urlretrieve(url, temporary)
        temporary.replace(target)
    return target


def build(project, name, extra=()):
    directory = WORK / name
    directory.mkdir(parents=True, exist_ok=True)
    run([QT, project, 'CONFIG+=release', 'CONFIG-=debug', 'CONFIG-=debug_and_release',
         'CONFIG+=vs2019', 'CONFIG+=precompile_header', 'DEFINES+=BOOST_ALL_NO_LIB',
         *extra], directory)
    run([WORK / 'jom/jom.exe', '-j2'], directory)


def main():
    for directory in [WORK, CACHE, LIB]: directory.mkdir(parents=True, exist_ok=True)
    assert sys.platform == 'win32' and shutil.which('cl'), 'Run from an x64 Visual Studio developer shell'
    os.environ.update(PRODUCT_VERSION='9.4.0', BUILD_NUMBER='131')
    # Reuse vendor runtime DLLs; generate import libraries from their public PE exports.
    vendor = WORK / 'vendor'
    if not (vendor / 'DesktopEditors.exe').exists():
        archive = download('https://github.com/ONLYOFFICE/DesktopEditors/releases/download/v9.4.0/DesktopEditors_x64.zip', 'DesktopEditors-9.4-x64.zip')
        with zipfile.ZipFile(archive) as z: z.extractall(vendor)
    converter = vendor / 'converter'
    for name in DLL_NAMES:
        dll = converter / (name + '.dll')
        exports = run(['dumpbin', '/nologo', '/exports', dll], capture=True).stdout
        symbols = [m.group(1) for line in exports.splitlines() if (m := re.match(r'^\s*\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+(\S+)', line))]
        assert symbols, name
        definition = LIB / (name + '.def')
        definition.write_text('LIBRARY '+name+'.dll\nEXPORTS\n'+'\n'.join(symbols)+'\n')
        run(['lib', '/nologo', '/machine:x64', '/def:'+str(definition), '/out:'+str(LIB/(name+'.lib'))])
    # ICU 74.2 is the version used by the 9.4 vendor runtime.
    icu = ROOT / 'Common/3dParty/icu'
    if not (icu / 'icu/include/unicode/ucnv.h').exists():
        archive = download('https://github.com/unicode-org/icu/releases/download/release-74-2/icu4c-74_2-Win64-MSVC2019.zip', 'icu74-win64.zip')
        with zipfile.ZipFile(archive) as z: z.extractall(icu / 'icu')
    icu_lib = icu / 'win_64/build'; icu_lib.mkdir(parents=True, exist_ok=True)
    shutil.copy2(icu / 'icu/lib64/icuuc.lib', icu_lib / 'icuuc.lib')
    # Boost's official 1.72 build recipes expect the MSVC 14.2 ABI/library naming.
    boost = WORK / 'boost_1_72_0'
    boost_dest = ROOT / 'Common/3dParty/boost/build/win_64'
    if not (boost_dest/'lib/libboost_regex-vc142-mt-x64-1_72.lib').exists():
        if not boost.exists():
            archive = download('https://archives.boost.io/release/1.72.0/source/boost_1_72_0.tar.bz2', 'boost-1.72.tar.bz2')
            with tarfile.open(archive) as t: t.extractall(WORK, filter='data')
        run(['cmd','/c','bootstrap.bat','vc142'], boost)
        setup = WORK/'boost-env.bat'; setup.write_text('@exit /b 0\n')
        compiler = shutil.which('cl').replace('\\', '/')
        config = WORK/'boost-user-config.jam'
        config.write_text(f'using msvc : 14.2 : "{compiler}" : <setup>"{setup.as_posix()}" <rewrite-setup-scripts>off ;\n')
        run([boost/'b2.exe','-j2','-d0','--layout=versioned','--user-config='+str(config),'--prefix='+str(boost_dest),
             'toolset=msvc-14.2','architecture=x86','address-model=64','threading=multi','variant=release','link=static','runtime-link=shared',
             '--with-system','--with-filesystem','--with-regex','--with-date_time','install'],boost)
        versioned = boost_dest/'include/boost-1_72/boost'
        if versioned.exists(): shutil.copytree(versioned, boost_dest/'include/boost', dirs_exist_ok=True)
    if not (WORK/'jom/jom.exe').exists():
        with zipfile.ZipFile(download('https://download.qt.io/official_releases/jom/jom.zip','jom.zip')) as z: z.extractall(WORK/'jom')
    projects = [
        ('OOXML/Projects/Linux/XlsbFormatLib/XlsbFormatLib.pro','xlsb'),
        ('MsBinaryFile/Projects/VbaFormatLib/Linux/VbaFormatLib.pro','vba'),
        ('OOXML/Projects/Linux/DocxFormatLib/DocxFormatLib.pro','docx'),
        ('OOXML/Projects/Linux/PPTXFormatLib/PPTXFormatLib.pro','pptx'),
        ('OOXML/Projects/Linux/BinDocument/BinDocument.pro','binary'),
        ('TxtFile/Projects/Linux/TxtXmlFormatLib.pro','txt'),
        ('RtfFile/Projects/Linux/RtfFormatLib.pro','rtf'),
        ('MsBinaryFile/Projects/PPTFormatLib/Linux/PPTFormatLib.pro','ppt'),
        ('MsBinaryFile/Projects/DocFormatLib/Linux/DocFormatLib.pro','doc'),
        ('OdfFile/Projects/Linux/OdfFormatLib.pro','odf'),
        ('MsBinaryFile/Projects/XlsFormatLib/Linux/XlsFormatLib.pro','xls'),
        ('Common/cfcpp/cfcpp.pro','compound'),
        ('X2tConverter/build/Qt/X2tConverter.pro','converter')]
    engine = ROOT/'build/bin/win_64/x2t.exe'
    marker = ROOT/'build/windows-native-cache.json'
    key = native_cache_key()
    cached = json.loads(marker.read_text()) if marker.exists() else {}
    libraries = ['CryptoPPLib','VbaFormatLib','OdfFormatLib','DocFormatLib','PptFormatLib',
        'RtfFormatLib','TxtXmlFormatLib','BinDocument','PPTXFormatLib','DocxFormatLib',
        'XlsbFormatLib','XlsFormatLib','CompoundFileLib']
    if (cached.get('key') == key and engine.exists() and
            all((LIB/(name+'.lib')).exists() for name in libraries) and
            hashlib.sha256(engine.read_bytes()).hexdigest() == cached.get('sha256')):
        print('Reusing native converter: exact source, build script and runner image match', flush=True)
    else:
        build(ROOT/'Common/3dParty/cryptopp/project/cryptopp.pro','crypto')
        for project,name in projects: build(ROOT/project,name)
        assert engine.exists()
        marker.write_text(json.dumps({'key':key,'sha256':hashlib.sha256(engine.read_bytes()).hexdigest()}))
    shutil.copy2(engine, converter/'x2t.exe')
    gtest = WORK/'googletest'
    if not gtest.exists():
        run(['git','clone','--depth','1','--branch','v1.13.0','https://github.com/google/googletest.git',gtest])
    os.environ['WPS_GTEST_ROOT']=str(gtest/'googletest').replace('\\','/')
    build(ROOT/'tools/wps-windows/native-tests.pro','tests')
    tests = ROOT/'build/bin/win_64/wps-cell-images-tests.exe'
    test_env = os.environ.copy(); test_env['PATH']=str(converter)+os.pathsep+test_env['PATH']
    result = subprocess.run([str(tests)],env=test_env,capture_output=True,text=True)
    (WORK/'native-tests.txt').write_text(result.stdout+result.stderr)
    print(result.stdout,flush=True); result.check_returncode()
    run([sys.executable,ROOT/'tools/wps-windows/verify.py',converter,WORK/'verification'])
    commit = run(['git','rev-parse','HEAD'],capture=True).stdout.strip()
    (vendor/'WPS-PATCH.txt').write_text('ONLYOFFICE 9.4 with native WPS embedded image import.\nSource: https://github.com/guozz1984/core/tree/'+commit+'\nAGPL-3.0; see included vendor license notices and source LICENSE.\n')
    shutil.copy2(ROOT/'LICENSE', vendor/'SOURCE-LICENSE.txt')
    shutil.copy2(ROOT/'tools/wps-windows/LEEME.txt',vendor/'LEEME-WPS.txt')
    output = ROOT/'windows-dist'; output.mkdir(exist_ok=True)
    manifest={'source_commit':commit,'platform':'windows-x64','x2t_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),'native_tests':'passed','roundtrip_tests':'passed','GUI_on_target_PC':'not yet tested'}
    (output/'build-manifest.json').write_text(json.dumps(manifest,indent=2))
    iscc = Path(os.environ['ProgramFiles(x86)'])/'Inno Setup 6/ISCC.exe'
    run([iscc,'/DAppSource='+str(vendor),'/DOutputDirectory='+str(output),ROOT/'tools/wps-windows/installer.iss'])
    setup=output/'ONLYOFFICE-WPS-9.4-Windows-x64-Setup.exe'; assert setup.exists()
    (output/'SHA256SUMS.txt').write_text(hashlib.sha256(setup.read_bytes()).hexdigest()+'  '+setup.name+'\n')
    print('Installer ready:',setup,flush=True)

if __name__=='__main__':
    if '--cache-key' in sys.argv: print(native_cache_key())
    else: main()
