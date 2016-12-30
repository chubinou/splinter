cmake_minimum_required(VERSION 2.8.12)
project(Splinter)

include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Widgets)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(splinter_SRCS
  main.cpp
  osd.h
  osd.cpp
  splitmodel.h
  splitmodel.cpp
  splitmodellist.h
  splitmodellist.cpp
  splitparser.h
  splitparser.cpp
  runner.h
  runner.cpp
  server.h
  server.cpp
  utils.h
  utils.cpp
  )

set(QML_SRC
  main.qml
  Section.qml
  MouseMenu.qml
  )

qt5_add_resources(RESOURCES qml.qrc)
add_executable(splinter ${splinter_SRCS} ${RESOURCES})

target_link_libraries(
  splinter
  PRIVATE
  Qt5::Qml Qt5::Quick Qt5::Widgets
  )
