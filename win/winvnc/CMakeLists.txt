include_directories(${CMAKE_BINARY_DIR}/win ${CMAKE_CURRENT_SOURCE_DIR})

# advapi32 needs to come before kernel32 in some cases for Windows 7 compatibility.
#  Windows 10 kernel32.dll exports a reference to CreateProcessAsUserA whereas Windows 7 advapi32.dll is the only place that function is exported.
#  Changing the order of the linked libraries allows the binary to be compatible with both versions.
set(CMAKE_CXX_STANDARD_LIBRARIES "-ladvapi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32" CACHE STRING "" FORCE)

add_executable(winvnc4 WIN32
  buildTime.cxx
  ControlPanel.cxx
  ManagedListener.cxx
  QueryConnectDialog.cxx
  STrayIcon.cxx
  VNCServerService.cxx
  VNCServerWin32.cxx
  winvnc.cxx
  winvnc.rc)

target_link_libraries(winvnc4 rfb rfb_win32 network rdr ws2_32.lib)

install(TARGETS winvnc4
  RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
)
