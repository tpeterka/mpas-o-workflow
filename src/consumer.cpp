#include <diy/mpi/communicator.hpp>
#include <diy/master.hpp>
#include <thread>
#include <dlfcn.h>
#include "pio.h"
#include "fmt/format.h"

// #define BAROCLINIC
#define SOMA

#ifdef BAROCLINIC
#define INFILE "output.nc"
#endif
#ifdef SOMA
#define INFILE "output/output.0001-01-01_00.00.00.nc"
#endif

#define MAX_DIMS 10

using communicator  = MPI_Comm;

int main(int argc, char* argv[])
{
    diy::mpi::environment   env(argc, argv, MPI_THREAD_MULTIPLE);
    diy::mpi::communicator  world;
    communicator            mpi_world       = MPI_COMM_WORLD;
    int                     ioproc_stride   = 1;
    int                     ioproc_start    = 0;
    int                     iosysid;
    int                     ncid;
    int                     format          = PIO_IOTYPE_NETCDF4P;
    PIO_Offset              elements_per_pe;
    int                     ioid;
    int                     ndims;
    int varid               = -1;
    std::vector<PIO_Offset> compdof;
    std::vector<int>        dim_len(MAX_DIMS);

    // set error handling so that scorpio does not catch the error
//     int ret;
//     if ((ret = PIOc_set_iosystem_error_handling(PIO_DEFAULT, PIO_RETURN_ERROR, NULL)))
//         return ret;

    // debug
    fmt::print(stderr, "*** consumer before opening file: local comm rank {} size {} ***\n", world.rank(), world.size());

    // init PIO
    PIOc_Init_Intracomm(mpi_world, world.size(), ioproc_stride, ioproc_start, PIO_REARR_SUBSET, &iosysid);

    // open file for reading
    PIOc_openfile(iosysid, &ncid, &format, INFILE, PIO_NOWRITE);

    // debug
    fmt::print(stderr, "*** consumer after opening file ***\n");

    // -------- bottomDepth --------

    // decomposition
    ndims           = 1;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 928;                  // size in each dimension TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 8521;                  // size in each dimension TODO: inquire from file
#endif
    elements_per_pe = dim_len[0] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // adding 1 fixes a scorpio bug I don't understand

    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &dim_len[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "bottomDepth", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for bottomDepth and before reading data ***\n", varid);

    // read the data
    std::vector<double> bottomDepth(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &bottomDepth[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "bottomDepth[{}] = {}\n", i, bottomDepth[i]);

    // -------- xEdge --------

    // decomposition
    ndims           = 1;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 2816;                 // size in each dimension TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 25898;                 // size in each dimension TODO: inquire from file
#endif
    elements_per_pe = dim_len[0] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // adding 1 fixes a scorpio bug I don't understand

    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &dim_len[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "xEdge", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for xEdge and before reading data ***\n", varid);

    // read the data
    std::vector<double> xEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &xEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "xEdge[{}] = {}\n", i, xEdge[i]);

    // -------- yEdge --------

    // decomposition
    ndims           = 1;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 2816;                 // size in each dimension TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 25898;                 // size in each dimension TODO: inquire from file
#endif
    elements_per_pe = dim_len[0] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // adding 1 fixes a scorpio bug I don't understand

    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &dim_len[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "yEdge", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for yEdge and before reading data ***\n", varid);

    // read the data
    std::vector<double> yEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &yEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "yEdge[{}] = {}\n", i, yEdge[i]);

    // -------- zEdge --------

    // decomposition
    ndims           = 1;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 2816;                 // size in each dimension TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 25898;                 // size in each dimension TODO: inquire from file
#endif
    elements_per_pe = dim_len[0] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // adding 1 fixes a scorpio bug I don't understand

    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &dim_len[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "zEdge", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for zEdge and before reading data ***\n", varid);

    // read the data
    std::vector<double> zEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &zEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "zEdge[{}] = {}\n", i, zEdge[i]);

    // -------- zTop --------

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    ndims           = 3;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 3;                    // timestep TODO: inquire from file
    dim_len[1]      = 928;                  // edge TODO: inquire from file
    dim_len[2]      = 20;                   // vertical level TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 1;                    // timestep TODO: inquire from file
    dim_len[1]      = 8521;                  // edge TODO: inquire from file
    dim_len[2]      = 60;                   // vertical level TODO: inquire from file
#endif
    elements_per_pe = dim_len[1] * dim_len[2] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // adding 1 fixes a scorpio bug I don't understand

    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &dim_len[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "zTop", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for zTop and before reading data ***\n", varid);

    // read the data
    std::vector<double> zTop(elements_per_pe);
    for (auto t = 0; t < dim_len[0]; t++)      // for all timesteps
    {
        PIOc_setframe(ncid, varid, t);
        PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &zTop[0]);

        // print the data values
//         for (int i = 0; i < elements_per_pe; i++)
//             fmt::print(stderr, "time t = {} zTop[{}] = {}\n", t, i, zTop[i]);
    }

    // -------- normalTransportVelocity --------

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    ndims           = 3;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 3;                    // timestep TODO: inquire from file
    dim_len[1]      = 2816;                 // edge TODO: inquire from file
    dim_len[2]      = 20;                   // vertical level TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 1;                    // timestep TODO: inquire from file
    dim_len[1]      = 25898;                 // edge TODO: inquire from file
    dim_len[2]      = 60;                   // vertical level TODO: inquire from file
#endif
    elements_per_pe = dim_len[1] * dim_len[2] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // adding 1 fixes a scorpio bug I don't understand

    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &dim_len[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "normalTransportVelocity", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for normalTransportVelocity and before reading data ***\n", varid);

    // read the data
    std::vector<double> normalTransportVelocity(elements_per_pe);
    for (auto t = 0; t < dim_len[0]; t++)      // for all timesteps
    {
        // memory mode crashes with varid -1
        PIOc_setframe(ncid, varid, t);
        PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &normalTransportVelocity[0]);

        // print the data values
//         for (int i = 0; i < elements_per_pe; i++)
//             fmt::print(stderr, "time t = {} normalTransportVelocity[{}] = {}\n", t, i, normalTransportVelocity[i]);
    }

    // -------- vertTransportVelocityTop --------

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    ndims           = 3;                    // dimensionality TODO: inquire from file
#ifdef BAROCLINIC
    dim_len[0]      = 3;                    // timestep TODO: inquire from file
    dim_len[1]      = 928;                  // edge TODO: inquire from file
    dim_len[2]      = 21;                   // vertical level TODO: inquire from file
#endif
#ifdef SOMA
    dim_len[0]      = 1;                    // timestep TODO: inquire from file
    dim_len[1]      = 8521;                  // edge TODO: inquire from file
    dim_len[2]      = 61;                   // vertical level TODO: inquire from file
#endif
    elements_per_pe = dim_len[1] * dim_len[2] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // adding 1 fixes a scorpio bug I don't understand

    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &dim_len[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the metadata (get variable ID)
    varid = -1;
    PIOc_inq_varid(ncid, "vertTransportVelocityTop", &varid);

    // debug
    fmt::print(stderr, "*** consumer after inquiring variable ID {} for vertTransportVelocityTop and before reading data ***\n", varid);

    // read the data
    std::vector<double> vertTransportVelocityTop(elements_per_pe);
    for (auto t = 0; t < dim_len[0]; t++)      // for all timesteps
    {
        // memory mode crashes with varid -1
        PIOc_setframe(ncid, varid, t);
        PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &vertTransportVelocityTop[0]);

        // print the data values
//         for (int i = 0; i < elements_per_pe; i++)
//             fmt::print(stderr, "time t = {} vertTransportVelocityTop[{}] = {}\n", t, i, vertTransportVelocityTop[i]);
    }

    // clean up
    PIOc_closefile(ncid);
    PIOc_freedecomp(iosysid, ioid);
    PIOc_finalize(iosysid);

    // debug
    fmt::print(stderr, "*** consumer after closing file ***\n");
}

