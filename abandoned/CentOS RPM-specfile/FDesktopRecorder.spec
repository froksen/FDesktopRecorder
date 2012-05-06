######################################################
# SpecFile: FDesktopRecorder.spec 
# Generato: http://www.mandrivausers.ro/
# MRB-Falticska Florin
######################################################
%define use_ccache	1
%define ccachedir	~/.ccache-OOo%{distsuffix}
%{?_with_ccache: %global use_ccache 1}
%{?_without_ccache: %global use_ccache 0}
#stella is a centos6 desktop remix by nux
%define  dist	stella
%define debug_package	%{nil}


Vendor:		MandrivaUsers.Ro <http://www.mandrivausers.ro/>
Name:		FDesktopRecorder
Version:	20120505
Release:	1%{?dist}
License:	GPLv2
Summary:	A program to record the screen
URL:		https://github.com/froksen/FDesktopRecorder/zipball/kdeapps-version
Group:		Applications/Multimedia
Source0:	https://github.com/froksen/FDesktopRecorder/tree/kdeapps-version/FDesktopRecorder-%{version}.tar.gz
Source1:	icon.png
BuildRequires:	qt4-devel, gcc-c++, make, kdelibs4-devel, cmake, kde-settings, desktop-file-utils
Requires:	ffmpeg, 
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Provides:	fdesktoprecorder = %{version}

%description
A program that can record a single window or your entire desktop using ffmpeq. 
Based on a script by Peter Lyberth from the 
danish GNU/Linux show "Kanal Tux" http://www.kanaltux.dk 


%prep
%setup -q -n %{name}-%{version}
rm -rf 'RPM (buildservice)'
rm -rf 'Desktop'
rm -rf source/FDesktopRecorder.pro
chmod 755 source 

%build
cd source 
%cmake_kde4  
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT

# biaries
mkdir -p $RPM_BUILD_ROOT%{_bindir}
install -p -m 755 source/build/FDesktopRecorder $RPM_BUILD_ROOT%{_bindir}

#knotify
%__install -d -m755 $RPM_BUILD_ROOT%{_datadir}/apps/%{name}
install -p  -m 755 knotification/FDesktopRecorder.notifyrc $RPM_BUILD_ROOT%{_datadir}/apps/%{name}

# menu entry
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/applications
cat << EOF > %{buildroot}%{_datadir}/applications/%{name}.desktop
[Desktop Entry]
Type=Application
Categories=AudioVideo;
Name=FDesktopRecorder
Comment=A program to record the screen
Exec=FDesktopRecorder
Icon=FDesktopRecorder
StartupNotify=true
Terminal=false
EOF


mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/pixmaps
install -p %{SOURCE1} ${RPM_BUILD_ROOT}%{_datadir}/pixmaps/%{name}.png

desktop-file-validate %{buildroot}%{_datadir}/applications/%{name}.desktop

%post
update-desktop-database &> /dev/null

%postun
update-desktop-database &> /dev/null

%clean 
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc CHANGELOG README TODO
%{_bindir}/FDesktopRecorder
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/apps/%{name}/FDesktopRecorder.notifyrc


%changelog
* Sat May 05 2012 Falticska Florin <symbianflo@fastwebnet.it>  20120505-69.1stella
- New git version:
 - Added: When recording a single window, there will be a red rectangle arround this. Created by Juergen Heinemann (Undefined).
 - Added: Tooltips to the preview player
 - Added: Possible to use another player for previewing your recordings, like Kaffeine or vlc.
 - Fixed: Text/Tooltips in Settingsdialog should now make more sense.
 - Fixed: In preview player the content is now always placed in the middle of the widget
 - Fixed: You can now only open one "preview player" at the time
 - Changed: Now uses icons from your KDE/GTK theme in the toolbar and systemtray.
 - Other minor changes.
- Backport for stella project
- MRB-Mandriva Users.Ro

* Mon Apr 09 2012 Falticska Florin <symbianflo@fastwebnet.it>  20120409-69.2stella
- New git version
 - Added a simple player for previewing your latest recording
 - Added a Messagebox to help you out how to record a "single window"
 - Changed icon that is displayed when recording
 - Various layout changes 
 - Danish translation updated
 - Other minor changes
- Backport for stella project
- MRB-Mandriva Users.Ro

* Thu Mar 22 2012 Falticska Florin <symbianflo@fastwebnet.it>  20120322-69.2stella
-New git version
 - Added KNotification events if recording successfully finished or if it failes to start 
 - Fixed some spellingmistakes
 - Fixed: MainWindow no longer is "squashed" when console is opened
 - Fixed: MainWindow no longer "jumps" up and down when consolewindow is opened and closed
 - Under-the-hood: Minor preformence improvements, better use of build-in Qt-methods
- Backport for stella project
- MRB-Mandriva Users.Ro

* Wed Feb 15 2012 Falticska Florin <symbianflo@fastwebnet.it>  20120215-69stella
- New version:
 - Settings: Possible to change audiosource
 - Settings: Added support for -apre and -vpre statements
 - Fixed: Reads number of audiochannels propperly
 - Fixed: Writes "latest recording date" propperly
 - Alot of under-the-hood improvements
- MRB-Mandriva Users.Ro

* Tue Feb 07 2012 Falticska Florin <symbianflo@fastwebnet.it> - 20120206-69.1stella
- Backport for stella project
- MRB-Mandriva Users.Ro

* Mon Feb 06 2012 Falticska Florin <symbianflo@fastwebnet.it>  20120206-69mrb-2011.0
 - Created a completly new GUI-design and a new way to think the program
 - Rewritten alot of the codebase
 - Fixed so programs saves the recordings propperly
 - Fixed problem with aspects of some resolutions
 - Added so that the filebasename can be the current date
- First build for mandriva
- MRB-Mandriva Users.Ro





