cmake_minimum_required(VERSION 3.22)

project(Photobox VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

find_package( OpenCV REQUIRED )
#find_package( Gphoto2 REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(PROJECT_SOURCES
        main.cpp
        gui.cpp
        gui.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Photobox
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        imageprozessor.h imageprozessor.cpp
        cameracontroler.h cameracontroler.cpp
        metaoverlay.h metaoverlay.cpp
    )
endif()

target_link_libraries(Photobox PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                        Qt${QT_VERSION_MAJOR}::PrintSupport
                        ${OpenCV_LIBS}
                        gphoto2)

include(GNUInstallDirs)
install(TARGETS Photobox
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Photobox)
endif()
