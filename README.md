pinso
=====

A fault localization system that precisely pinpoints the root cause of concurrency bugs.
The goal of Pinso is to help programmers triage a concurrency
bug by analyzing a failed execution and automatically isolate
its root causes. It takes advantage of the fact that concurrency
bugs are non-deterministic. For a given bug triggering input,
there correspond a huge number of legal thread interleavings,
many of which tend to produce correct executions. Our observation
shows that, correct interleavings tend to manifest much
more frequently than the incorrect interleavings for a given
bug triggering input, especially in applications that adhere
to strict software quality assurance process. Pinso exploits
the above observation to isolate the root causes of a failed
execution.

1.1 OS and kernel
=================

Currently, the tool only supports Linux.

Tested Arch:
  x86_64

Tested OS:
  Redhat Enterprise Linux 5.4
  
  Ubuntu Desktop 10.10

1.2 Building essentials
=======================

GNU make version 3.81 or higher.

PIN binary instrumentation tool, latest version.

Google protobuf tool, version 2.4.1 or higher.

1.3 Target programs
===================

In general, our tool can test any x86 binary with the following
conditions:
  1) Should use dynamic linking
  2) Preferably use -fno-omit-frame-pointer when compile
  3) Preferably use pthread library for threading and synchronization


#######################################
# 2. How to build the package         #
#######################################

1) Download and install
   a) the latest PIN binary instrumentation tool
   b) the latest Google protobuf tool

2) Set environment variable PIN_HOME and PROTOBUF_HOME accordingly

3) Make the package

   $ make compiletype=debug
   (To build the debug version)

   $ make compiletype=release
   (To build the release version. Slow assertions are removed.)
