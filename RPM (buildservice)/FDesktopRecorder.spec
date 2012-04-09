#
# spec file for package 
#
# Copyright (c) 2010 SUSE LINUX Products GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

# norootforbuild

Name:           FDesktopRecorder
Version:	20120409
Release:	1
License:	GNU GPLv2
Summary:	A program to record the screen
#Url:		
#Group:
Source:		FDesktopRecorder.tar.gz
Source2:	fdesktoprecorder.png
Source3:	FDesktopRecorder.desktop
Source4: 	FDesktopRecorder.notifyrc
#Patch:
#BuildRequires:	desktop-file-utils, libqt4-devel, gcc-c++, make, gcc46
BuildRequires:	desktop-file-utils, libqt4-devel, gcc-c++, make, gcc46, libkde4-devel 
Requires:	ffmpeg
#PreReq:
#Provides:
Packager:	Ole Holm "Froksen" Frandsen
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
A program that can record a single window or your entire desktop using ffmpeq. Based on a script by Peter Lyberth from the danish GNU/Linux show "Kanal Tux" http://www.kanaltux.dk 


%prep
%setup -q

%build
qmake FDesktopRecorder.pro
make

%install
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/applications
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/pixmaps
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/kde4/apps/FDesktopRecorder

install -p -m 755 FDesktopRecorder $RPM_BUILD_ROOT/%{_bindir}

desktop-file-install                                    \
--dir=${RPM_BUILD_ROOT}%{_datadir}/applications         \
%{SOURCE3}

install -p %{SOURCE2} ${RPM_BUILD_ROOT}%{_datadir}/pixmaps/
install -p %{SOURCE4}  ${RPM_BUILD_ROOT}%{_datadir}/kde4/apps/FDesktopRecorder/

%clean
%{?buildroot:%__rm -rf "%{buildroot}"}

%postun

%files
#%defattr(-,root,root)
#%doc ChangeLog README COPYING
/usr/bin/FDesktopRecorder*
/usr/share/applications/FDesktopRecorder.desktop*
/usr/share/pixmaps/fdesktoprecorder.png*
/usr/share/kde4
/usr/share/kde4/apps
/usr/share/kde4/apps/FDesktopRecorder
/usr/share/kde4/apps/FDesktopRecorder/FDesktopRecorder.notifyrc*

%changelog
* Mon Apr 09 2012 Ole Holm Frandsen
- Added a simple player for previewing your latest recording
- Added a Messagebox to help you out how to record a "single window"
- Changed icon that is displayed when recording
- Various layout changes 
- Danish translation updated
- Other minor change
s
* Fri Mar 9 2012 Ole Holm Frandsen
- Added KNotification events if recording successfully finished or if it failes to start 
- Fixed some spellingmistakes
- Fixed: MainWindow no longer is "squashed" when console is opened
- Fixed: MainWindow no longer "jumps" up and down when consolewindow is opened and closed
- Under-the-hood: Minor preformence improvements, better use of build-in Qt-methods

* Wed Feb 15 2012 Ole Holm Frandsen
- Settings: Possible to change audiosource
- Settings: Added support for -apre and -vpre statements
- Fixed: Reads number of audiochannels propperly
- Fixed: Writes "latest recording date" propperly
- Alot of under-the-hood improvements.

* Thu Feb 09 2012 Ole Holm Frandsen
- Added: Settings: If resert button is pressed you will see a dialog where you have to accept.
- Added: Settings: You can now change the application language.
- Fixed: If recording, the statusbar will keep on showing this.
- Fixed: When "stop" button is pressed, it will be disabled this prevens crashes.
- Fixed: In settings: If basename is blank it will use default.
- Changed: If recording is successfull, the filename is visible in the Gui, makes it easy to find.
- Changed: desktopfile description
- Minor changes.

* Mon Feb 06 2012 Ole Holm Frandsen
- Created a completly new GUI-design and a new way to think the program
- Rewritten alot of the codebase
- Fixed so programs saves the recordings propperly
- Fixed problem with aspects of some resolutions
- Added so that the filebasename can be the current date

* Sat Jan 28 2012 Ole Holm Frandsen
- MainWindow: Changed so that it uses buildin Qt function to determine fullscreen resolution.
- MainWindow: Alerts if the filename/recording already exists
- MainWindow: The "Browse button" is now named "Browse" WOW! :-D
- Settings: Change default startup name
- Settings: Change default startup format
- Settings: Change default startup recording device
- Systemtray: Remembers the latest recording.
- Added German translation by Christian Räss (saftsocken)
- Other minor fixes

* Tue Jan 17 2012 Ole Holm Frandsen
- Added more videooutput formats.
- Some "comments" cleanups
- Minor fixes

* Sat Jan 07 2012 Ole Holm Frandsen
- Fixed program in "settings" which meant that you couldnt change audiocodec
- Fixed so if want to record a "single window" the dimensions is correct
- Added so you let ffmpeg auto-detect video and audio codec
- Merged "Application settings" and "Settings" into one dialog. 


* Wed Jan 04 2012 Ole Holm Frandsen
- Added "Application settings" section
- Fixed problem with "incorrect" resolutions
- Fixed so that "custom resolution" works again
- Updated danish translation
- Fixed problem with QSettings
- Minor fixes

* Thu Dec 29 2011 Ole Holm Frandsen
- No more temp-scripts, everything runs directly in Qt. This means better performance and alot of issues fixed.
- Minor fixes

* Thu Dec 29 2011 Ole Holm Frandsen
- Fixed so that the program does not crash when opening the settingsdialog if settings.ini file does not exists.
- Renamed the project to "FDesktopRecorder"
- Fixes minor issues with the danish translation
- Minor fixes

* Wed Dec 28 2011 Ole Holm Frandsen
- First english release
