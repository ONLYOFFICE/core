$CultureInfos =([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ))






foreach($culture in $CultureInfos) {
 $p1 = $culture.Name
$p2 = $culture.NumberFormat.NumberDecimalSeparator
$p3 = $culture.NumberFormat.NumberGroupSeparator
Set-Content  $pathMacro = $(pwd).Path + "\$p2.txt" $p2
Set-Content -Path $pathoutput = $(pwd).Path + "\$p1.txt" -Value "$p3" -Encoding "utf8"
}