$gclient_path = "depot_tools\gclient.bat"
$fetch_path = "depot_tools\fetch.bat"
(Get-Content $gclient_path) | ForEach-Object { $_ -replace "^(`"%~dp0python`")", "call $&" } | Set-Content $gclient_path
(Get-Content $fetch_path) | ForEach-Object { $_ -replace "^(%~dp0python)", "call $&" } | Set-Content $fetch_path
