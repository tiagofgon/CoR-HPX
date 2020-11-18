# PlaCoR + HPX
Resource oriented Computing Platform

Caso as dependências da biblioteca não estejam instaladas nos caminhos padrão do sistema, é possível atribuir o caminho para cada biblioteca utilizando as seguintes variáveis:

LibSSH_ROOT: caminho para a pasta do pacote libssh.


Para compilar, criar uma diretoria dentro da diretoria principal e executar:
(necessário definir a variável LibSSH_ROOT caso seja necessário)

cmake -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..

## Execução
### module
./corhpx app ctx 1 0 ../examples/libmodule.so

### basicOperations
./corhpx app ctx 1 0 ../examples/libbasicOperations.so

### parallel
consola 1:
./corhpx app ctx 2 0 ../examples/libparallel.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=..../examples

consola 2:
./corhpx app ctx 2 0 ../examples/libparallel.so --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=..../examples

### spawn
./corhpx app ctx 1 0 ../examples/libspawn.so
