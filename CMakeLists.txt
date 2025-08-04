cmake_minimum_required(VERSION 3.16)

project(Learning_project VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

file(GLOB UI_FILES "./UIFile/*.ui")
file(GLOB SOURCES "./Source/*.cpp")
file(GLOB HEADERS "./Header/*.h")

file(GLOB JPG_FILES 
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}  # 关键：添加RELATIVE参数
    "Resources/images/*.jpg"
)
file(GLOB JPEG_FILES 
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "Resources/images/*.jpeg"
)
file(GLOB PNG_FILES 
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "Resources/images/*.png"
)
file(GLOB ICO_FILES 
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "Resources/images/*.ico"
)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/UIFile 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Header
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(IMAGE_FILES ${JPG_FILES} ${JPEG_FILES} ${PNG_FILES} ${ICO_FILES})

qt_add_executable(Learning_project
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    main.cpp
    mainwindow.ui
)

qt_add_resources(Learning_project "app_resources"
    PREFIX "/"
    FILES ${IMAGE_FILES}
)

target_link_libraries(Learning_project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(Learning_project PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Header
    ${CMAKE_CURRENT_SOURCE_DIR}/UIFile
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Learning_project PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Learning_project PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Learning_project
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
