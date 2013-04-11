include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	)
	
#Installs the desktopfile
install( FILES fdesktoprecorder.notifyrc DESTINATION share/kde4/apps/fdesktoprecorder)
