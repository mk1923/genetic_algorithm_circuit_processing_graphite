# Gerardium Rush - Genetic Algorithm for Optimal Mineral Recovery

## Overview

#### This project aims to optimize mineral recovery using a Genetic Algorithm (GA) approach. The goal is to design circuits of separation units to maximize the recovery of a valuable mineral, Gerardium, while minimizing the recovery of waste materials. The project is structured to provide a modular software solution that includes a genetic algorithm, a circuit simulator, a validity checker, and a visualization tool.

## Table of Contents

- #### Project Description
- #### Installation
- #### Usage
- #### Directory Structure
- #### Modules
- #### Algorithm Description
- #### Evaluation and Performance
- #### License

## Project Description

### Objective

#### The main objective is to optimize the design of circuits consisting of separation units. Each unit can produce three output streams: high-grade concentrate, intermediate-grade concentrate, and tailings. The performance of the circuit is evaluated based on the recovery and purity of Gerardium in the concentrate stream, with economic penalties for waste material.

### Methodology

#### A genetic algorithm is used to explore the vast number of possible circuit configurations, searching for the optimal design. The algorithm simulates natural selection through mutation and crossover processes to evolve better circuit designs over successive generations.

## Installation

### Prerequisites

- #### C++ compiler (supporting C++11 or later)
- #### CMake (for building the project)
- #### Python (for post-processing and visualization)

### Building the Project

#### 1. Clone the repository:

```bash
git clone https://github.com/ese-msc-2023/acs-gerardium-rush-sphalerite.git
cd acs-gerardium-rush-sphalerite
```

#### 2. Create a build directory and navigate to it:

```bash
mkdir build
cd build
```

#### 3. Run CMake and build:
```bash
cmake ..
make
```
**For macOS, Run**
```bash
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13
make
```

## Usage

### Running the Genetic Algorithm

#### 1. Navigate to the build directory:
```bash
cd build
```

#### 2. Execute the main program:
```bash
./bin/Circuit_Optimizer
```

### Running the Genetic Algorithm in parallel using openmp

#### 1. Uncomment all ***4*** `# pragma omp parallel for` inside `Genetic_algorithm.cpp`

#### 2. Navigate to the build directory:
```bash
cd build
```

#### 3. Compile the program after uncomment the parallel script
```bash
cmake .. -DCMAKE_C_COMPILER=gcc-13 -DCMAKE_CXX_COMPILER=g++-13
make
```

#### 4. Execute the main program:
```bash
OMP_NUM_THREADS={NUM_OF_THREDAS} ./bin/Circuit_Optimizer
```
**The {NUM_OF_THREDAS} is the number of threads you want to run the program.**

### Run the post_process

### After runing the program following the above steps, there will be a `vecotor_data.txt` file inside `post_process` folder. You can run the post process code inside `post_process` folder:
```bash
python visualisation.py
```
OR **in the root dictionary**
```bash
./run_visualisation.sh
```



## Directory Structure
```
├── CMakeLists.txt
├── Doxyfile.txt
├── LICENSE
├── Problem Statement for Genetic Algorithms project 2024.pdf
├── README.md
├── environment.yml
├── evaluate.pbs
├── include
│   ├── CCircuit.h
│   ├── CSimulator.h
│   ├── CUnit.h
│   ├── Genetic_Algorithm.h
│   └── hyper.h
├── post_process
│   ├── __init__.py
│   └── visualisation.py
├── requirements.txt
├── rng_examples
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── basic_rand.cpp
│   ├── fast_random.cpp
│   ├── race_random.cpp
│   └── slow_random.cpp
├── run_visualisation.sh
├── setup.py
├── src
│   ├── CCircuit.cpp
│   ├── CMakeLists.txt
│   ├── CSimulator.cpp
│   ├── CUnit.cpp
│   ├── Genetic_Algorithm.cpp
│   ├── hyper.cpp
│   └── main.cpp
└── tests
    ├── CMakeLists.txt
    ├── test_circuit.cpp
    ├── test_circuit_simulator.cpp
    ├── test_genetic_algorithm.cpp
    └── test_validity_checker.cpp
```


## Modules

### Genetic Algorithm

- #### File: `Genetic_Algorithm.cpp`, `Genetic_Algorithm.h`
- #### Description: Implements the genetic algorithm to optimize circuit configurations.

### Circuit Simulator

- #### File: `CSimulator.cpp`, `CSimulator.h`
- #### Description: Simulates the mass balance and performance of given circuit configurations.

### Circuit Definition

- #### File: `CCircuit.cpp`, `CCircuit.h`
- #### Description: Defines the properties and behaviors of individual separation circuit.

### Unit Definition

- #### File: `CUnit.cpp`, `CUnit.h`
- #### Description: Defines the properties and behaviors of individual separation units.

### Hyperparamater grid search

- #### File: `hyper.h`, `hyper.cpp`
- #### Description: For ***large number*** of units, you do not know what the best hyperparameter (e.g. number of population) is. Then, you can pre-search the parameters first. This may save you time. Hint: Only try it when the number of units is very very big.

### Main Execution

- #### File: `main.cpp`
- #### Description: Entry point of the program, orchestrates the genetic algorithm and simulation processes.

## Algorithm Description

### Genetic Algorithm Steps

#### 1. Initialization: Generate an initial population of random circuit configurations.

#### 2. Fitness Evaluation: Calculate the fitness of each configuration based on the amount of geradium and waste.

#### 3. Selection: Select parent configurations based on fitness for reproduction.

#### 4. Crossover: Create new configurations by combining parts of parent configurations.

#### 5. Mutation: Introduce random changes to some configurations to maintain diversity.

#### 6. Replacement: Replace the old population with the new one.

#### 7. Iteration: Repeat the process for a set number of generations or until convergence.

### Performance Evaluation

- #### Fitness Function: Evaluates the economic value of the final concentrate stream, considering the recovery of Gerardium and penalties for waste.
- #### Validity Checks: Ensure the generated configurations are feasible and can be simulated to convergence.

## Evaluation and Performance

### Optimization Goals

- #### Maximize: Amount of Gerardium in the concentrate stream.
- #### Minimize: Amount of waste material in the concentrate stream.

### Parameters to Tune

- #### Population size
- #### Number of generations
- #### Mutation rate
- #### Crossover probability

### Convergence and Robustness

#### The genetic algorithm's performance is evaluated based on its convergence speed and robustness to initial conditions and parameter settings.

## License

#### This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Reference

GitHub and OpenAI, "GitHub Copilot," 2024. [Online]. Available: https://copilot.github.com.