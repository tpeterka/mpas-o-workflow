ftk -f mpas-o-pt --input ./mpas.yaml --output mpas.vtp --ptgeo-seeds \
16,25,50,13,-15,17,1,-10,-10 --ptgeo-checkpoint-days 30 --timesteps 1 --geo \
--accelerator cuda --pt-delta-t  252288000
