#include "Mesh.h"

#include <cstdlib>
#include <iostream>

//added
#include <omp.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define POLY2(i, j, imin, jmin, ni) (((i) - (imin)) + ((j)-(jmin)) * (ni))

Mesh::Mesh(const InputFile* input):
    input(input)
{
    allocated = false;

    NDIM = 2;

    n = new int[NDIM];
    min = new int[NDIM];
    max = new int[NDIM];
    dx = new double[NDIM];

    int nx = input->getInt("nx", 0);
    int ny = input->getInt("ny", 0);

    min_coords = new double[NDIM];
    max_coords = new double[NDIM];

    min_coords[0] = input->getDouble("xmin", 0.0);
    max_coords[0] = input->getDouble("xmax", 1.0);
    min_coords[1] = input->getDouble("ymin", 0.0);
    max_coords[1] = input->getDouble("ymax", 1.0);

    // setup first dimension.
    n[0] = nx;
    min[0] = 1;
    max[0] = nx;

    dx[0] = ((double) max_coords[0]-min_coords[0])/nx;

    // setup second dimension.
    n[1] = ny;
    min[1] = 1;
    max[1] = ny;

    dx[1] = ((double) max_coords[1]-min_coords[1])/ny;
    
    allocate();
}

void Mesh::allocate()
{
    allocated = true;

    int nx = n[0];
    int ny = n[1];

    /* Allocate arrays */
    u1 = new double[(nx+2) * (ny+2)];       // Holds updated solution value
    u0 = new double[(nx+2) * (ny+2)];       // Holds current solution value
    
    /* Allocate and initialise coordinate arrays */
    cellx = new double[nx+2];
    celly = new double[ny+2];

    double xmin = min_coords[0];
    double ymin = min_coords[1];

    for (int i=0; i < nx+2; i++) {
        cellx[i]=xmin+dx[0]*(i-1);
    }

    for (int i = 0; i < ny+2; i++) {
        celly[i]=ymin+dx[1]*(i-1);
    }
}

double* Mesh::getU0()
{
    return u0;
}

double* Mesh::getU1()
{
    return u1;
}

double* Mesh::getDx()
{
    return dx;
}

int* Mesh::getMin()
{
    return min;
}

int* Mesh::getMax()
{
    return max;
}

int Mesh::getDim()
{
    return NDIM;
}

int* Mesh::getNx()
{
    return n;
}

int* Mesh::getNeighbours()
{
    return neighbours;
}

double* Mesh::getCellX()
{
    return cellx;
}

double* Mesh::getCellY()
{
    return celly;
}

double Mesh::getTotalTemperature()
{

    // Begin loop runtime counter
    double seconds = omp_get_wtime();


    if(allocated) {
        double temperature = 0.0;
        int x_min = min[0];
        int x_max = max[0];
        int y_min = min[1]; 
        int y_max = max[1]; 

        int nx = n[0]+2;

        // parallised with reduction on temperature logic 
        #pragma omp parallel for reduction(+:temperature)
        for(int k=y_min; k <= y_max; k++) {
            for(int j=x_min; j <= x_max; j++) {

                int n1 = POLY2(j,k,x_min-1,y_min-1,nx);

                temperature += u0[n1];
            }
        }

        // end loop runtime counter
        seconds = omp_get_wtime() - seconds;
        std::cout << std::endl;
        std::cout << "Mesh Loop RunTime: " << seconds << std::endl;
        std::cout << std::endl;

        // Create or append benchmark csv
        FILE* fp = fopen("loop_benchmark.csv", "a");

        // write the runtime and inputdeck to the csv
        fprintf(fp, "%s,%lf\n", "mesh-loop", seconds);

        // close the csv
        fclose(fp);

        return temperature;
    } else {
        return 0.0;
    }
}
