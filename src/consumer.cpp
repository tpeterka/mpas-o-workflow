#include <diy/mpi/communicator.hpp>
#include <diy/master.hpp>
#include <thread>
#include <dlfcn.h>
#include "pio.h"
#include "fmt/format.h"
#include "opts.h"

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
    int                     varid;;
    int                     ndims;
    std::vector<int>        dimids;
    std::vector<PIO_Offset> dimlens;
    std::vector<int>        int_dimlens;    // int version of dimlens (scorpio API mismatch)
    std::vector<PIO_Offset> compdof;
    std::string             infile;
    bool                    help;

    // get command line arguments
    using namespace opts;
    Options ops;
    ops
        >> Option('f', "infile",    infile,         "input file name")
        >> Option('h', "help",      help,           "show help")
        ;

    if (!ops.parse(argc,argv) || help)
    {
        if (world.rank() == 0)
        {
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n";
            std::cout << "Reads various datasets.\n";
            std::cout << ops;
        }
        return 1;
    }

    // set error handling so that scorpio does not catch the error
    int ret;
    if ((ret = PIOc_set_iosystem_error_handling(PIO_DEFAULT, PIO_RETURN_ERROR, NULL)))
        return ret;

    // debug
    fmt::print(stderr, "*** consumer before opening file: local comm rank {} size {} ***\n", world.rank(), world.size());

    // init PIO
    PIOc_Init_Intracomm(mpi_world, world.size(), ioproc_stride, ioproc_start, PIO_REARR_SUBSET, &iosysid);

    // open file for reading
    PIOc_openfile(iosysid, &ncid, &format, infile.c_str(), PIO_NOWRITE);

    // debug
    fmt::print(stderr, "*** consumer after opening file ***\n");

    // -------- bottomDepth --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "bottomDepth", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: bottomDepth varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    elements_per_pe = dimlens[0] / world.size();
    compdof.resize(elements_per_pe);
    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // scorpio compdofs start at 1
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &int_dimlens[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> bottomDepth(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &bottomDepth[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "bottomDepth[{}] = {}\n", i, bottomDepth[i]);

    // -------- xEdge --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "xEdge", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: xEdge varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    elements_per_pe = dimlens[0] / world.size();
    compdof.resize(elements_per_pe);
    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // scorpio compdofs start at 1
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &int_dimlens[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> xEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &xEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "xEdge[{}] = {}\n", i, xEdge[i]);

    // -------- yEdge --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "yEdge", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: yEdge varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    elements_per_pe = dimlens[0] / world.size();
    compdof.resize(elements_per_pe);
    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // scorpio compdofs start at 1
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &int_dimlens[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> yEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &xEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "yEdge[{}] = {}\n", i, yEdge[i]);

    // -------- zEdge --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "zEdge", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: zEdge varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    elements_per_pe = dimlens[0] / world.size();
    compdof.resize(elements_per_pe);
    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;    // scorpio compdofs start at 1
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims, &int_dimlens[0], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> zEdge(elements_per_pe);
    PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &zEdge[0]);

    // print the data values
//     for (int i = 0; i < elements_per_pe; i++)
//         fmt::print(stderr, "zEdge[{}] = {}\n", i, zEdge[i]);

//     // -------- zTop --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "zTop", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: zTop varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    elements_per_pe = dimlens[1] * dimlens[2] / world.size();
    compdof.resize(elements_per_pe);
    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // scorpio compdofs start at 1
    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &int_dimlens[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> zTop(elements_per_pe);
    for (auto t = 0; t < dimlens[0]; t++)      // for all timesteps
    {
        PIOc_setframe(ncid, varid, t);
        PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &zTop[0]);

        // print the data values
//         for (int i = 0; i < elements_per_pe; i++)
//             fmt::print(stderr, "time t = {} zTop[{}] = {}\n", t, i, zTop[i]);
    }

    // -------- normalTransportVelocity --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "normalTransportVelocity", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: normalTransportVelocity varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    elements_per_pe = dimlens[1] * dimlens[2] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // scorpio compdofs start at 1

    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &int_dimlens[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> normalTransportVelocity(elements_per_pe);
    for (auto t = 0; t < dimlens[0]; t++)      // for all timesteps
    {
        // memory mode crashes with varid -1
        PIOc_setframe(ncid, varid, t);
        PIOc_read_darray(ncid, varid, ioid, (PIO_Offset)elements_per_pe, &normalTransportVelocity[0]);

        // print the data values
//         for (int i = 0; i < elements_per_pe; i++)
//             fmt::print(stderr, "time t = {} normalTransportVelocity[{}] = {}\n", t, i, normalTransportVelocity[i]);
    }

    // -------- vertTransportVelocityTop --------

    // read the metadata
    varid = -1;
    PIOc_inq_varid(ncid, "vertTransportVelocityTop", &varid);
    PIOc_inq_varndims(ncid, varid, &ndims);
    dimids.resize(ndims);
    dimlens.resize(ndims);
    int_dimlens.resize(ndims);
    PIOc_inq_vardimid(ncid, varid, &dimids[0]);
    for (auto i = 0; i < ndims; i++)
    {
        PIOc_inq_dimlen(ncid, dimids[i], &dimlens[i]);
        int_dimlens[i] = dimlens[i];
    }

    // debug
    fmt::print(stderr, "*** consumer: vertTransportVelocityTop varid {} ndims {} dimids [{}] dimlens [{}] ***\n",
            varid, ndims, fmt::join(dimids, ","), fmt::join(dimlens, ","));

    // decomposition
    // even though it's a 3d dataspace, time is taken separately, and the decomposition is the
    // remaining 2d dimensions
    elements_per_pe = dimlens[1] * dimlens[2] / world.size();
    compdof.resize(elements_per_pe);

    for (int i = 0; i < elements_per_pe; i++)
        compdof[i] = world.rank() * elements_per_pe + i + 1;        // scorpio compdofs start at 1

    // starting dim_len at index 1 because index 0 is the time step
    PIOc_InitDecomp(iosysid, PIO_DOUBLE, ndims - 1, &int_dimlens[1], (PIO_Offset)elements_per_pe, &compdof[0], &ioid, NULL, NULL, NULL);

    // read the data
    std::vector<double> vertTransportVelocityTop(elements_per_pe);
    for (auto t = 0; t < dimlens[0]; t++)      // for all timesteps
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

