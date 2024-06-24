cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH <path_to_qt>/Qt/<qt_version>/<compiler>/lib/cmake)
set(CMAKE_PREFIX_PATH "C:/5.15.11_x64/lib/cmake")
project(syncConfig_UI)

# Set up Qt
find_package(Qt5 COMPONENTS Core Gui Widgets QUIET)
find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(Qt5_FOUND)
	message(STATUS "Qt5 found")
	set(EZ_FOUND_QT_VERSION Qt5)
elseif(Qt6_FOUND)
	message(STATUS "Qt6 found")
	set(EZ_FOUND_QT_VERSION Qt6)
else()
	message(FATAL_ERROR "Qt5 or Qt6 not found! If you have Qt5 or Qt6 installed, please set CMAKE_PREFIX_PATH to hold Qt cmake directory. Example:\n'set(CMAKE_PREFIX_PATH <path_to_qt>/Qt/<qt_version>/<compiler>/lib/cmake)'")
endif()

# Set up build type
set(EZ_BUILD_TYPE "Release" CACHE STRING "Sets build type: 'Debug' or 'Release'")
if(${EZ_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE Debug)
elseif(${EZ_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_BUILD_TYPE Release)
else()
    message(FATAL_ERROR "Invalid build type '${EZ_BUILD_TYPE}' specified. Use 'Debug' or 'Release'")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}. You can change this by specifying -DEZ_BUILD_TYPE='Debug' or 'Release'")

# Set up sources
set(EZ_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
#set(EZ_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(EZ_SOURCES "${EZ_SOURCES_DIR}/Main.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/MainWindow.cpp"
			   "${EZ_SOURCES_DIR}/Data/MainController.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/FieldsWidget.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/OptionsWidget.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/AddOptionsWidget.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/ConnectionWidget.cpp"
			   "${EZ_SOURCES_DIR}/Widgets/DetailsContainer.cpp"
			   "${EZ_SOURCES_DIR}/Data/items/ConnectionItem.cpp"
			   "${EZ_SOURCES_DIR}/Data/items/FieldItem.cpp"
			   "${EZ_SOURCES_DIR}/Data/items/OptionItem.cpp"
			   "${EZ_SOURCES_DIR}/Data/items/AdditionalOptionItem.cpp"
			   "${EZ_SOURCES_DIR}/Data/items/RootItem.cpp"
			   "${EZ_SOURCES_DIR}//Data/controller/ConnectionController.cpp"
			   "${EZ_SOURCES_DIR}//Data/controller/FieldsController.cpp"
			   "${EZ_SOURCES_DIR}/Data/GenerateItems.cpp"
			   "${EZ_SOURCES_DIR}/Data/Parser.cpp"
			   "${EZ_SOURCES_DIR}/Data/Tables.cpp"
			   "${EZ_SOURCES_DIR}/Data/TreeItem.cpp"
			   "${EZ_SOURCES_DIR}/Data/TreeModel.cpp")
set(EZ_HEADERS "${EZ_SOURCES_DIR}/Widgets/MainWindow.h"
			   "${EZ_SOURCES_DIR}/Data/MainController.h"
			   "${EZ_SOURCES_DIR}/Widgets/FieldsWidget.h"
			   "${EZ_SOURCES_DIR}/Widgets/OptionsWidget.h"
			   "${EZ_SOURCES_DIR}/Widgets/AddOptionsWidget.h"
			   "${EZ_SOURCES_DIR}/Widgets/ConnectionWidget.h"
			   "${EZ_SOURCES_DIR}/Widgets/DetailsContainer.h"
			   "${EZ_SOURCES_DIR}/Data/ConnectionController.h"
			   "${EZ_SOURCES_DIR}/Data/FieldsController.h"
			   "${EZ_SOURCES_DIR}/Data/Parser.h"
			   "${EZ_SOURCES_DIR}/Data/TreeItem.h"
			   "${EZ_SOURCES_DIR}/Data/DerivedItems.h"
			   "${EZ_SOURCES_DIR}/Data/TreeModel.h"
			   "${EZ_SOURCES_DIR}/Data/Tables.h")
set(EZ_MOC_SOURCES "${EZ_SOURCES_DIR}/Widgets/MainWindow.h"
                   "${EZ_SOURCES_DIR}/Widgets/ConnectionWidget.h"
                   "${EZ_SOURCES_DIR}/Widgets/OptionsWidget.h"
                   "${EZ_SOURCES_DIR}/Widgets/AddOptionsWidget.h"
                   "${EZ_SOURCES_DIR}/Widgets/FieldsWidget.h"
                   "${EZ_SOURCES_DIR}/Widgets/DetailsContainer.h"
                   "${EZ_SOURCES_DIR}/Data/ConnectionController.h"
                   "${EZ_SOURCES_DIR}/Data/FieldsController.h"
                   "${EZ_SOURCES_DIR}/Data/TreeModel.h")
set(EZ_UIC_SOURCES "${EZ_SOURCES_DIR}/MainWindow.ui"
				   "${EZ_SOURCES_DIR}/Fields.ui"
				   "${EZ_SOURCES_DIR}/SaveOption.ui"
				   "${EZ_SOURCES_DIR}/Options.ui"
				   "${EZ_SOURCES_DIR}/AddOptions.ui"
				   "${EZ_SOURCES_DIR}/DetailsContainer.ui"
                   "${EZ_SOURCES_DIR}/Connection.ui")
				   
file(GLOB HEADER_FILES "${EZ_SOURCES_DIR}/*.h")
file(GLOB HEADER_FILES_DATA "${EZ_SOURCES_DIR}/Data/*.h")
file(GLOB HEADER_FILES_WIDGETS "${EZ_SOURCES_DIR}/Widgets/*.h")
file(GLOB HEADER_FILES_AUTOGEN "${EZ_SOURCES_DIR}/autogen/*.h")

file(GLOB SOURCE_FILES "${EZ_SOURCES_DIR}/*.cpp")
file(GLOB SOURCE_FILES_DATA "${EZ_SOURCES_DIR}/Data/*.cpp")
file(GLOB SOURCE_FILES_ITEMS "${EZ_SOURCES_DIR}/Data/items/*.cpp")
file(GLOB SOURCE_FILES_CONTROLLER "${EZ_SOURCES_DIR}/Data/controller/*.cpp")
file(GLOB SOURCE_FILES_WIDGETS "${EZ_SOURCES_DIR}/Widgets/*.cpp")
file(GLOB SOURCE_FILES_UI "${EZ_SOURCES_DIR}/*.ui")

source_group("Header Files" FILES ${HEADER_FILES})
source_group("Header Files\\Widgets" FILES ${HEADER_FILES_WIDGETS})
source_group("Header Files/Data" FILES ${HEADER_FILES_DATA})
source_group("Header Files/GeneratedFiles" FILES ${EZ_SOURCES_DIR}/autogen/ui_connection.h})
source_group("GeneratedFiles" REGULAR_EXPRESSION "(\\ui).*(.h$)|(\\mocs_).*(.cpp$)|\\.stamp$|\\.rule$")

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Source Files/Widgets" FILES ${SOURCE_FILES_WIDGETS})
source_group("Source Files/Data" FILES ${SOURCE_FILES_DATA})
source_group("Source Files/Data/Items" FILES ${SOURCE_FILES_ITEMS})
source_group("Source Files/Data/Controller" FILES ${SOURCE_FILES_CONTROLLER})
source_group("ui Files" FILES ${SOURCE_FILES_UI})

# Set up executable
set(EZ_EXECUTABLE_NAME ${PROJECT_NAME})
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUTCONFIG}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUTCONFIG}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUTCONFIG}")
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)
add_executable(${EZ_EXECUTABLE_NAME} ${EZ_SOURCES} ${EZ_HEADERS} ${EZ_MOC_SOURCES} ${EZ_UIC_SOURCES})
set_target_properties(${EZ_EXECUTABLE_NAME} PROPERTIES AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/autogen")
target_link_libraries(${EZ_EXECUTABLE_NAME} ${EZ_FOUND_QT_VERSION}::Core ${EZ_FOUND_QT_VERSION}::Gui ${EZ_FOUND_QT_VERSION}::Widgets)
target_include_directories(${PROJECT_NAME}

    PUBLIC
        Main/include
    $<$<CONFIG:DEBUG>:${CMAKE_CURRENT_SOURCE_DIR}/autogen/include_Debug> 
    $<$<CONFIG:RELEASE>:${CMAKE_CURRENT_SOURCE_DIR}/autogen/include_Release>
)

# Set up compiler options
if(MSVC)
    target_compile_options(${EZ_EXECUTABLE_NAME} PRIVATE
		/W3
		#/WX
		/permissive
	)
else()
    target_compile_options(${EZ_EXECUTABLE_NAME} PRIVATE
        -Wall
        -Wextra
        -Werror
        -pedantic
        -Wno-unused-variable
        -Wno-unused-parameter
        -Wshadow
        -Wformat=2
    )
endif()

# Set up startup project for Visual Studio
if("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EZ_EXECUTABLE_NAME})
endif()
