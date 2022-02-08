$CultureInfos =([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ))






foreach($culture in $CultureInfos) {
 $p1 = $culture.Name
$p2 = $culture.NumberFormat.NumberDecimalSeparator
$p3 = $culture.NumberFormat.NumberGroupSeparator
 $pathMacro = $(pwd).Path + "\$p2.txt" 
 $pathoutput = $(pwd).Path + "\$p1.txt"
Set-Content "$pathMacro" $p2
Set-Content -Path "$pathoutput" -Value "$p3" -Encoding "utf8"
}