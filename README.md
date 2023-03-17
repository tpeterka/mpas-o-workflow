# Instructions for Building and Running MPAS-Ocean in a Wilkins Workflow

Installation is done through Spack. If you don't have Spack installed or if Spack is new to you, go [here](https://spack.readthedocs.io/en/latest/) first.

Clone this repository and cd into it.

-----

## Setting up Spack  environment

### First time: create and load the Spack environment for MPAS-Ocean

```
source create-mpas.sh
```

### Subsequent times: load the Spack environment for MPAS-Ocean

```
source load-mpas.sh
```

-----

## Building MPAS-Ocean

### First time: clone MPAS-Ocean

```
git clone https://github.com/E3SM-Project/E3SM
cd E3SM
git submodule update --init --recursive
```

### Build MPAS-Ocean

```
cd components/mpas-ocean
make clean              # if dirty
make gfortran
```
-----

## Setting up a test case to execute

### First time: install compass and create compass environment

```
git clone https://github.com/MPAS-Dev/compass.git compass-env-only
cd compass-env-only
git submodule update --init --recursive
./conda/configure_compass_env.py --conda ~/miniconda3 --env_only
source load_dev_compass_1.2.0-alpha.4.sh
```

### Create test case for the executable

```
source ~/compass-env-only/load_dev_compass_1.2.0-alpha.4.sh
compass setup -t ocean/baroclinic_channel/10km/default -w ~/spack-baroclinic-test -p ~/climate/E3SM/components/mpas-ocean -f ~/compass-env-only/compass.cfg
```

### Run the test case

```
source ~/compass-env-only/load_dev_compass_1.2.0-alpha.4.sh
source ~/climate/load-mpas.sh
cd ~/spack-baroclinic-test/ocean/baroclinic_channel/10km/default
compass run
```



