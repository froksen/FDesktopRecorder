currentdate=$(date '+%Y%m%d')
copyfrom="../../FDesktopRecorder/source"
copyto="FDesktopRecorder"
newfoldername="FDesktopRecorder-"$currentdate

updatetarball()
{
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


osc()
{
  build()
  {
     echo "Building"
     xterm -e "osc build openSUSE_12.1 x86_64 FDesktopRecorder.spec"
  }
  commit()
  {
    echo "Commiting! Please enter Message"
    xterm -e "osc commit"
    
    echo "Copying OSC files to project homefolder!"
    cd ..
    cp -rf "FDesktopRecorder" "../FDesktopRecorder/RPM (buildservice)"
  }


  while true; do
      read -p "OSC: * (B)uild for openSUSE * (C)ommit to OSC" bc
      case $bc in
	  [Bb]* ) build; break;;
	  [Cc]* ) commit;;
	  * ) echo "Valid option";;
      esac
  done 
}


while true; do
    read -p "* (U)pdate tarball * (E)dit specfile *(O)SC" ueo
    case $ueo in
        [Uu]* ) updatetarball; break;;
        [Ee]* ) editspecfile; break;;
        [Oo]* ) osc;;
        * ) echo "Valid option";;
    esac
done