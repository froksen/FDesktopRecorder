include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	)
	
#Installs the desktopfile
install( FILES fdesktoprecorder.desktop DESTINATION share/applications/)
install( FILES fdesktoprecorder.png DESTINATION share/pixmaps/)
