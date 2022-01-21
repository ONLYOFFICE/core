$Lcids = ([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ) | Select-Object -ExpandProperty Name | Select-Object -Skip 1)

foreach($i in $Lcids) {

	echo Set-Culture $i
	Set-Culture $i
	Start-Sleep -Seconds 1

	$pathMacro = $(pwd).Path + "\macros-locale-data.xlsm"
	echo .\run-macros-data.vbs $pathMacro $i
 
	.\run-macros-data.vbs $pathMacro $i
	Start-Sleep -Seconds 7
 
}

Set-Culture ru-Ru
 
 



