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
    base_type(idp),
    container{idp},
    dynamic_organizer{idp, module}
{
    std::cout << "Criado um componente \"Teste\"" << std::endl;
}


Teste::~Teste() = default;





/* DynamicOrganizer interface */
void Teste::Join(idp_t idp, std::string const& name) {
    return dynamic_organizer.Join(idp, name);
}

void Teste::Leave(idp_t idp) {
    return dynamic_organizer.Leave(idp);
}

std::string Teste::GetModuleName() {
    return dynamic_organizer.GetModuleName();
}

std::size_t Teste::GetTotalMembers() {
    return dynamic_organizer.GetTotalMembers();
}

std::vector<idp_t> Teste::GetMemberList() {
    return dynamic_organizer.GetMemberList();
}

idp_t Teste::GetIdp1(idm_t idm) {
    return dynamic_organizer.GetIdp(idm);
}

idp_t Teste::GetIdp2(std::string const& name) {
    return dynamic_organizer.GetIdp(name);
}

idm_t Teste::GetIdm1(idp_t idp) {
    return dynamic_organizer.GetIdm(idp);
}

idm_t Teste::GetIdm2(std::string const& name) {
    return dynamic_organizer.GetIdm(name);
}

idp_t Teste::GetDynamicIdp() {
    return dynamic_organizer.GetDynamicIdp();
}



/* Container interface */
std::string Teste::GetGlobalContext()
{
    return container.GetGlobalContext();
}

std::string Teste::GetLocalContext()
{
    return container.GetLocalContext();
}

unsigned int Teste::GetTotalPods()
{
    return container.GetTotalPods();
}

unsigned int Teste::GetTotalDomains()
{
    return container.GetTotalDomains();
}

idp_t Teste::GetActiveResourceIdp(size_t id)
{
    return container.GetActiveResourceIdp(id);
}

idp_t Teste::GetPredecessorIdp(idp_t idp)
{
    return container.GetPredecessorIdp(idp);
}

idp_t Teste::Spawn(std::string const& context, unsigned int npods, std::string const& module, std::vector<std::string> const& args, std::vector<std::string> const& hosts)
{
    return container.Spawn(context, npods, module, args, hosts);  
}

idp_t Teste::GetContainerIdp()
{
    return container.GetContainerIdp();  
}



}

typedef cor::Teste Teste;
typedef hpx::components::component<Teste> Teste_type;

HPX_REGISTER_DERIVED_COMPONENT_FACTORY(Teste_type, Teste, "Resource");




// DynamicOranizer actions
typedef cor::Teste::Join_action_Teste Join_action_Teste;
typedef cor::Teste::Leave_action_Teste Leave_action_Teste;
typedef cor::Teste::GetModuleName_action_Teste GetModuleName_action_Teste;
typedef cor::Teste::GetTotalMembers_action_Teste GetTotalMembers_action_Teste;
typedef cor::Teste::GetMemberList_action_Teste GetMemberList_action_Teste;
typedef cor::Teste::GetIdp1_action_Teste GetIdp1_action_Teste;
typedef cor::Teste::GetIdp2_action_Teste GetIdp2_action_Teste;
typedef cor::Teste::GetIdm1_action_Teste GetIdm1_action_Teste;
typedef cor::Teste::GetIdm2_action_Teste GetIdm2_action_Teste;
typedef cor::Teste::GetDynamicIdp_action_Teste GetDynamicIdp_action_Teste;

HPX_REGISTER_ACTION(Join_action_Teste);
HPX_REGISTER_ACTION(Leave_action_Teste);
HPX_REGISTER_ACTION(GetModuleName_action_Teste);
HPX_REGISTER_ACTION(GetTotalMembers_action_Teste);
HPX_REGISTER_ACTION(GetMemberList_action_Teste);
HPX_REGISTER_ACTION(GetIdp1_action_Teste);
HPX_REGISTER_ACTION(GetIdp2_action_Teste);
HPX_REGISTER_ACTION(GetIdm1_action_Teste);
HPX_REGISTER_ACTION(GetIdm2_action_Teste);
HPX_REGISTER_ACTION(GetDynamicIdp_action_Teste);



// Container actions
typedef cor::Teste::GetGlobalContext_action_Teste GetGlobalContext_action_Teste;
typedef cor::Teste::GetLocalContext_action_Teste GetLocalContext_action_Teste;
typedef cor::Teste::GetTotalPods_action_Teste GetTotalPods_action_Teste;
typedef cor::Teste::GetTotalDomains_action_Teste GetTotalDomains_action_Teste;
typedef cor::Teste::GetActiveResourceIdp_action_Teste GetActiveResourceIdp_action_Teste;
typedef cor::Teste::GetPredecessorIdp_action_Teste GetPredecessorIdp_action_Teste;
typedef cor::Teste::Spawn_action_Teste Spawn_action_Teste;
typedef cor::Teste::GetContainerIdp_action_Teste GetContainerIdp_action_Teste;

HPX_REGISTER_ACTION(GetGlobalContext_action_Teste);
HPX_REGISTER_ACTION(GetLocalContext_action_Teste);
HPX_REGISTER_ACTION(GetTotalPods_action_Teste);
HPX_REGISTER_ACTION(GetTotalDomains_action_Teste);
HPX_REGISTER_ACTION(GetActiveResourceIdp_action_Teste);
HPX_REGISTER_ACTION(GetPredecessorIdp_action_Teste);
HPX_REGISTER_ACTION(Spawn_action_Teste);
HPX_REGISTER_ACTION(GetContainerIdp_action_Teste);


// typedef hpx::components::component<Teste> server_type;
// HPX_REGISTER_COMPONENT(server_type, Teste);