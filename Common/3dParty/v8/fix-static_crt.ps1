$gn_path = "v8\build\config\win\BUILD.gn"
(Get-Content $gn_path) | ForEach-Object { $_ -replace ":static_crt", ":dynamic_crt" } | Set-Content $gn_path
