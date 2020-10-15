#include "cor/resources/teste_empty.hpp"

namespace cor
{

// TesteEmpty::TesteEmpty() :
//     base_type(),
//     container{},
//     dynamic_organizer{}
// {
//     std::cout << "Criado um componente \"TesteEmpty\"" << std::endl;
// }

TesteEmpty::TesteEmpty() = default;

TesteEmpty::TesteEmpty(idp_t idp, std::string const& module) :
    base_type(idp)
{
    std::cout << "Criado um componente \"TesteEmpty\"" << std::endl;
}


TesteEmpty::~TesteEmpty() = default;



}

typedef cor::TesteEmpty TesteEmpty;
typedef hpx::components::component<TesteEmpty> TesteEmpty_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(TesteEmpty_type, TesteEmpty, "Resource");