include_directories( . )

set ( OWN_SOURCES main.cpp grid.cpp )
set ( OWN_HEADERS 
  ../include/grid.h
  ../include/ray.h
	)
  
  
  
  add_executable( adpvt ${OWN_SOURCES} ${OWN_HEADERS} )
 
