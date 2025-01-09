#!/bin/bash

unset PROFILE_PRELIB

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

spack add mpich@4
spack add hdf5@1.14+hl+mpi
# spack develop lowfive@master build_type=Debug
spack add lowfive
spack add wilkins
spack add henson+python+mpi-wrappers
# spack develop netcdf-c@main+mpi build_system=cmake build_type=Debug
# spack add netcdf-c@main+mpi
spack add netcdf-c@4.9+mpi build_system=cmake
spack add parallel-netcdf
spack add netcdf-fortran@4.5.3
# spack develop mpas-o-scorpio@master+hdf5 build_type=Debug
spack add mpas-o-scorpio+hdf5

# following is for optional debugging; comment out if not needed
# spack add gdb
# spack add cgdb
# spack add tmux

# the following is for optional particle tracing with ftk; comment out if not needed
# spack add cuda                  # for GCE
spack add cuda@12.2             # for perlmutter
spack add vtk@9.2.2
spack develop ndarray+hdf5+netcdf+mpi+cuda+vtk build_type=Debug
spack add ndarray+hdf5+netcdf+mpi+cuda+vtk
spack develop ftk@mpas+cuda ^ndarray+hdf5+netcdf+mpi+cuda+vtk build_type=Debug
spack add ftk@mpas+cuda ^ndarray+hdf5+netcdf+mpi+cuda+vtk

# install everything in environment
echo "installing dependencies in environment"
spack install henson        # install henson so that henson path is set
export HENSON=`spack location -i henson`
spack install

spack env deactivate
