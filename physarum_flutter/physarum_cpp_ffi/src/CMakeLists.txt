# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(physarum_cpp_ffi_library VERSION 0.0.1 LANGUAGES CXX)

add_library(physarum_cpp_ffi SHARED
  "physarum_cpp_ffi.cpp"
  
  "AgentGraphAnalyser.cpp"
  "FlutterAdapter.cpp"
  "Generator.cpp"
  "Location.cpp"
  "SimulationInOutput.cpp"
  "SlimeAgent.cpp"
  "SlimeAgentsFabric.cpp"
  "SlimeMoldSimulation.cpp"
)

set_target_properties(physarum_cpp_ffi PROPERTIES
  PUBLIC_HEADER SlimeMoldClass.h
  OUTPUT_NAME "physarum_cpp_ffi"
)

target_compile_definitions(physarum_cpp_ffi PUBLIC DART_SHARED_LIB)
