@echo off
echo ---------------------------------------------------
echo Updating JMultiSliceLib C++ Source Backups.
echo Warning: This is a service tool for developer only.
echo ---------------------------------------------------
echo Removing previous zip files.
del "JMSLib_SRC_Backup.zip" /Q
echo ---------------------------------------------------
echo Zipping up all files:
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.f"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.in"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.h"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.cpp"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.cuh"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.cu"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.rtf"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.txt"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.manifest"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.bat"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "*.md"
"C:\Program Files\7-Zip\7z.exe" a -r "JMSLib_SRC_Backup.zip" "LICENSE"
echo ---------------------------------------------------
echo Done.