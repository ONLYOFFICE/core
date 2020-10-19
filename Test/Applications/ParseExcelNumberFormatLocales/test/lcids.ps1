$CultureInfos =([System.Globalization.CultureInfo]::GetCultures( [System.Globalization.CultureTypes]::AllCultures ))






foreach($culture in $CultureInfos) {
 $p1 = $culture.Name
$p2 = $culture.NumberFormat.NumberDecimalSeparator
$p3 = $culture.NumberFormat.NumberGroupSeparator
Set-Content "C:\Users\maxim\Downloads\Telegram Desktop\DecSep\$p1.txt" $p2
Set-Content -Path "C:\Users\maxim\Downloads\Telegram Desktop\GrSep\$p1.txt" -Value "$p3" -Encoding "utf8"
}