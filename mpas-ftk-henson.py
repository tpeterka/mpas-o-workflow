#!/usr/bin/env python3

from mpi4py import MPI
import pyhenson as h
import lowfive
from pathlib import Path
import os

world = MPI.COMM_WORLD.Dup()
size = world.Get_size()

passthru = True
consumer_procs = 1 # make sure consumer_procs + MPAS_O procs = size

# pm = h.ProcMap(world, [("producer", size)])
# pm = h.ProcMap(world, [("consumer", size)])
pm = h.ProcMap(world, [("producer", size - consumer_procs), ("consumer", consumer_procs)])

nm = h.NameMap()

if pm.group() == "producer":
    tag = 0
#     lowfive.create_logger("info")
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("consumer", tag))
    if passthru:
        vol.set_passthru("*", "*")
    else:
        vol.set_memory("*", "*")
    # set the following path to point to your installation of E3SM
#     prod = h.Puppet(str(Path.home()) + "/climate/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    prod = h.Puppet(str(Path.home()) + "/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    "namelist.ocean", "-s", "streams.ocean"], pm, nm)
    prod.proceed()
    if passthru:
        h.to_mpi4py(pm.intercomm("consumer", tag)).barrier()
else:
    tag = 0
#     lowfive.create_logger("info")
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("producer", tag))
    if passthru:
        vol.set_passthru("*", "*")
    else:
        vol.set_memory("*", "*")
    vol.set_intercomm("*", "*", 0)

    cons = h.Puppet(str(os.environ['FTK']) + "/bin/ftk_shared.so",
    ["-f", "mpas-o-pt", "--input", "./mpas.yaml", "--output", "mpas.vtp", "--ptgeo-seeds",
    "50,20,50,50,-20,20,1,-10,-10", "--ptgeo-checkpoint-days", "1", "--timesteps", "15", "--geo",
    "--accelerator", "cuda"], pm, nm)

    if passthru:
        h.to_mpi4py(pm.intercomm("producer", tag)).barrier()
    cons.proceed()

