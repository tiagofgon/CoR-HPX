# PlaCoR + HPX
Resource oriented Computing Platform

Caso as dependências da biblioteca não estejam instaladas nos caminhos padrão do sistema, é possível atribuir o caminho para cada biblioteca utilizando as seguintes variáveis:

Spread_ROOT: caminho para a pasta do pacote Spread.
SSRCSpread_ROOT: caminho para a pasta do pacote libssrcspread.
LibEvent_ROOT: caminho para a pasta do pacote libevent.
Cereal_ROOT: caminho para a pasta do pacote cereal.
LibSSH_ROOT: caminho para a pasta do pacote libssh.
BOOST_ROOT: caminho para a pasta do pacote boost.
ROOT_ROOT: caminho para a pasta do pacote ROOT.



Para compilar, criar uma diretoria dentro da diretoria principal e executar:
(necessário definir também as variáveis anteriores)

cmake -DHPX_DIR=$HPX_LOCATION/lib/cmake/HPX -DCMAKE_PREFIX_PATH=$HPX_LOCATION -DCMAKE_INSTALL_PREFIX=$INSTALATION_DIR ..
