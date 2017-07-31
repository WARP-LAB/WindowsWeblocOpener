# Windows Webloc Opener

**Open `.webloc` files (macOS URL shortcut files) in Microsoft Windows in default browser.**

Simple middleware app that parses `.webloc` file (macOS URL shortcut file) and passes found URL to a system call that opens the URL in default web browser.

## Install

* Go to this repo releases tab or follow [this link](https://github.com/WARP-LAB/WindowsWeblocOpener/releases/latest) to download latest `WindowsWeblocOpener-<version>.zip`.
* Unzip the release.
* Copy `WindowsWeblocOpener` directory containing `WindowsWeblocOpener.exe` somewhere, say `C:\Program Files (x86)\WindowsWeblocOpener\WindowsWeblocOpener.exe`  .
* Right click `.webloc` file and choose *Open With / Choose another app*.
* In the opened dialog window check *Always use this app to open .webloc files*.
* In the app selection list click *More apps* and scroll down for *Look for another app on this PC*.
* Navigate to *WindowsWeblocOpener.exe* and select it.
* Profit.

## Compatibility

Tested on

* Windows 10 x64
* Webloc files created by Safari on macOS
* Webloc files created by Chrome on macOS
* Webloc files created by Firefox on macOS

*Interweb tells that the current format for weblocs was introduced in Mac OS X 10.4.*

## Todo / enhancements / help wanted

Get rid of console window that appears for a split of a second.  
Does that mean rewriting this to `WinMain` routine (meaning C++?) and building w/ `/link /subsystem:windows /entry:mainCRTStartup`?

## Building 

App was built using [CL (Compiler Command-Line Syntax)](https://msdn.microsoft.com/en-us/library/610ecb4h.aspx) through [Developer Command Prompt](https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs) on a MSW10 machine that has *Visual Studio Community 2017* on it.

### Production

Open Developer Command Prompt

Create directory `Z:\Path\To\WindowsWeblocOpener\build\prod` as `cl` does not like to put products under non existing dirs.

```
cd X:\Path\To\WindowsWeblocOpener\src
cl main.c /Fe..\build\prod\WindowsWeblocOpener /DDEBUG=0
```

### Develop & debug

Open Developer Command Prompt

```
cd X:\Path\To\WindowsWeblocOpener\src
cl main.c /FeWindowsWeblocOpener
WindowsWeblocOpener.exe Z:\Path\To\WindowsWeblocOpener\testfiles\safari.webloc
```
