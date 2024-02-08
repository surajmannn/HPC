#include <iostream>
#include <string>
#include <cstdlib>

#include "InputFile.h"
#include "Driver.h"

//added
#include <omp.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{

    // Begin program runtime counter
    double seconds = omp_get_wtime();

    if (argc != 2) {
        std::cerr << "Usage: deqn <filename>" << std::endl;
        exit(1);
    }

    const char* filename = argv[1];
    InputFile input(filename);

    // get name of input deck and remove ../test/ from the name
    const char *input_deck = strstr(filename, "../test/");
    input_deck += strlen("../test/");

    std::string problem_name(filename);

    int len = problem_name.length();

    if(problem_name.substr(len - 3, 3) == ".in")
        problem_name = problem_name.substr(0, len-3);

    // Strip out leading path
    size_t last_sep = problem_name.find_last_of("/");

    if (last_sep != std::string::npos) {
        last_sep = last_sep + 1;
    } else {
        last_sep = 0;
    }

    problem_name = problem_name.substr(last_sep, problem_name.size());

    Driver driver(&input, problem_name);

    driver.run();

    // get the time taken for the program to run
    seconds = omp_get_wtime() - seconds;
    std::cout << "Total Time to Run: " << seconds << std::endl;

    // Create or append benchmark csv
    FILE* fp = fopen("benchmark.csv", "a");

    // write the runtime and inputdeck to the csv
    fprintf(fp, "%s,%lf\n", input_deck, seconds);

    // close the csv
    fclose(fp);

    return 0;
}
