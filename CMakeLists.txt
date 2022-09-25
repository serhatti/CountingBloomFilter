cmake_minimum_required (VERSION  3.0)

project(BloomFilter)

set(CMAKE_CXX_FLAGS "-pedantic -Werror=conversion -Wshadow -Werror=float-equal -Wextra -Wfatal-errors -O3 -Wall")

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX  ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING " " FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

install(DIRECTORY BloomFilter DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/data)

add_subdirectory(${PROJECT_SOURCE_DIR}/test)

#deploy a helper setup script
FILE(WRITE ${CMAKE_INSTALL_PREFIX}/setup.sh
"export BF_DICT=${CMAKE_INSTALL_PREFIX}/data/words.txt\n"
"export PATH=\${PATH}:${CMAKE_INSTALL_PREFIX}/bin"
)
