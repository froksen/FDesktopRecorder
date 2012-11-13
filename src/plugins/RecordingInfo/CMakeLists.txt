MESSAGE( "Compiling Plugin: RecordingInfo" )
include ( ${QT_USE_FILE} ${KDE4Defaults})
include_directories (
	${CMAKE_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
	)
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	${QT_KDE4_INCLUDE_DIR} ${QT_KDE4_LIBRARY} ${KDE4_INCLUDES} ${KDE4_KDECORE_LIBS} )
	
add_definitions(${QT_DEFINITIONS})
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)
add_definitions(-DQT_NO_DEBUG)
cmake_minimum_required(VERSION 2.8)
set(myplugin_SRCS
    recordinginfo.cpp
)

set(myplugin_MOC_HDRS
    recordinginfo.h
	)

	

    
QT4_WRAP_CPP(myplugin_MOC_SRCS ${myplugin_MOC_HDRS})
add_library(recordinginfo SHARED ${myplugin_SRCS} ${myplugin_MOC_SRCS})
target_link_libraries(recordinginfo   ${QT_LIBRARIES} ${QT_QTXML_LIBRARY})

INSTALL (TARGETS recordinginfo LIBRARY DESTINATION share/fdesktoprecorder/plugins)
