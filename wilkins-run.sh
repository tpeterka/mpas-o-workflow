echo "Running 4 producer processes + 2 consumer processes"
mpirun -n 6 -l python3 -u \
/home/tpeterka/climate/mpas-o-workflow/wilkins-master.py \
/home/tpeterka/climate/mpas-o-workflow/wilkins-config.yaml

# works when using old version of custom modified wilkins-master.py above
# crashes in memory mode when running a wilkins-master.py taken from the wilkins repo:
# /home/tpeterka/software/moab-workflow/wilkins-master.py \

