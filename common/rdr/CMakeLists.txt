include_directories(${CMAKE_SOURCE_DIR}/common ${ZLIB_INCLUDE_DIRS})

add_library(rdr STATIC
  Exception.cxx
  FdInStream.cxx
  FdOutStream.cxx
  FileInStream.cxx
  HexInStream.cxx
  HexOutStream.cxx
  InStream.cxx
  RandomStream.cxx
  TLSException.cxx
  TLSInStream.cxx
  TLSOutStream.cxx
  ZlibInStream.cxx
  ZlibOutStream.cxx)

set(RDR_LIBRARIES ${ZLIB_LIBRARIES} os)
if(WIN32 AND NOT BUILD_STATIC)
  add_win_dll_dep(zlib1.dll)
endif()
if(GNUTLS_FOUND)
  set(RDR_LIBRARIES ${RDR_LIBRARIES} ${GNUTLS_LIBRARIES})
  if(WIN32 AND NOT BUILD_STATIC)
    add_win_dll_dep(libgnutls-30.dll)
  endif()
endif()
if(WIN32)
	set(RDR_LIBRARIES ${RDR_LIBRARIES} ws2_32)
endif()

target_link_libraries(rdr ${RDR_LIBRARIES})

if(UNIX)
  libtool_create_control_file(rdr)
endif()
