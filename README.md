# PlaCoR + HPX
Resource oriented Computing Platform

Bibliotecas necessárias:
- HPX (1.4.1 mínimo)
- Libssh (0.8 mínimo)
- Cmake (3 mínimo)

## Compilação
$HPX_LOCATION -> Localização da biblioteca hpx
$Libssh_LOCATION -> Localização da biblioteca Libssh
$INSTALATION_DIR -> Destino para a instalação do PlaCoR + HPX

cmake -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DLibSSH_ROOT=$Libssh_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..

## Execução
### module
corhpx app ctx 1 0 libmodule.so

### basicOperations
corhpx app ctx 1 0 libbasicOperations.so

### parallel
consola 1:
corhpx app ctx 2 0 ../examples/libparallel.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=..../examples

consola 2:
corhpx app ctx 2 0 ../examples/libparallel.so --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=..../examples

### spawn
corhpx app ctx 1 0 libspawn.so
