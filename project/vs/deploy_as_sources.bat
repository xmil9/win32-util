::
:: Deploys project and sources of the win32-util library to a given
:: directory. Will create a subdirectory for the library.
:: Example: > .\deploy_as_sources.bat C:\libs
::
@echo off
setlocal

set outDir=%1
set thisDir=%~dp0

set src=%thisDir%..\..
set dest=%outDir%\win32_util

:: Deploy code.
mkdir %dest%
robocopy %src% %dest% *.h
robocopy %src% %dest% *.cpp

:: Deploy project.
set from=%src%\project\vs
set to=%dest%\project\vs
mkdir %to%
robocopy %from% %to% win32_util.vcxproj
robocopy %from% %to% win32_util.vcxproj.filters
robocopy %from% %to% deploy_as_sources.bat

:: Deploy dependencies by calling deployment scripts of each dependency.
call %src%\dependencies\essentutils\project\vs2019\deploy_as_sources.bat %dest%\dependencies
