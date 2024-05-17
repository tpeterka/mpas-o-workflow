#!/bin/bash

export SPACKENV=mpas
export YAML=$PWD/env.yaml

# create spack environment
echo "creating spack environment $SPACKENV"
spack env deactivate > /dev/null 2>&1
spack env remove -y $SPACKENV > /dev/null 2>&1
spack env create $SPACKENV $YAML

# activate environment
echo "activating spack environment"
spack env activate $SPACKENV

# spack develop lowfive@master build_type=Debug
spack add lowfive
spack add wilkins
spack add henson+python+mpi-wrappers

# spack develop netcdf-c@4.9+mpi
# spack add netcdf-c@4.9+mpi cflags='-g'
spack add netcdf-c@4.9+mpi

# spack develop mpas-o-scorpio@master+hdf5 build_type=Debug
spack add mpas-o-scorpio+hdf5

# the following is for particle tracing with ftk; comment out if not needed
# spack add cuda
# spack add vtk@9.2.2
# spack add ndarray+hdf5+netcdf+mpi+cuda+vtk
# spack develop ftk@mpas+cuda ^ndarray+hdf5+netcdf+mpi+cuda+vtk
# spack add ftk@mpas+cuda ^ndarray+hdf5+netcdf+mpi+cuda+vtk

# install everything in environment
echo "installing dependencies in environment"
spack install

# reset the environment (workaround for spack behavior)
spack env deactivate
spack env activate $SPACKENV

# set build flags
echo "setting flags for building MPAS-Ocean"
export MPAS_EXTERNAL_LIBS=""
export MPAS_EXTERNAL_LIBS="${MPAS_EXTERNAL_LIBS} -lgomp"
export NETCDF=`spack location -i netcdf-c`
export NETCDFF=`spack location -i netcdf-fortran`
export PNETCDF=`spack location -i parallel-netcdf`
export PIO=`spack location -i mpas-o-scorpio`
export HDF5=`spack location -i hdf5`
export LOWFIVE=`spack location -i lowfive`
export HENSON=`spack location -i henson`
export USE_PIO2=true
export OPENMP=false
export HDF5_USE_FILE_LOCKING=FALSE
export MPAS_SHELL=/bin/bash
export CORE=ocean
export SHAREDLIB=true
export PROFILE_PRELIB="-L$HENSON/lib -lhenson-pmpi"

# set LD_LIBRARY_PATH
echo "setting flags for running MPAS-Ocean"
export LD_LIBRARY_PATH=$NETCDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PNETCDF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$NETCDFF/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HDF5/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PIO/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LOWFIVE/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HENSON/lib:$LD_LIBRARY_PATH

# enable VOL plugin
export HDF5_PLUGIN_PATH=$LOWFIVE/lib
export HDF5_VOL_CONNECTOR="lowfive under_vol=0;under_info={};"

# give openMP 1 core for now to prevent using all cores for threading
# could set a more reasonable number to distribute cores between mpi + openMP
export OMP_NUM_THREADS=1
