# PlaCoR + HPX
Resource oriented Computing Platform

Caso as dependências da biblioteca não estejam instaladas nos caminhos padrão do sistema, é possível atribuir o caminho para cada biblioteca utilizando as seguintes variáveis:

LibSSH_ROOT: caminho para a pasta do pacote libssh.


Para compilar, criar uma diretoria dentro da diretoria principal e executar:
(necessário definir também a variável anterior)

cmake -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..
