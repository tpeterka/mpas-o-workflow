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
#     cons = h.Puppet(str(os.environ['FTK']) + "/bin/ftk_shared.so",
#             ["-f", "mpas-o-pt", "--input", "/nfs/gce/projects/PEDAL-GCE/hguo/data/20210421_sim7_CORE_60to30E2r2/mpas.yaml",
#             "--output", "mpas.vtp", "--ptgeo-seeds", "101,49,50,101,-48,-47,1,-1400,-1400", "--ptgeo-checkpoint-days", "1",
#             "--timesteps", "4", "--geo", "--accelerator", "cuda"], pm, nm)

    cons = h.Puppet(str(os.environ['FTK']) + "/bin/ftk_shared.so",
    ["-f", "mpas-o-pt", "--input", "./mpas.yaml", "--output", "mpas.vtp", "--ptgeo-seeds",
    "16,25,50,13,-15,17,1,-10,-10", "--ptgeo-checkpoint-days", "30", "--timesteps", "1", "--geo",
    "--accelerator", "cuda", "--pt-delta-t",  "252288000"], pm, nm)

    if passthru:
        h.to_mpi4py(pm.intercomm("producer", tag)).barrier()
    cons.proceed()

