#include <diy/mpi/communicator.hpp>
#include <diy/master.hpp>
#include <thread>
#include <dlfcn.h>
#include "pio.h"
#include "fmt/format.h"

// #define NDIM 1
// #define DIM_LEN 1
// #define VAR_NAME "normalTransportVelocity"

#define NDIM 1
#define DIM_LEN 800
#define VAR_NAME "bottomDepth"

#define INFILE "output.nc"

using communicator  = MPI_Comm;

int main(int argc, char* argv[])
{
    diy::mpi::environment     env(argc, argv, MPI_THREAD_MULTIPLE);
    diy::mpi::communicator    world;
    communicator mpi_world = MPI_COMM_WORLD;

    // PIO defs
    int my_rank = world.rank();
    int ntasks  = world.size();
    int ioproc_stride = 1;
    int ioproc_start = 0;
    int iosysid;
    int ncid;
    int format = PIO_IOTYPE_NETCDF4P;
    double *buffer = NULL;
    PIO_Offset elements_per_pe;
    PIO_Offset *compdof = NULL;
    int dim_len[1] = {DIM_LEN};
    int ioid;
    int varid = -1;

    // set error handling so that scorpio does not catch the error
    int ret;
    if ((ret = PIOc_set_iosystem_error_handling(PIO_DEFAULT, PIO_RETURN_ERROR, NULL)))
        return ret;

    // debug
    fmt::print(stderr, "consumer: local comm rank {} size {}\n", my_rank, ntasks);

//     // debugging
//     if (shared)
//     {
//         fmt::print(stderr, "Consumer metadata hierarchy:\n");
//         shared_vol_plugin.print_files();
//     }

    // init PIO
    PIOc_Init_Intracomm(mpi_world, ntasks, ioproc_stride, ioproc_start, PIO_REARR_SUBSET, &iosysid);

    // decomposition
    elements_per_pe = DIM_LEN / ntasks;
    compdof = (MPI_Offset*)(malloc(elements_per_pe * sizeof(PIO_Offset)));

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = (my_rank * elements_per_pe + i + 1) + 10;

    PIOc_InitDecomp(iosysid, PIO_DOUBLE, NDIM, dim_len, (PIO_Offset)elements_per_pe, compdof, &ioid, NULL, NULL, NULL);
    free(compdof);

    // debug
    fmt::print(stderr, "*** consumer before opening file ***\n");

    // open file for reading
    PIOc_openfile(iosysid, &ncid, &format, INFILE, PIO_NOWRITE);

    // debug
    fmt::print(stderr, "*** consumer after opening file and before inquiring variable ID ***\n");

    // read the metadata (get variable ID)
    PIOc_inq_varid(ncid, VAR_NAME, &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} and before reading data ***\n", varid);
    fmt::print(stderr, "*** consumer elements_per_pe {} ***\n", elements_per_pe);

    // read the data
    buffer = (double*)(malloc(elements_per_pe * sizeof(double)));
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, buffer);

    // print the data values
    for (int i = 0; i < elements_per_pe; i++)
        fmt::print(stderr, "buffer[{}] = {}\n", i, buffer[i]);

    free(buffer);

    // debug
    fmt::print(stderr, "*** consumer after reading data and before closing file ***\n");

    // clean up
    PIOc_closefile(ncid);
    PIOc_freedecomp(iosysid, ioid);
    PIOc_finalize(iosysid);

    // debug
    fmt::print(stderr, "*** consumer after closing file ***\n");
}

