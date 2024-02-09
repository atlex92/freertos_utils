file(GLOB_RECURSE SOURCES  *.cpp *.c)
set(srcs ${SOURCES})

set(includedirs
inc
)

idf_component_register(INCLUDE_DIRS ${includedirs} SRCS ${srcs} REQUIRES)