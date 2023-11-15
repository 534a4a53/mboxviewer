#
# Example script to open selected mail file
# The unicode-open-selected-mail-file.ps1 must be UCS-2 LE BOM encoded to support Unicode encoded file names .
# You can use open-selected-mail-file.cmd for non unicode encoded mail file names.
#
# You can use free Notepad++ to create UCS-2 LE BOM encoded ps1 file
# Start Nodepad++ and select New file to create new file.
# Open "Encoding" option and select "UCS-2 LE BOM" encoding
# User double quotion marks "" to quote a string or variable within quoted string
#
# Run "Powershell .\unicode-open-selected-mail-file.ps1 from Command Window"

# Set Set-PSDebug to 2 to trace each line execution. It is similar to cmd "@echo on"
Set-PSDebug -Trace 0

$procdir = "F:\Documents\GIT1.0.3.40\mboxviewer\x64\Debug"

$mailFile = "F:\MBOX\Temu拼多多陪你迎兔年\Temu拼多多陪你迎兔年 - Copy.eml"
	
Start-Process -FilePath "$procdir\mboxview.exe" -wait -args "-MAIL_FILE=""$name"""


