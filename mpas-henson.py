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
    lowfive.create_logger("trace")
    vol = lowfive.create_MetadataVOL()
    vol.set_passthru("*", "*")
#     vol.set_memory("*", "*")
    prod = h.Puppet("/home/tpeterka/climate/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    "namelist.ocean", "-s", "streams.ocean"], pm, nm)
#     prod = h.Puppet("/home/tpeterka/climate/E3SM/components/mpas-ocean/ocean_model.so", [], pm, nm)
    prod.proceed()
else:
    lowfive.create_logger("trace")
    vol = lowfive.create_MetadataVOL()
    vol.set_passthru("*", "*")
#     vol.set_memory("*", "*")
    cons = h.Puppet("/home/tpeterka/climate/mpas-o-workflow/install/bin/consumer.so", [], pm, nm)
    cons.proceed()

