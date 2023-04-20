# Instructions for Building MPAS-Ocean to Run in a Wilkins Workflow

Installation is done through Spack. If you don't have Spack installed or if Spack is new to you, go [here](https://spack.readthedocs.io/en/latest/) first.

Clone this repository and cd into it. These instructions assume there is a top-level directory called climate.

```
mkdir ~/climate
cd ~/climate
git clone https://github.com/tpeterka/mpas-o-workflow
cd mpas-o-workflow
```

-----

## Setting up Spack environment

### First time: create and load the Spack environment for MPAS-Ocean

```
cd ~/climate/mpas-o-workflow
source ./create-mpas.sh     # requires being in the same directory to work properly
```

### Subsequent times: load the Spack environment for MPAS-Ocean

```
source ~/climate/mpas-o-workflow/load-mpas.sh
```

-----

## Building MPAS-Ocean

### First time: clone MPAS-Ocean

```
cd ~/climate
git clone https://github.com/E3SM-Project/E3SM
cd E3SM
git submodule update --init --recursive
```

## First time: modify MPAS-Ocean makefiles to link to Henson

Edit ~climate/E3SM/components/mpas-ocean/Makefile:

Insert at line 596:
`LIBS += -L $(HENSON)/lib -lhenson`

Insert at line 732:
`LDFLAGS += -shared`

Edit line 1002 to add .so to executable name: `$(EXE_NAME).so`

### Build MPAS-Ocean

```
cd ~/climate/E3SM/components/mpas-ocean
make clean              # if dirty
make -j gfortran
```
This will take ~ 5 minutes to compile.

### Create a run script for MPAS-Ocean

Edit (create) `~/climate/E3SM/components/mpas-ocean/ocean_model`:

`python3 ~/climate/mpas-o-workflow/mpas-henson.py`

Set permissions of `ocean_model` to executable:

`chmod 755 ~/climate/E3SM/components/mpas-ocean/ocean_model`

-----

## Setting up a test case to execute

Compass is an E3SM system for generating and running test cases for MPAS-Ocean, and relies on conda environments. The instructions below assume you have conda or miniconda already installed. If not, go [here](https://docs.conda.io/en/latest/miniconda.html) first.

### First time: install Compass and create Compass environment

```
cd ~
git clone https://github.com/MPAS-Dev/compass.git compass-env-only
cd ~/compass-env-only
git submodule update --init --recursive
./conda/configure_compass_env.py --conda ~/miniconda3 --env_only
source load_dev_compass_1.2.0-alpha.4.sh        # load_dev_compass-1.2.0-alpha.4.sh is the script created by the previous command
```

### First time: create a compass configuration file for a new machine

Assumes the config file is named ~/compass-env-only/compass.cfg and has these contents, or similar (yours may vary)

```
# This file contains some common config options you might want to set

# The paths section describes paths to databases and shared compass environments
[paths]

# A root directory where MPAS standalone data can be found
database_root = /home/tpeterka/compass/mpas_standalonedata

# The parallel section describes options related to running tests in parallel
[parallel]

# parallel system of execution: slurm or single_node
system = single_node

# whether to use mpirun or srun to run the model
parallel_executable = mpiexec

# cores per node on the machine, detected automatically by default
# cores_per_node = 4
```

### First time: create test case for the executable

Assumes that `load_dev_compass_1.2.0-alpha.4.sh` is the name of the conda environment load script created initially

```
source ~/compass-env-only/load_dev_compass_1.2.0-alpha.4.sh
compass setup -t ocean/baroclinic_channel/10km/default -w ~/spack-baroclinic-test -p ~/climate/E3SM/components/mpas-ocean -f ~/compass-env-only/compass.cfg
```
Set the output file type for the test case:

Edit `~/spack-baroclinic-test/ocean/baroclinic_channel/10km/default/forward/streams.ocean`.

Add `io_type="netcdf4">` to the `<stream name="output"` section of the file:

```
<stream name="output"
        type="output"
        filename_template="output.nc"
        filename_interval="01-00-00_00:00:00"
        reference_time="0001-01-01_00:00:00"
        clobber_mode="truncate"
        precision="double"
        output_interval="0000_00:00:01"
        io_type="netcdf4">

    <var_struct name="tracers"/>
    <var name="xtime"/>
    <var name="normalVelocity"/>
    <var name="layerThickness"/>
</stream>
```

If you want to use the output for particle tracing, append additional stream `mesh` and additional variables `ssh`, `normalTransportVelocity`, `vertTransportVelocityTop`, and `zTop` to the `stream name="output"` section of
the `streams.ocean` file:

```
<stream name="output"
        type="output"
        filename_template="output.nc"
        filename_interval="01-00-00_00:00:00"
        reference_time="0001-01-01_00:00:00"
        clobber_mode="truncate"
        precision="double"
        output_interval="0000_00:00:01"
        io_type="netcdf4">

    <var_struct name="tracers"/>
    <var name="xtime"/>
    <var name="normalVelocity"/>
    <var name="layerThickness"/>
    <stream name="mesh"/>
    <var name="ssh"/>
    <var name="normalTransportVelocity"/>
    <var name="vertTransportVelocityTop"/>
    <var name="zTop"/>
</stream>
```

### Run the test case

Assumes that `load_dev_compass_1.2.0-alpha.4.sh` is the name of the conda environment load script created initially

```
source ~/compass-env-only/load_dev_compass_1.2.0-alpha.4.sh
source ~/climate/mpas-o-workflow/load-mpas.sh
cd ~/spack-baroclinic-test/ocean/baroclinic_channel/10km/default
compass run
```



