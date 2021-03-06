// Tiago Gonçalves & António Pina - University of Minho - LIP, 2021
//
// Queue it's a class derived from Container.
// This program is intended to access a resource created on another context, in this case, on another pod
//
// Program to be run in parallel with two pods, only.
// Each pod will remotely create an agent in the location of the other pod.
//    Compile with: --hpx:ini=hpx.component_paths= "location of the example"
//
// Console 1: ./corhpx apps ctx 2 1 0 ../examples/libaccess_remote.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities --hpx:ini=hpx.component_paths=../examples
// Console 2: ./corhpx apps ctx 2 1 0 ../examples/libaccess_remote.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker --hpx:ini=hpx.component_paths=../examples
// 
// With MPI in one console only: mpirun -np 2 ./corhpx apps ctx 2 1 0 ../examples/libaccess_remote.so --hpx:ini=hpx.component_paths=../examples
//
// With virtual pods: ./corhpx apps ctx 1 2 0 ../examples/libaccess_remote.so --hpx:ini=hpx.component_paths=../examples
//

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

class Container
{
public:
    Container(int id = 42) :
        _id(id)
    {}

    int GetId() const {
        return _id;
    }

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & _id;
    }

private:
    friend class hpx::serialization::access;
    int _id;
};


template <typename T>
class Queue : public Container
{
public:
    Queue() = default;
    Queue(int id) :
        Container{id}
    {}

    template <typename ... Args>
    void Push(Args ... args) {
        (_fifo.push_back(args), ...);
    }

    T Pop() {
        T element = _fifo.front();
        _fifo.erase(_fifo.begin());
        return element;
    }

    size_t Size() {
        return _fifo.size();
    }
    
    template <typename Archive>
    void serialize(Archive& ar, unsigned) {   
        ar & hpx::serialization::base_object<Container>(*this);
        ar& _fifo;
    }

private:
    friend class hpx::serialization::access;
    std::vector<T> _fifo;
};


typedef Queue<int> queue_type;
REGISTER_DATA(queue_type); // need to register new Data type

void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    std::cout << "ola 1, com rank = " << rank << std::endl;

    // create resource Data in rank 1
    if(rank == 1) {
        auto data = domain->CreateLocal<cor::Data_Client<queue_type>>(domain->Idp(), "data");
        std::cout << "ola 2" << std::endl;
        // Push 5 in the queue through local resource pointer
        data->AcquireWrite();
        auto obj = data->Get();
        // obj->Push(5);
        // obj = nullptr;
        data->ReleaseWrite();

        auto data_idp1 = domain->GetIdp("data");
        std::cout << "data idp: " << data_idp1 << std::endl;
    }

    // // make sure Data is created to be accessed on rank 0
    // auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
    // barrier->Synchronize();

    // // Access Data in rank 0
    // if(rank == 0) {
    //     std::vector<idp_t> remote_domains = domain->GetRemoteDomains();

    //     // create replica from remote domain
    //     auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domains[0], domain->Idp(), "");

    //     auto data_idp = remote_domain->GetIdp("data");
    //     std::cout << "data idp: " << data_idp << std::endl;

    //     // create replica from remote Data
    //     auto data = domain->CreateReference<cor::Data_Client<queue_type>>(data_idp, domain->Idp(), "dataReplica");

    //     // print size of queue
    //     data->AcquireWrite();
    //     auto obj = data->Get();
    //     //std::cout << obj->Size() << std::endl;
    //     data->ReleaseWrite();
    // }


}