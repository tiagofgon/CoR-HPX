/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    Program to be run in parallel with two processes, only.
    Compile with --hpx:ini=hpx.component_paths= $location_of_the_example

    To run: 
        Console 1: ./corhpx apps ctx 2 0 ../examples/libdata_example_2.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=../examples
        Console 2: ./corhpx apps ctx 2 0 ../examples/libdata_example_2.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=../examples
        With MPI: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libdata_example_2.so --hpx:ini=hpx.component_paths=../examples
*/


#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}


class Object_2
{
public:
    friend class hpx::serialization::access;

    Object_2() = default;
    ~Object_2() = default;

	auto operator()(int elem) {
		std::cout << id << " ";
	}

    void setName(std::string name) {
        this->name = name;
    }
    void setId(size_t id) {
        this->id = id;
    }
    std::string getName() {
        return name;
    }
    size_t getId() {
        return id;
    }
private:
    std::string name;
    size_t id;

	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & id;
        ar & name;
	}
};


REGISTER_DATA(Object_2); // need to register new Data type


// Functions to interact with the Data
struct Funcion_Object_2_1 {
    void operator()(Object_2 &obj, int i) {
        obj.setId(i);
    }
};
hpx::function<void(Object_2&, int)> setId = Funcion_Object_2_1();

struct Funcion_Object_2_2 {
    size_t operator()(Object_2 &obj) {
        return obj.getId();
    }
};
hpx::function<size_t(Object_2&)> getId = Funcion_Object_2_2();



void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    if(rank == 1) {
        auto data = domain->CreateLocal<cor::Data_Client<Object_2>>(domain->Idp(), "data");

        data->AcquireWrite();
        auto obj = data->Get();
        obj->setId(5);
        data->ReleaseWrite();
    }

    auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
    barrier->Synchronize();

    if(rank == 0) {
        std::vector<idp_t> remote_domains = domain->GetRemoteDomains();
        auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domains[0], domain->Idp(), "");
        auto data_idp = remote_domain->GetIdp("data");
        std::cout << "data idp: " << data_idp << std::endl;
        auto data = domain->CreateReference<cor::Data_Client<Object_2>>(data_idp, domain->Idp(), "dataReplica");

        // getId through an action
        size_t res = data->Run(getId).get();
        std::cout << res << std::endl;

        // getId directly on the object, migrating to this domain on Get()
        data->AcquireWrite();
        auto obj = data->Get();
        std::cout << obj->getId() << std::endl;
        data->ReleaseWrite();
    }


}