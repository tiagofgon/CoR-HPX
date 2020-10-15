#ifndef COR_TESTE_EMPTY_HPP
#define COR_TESTE_EMPTY_HPP

#include <string>

#include "cor/resources/resource.hpp"

namespace cor
{

struct TesteEmpty: public hpx::components::abstract_migration_support< hpx::components::component_base<TesteEmpty>, Resource >
{

using base_type = hpx::components::abstract_migration_support<
    hpx::components::component_base<TesteEmpty>, Resource >;

typedef hpx::components::component_base<TesteEmpty>::wrapping_type wrapping_type;
typedef TesteEmpty type_holder;
typedef Resource base_type_holder;

typedef int data_type;

friend class ResourceManager;

    TesteEmpty();
    explicit TesteEmpty(idp_t idp, std::string const& module);
    ~TesteEmpty();

    // Components that should be migrated using hpx::migrate<> need to
    // be Serializable and CopyConstructable. Components can be
    // MoveConstructable in which case the serialized data is moved into the
    // component's constructor.
    TesteEmpty(TesteEmpty&& rhs) :
        base_type(std::move(rhs)),
        variavel(rhs.variavel)
    {}

    TesteEmpty& operator=(TesteEmpty&& rhs)
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
