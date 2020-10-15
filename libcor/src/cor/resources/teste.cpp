#include "cor/resources/teste.hpp"

namespace cor
{

// Teste::Teste() :
//     base_type(),
//     container{},
//     dynamic_organizer{}
// {
//     std::cout << "Criado um componente \"Teste\"" << std::endl;
// }

Teste::Teste() = default;

Teste::Teste(idp_t idp, std::string const& module) :
    base_type(idp)
{
    std::cout << "Criado um componente \"Teste\"" << std::endl;
}


Teste::~Teste() = default;



}

typedef cor::Teste Teste;
typedef hpx::components::component<Teste> Teste_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Teste_type, Teste, "Resource");