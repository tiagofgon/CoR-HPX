// /*|────────────────────────────────────────────────────────|*/
// /*|   Tiago Gonçalves - University of Minho - LIP, 2021    |*/
// /*|────────────────────────────────────────────────────────|*/

// /*
//     To run: ./corhpx apps ctx 1 0 ../examples/libdata_with_user_object.so --hpx:ini=hpx.component_paths=../examples
// */

// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
// }


// class MyObject
// {
// public:
//     friend class hpx::serialization::access;

//     MyObject() = default;
//     ~MyObject() = default;

// 	auto operator()(int elem) {
// 		std::cout << id << " ";
// 	}

//     void setName(std::string name) {
//         this->name = name;
//     }
//     void setId(size_t id) {
//         this->id = id;
//     }
//     std::string getName() {
//         return name;
//     }
//     size_t getId() {
//         return id;
//     }
// private:
//     std::string name;
//     size_t id;

// 	template <typename Archive>
// 	void serialize(Archive& ar, unsigned) {
//         ar & id;
//         ar & name;
// 	}
// };
// REGISTER_DATA(MyObject); // need to register new Data type


// // Functions to interact with the Data
// struct Funcion_object1 {
//     void operator()(MyObject* obj, int i) {
//         obj->setId(i);
//     }
// };
// hpx::function<void(MyObject*, int)> setId = Funcion_object1();

// struct Funcion_object2 {
//     size_t operator()(MyObject* obj) {
//         return obj->getId();
//     }
// };
// hpx::function<size_t(MyObject*)> getId = Funcion_object2();




// void Main(int argc, char *argv[])
// {
//     // obter o domínio local
//     auto domain = cor::GetDomain();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);


//     // // criar um dado no qual irá ser escrito o idp do agente que irá ser criado
//     auto data = domain->CreateLocal<cor::Data_Client<MyObject>>(domain->Idp(), "data");

//     // setId with 6 through an action
//     data->Run(setId, 6).get();

//     // getId through an action
//     size_t res = data->Run(getId).get();
//     std::cout << res << std::endl;

//     // getId directly on the object
//     data->AcquireWrite();
//     auto obj = data->Get();
//     std::cout << obj->getId() << std::endl;
//     data->ReleaseWrite();

// }



/*|────────────────────────────────────────────────────────|*/
/*|   Tiago Gonçalves - University of Minho - LIP, 2021    |*/
/*|────────────────────────────────────────────────────────|*/

/*
    To run: ./corhpx apps ctx 1 0 ../examples/libdata_with_user_object.so --hpx:ini=hpx.component_paths=../examples
*/

// #include "cor/cor.hpp"

// extern "C"
// {
//     void Main(int argc, char *argv[]);
// }

// class MyObject
// {
// public:
//     friend class hpx::serialization::access;

//     MyObject() = default;
//     ~MyObject() = default;

// 	auto operator()(int elem) {
// 		std::cout << id << " ";
// 	}

//     void setName(std::string name) {
//         this->name = name;
//     }
//     void setId(size_t id) {
//         this->id = id;
//     }
//     std::string getName() {
//         return name;
//     }
//     size_t getId() {
//         return id;
//     }
// private:
//     std::string name;
//     size_t id;

// 	template <typename Archive>
// 	void serialize(Archive& ar, unsigned) {
//         ar & id;
//         ar & name;
// 	}
// };
// REGISTER_DATA(MyObject); // need to register new Data type

// void Main(int argc, char *argv[])
// {
//     // obter o domínio local
//     auto domain = cor::GetDomain();
//     auto agent_idp = domain->GetActiveResourceIdp();
//     auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);
//     auto clos_idp = domain->GetPredecessorIdp(agent_idp);
//     auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
//     auto rank = clos->GetIdm(agent_idp);

//     auto data = domain->CreateLocal<cor::Data_Client<MyObject>>(domain->Idp(), "data");


//     data->AcquireWrite();
//     auto obj = data->Get();
//     obj->setId(5);
//     data->ReleaseWrite();


//     auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
//     barrier->Synchronize();

//     if(rank == 1) {
//         auto data_idp1 = domain->GetIdp("data");
//         std::cout << "data idp: " << data_idp1 << std::endl;
//     }


//     if(rank == 0) {
//         std::vector<idp_t> remote_domains = domain->GetRemoteDomains();
//         auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domains[0], domain->Idp(), "");
//         auto data_idp = remote_domain->GetIdp("data");
//         std::cout << "data idp: " << data_idp << std::endl;
//         auto data = domain->CreateReference<cor::Data_Client<MyObject>>(data_idp, domain->Idp(), "dataReplica");

//         data->AcquireWrite();
//         auto obj = data->Get();
//         std::cout << obj->getId() << std::endl;
//         data->ReleaseWrite();
//     }


// }






#include "cor/cor.hpp"
//#include "queue_interface.hpp"
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





typedef MyQueue<int> myqueue_type2;
REGISTER_DATA(myqueue_type2); // need to register new Data type

void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    auto data = domain->CreateLocal<cor::Data_Client<myqueue_type2>>(domain->Idp(), "data");


    data->AcquireWrite();
    auto obj = data->Get();
    obj->Push(5);
    data->ReleaseWrite();


    auto barrier = domain->CreateCollective<cor::Barrier_Client>(clos_idp, domain->Idp(), "barrier", clos_idp);
    barrier->Synchronize();

    if(rank == 1) {
        auto data_idp1 = domain->GetIdp("data");
        std::cout << "data idp: " << data_idp1 << std::endl;
    }


    if(rank == 0) {
        std::vector<idp_t> remote_domains = domain->GetRemoteDomains();
        auto remote_domain = domain->CreateReference<cor::Domain_Client>(remote_domains[0], domain->Idp(), "");
        auto data_idp = remote_domain->GetIdp("data");
        std::cout << "data idp: " << data_idp << std::endl;
        auto data = domain->CreateReference<cor::Data_Client<myqueue_type2>>(data_idp, domain->Idp(), "dataReplica");

        data->AcquireWrite();
        auto obj = data->Get();
        std::cout << obj->Size() << std::endl;
        data->ReleaseWrite();
    }


}