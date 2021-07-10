# CoR-HPX
Resource Oriented Computing Platform

# Installation

## Requirements
The CoR-HPX library requires C++17 features. If you use GCC you would need GCC >= 7 (GCC 7.4.0 are used for development and testing). CMake 3.12 (or later) is required for the build and testing process of the Runtime. Libssh 0.8 (or later) is also required to spawning processes dynamicly. 

The CoR-HPX library is built on HPX. In order to build HPX as the necessary requirement, please refer to either:
<br />
**HPX Wiki:** [https://github.com/STEllAR-GROUP/hpx/wiki/HPX-Resource-Guide](https://github.com/STEllAR-GROUP/hpx/wiki/HPX-Resource-Guide)
<br />
**HPX Documentation:** [https://hpx-docs.stellar-group.org/latest/html/index.html](https://hpx-docs.stellar-group.org/latest/html/index.html)
<br />
<br />
summary:
- GCC (>= 7.0.0)
- HPX (>= 1.5.0)
- Libssh (>= 0.8)
- Cmake (>= 3.12)

## Getting the Source
~~~
git clone https://github.com/tiagofgon/CoR-HPX
~~~

## Running the Setup

- $HPX_LOCATION -> hpx library location
- $Libssh_LOCATION -> Libssh library location
- $INSTALATION_DIR -> CoR-HPX target installation

~~~
cd cor-hpx
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DLibSSH_ROOT=$Libssh_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..
~~~

## Building
~~~
make -jN
make install
~~~

To use all features without defining paths  (**recommended**): <br />
~~~
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/examples
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/cap6
export PATH=$PATH:$INSTALATION_DIR/bin
~~~

## Running some examples
### module
Print active resource idp (thread/agent identifier)
~~~ 
$ corhpx app ctx 1 0 libmodule.so
~~~

### basicOperations
Exchange data between two agents
~~~ 
$ corhpx app ctx 1 0 libbasicOperations.so
~~~ 

### parallel
Launching a program with multiple domains(processes): using two consoles or using mpi

~~~ 
$ corhpx app ctx 2 0 /cor-hpx_installation_dir/examples/libparallel.so --hpx:hpx=localhost:1337
$ corhpx app ctx 2 0 /cor-hpx_installation_dir/examples/libparallel.so --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:worker
~~~ 

using mpi: (N == number of domains(processes))<br />
~~~ 
$ mpirun -np N corhpx apps ctx N 0 /cor-hpx_installation_dir/examples/libparallel.so
~~~ 

### spawn
Dynamically spawn a new process
~~~ 
$ corhpx app ctx 1 0 libspawn.so
~~~ 