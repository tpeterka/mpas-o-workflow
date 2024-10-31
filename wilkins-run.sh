export WILKINS=`spack location -i wilkins`

# works when using old version of custom modified wilkins-master.py
# echo "Running 4 producer processes + 2 consumer processes"
# mpirun -n 6 -l python3 -u \
# /home/tpeterka/wilkins-master.py \
# /home/tpeterka/climate/mpas-o-workflow/wilkins-config.yaml -v 2

# crashes in memory mode when running a wilkins-master.py taken from the wilkins repo
echo "Running 4 producer processes + 2 consumer processes"
mpirun -n 6 -l python3 -u \
$WILKINS/bin/wilkins-master.py \
/home/tpeterka/climate/mpas-o-workflow/wilkins-config.yaml
