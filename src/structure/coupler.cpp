#include "cup_lib.h"
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv){

    MPI_Init(&argc, &argv);
    printf("It's coupler time ;)");
    MPI_Fint comms_shell = MPI_Comm_c2f(MPI_COMM_WORLD);

    main_cup(argc, argv, comms_shell);
}