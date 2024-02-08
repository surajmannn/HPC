#!/bin/bash

#SBATCH --job-name=ovenchain 		# job name for tracking
#SBATCH --account=cs402users		# Slurm account which will be running the job
#SBATCH --partition=cs402-openmp	# Partition you wish to use
#SBATCH --nodes=1			        # Number of computer nodes used
#SBATCH --ntasks-per-node=1		    # Number of tasks per node
#SBATCH --cpus-per-task=40		    # Number of CPU cores used on each task (max 40)
#SBATCH --time=00:30:00			    # Max amount of time script can take

export OMP_NUM_THREADS=16           # Number of threads to run
#export OMP_PROC_BIND=true          # Environment variables
export OMP_SCHEDULER='Guided'       # scheduling type
#export OMP_THREAD_LIMIT=16         # amount of threads

module load cs402-mpi

ITERATIONS=3

cd build

INPUT_DECKS=("grid_5000.in" "grid_10000.in" "grid_20000.in")

# Runs the input decks over iterations to record runtimes over multiple instances
for input_deck in "${INPUT_DECKS[@]}"
do
    for ((i=1;i<=$ITERATIONS;i++))
    do
        srun ./deqn ../test/"$input_deck"
    done
done
