export FTK=`spack location -i ftk`
unset HDF5_PLUGIN_PATH
unset HDF5_VOL_CONNECTOR

$FTK/bin/ftk -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
16,25,50,13,-15,17,1,-10,-10 --ptgeo-checkpoint-days 30 --timesteps 1 --geo \
--accelerator cuda --pt-delta-t  252288000

# henson-exec -- $FTK/bin/ftk_shared.so \
# -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
# 16,25,50,13,-15,17,1,-10,-10 --ptgeo-checkpoint-days 30 --timesteps 1 --geo \
# --accelerator cuda --pt-delta-t  252288000
