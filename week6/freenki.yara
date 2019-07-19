rule Freenki
{
  strings:
    $reg = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run" wide
    $regkey = "runsample" wide
    $getaddapteraddr_fail = "Call to GetAdaptersAddresses failed.\n" ascii
    $screenshot_fail = "failed to take the screenshot. err: %d\n" ascii

  condition:
    $reg and $regkey and $getaddapteraddr_fail and $screenshot_fail
}
