#ifndef COR_TESTE_HPP
#define COR_TESTE_HPP

#include <string>

#include "cor/resources/resource.hpp"

namespace cor
{

struct Teste: public hpx::components::abstract_migration_support< hpx::components::component_base<Teste>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<Teste>, Resource >;

typedef hpx::components::component_base<Teste>::wrapping_type wrapping_type;
typedef Teste type_holder;
typedef Resource base_type_holder;

typedef int data_type;

friend class ResourceManager;

    Teste();
    explicit Teste(idp_t idp, std::string const& module);
    ~Teste();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    Teste(Teste&& rhs) :
        base_type(std::move(rhs)),
        variavel(rhs.variavel)
    {}

    Teste& operator=(Teste&& rhs)
    {
        this->Resource::operator=(std::move(static_cast<Resource&>(rhs)));
        variavel = rhs.variavel;
        return *this;
    }


    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & hpx::serialization::base_object<Resource>(*this);
        ar & variavel;
    }


private:
    int variavel;

};


}


#endif
