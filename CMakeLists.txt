cmake_minimum_required(VERSION 2.8.12)
project(Splinter)

include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Qml Quick)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

set(tutorial_SRCS
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
