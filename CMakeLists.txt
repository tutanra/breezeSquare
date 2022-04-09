project(breezeSquare)
set(PROJECT_VERSION "5.12.80")
set(PROJECT_VERSION_MAJOR 5)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

include(WriteBasicConfigVersionFile)
include(FeatureSummary)

  find_package(ECM 0.0.9 REQUIRED NO_MODULE)
  set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_SOURCE_DIR}/cmake)

  include(ECMInstallIcons)
  include(KDEInstallDirs)
  include(KDECMakeSettings)
  include(KDECompilerSettings NO_POLICY_SCOPE)
  include(GenerateExportHeader)
 
find_package(KDecoration2 REQUIRED)

add_definitions(-DTRANSLATION_DOMAIN="breeze_kwin_deco")

#find_package(KF5 REQUIRED COMPONENTS CoreAddons GuiAddons ConfigWidgets WindowSystem I18n)
#find_package(Qt5 CONFIG REQUIRED COMPONENTS DBus)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons GuiAddons ConfigWidgets WindowSystem I18n IconThemes)
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS DBus)

### XCB
find_package(XCB COMPONENTS XCB)
set_package_properties(XCB PROPERTIES
  DESCRIPTION "X protocol C-language Binding"
  URL "https://xcb.freedesktop.org"
  TYPE OPTIONAL
  PURPOSE "Required to pass style properties to native Windows on X11 Platform"
)

if(UNIX AND NOT APPLE)

  set(BREEZE_HAVE_X11 ${XCB_XCB_FOUND})
  if (XCB_XCB_FOUND)
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS X11Extras)
  endif()

else()

  set(BREEZE_HAVE_X11 FALSE)

endif()

add_subdirectory(libbreezecommon)

################# configuration #################
configure_file(config-breeze.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-breeze.h )

################# includes #################
include_directories(${CMAKE_SOURCE_DIR}/libbreezecommon)
include_directories(${CMAKE_BINARY_DIR}/libbreezecommon)

################# newt target #################
### plugin classes
set(breezedecoration_SRCS
    breezebutton.cpp
    breezedecoration.cpp
    breezeexceptionlist.cpp
    breezesettingsprovider.cpp
    breezesizegrip.cpp
    breezeboxshadowrenderer.cpp
    breezetextbutton.cpp
    breezeappmenubuttongroup.cpp
    breezeappmenumodel.cpp
    breezemenuoverflowbutton.cpp
    
)

kconfig_add_kcfg_files(breezedecoration_SRCS breezeSquaresettings.kcfgc)

### config classes
### they are kept separately because they might move in a separate library in the future
set(breezedecoration_config_SRCS
    config/breezeconfigwidget.cpp
    config/breezedetectwidget.cpp
    config/breezeexceptiondialog.cpp
    config/breezeexceptionlistwidget.cpp
    config/breezeexceptionmodel.cpp
    config/breezeitemmodel.cpp
)

set(breezedecoration_config_PART_FORMS
   config/ui/breezeconfigurationui.ui
   config/ui/breezedetectwidget.ui
   config/ui/breezeexceptiondialog.ui
   config/ui/breezeexceptionlistwidget.ui
)

ki18n_wrap_ui(breezedecoration_config_PART_FORMS_HEADERS ${breezedecoration_config_PART_FORMS})

include(ECMQtDeclareLoggingCategory)
add_subdirectory(libdbusmenuqt)

### build library
add_library(breezeSquaredecoration MODULE
    ${breezedecoration_SRCS}
    ${breezedecoration_config_SRCS}
    ${breezedecoration_config_PART_FORMS_HEADERS})

target_link_libraries(breezeSquaredecoration
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::DBus
    PRIVATE
        dbusmenuqt
        breezecommon5
        KDecoration2::KDecoration
        KF5::ConfigCore
        KF5::CoreAddons
        KF5::ConfigWidgets
        KF5::GuiAddons
        KF5::I18n
        KF5::IconThemes
        KF5::WindowSystem)

if(BREEZE_HAVE_X11)
  target_link_libraries(breezeSquaredecoration
    PUBLIC
      Qt::X11Extras
      XCB::XCB)
endif()

install(TARGETS breezeSquaredecoration DESTINATION ${KDE_INSTALL_PLUGINDIR}/org.kde.kdecoration2)
install(FILES config/breezeSquaredecorationconfig.desktop DESTINATION  ${KDE_INSTALL_KSERVICES5DIR})
