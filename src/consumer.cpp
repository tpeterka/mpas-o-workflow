#include <diy/mpi/communicator.hpp>
#include <thread>
#include <dlfcn.h>
#include <pio.h>

#define NDIM 1
#define DIM_LEN 1024
#define DIM_NAME "x"
#define VAR_NAME "foo"
#define START_DATA_VAL 100

using communicator  = MPI_Comm;

int main(int argc, char* argv[])
{
    diy::mpi::environment     env(argc, argv, MPI_THREAD_MULTIPLE);
    diy::mpi::communicator    world;

    diy::mpi::communicator local_(local);

    // PIO defs
    int my_rank = local_.rank();
    int ntasks  = local_.size();
    int ioproc_stride = 1;
    int ioproc_start = 0;
    int iosysid;
    int ncid;
    int format = PIO_IOTYPE_NETCDF4P;
    int *buffer = NULL;
    PIO_Offset elements_per_pe;
    PIO_Offset *compdof = NULL;
    int dim_len[1] = {DIM_LEN};
    int ioid;
    int varid = -1;

    // debug
    fmt::print(stderr, "consumer: local comm rank {} size {}\n", my_rank, ntasks);

//     // debugging
//     if (shared)
//     {
//         fmt::print(stderr, "Consumer metadata hierarchy:\n");
//         shared_vol_plugin.print_files();
//     }

    // init PIO
    PIOc_Init_Intracomm(local, ntasks, ioproc_stride, ioproc_start, PIO_REARR_SUBSET, &iosysid);

    // decomposition
    elements_per_pe = DIM_LEN / ntasks;
    compdof = (MPI_Offset*)(malloc(elements_per_pe * sizeof(PIO_Offset)));

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = (my_rank * elements_per_pe + i + 1) + 10;

    PIOc_InitDecomp(iosysid, PIO_INT, NDIM, dim_len, (PIO_Offset)elements_per_pe, compdof, &ioid, NULL, NULL, NULL);
    free(compdof);

    // debug
    fmt::print(stderr, "*** consumer before opening file ***\n");

    // open file for reading
    PIOc_openfile(iosysid, &ncid, &format, "example1.nc", PIO_NOWRITE);

    // debug
    fmt::print(stderr, "*** consumer after opening file and before inquiring variable ID ***\n");

    // read the metadata (get variable ID)
    PIOc_inq_varid(ncid, VAR_NAME, &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} and before reading data ***\n", varid);

    // read the data
    buffer = (int*)(malloc(elements_per_pe * sizeof(int)));
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, buffer);
    // check the data values
    for (int i = 0; i < elements_per_pe; i++)
    {
        if (buffer[i] != START_DATA_VAL + my_rank)
        {
            fmt::print(stderr, "*** consumer error: buffer[{}] = {} which should be {} ***\n", i, buffer[i], START_DATA_VAL + my_rank);
            abort();
        }
    }
    free(buffer);

    // debug
    fmt::print(stderr, "*** consumer after reading data and before closing file ***\n");

    // clean up
    PIOc_closefile(ncid);
    PIOc_freedecomp(iosysid, ioid);
    PIOc_finalize(iosysid);
    if (!shared)
        H5Pclose(plist);

    // debug
    fmt::print(stderr, "*** consumer after closing file ***\n");
}
