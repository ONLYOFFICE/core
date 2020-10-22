$Lcids = ([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ) | Select-Object -ExpandProperty Name)

 foreach($i in $Lcids) {

echo Set-Culture $i
 Set-Culture $i
  Start-Sleep -Seconds 1

  $pathMacro = $(pwd).Path + "\macros-locale-data.xlsm"
  $pathOutput = $(pwd).Path + "\" + $i + ".txt"
 echo .\run-macros-data.vbs $i
 
  .\run-macros-data.vbs $i
  Start-Sleep -Seconds 7
 
}

Set-Culture ru-Ru
 
 



