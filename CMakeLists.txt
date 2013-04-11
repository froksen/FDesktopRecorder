project( fdesktoprecorder )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
find_package ( KDE4 REQUIRED) 
find_package (ALSA REQUIRED)

include ( ${QT_USE_FILE} ${KDE4Defaults})
include_directories (
	${CMAKE_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
	)
	
add_subdirectory(src)
add_subdirectory(desktop)
add_subdirectory(knotification)
