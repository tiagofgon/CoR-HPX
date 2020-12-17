// // #include "cor/cor_hpx.hpp"

// // extern "C"
// // {
// //     void Main(int rsc_idp);
// // }

// // void Main(int rsc_idp)
// // {
// //     auto domain = cor::GetDomain();
// //     auto agent_idp = domain->GetActiveResourceIdp();
// //     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
// //     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
// //     auto clos_size = clos->GetTotalMembers();
// //     auto rank = clos->GetIdm(agent_idp);
// //     auto parent_idp = clos->GetParent();

// //     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
// // }



// #include "cor/cor.hpp"
// // #include <hpx/hpx.hpp>



// // struct Object: public hpx::components::component_base<Object> 
// // {
// //     Object(int base_data = 0) :
// //         _base_data(base_data) {
// //         std::cout << "Object()" << std::endl;
// //     }
// // private:
// //     int _base_data;
// // };

// // HPX_REGISTER_COMPONENT_MODULE();
// // HPX_REGISTER_DYNAMIC_MODULE();


// // typedef hpx::components::component<Object> Object_type;
// // HPX_REGISTER_COMPONENT(Object_type, Object);



// struct BaseObject: public hpx::components::abstract_base_migration_support< hpx::components::abstract_component_base<BaseObject> >
// {


//     BaseObject(int base_data = 0) :
//         base_data_(base_data)
//     {
//         std::cout << "BaseObject()" << std::endl;
//     }

//     virtual ~BaseObject() = default;

//     BaseObject(BaseObject&& rhs)
//       : base_data_(std::move(rhs.base_data_))
//     {
//     }

//     BaseObject& operator=(BaseObject&& rhs)
//     {
//         base_data_ = std::move(rhs.base_data_);
//         return *this;
//     }

//     template <typename Archive>
//     void serialize(Archive& ar, unsigned version)
//     {
//         ar & base_data_;
//     }

// private:
//     int base_data_;

// };

// HPX_DEFINE_GET_COMPONENT_TYPE(BaseObject);






// struct Object: public hpx::components::abstract_migration_support< hpx::components::component_base<Object>, BaseObject >
// {

// using base_type = hpx::components::abstract_migration_support<
//     hpx::components::component_base<Object>, BaseObject >;

// typedef hpx::components::component_base<Object>::wrapping_type wrapping_type;
// typedef Object type_holder;
// typedef BaseObject base_type_holder;


//     Object(int base_data = 0) :
//         base_type(base_data)
//     {
//         std::cout << "Object()" << std::endl;
//     }


//     Object(Object&& rhs) :
//         base_type(std::move(rhs))
//     {}


//     Object& operator=(Object&& rhs)
//     {
//         this->BaseObject::operator=(std::move(static_cast<BaseObject&>(rhs)));
//         return *this;
//     }


//     template <typename Archive>
//     void serialize(Archive& ar, unsigned version)
//     {
//         ar & hpx::serialization::base_object<BaseObject>(*this);
//     }

// };


// typedef hpx::components::component<Object> server_type;
// HPX_REGISTER_DERIVED_COMPONENT_FACTORY(server_type, Object,
//     "BaseObject");

// HPX_REGISTER_COMPONENT_MODULE();

// extern "C"
// {
//     void Main(int rsc_idp);
// }


// void Main(int rsc_idp)
// {
//     auto domain = cor::GetDomain().get();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
//     auto clos_size = clos->GetTotalMembers();
//     auto rank = clos->GetIdm(agent_idp);
//     auto parent_idp = clos->GetParent();


//     std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";

//     // domain->Migrar();


//     auto meta_domain = domain->GetLocalResource<cor::Domain_Client>(cor::MetaDomain);
//     auto member_list = meta_domain->GetMemberList();
//     member_list.erase(std::remove(member_list.begin(), member_list.end(), cor::MetaDomain), member_list.end());
//     member_list.erase(std::remove(member_list.begin(), member_list.end(), domain->Idp()), member_list.end());

//     std::string const& barrier_name = "barrierlll";
//     idp_t const& clos_idpp = clos_idp;
//     auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), barrier_name, clos_idpp);
    
//     auto id_gid = hpx::new_<Object>(hpx::find_here(), 99).get();


//     if(rank==0) {
//         auto remote_agent_idp = domain->GetIdp(1);

//         auto data = domain->CreateLocal<cor::Data_Client<int>>(domain->Idp(), "data", 89);

//         // std::cout << "Localidade ANTES de migrar: " << hpx::get_colocation_id(hpx::launch::sync, data->GetGid()) << std::endl;
//         // std::cout << "Localidade DEPOIS de migrar: " << hpx::get_colocation_id(hpx::launch::sync, data->GetGid()) << std::endl;
//         std::cout << "Meu Domain idp: " << domain->Idp() << ", Remote Domain idp: " << member_list[0] << "\n";
//     }
//     barrier->Synchronize();
//     if(rank==1) {
//         auto remote_agent_idp = domain->GetIdp(0);
//         auto remote_domain_idp = member_list[0];

//         auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domain_idp, domain->Idp(), "");
//         auto data_idp = remote_domain->GetIdp("data");
//         auto data = domain->CreateReference<cor::Data_Client<int>>(data_idp, domain->Idp(), "data");

//         data->Acquire();
//         auto final_value = **data;
//         data->Release();
//         std::cout << "Valor do Data: " << final_value << "\n";
//         std::cout << "Meu Domain idp: " << domain->Idp() << ", Remote Domain idp: " << member_list[0] << "\n";
//     }


// }
