#!/bin/bash

export SPACKENV=mpas
spack env deactivate > /dev/null 2>&1

# set spack locations and vars for building mpas-o
unset MPAS_EXTERNAL_LIBS
unset MPAS_EXTERNAL_LIBS
unset NETCDF
unset NETCDFF
unset PNETCDF
unset PIO
unset HDF5
unset LOWFIVE
unset HENSON
unset USE_PIO2
unset OPENMP
unset HDF5_USE_FILE_LOCKING
# unset MPAS_SHELL=/bin/bash
unset CORE
unset SHAREDLIB
unset PROFILE_PRELIB


unset HDF5_PLUGIN_PATH
unset HDF5_VOL_CONNECTOR

unset OMP_NUM_THREADS
