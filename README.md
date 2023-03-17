# Instructions for building and running MPAS-Ocean in a Wilkins Workflow

Installation is done through Spack. If you don't have Spack installed or if Spack is new to you, go [here](https://spack.readthedocs.io/en/latest/) first.

# Clone this repository and cd into it

# First time: create and load the Spack environment for MPAS-Ocean

```
source create-mpas.sh
```

# Subsequent times: load the Spack environment for MPAS-Ocean

```
source load-mpas.sh
```

# First time: clone MPAS-Ocean

```
git clone https://github.com/E3SM-Project/E3SM
cd E3SM
git submodule update --init --recursive
```

# Build MPAS-Ocean

```
cd components/mpas-ocean
make clean              # if dirty
make gfortran
```

