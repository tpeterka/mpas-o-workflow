#!/usr/bin/env python3

from mpi4py import MPI
import pyhenson as h
import lowfive

memory = False
passthru = True

world = MPI.COMM_WORLD.Dup()
size = world.Get_size()

pm = h.ProcMap(world, [("producer", size)])
nm = h.NameMap()

if pm.group() == "producer":
    lowfive.create_logger("trace")
    vol = lowfive.create_MetadataVOL()
    vol.set_passthru("*", "*")
#     vol.set_memory("*", "*")
    prod = h.Puppet("/home/tpeterka/climate/E3SM/components/mpas-ocean/ocean_model.so", ["-n",
    "namelist.ocean", "-s", "streams.ocean"], pm, nm)
    prod.proceed()

