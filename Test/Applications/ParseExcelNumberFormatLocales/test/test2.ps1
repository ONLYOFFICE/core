$Lcids = ([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ) | Select-Object -ExpandProperty Name)

 foreach($i in $Lcids) {

echo Set-Culture $i
 Set-Culture $i
  Start-Sleep -Seconds 1
 echo .\run2.vbs $i
 
  .\run2.vbs $i
  Start-Sleep -Seconds 7
 
}

Set-Culture ru-Ru
 
 



