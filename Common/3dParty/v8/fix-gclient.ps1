$gclient_path = "depot_tools\gclient.bat"
(Get-Content $gclient_path) | ForEach-Object { $_ -replace "^(`"%~dp0python`")", "call $&" } | Set-Content $gclient_path
