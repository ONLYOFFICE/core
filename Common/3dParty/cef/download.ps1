Add-Type -AssemblyName System.IO.Compression.FileSystem

# ----------------------------------------------------------------------------------------------
# download a file
# ----------------------------------------------------------------------------------------------
function Download-File
{
	param ([string]$url,[string]$file)
	
	$downloadRequired = $true
	if (Test-Path $file)
	{
		$localModified = (Get-Item $file).CreationTime
		$webRequest = [System.Net.HttpWebRequest]::Create($url)
		$webRequest.Method = "HEAD"
		$webResponse = $webRequest.GetResponse()
		$remoteLastModified = ($webResponse.LastModified) -as [DateTime] 
		$webResponse.Close()

		if ($remoteLastModified -gt $localModified)
		{
			Write-Host "$file is out of date"
		}
		else
		{
			$downloadRequired = $false
		}
	}

	if ($downloadRequired)
	{
		$clnt = new-object System.Net.WebClient
		Write-Host "Downloading from $url to $file"
		$clnt.DownloadFile($url, $file)
	}
	else
	{
		Write-Host "$file is up to date."
	}
	
	return $downloadRequired
}

# ----------------------------------------------------------------------------------------------
# unzip a file
# ----------------------------------------------------------------------------------------------
function RemoveCef
{
	param([string]$outpath)

	if (Test-Path $outpath)
	{
		Write-Host "Remove folder $outpath"
    Remove-Item $outpath -Force -Recurse
	}
}

$output = Download-File -url $args[0] -file $args[1]
if ($output)
 {
	 RemoveCef -outpath $args[2]
 }