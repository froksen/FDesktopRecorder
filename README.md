#NOTE: As of today (19. August 2013) Artur Lacki (Lacky) have taken over development and mangement of this project. He has decided to use Bitbucket instead of Github. So for more information and latest sourcecode please visit: https://bitbucket.org/Lacky/fdesktoprecorder/


#FDesktopRecorder

##Description
FDesktopRecorder is a program that can video record a single window or your entire desktop using FFmpeg.

###This can be handy for:
* "How-to's"
* Product demoes
* in-game videoes
* Educational videoes 
* Recording Interactive whiteboards like "SMART Boards"
* etc.

###Basic features:
* Video record a single window
* Video record your entire desktop
* Video record a region of your desktop

Based on a script by Peter Lyberth from the danish GNU/Linux show "Kanal Tux" http://www.kanaltux.dk 

##About
Created by: Ole "Froksen" Holm Frandsen
You can contact me via kde-apps.org. Find me under the username "froksen".

##Translations
For information about who has translated this program. Please visit Transifex: https://www.transifex.com/projects/p/FDesktopRecorder/

###Help with translation?
All translation is done via Transifex, please visit the project page at: https://www.transifex.com/projects/p/FDesktopRecorder/

##Development
###'master' branch
This branch contains the latest "released" version. Since this is a hoppy project and much more, I will not call this branch "stabile" but "usable" :-)
If you want to run or package the project, this is the branch for you.

###'development' branch
This branch contains changes made since the last release and contains all active development. Although I try to make everything run, there is absolut no garantie. Features might be added, changed or deleted again :-)
When I release a new version, the development branch will be merged into the master branch.
If you want to send pull requests or want to help with coding and so on, this is the branch for you :-)

###Requires to run
* ffmpeq is used for the recording
* libqt4
* libkde4
* xwininfo
* Optional:
  - kaffeine (the default preview player)

###Note to packagers
Currently CMake and QMake are equally supported. Although I might move towards only using CMake in the future. So if you haven't packaged the FDesktopRecorder before, use
CMake to build the program.

Please also read the "KNotifications" section below.
  
###Requires to build 
(NOTE: these are openSUSE 12.1 package names)
  ```
desktop-file-utils, libqt4-devel, gcc-c++, make, gcc46, libkde4-devel
  ```
###KNotifications
#### QMake 
Please note, that for Knotfications to work the "FDesktopRecorder.notifyrc" file must be placed in "/usr/share/kde4/apps/FDesktopRecorder". If Folder does not exists then create it.
This is currently beeing done for openSUSE by the RPM-file.

This way of installing the *.notifyrc file might change in the future.. :-)

#### CMake 
If you are using CMake to build this project, you can just do "# make install" after the "CMake-steps" below. 
This will install the *.notifyrc file in the correct place. If you are building an package (.rpm,.deb etc), please also read the "QMake" section above.

###How to build/compile via qmake
  ```
    $ cd <FDesktopRecorder-folder>
    $ cd source
    $ qmake FDesktopRecorder.pro
    $ make
  ```
###How to build/compile via CMake from/in version 20121012
  ```
    $ cd <FDesktopRecorder-folder>
    $ mkdir build
    $ cd build/
    $ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
    $ make
  ```
  (You can do "make install" after this to install)