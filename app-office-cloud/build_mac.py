"""Build the cloud companion and launcher; preserve the verified native WPS converter."""
from pathlib import Path
import hashlib,json,os,plistlib,shutil,subprocess,sys
from prepare import prepare
root=Path(__file__).resolve().parents[1]
out=Path(sys.argv[1]).resolve();source=Path(sys.argv[2]).resolve() if len(sys.argv)>2 else Path('/Applications/ONLYOFFICE WPS.app')
stage=out/'mac-staging';app=stage/'ONLYOFFICE WPS.app';stage.mkdir(parents=True,exist_ok=True)
subprocess.run(['ditto',str(source),str(app)],check=True)
engine=app/'Contents/Resources/converter/x2t';expected='7e0203dc42b79ad992401fd2f46d6c75914ef276eacd00607f1fd17fc3ce7749'
assert hashlib.sha256(engine.read_bytes()).hexdigest()==expected
mac=app/'Contents/MacOS';main=mac/'ONLYOFFICE WPS';native=mac/'ONLYOFFICE WPS Native'
if not native.exists():main.rename(native)
subprocess.run(['go','build','-trimpath','-ldflags=-s -w','-o',str(mac/'MIDIACloud'),'.'],cwd=root/'app-office-cloud',check=True)
subprocess.run(['clang','-mmacosx-version-min=13.0','-Os',str(root/'app-office-cloud/launcher.c'),'-o',str(main)],check=True)
counts=prepare(app)
p=app/'Contents/Info.plist';d=plistlib.loads(p.read_bytes());d['CFBundleVersion']='563.2';d['LSMinimumSystemVersion']='13.0';p.write_bytes(plistlib.dumps(d))
shutil.copy2(root/'app-office-cloud/LEEME.md',stage/'LEEME.md')
shutil.copy2(root/'app-office-cloud/LEEME.md',app/'Contents/Resources/MIDIA-CLOUD-LEEME.md')
link=stage/'Applications'
if not link.exists():link.symlink_to('/Applications')
for p in [mac/'MIDIACloud',main,native,app]:subprocess.run(['codesign','--force','--deep','--sign','-',str(p)],check=True)
subprocess.run(['codesign','--verify','--deep','--strict',str(app)],check=True)
image=out/'ONLYOFFICE-WPS-Cloud-9.4.0.2-macos-arm64.dmg'
subprocess.run(['hdiutil','create','-volname','ONLYOFFICE WPS + MIDIA Cloud','-srcfolder',str(stage),'-format','UDZO','-ov',str(image)],check=True)
report={'filename':image.name,'bytes':image.stat().st_size,'sha256':hashlib.sha256(image.read_bytes()).hexdigest(),'engine_sha256':expected,'autosave_calls':counts,'signature':'ad-hoc, deep strict passed','arch':'arm64','minimum_macos':'13.0'}
(out/'mac-cloud-verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
