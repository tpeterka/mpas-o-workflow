#!/usr/bin/env python3

from mpi4py import MPI
import pyhenson as h
import lowfive

memory = False
passthru = True

world = MPI.COMM_WORLD.Dup()
size = world.Get_size()

# pm = h.ProcMap(world, [("producer", size)])
pm = h.ProcMap(world, [("producer", size - 1), ("consumer", 1)])

nm = h.NameMap()

if pm.group() == "producer":
    tag = 0
    lowfive.create_logger("trace")
    vol = lowfive.create_MetadataVOL()
#     vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("consumer", tag))
    vol.set_passthru("*", "*")
#     vol.set_memory("*", "*")
#     vol.set_intercomm("*", "*", 0)
    prod = h.Puppet("/home/tpeterka/climate/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    "namelist.ocean", "-s", "streams.ocean"], pm, nm)
    prod.proceed()
else:
    tag = 0
    lowfive.create_logger("trace")
    vol = lowfive.create_MetadataVOL()
#     vol = lowfive.create_DistMetadataVOL(pm.local(), pm.intercomm("producer", tag))
    vol.set_passthru("*", "*")
#     vol.set_memory("*", "*")
#     vol.set_intercomm("*", "*", 0)
    cons = h.Puppet("/home/tpeterka/climate/mpas-o-workflow/install/bin/consumer.so", [], pm, nm)
    cons.proceed()

