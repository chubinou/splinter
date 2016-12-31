About
=====

Splinter is a simple timer for speedrunners that works on Linux

This tool has been created because most tools existing are either for windows
only (wsplit, livesplit, ...) or aren't displayed properly over fullscreen
applications (llianfair ...)

Splinter runs and has been tested on Linux but it should works on other
platforms too.

Compiling
=========

You need to have cmake and Qt5 dev (>= 5.6) pacakges installed.

compile steps are:

.. code::

   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make

Splits configuration
====================

Splinter uses json files to define game properties and splits, and should look like

.. code:: json

   {
     "game": "the game title",
     "category" : "the category runned",
     "splits" : [
         {"title" : "world 1"},
         {"title" : "world 2"},
         {"title" : "world 3"},
         {"title" : "the end"}
     ]
   }

configuration can either be loaded at startup using program flag "--config" or
using context menu

Split server
============

Splinter can be remote controlled via TCP/IP, the default port is **16834**. at
the moment this is the only way to notify splits times.

the protocol is line based, and commands are in the form:

.. code::

   "command\r\n"

or

.. code::

   "command arg arg...\r\n"

when responses are issued, they also are in the form

.. code::

   "value\r\n"

It uses the same protocol as livesplit server and should be compatible with most
existing scripts.

At the moment, following commands are implemented. they should be self
explanatory:

* starttimer
* startorsplit
* split
* unsplit
* skipsplit
* pause
* resume
* reset
* getcurrenttime (return the time in miliseconds)
* getcurrenttimerphase (return NotRunning, Running, Ended, Paused)

a sample split script is provided in the sample folder
