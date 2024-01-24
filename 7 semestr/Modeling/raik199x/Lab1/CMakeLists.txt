cmake_minimum_required(VERSION 3.5)

project(Lab1 VERSION 0.1 LANGUAGES CXX)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/include/thirdParty/qcustomplot/qcustomplot.cpp")
    message("${CMAKE_SOURCE_DIR}/include/thirdParty/qcustomplot/qcustomplot.cpp was not found. Internet connection and git command required.")
    execute_process(
        COMMAND wget -P ${CMAKE_SOURCE_DIR}/include/thirdParty https://www.qcustomplot.com/release/2.1.1/QCustomPlot.tar.gz
    )
    execute_process(
        COMMAND tar -xvzf ${CMAKE_SOURCE_DIR}/include/thirdParty/QCustomPlot.tar.gz -C ${CMAKE_SOURCE_DIR}/include/thirdParty/
    )
    execute_process(
        COMMAND rm ${CMAKE_SOURCE_DIR}/include/thirdParty/QCustomPlot.tar.gz
    )
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH /home/raik199x/Qt/6.5.0/gcc_64)

set(BUILD_TESTING OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)
find_package(OpenMP REQUIRED)

include_directories(backend frontend include include/thirdParty/qcustomplot)

set(PROJECT_SOURCES
        main.cpp

        frontend/mainwindow.cpp

        frontend/TableWidget.cpp
        frontend/LehmerWidget.cpp
        frontend/MicWidget.cpp
        frontend/MiddleSquaresWidget.cpp

        backend/RandomGenerators/Lehmer.cpp
        backend/RandomGenerators/Mic.cpp
        backend/RandomGenerators/MiddleSquares.cpp
        backend/RandomGenerators/Cplusplus.cpp


        backend/Nists.cpp
        backend/Threading.cpp

        backend/NistOperationalFunctions/MatrixOperations.cpp
        backend/NistOperationalFunctions/GraphicsOperations.cpp
        backend/NistOperationalFunctions/FileOperatons.cpp
        backend/NistOperationalFunctions/MathOperations.cpp
        backend/NistOperationalFunctions/AlgorithmOperations.cpp

        include/thirdParty/qcustomplot/qcustomplot.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lab1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Lab1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Lab1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Lab1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Lab1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    OpenMP::OpenMP_CXX
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Lab1)
endif()
set_target_properties(Lab1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Lab1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Lab1)
endif()
