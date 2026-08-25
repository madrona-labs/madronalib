@echo off
REM Windows Terminal entry point for rebuild-vs.ps1. The execution policy on a
REM stock Windows install blocks .ps1 files, so run it through an explicit bypass.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0rebuild-vs.ps1" %*
