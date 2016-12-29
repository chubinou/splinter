cmake_minimum_required(VERSION 2.8.12)
project(Splinter)


find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Qml Quick)

set(CMAKE_AUTORCC ON)

set(tutorial_SRCS
  main.cpp
  osd.h
  osd.cpp
  splitmodel.h
  splitmodel.cpp
  runner.h
  runner.cpp
  server.h
  server.cpp
  splitmodellist.h
  splitmodellist.cpp
  utils.h
  utils.cpp
  qml.qrc
  )

set(QML_SRC
  main.qml
  )

add_executable(tutorial ${tutorial_SRCS})

target_link_libraries(
  tutorial
  PRIVATE
  Qt5::Qml Qt5::Quick
  )
