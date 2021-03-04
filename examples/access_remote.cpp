// Tiago Gonçalves & António Pina - University of Minho - LIP, 2021
//
// MyQueue it's a class derived from Container.
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
    Container() = default;
    Container(int id) : _id{id} {}
    ~Container() = default;

    int GetID() const {
        return _id;
    }

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & _id;
    }

private:
    int _id;
};

template <typename T>
class MyQueue : public Container
{
public:
    MyQueue() = default;
    ~MyQueue() = default;

    MyQueue(int id) : Container(id) {}

    template <typename ... Args>
    void Push(Args ... args) {
        (myqueue.push_back(args), ...);
    }

    T Pop() {
        T element = myqueue.front();
        myqueue.erase(myqueue.begin());
        return element;
    }

    size_t Size() {
        return myqueue.size();
    }

    friend class hpx::serialization::access;
    
    template <typename Archive>
    void serialize(Archive& ar, unsigned) {   
        ar& myqueue;
    }

private:
    std::vector<T> myqueue;
};


typedef MyQueue<int> queue_type;
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


    // create resource Data in rank 1
    if(rank == 1) {
        auto data = domain->CreateLocal<cor::Data_Client<queue_type>>(domain->Idp(), "data");

        // Push 5 in the queue through local resource pointer
        data->AcquireWrite();
        auto obj = data->Get();
        obj->Push(5);
        data->ReleaseWrite();

        auto data_idp1 = domain->GetIdp("data");
        std::cout << "data idp: " << data_idp1 << std::endl;
    }

    // make sure Data is created to be accessed on rank 0
    auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
    barrier->Synchronize();

    // Access Data in rank 0
    if(rank == 0) {
        std::vector<idp_t> remote_domains = domain->GetRemoteDomains();

        // create replica from remote domain
        auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domains[0], domain->Idp(), "");

        auto data_idp = remote_domain->GetIdp("data");
        std::cout << "data idp: " << data_idp << std::endl;

        // create replica from remote Data
        auto data = domain->CreateReference<cor::Data_Client<queue_type>>(data_idp, domain->Idp(), "dataReplica");

        // print size of queue
        data->AcquireWrite();
        auto obj = data->Get();
        std::cout << obj->Size() << std::endl;
        data->ReleaseWrite();
    }


}