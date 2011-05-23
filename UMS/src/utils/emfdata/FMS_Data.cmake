#
# FMS_Data.cmake
# Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
#
# EMF4CPP is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# EMF4CPP is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -funroll-loops")

set(FMS_Data_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataPackage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataPackageImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataFactoryImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StringList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StringListImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStat.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStatImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/CpFileOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/CpFileOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsDirOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsDirOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/MvFileOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/MvFileOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsTransferOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsTransferOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStatList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStatListImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StopTransferOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StopTransferOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransfer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransferImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransferList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransferListImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/HeadOfFileOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/HeadOfFileOptionsImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/TailOfFileOptions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/TailOfFileOptionsImpl.cpp
   )
   
set(FMS_Data_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data_forward.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataPackage.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataFactory.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StringList.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStat.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/CpFileOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsDirOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/MvFileOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsTransferOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStatList.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StopTransferOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransfer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransferList.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/HeadOfFileOptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/TailOfFileOptions.hpp
   )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data_forward.hpp DESTINATION include/emf4cpp/)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataFactory.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FMS_DataPackage.hpp DESTINATION include/emf4cpp/FMS_Data)   
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StringList.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStat.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/CpFileOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsDirOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/MvFileOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/LsTransferOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileStatList.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/StopTransferOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransfer.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/FileTransferList.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/HeadOfFileOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/FMS_Data/TailOfFileOptions.hpp DESTINATION include/emf4cpp/FMS_Data)
   
include_directories(${CMAKE_CURRENT_SOURCE_DIR} /home/ibrahima/Projets/ProjetEDF/Vishnu/core/deps/emf_bin/../include/emf4cpp /home/ibrahima/Projets/ProjetEDF/Vishnu/core/deps/emf_bin/../include/emf4cpp)

add_library(emf4cpp-FMS_Data SHARED ${FMS_Data_HEADERS} ${FMS_Data_SOURCES})
set_target_properties(emf4cpp-FMS_Data PROPERTIES VERSION 0.0.1 SOVERSION 1)

install(TARGETS emf4cpp-FMS_Data DESTINATION lib)

