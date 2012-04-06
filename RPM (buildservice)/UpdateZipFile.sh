currentdate=$(date '+%Y%m%d')
copyfrom="/home/froksen/ludvikling/FDesktopRecorder/source"
copyto="FDesktopRecorder"
newfoldername="FDesktopRecorder-"$currentdate

updatetarball()
{
  #removes the old tarball
  rm FDesktopRecorder.tar.gz

  #Copies the folder
  cp -rf $copyfrom $copyto

  #renames the folder
  mv "FDesktopRecorder" $newfoldername

  #Creates the tarball
  tar cvzf FDesktopRecorder.tar.gz $newfoldername

  #Removes the tempfolders
  rm -rf $newfoldername
}

editspecfile()
{
  vim "FDesktopRecorder.spec"
}

osc_build()
{
    echo "Building"
    xterm -e "osc build openSUSE_12.1 x86_64 FDesktopRecorder.spec"
}

osc_commit()
{
  echo "Commiting! Please enter Message"
  xterm -e "osc commit"
  
  echo "Copying OSC files to project homefolder!"
  cd ..
  cp -rf "FDesktopRecorder" "../FDesktopRecorder/RPM (buildservice)"
}

do_all()
{
  updatetarball
  editspecfile
  osc_build
  osc_commit
}

while true; do
    read -p "* Do (A)ll * (U)pdate tarball * (E)dit specfile *OSC (B)uild for openSUSE 12.1 *OSC (C)ommit" uebca
    case $uebca in
	[Aa]* ) do_all; break;;
        [Uu]* ) updatetarball; break;;
        [Ee]* ) editspecfile; break;;
	[Cc]* ) osc_commit; break;;
        [Bb]* ) osc_build;;
        * ) echo "Valid option";;
    esac
done