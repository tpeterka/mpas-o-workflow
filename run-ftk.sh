export FTK=`spack location -i ftk`
unset HDF5_PLUGIN_PATH
unset HDF5_VOL_CONNECTOR

# for 1 year every 2 days
# $FTK/bin/ftk -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
# 50,20,50,50,-20,20,1,-10,-10 --ptgeo-checkpoint-days 1 --timesteps 180 --geo \
# --accelerator cuda

# for 4 year every 15 days
$FTK/bin/ftk -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
50,20,50,50,-20,20,1,-10,-10 --ptgeo-checkpoint-days 1 --timesteps 96 --geo \
--accelerator cuda

# henson-exec -- $FTK/bin/ftk_shared.so \
# -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
# 50,20,50,50,-20,20,1,-10,-10 --ptgeo-checkpoint-days 30 --timesteps 1 --geo \
# --accelerator cuda --pt-delta-t  252288000
