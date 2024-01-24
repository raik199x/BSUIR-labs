cmake_minimum_required(VERSION 3.5)

project(Lab2 VERSION 0.1 LANGUAGES CXX)

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

include_directories(backend frontend include)

set(PROJECT_SOURCES
        main.cpp

        frontend/mainwindow.cpp

        frontend/LehmerWidget.cpp
        frontend/MiddleSquaresWidget.cpp

        frontend/SelectionSizeWidget.cpp
        frontend/DistributionMenu.cpp
        frontend/MathStats.h

        backend/RandomGenerators/Lehmer.cpp
        backend/RandomGenerators/MiddleSquares.cpp
        backend/RandomGenerators/distributions.cpp

        backend/OperationalFunctions/GraphicsOperations.cpp

        include/thirdParty/qcustomplot/qcustomplot.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lab2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(Lab2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport OpenMP::OpenMP_CXX ${GSL_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Lab2)
endif()
set_target_properties(Lab2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Lab2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Lab2)
endif()
