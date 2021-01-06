# PlaCoRHPX
Resource oriented Computing Platform

# Installation

## Requirements
The PlaCoRHPX library requires C++17 features, if you use GCC you need GCC >= 7 (GCC 7.4.0 are used for development and testing). CMake 3.0 (or later) is required for the build and testing process of the Runtime. Libssh 0.8 (or later) is also required to spawning processes dynamicly. 

The PlaCoRHPX library is built on HPX. In order to build HPX as the necessary requirement, please refer to either:
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
- Cmake (>= 3.0)

## Getting the Source
`git clone https://github.com/tiagofgon/placor-hpx`

## Running the Setup

- $HPX_LOCATION -> hpx library location
- $Libssh_LOCATION -> Libssh library location
- $INSTALATION_DIR -> PlaCoRHPX target installation

```
cd placor-hpx
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DLibSSH_ROOT=$Libssh_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..
```

## Building and Testing
make -jN <br />
make install <br />

```
To use all features without defining paths  (recommended): <br />
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/examples
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$INSTALATION_DIR/cap6
export PATH=$PATH:$INSTALATION_DIR/bin
```

### module
`corhpx app ctx 1 0 libmodule.so`

### basicOperations
`corhpx app ctx 1 0 libbasicOperations.so`

### parallel
console 1:<br />
`corhpx app ctx 2 0 ../examples/libparallel.so --hpx:hpx=localhost:1337 --hpx:ini=hpx.component_paths=..../examples`

console 2:<br />
`corhpx app ctx 2 0 ../examples/libparallel.so --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:worker --hpx:ini=hpx.component_paths=/placor-hpx_installation_dir/examples`

using mpi:<br />
`mpirun -np N ./corhpx apps ctx N 0 /placor-hpx_installation_dir/examples/libparallel.so`

### spawn
`corhpx app ctx 1 0 libspawn.so`
