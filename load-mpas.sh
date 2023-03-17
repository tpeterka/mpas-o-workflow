#!/bin/bash

# activate the environment
export SPACKENV=mpas
spack env deactivate > /dev/null 2>&1
spack env activate $SPACKENV
echo "activated spack environment $SPACKENV"

# set spack locations and vars for building mpas-o
export MPAS_EXTERNAL_LIBS=""
export MPAS_EXTERNAL_LIBS="${MPAS_EXTERNAL_LIBS} -lgomp"
export NETCDF=`spack location -i netcdf-c`
export NETCDFF=`spack location -i netcdf-fortran`
export PNETCDF=`spack location -i parallel-netcdf`
export PIO=`spack location -i scorpio`
export HDF5=`spack location -i hdf5`
export LOWFIVE=`spack location -i lowfive`
export HENSON=`spack location -i henson`
export USE_PIO2=true
export OPENMP=true
export HDF5_USE_FILE_LOCKING=FALSE
export MPAS_SHELL=/bin/bash
export CORE=ocean
export SHAREDLIB=true
echo "environment variables are set for building MPAS-Ocean"

# set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$NETCDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$NETCDFF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HDF5/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PIO/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LOWFIVE/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HENSON/lib:$LD_LIBRARY_PATH
echo "library paths are set for running MPAS-Ocean"

export HDF5_PLUGIN_PATH=$LOWFIVE/lib
export HDF5_VOL_CONNECTOR="lowfive under_vol=0;under_info={};"
echo "environment variables are set for running LowFive"

