#!/bin/sh
#
# Script to prepare the release for VISHNU source code (All modules)
# Author K. Coulomb (kevin.coulomb@sysfera.com)
# Date 18/04/11
#

if [ ! $# -eq 1 ]; then
  echo "Usage: " $0 "<no_version: 1.0.x>"
  exit 1
else
  NO_VERSION=$1
  echo "Preparing VISHNU v" $NO_VERSION "..."
fi

if [ ! -f copyright ]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi

# Create temporary directory
rm -rf /tmp/prepaRel
mkdir /tmp/prepaRel
path=/tmp/prepaRel/VISHNU_$NO_VERSION
mkdir $path

# Copy root cmake list, copyright, README
cp CMakeLists.txt $path/
cp copyright $path/
cp README $path/

# Copy doxygen generator
cp Doxyfile $path/
cp Doxyfile_API $path/

# Copy doxygen documentation file
cp -r doc $path/

# Copy the licence
cp -r Licence $path/

# Copy Cmake find files
cp -r Cmake $path/

# Copy sql scripts
mkdir $path/core
mkdir $path/core/database
cp -r core/database/database_init.sql $path/core/database/
cp -r core/database/postgre_create.sql $path/core/database/

# Copy core sources
mkdir $path/core/src/
mkdir $path/core/src/config
mkdir $path/core/src/database
mkdir $path/core/src/exception
mkdir $path/core/src/registry
mkdir $path/core/src/utils
mkdir $path/core/test
mkdir $path/core/test/cfgs
mkdir $path/core/test/src
cp core/src/CMakeLists.txt $path/core/src/
cp core/src/config/CMakeLists.txt $path/core/src/config/
cp core/src/database/CMakeLists.txt $path/core/src/database/
cp core/src/exception/CMakeLists.txt $path/core/src/exception/
cp core/src/registry/CMakeLists.txt $path/core/src/registry/
cp core/src/utils/CMakeLists.txt $path/core/src/utils/
cp core/test/CMakeLists.txt $path/core/test/
cp core/test/src/CMakeLists.txt $path/core/test/src/
cp core/test/cfgs/CMakeLists.txt $path/core/test/cfgs/
cp core/src/config/*pp $path/core/src/config/
cp core/src/database/*pp $path/core/src/database/
cp core/src/exception/*pp $path/core/src/exception/
cp core/src/registry/*pp $path/core/src/registry/
cp core/src/utils/*pp $path/core/src/utils/
cp core/src/utils/*i $path/core/src/utils/
cp core/test/src/*pp $path/core/test/src/
cp core/test/src/*h.in $path/core/test/src/
cp core/test/cfgs/*cfg.in $path/core/test/cfgs/
cp -r core/test/third-party $path/core/test

###############################################################################
#                                    U M S                                    #
###############################################################################

# Copy the sendmail script
cp core/src/utils/sendmail.py $path/core/src/utils/

# Copy UMS sources
mkdir $path/UMS
mkdir $path/UMS/src/
cp UMS/src/CMakeLists.txt $path/UMS/src/
cp -r UMS/src/api $path/UMS/src
cp -r UMS/src/cli $path/UMS/src
cp -r UMS/src/client $path/UMS/src
cp -r UMS/src/sed $path/UMS/src
cp -r UMS/src/server $path/UMS/src
cp -r UMS/src/utils $path/UMS/src

# Copy UMS man
mkdir $path/UMS/doc/
mkdir $path/UMS/doc/man

# Copy tests
mkdir $path/UMS/test
mkdir $path/UMS/test/src
cp -r UMS/test/src/sql $path/UMS/test/src
cp UMS/test/src/*pp $path/UMS/test/src
cp UMS/test/src/*.in $path/UMS/test/src
cp UMS/test/src/CMakeLists.txt $path/UMS/test/src

# Copy man pages
cp  UMS/doc/man/CMakeLists.txt $path/UMS/doc/man/
cp  -r UMS/doc/man/man1 $path/UMS/doc/man/
cp  -r UMS/doc/man/man3 $path/UMS/doc/man/

###############################################################################
#                                    T M S                                    #
###############################################################################

mkdir $path/TMS
mkdir $path/TMS/src/
cp TMS/src/CMakeLists.txt $path/TMS/src/
cp -r TMS/src/api $path/TMS/src
cp -r TMS/src/cli $path/TMS/src
cp -r TMS/src/client $path/TMS/src
cp -r TMS/src/sed $path/TMS/src
cp -r TMS/src/server $path/TMS/src
cp -r TMS/src/utils $path/TMS/src
cp -r TMS/src/utils_torque $path/TMS/src
cp -r TMS/src/slave $path/TMS/src

# Copy tests
mkdir $path/TMS/test
mkdir $path/TMS/test/src
cp -r TMS/test/src/sql $path/TMS/test/src
cp TMS/test/src/*pp $path/TMS/test/src
cp TMS/test/src/config.h.in $path/TMS/test/src
cp TMS/test/src/CMakeLists.txt $path/TMS/test/src

# Copy man pages
mkdir $path/TMS/doc/
mkdir $path/TMS/doc/man
cp  TMS/doc/man/CMakeLists.txt $path/TMS/doc/man/
cp  -r TMS/doc/man/man1 $path/TMS/doc/man/
cp  -r TMS/doc/man/man3 $path/TMS/doc/man/

###############################################################################
#                                 All modules                                 #
###############################################################################

cp scripts/setCopyright.sh $path/

cd $path

./setCopyright.sh copyright

rm setCopyright.sh

cd -

# Copy emf4cpp files
mkdir $path/core/deps/
cp -r core/deps/emf4cpp $path/core/deps/

cd $path

rm -f tmp.txt

# Remove .project
rm -f UMS/src/utils/emfdata/UMS_Data/.project
rm -f UMS/src/utils/emfdata/UMS_Data/.cproject
rm core/deps/emf4cpp/.project
rm core/deps/emf4cpp/.cproject

# remove all ~ files
for i in  $(find . -name "*~") ; do rm $i;  done ;

# remove all bak files
for i in  $(find . -name "*bak") ; do rm $i;  done ;

# remove all build dir
for i in  $(find . -name "*build") ; do rm -rf $i;  done ;

cd ..

# Archive to send
tar -czvf VISHNU_v${NO_VERSION}.tgz VISHNU_$NO_VERSION

# Moving archive in /tmp
mv VISHNU_v${NO_VERSION}.tgz /tmp/

