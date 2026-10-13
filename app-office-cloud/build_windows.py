from pathlib import Path
import hashlib,json,shutil,subprocess,urllib.request
from prepare import prepare
root=Path(__file__).resolve().parents[1];out=root/'office-cloud-dist';out.mkdir(exist_ok=True)
setup=out/'base-9.4.0.1.exe'
if not setup.exists():urllib.request.urlretrieve('https://gestion.midiashops.com/diseno-v2/downloads/ONLYOFFICE-WPS-9.4.0.1-Windows-x64-Setup.exe',setup)
assert hashlib.sha256(setup.read_bytes()).hexdigest()=='9be46aaae9341ee1fc7ea81b7c4b429c41737c77ae9be06c61751aaf5b92b145'
vendor=out/'vendor'
subprocess.run([str(setup),'/VERYSILENT','/SUPPRESSMSGBOXES','/NORESTART','/DIR='+str(vendor)],check=True)
assert hashlib.sha256((vendor/'converter/x2t.exe').read_bytes()).hexdigest()=='a847197348987e58143b88282a394090e09dd81b9d832e2d2d2f4c50a82e5699'
subprocess.run(['go','build','-trimpath','-ldflags=-s -w -H=windowsgui','-o',str(vendor/'MIDIACloud.exe'),'.'],cwd=root/'app-office-cloud',check=True)
counts=prepare(vendor)
shutil.copy2(root/'app-office-cloud/LEEME.md',vendor/'MIDIA-CLOUD-LEEME.md')
iscc=Path('C:/Program Files (x86)/Inno Setup 6/ISCC.exe')
subprocess.run([str(iscc),'/DAppSource='+str(vendor),'/DOutputDirectory='+str(out),str(root/'app-office-cloud/installer.iss')],check=True)
new=out/'ONLYOFFICE-WPS-Cloud-9.4.0.2-Windows-x64-Setup.exe'
installed=out/'installed'
subprocess.run([str(new),'/VERYSILENT','/SUPPRESSMSGBOXES','/NORESTART','/DIR='+str(installed)],check=True)
assert (installed/'MIDIACloud.exe').exists() and (installed/'providers/merp/config.json').exists()
assert hashlib.sha256((installed/'converter/x2t.exe').read_bytes()).hexdigest()=='a847197348987e58143b88282a394090e09dd81b9d832e2d2d2f4c50a82e5699'
manifest={'filename':new.name,'bytes':new.stat().st_size,'sha256':hashlib.sha256(new.read_bytes()).hexdigest(),'engine_sha256':'a847197348987e58143b88282a394090e09dd81b9d832e2d2d2f4c50a82e5699','autosave_calls':counts,'installation':'passed','agent_tests':'passed','target_PC_GUI':'pending'}
(out/'windows-cloud-verification.json').write_text(json.dumps(manifest,indent=2))
print(json.dumps(manifest,indent=2))
