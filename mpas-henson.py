#!/usr/bin/env python3

from mpi4py import MPI
import pyhenson as h
import lowfive
from pathlib import Path
import os

passthru = True

world = MPI.COMM_WORLD.Dup()
size = world.Get_size()

# pm = h.ProcMap(world, [("producer", size)])
pm = h.ProcMap(world, [("producer", size - 1), ("consumer", 1)])

nm = h.NameMap()

if pm.group() == "producer":
    tag = 0
    lowfive.create_logger("trace")
#     vol = lowfive.create_MetadataVOL()
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("consumer", tag))
    if passthru:
        vol.set_passthru("*", "*")
    else:
        vol.set_memory("*", "*")
    prod = h.Puppet(str(Path.home()) + "/climate/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    "namelist.ocean", "-s", "streams.ocean"], pm, nm)
    print("rank", world.rank, "pid", os.getpid())
    prod.proceed()
    if passthru:
        print("producer before barrier")
        h.to_mpi4py(pm.intercomm("consumer", tag)).barrier()
        print("producer after barrier")
else:
    tag = 0
#     lowfive.create_logger("trace")
#     vol = lowfive.create_MetadataVOL()
    vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("producer", tag))
    if passthru:
        vol.set_passthru("*", "*")
    else:
        vol.set_memory("*", "*")
    vol.set_intercomm("*", "*", 0)
    cons = h.Puppet(str(Path.home()) + "/climate/mpas-o-workflow/install/bin/consumer.so", [], pm, nm)
    if passthru:
        print("consumer before barrier")
        h.to_mpi4py(pm.intercomm("producer", tag)).barrier()
        print("consumer after barrier")
    cons.proceed()

