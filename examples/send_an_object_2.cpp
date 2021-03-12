/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    Program to be run in parallel with two processes, only.
    Sends a user object from one process to another.

    To run: 
        Console 1: ./corhpx apps ctx 2 0 ../examples/libsend_an_object_2.so --hpx:hpx=localhost:1337 --hpx:expect-connecting-localities
        Console 2: ./corhpx apps ctx 2 0 ../examples/libsend_an_object_2.so --hpx:hpx=localhost:1338 --hpx:agas=localhost:1337 --hpx:run-hpx-main --hpx:expect-connecting-localities --hpx:worker
        With MPI: mpirun -np 2 ./corhpx apps ctx 2 0 ../examples/libsend_an_object_2.so
*/


#include "cor/cor.hpp"

class MyObject
{
public:
    friend class hpx::serialization::access;

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

extern "C"
{
    void Main(int argc, char *argv[]);
}

void Main(int argc, char *argv[])
{
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();

    auto agent = domain->GetLocalResource<cor::Agent_Client<void(char**)>>(agent_idp);

    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto clos_size = clos->GetTotalMembers();
    auto rank = clos->GetIdm(agent_idp);
    auto parent_idp = clos->GetParent();

    // rank 0 envia a menagem para o rank 1
    if(rank==0) {
        auto other_rank = clos->GetIdp(1);

        MyObject obj;
        obj.setName("Abelha Maia");
        obj.setId(42);

        cor::Message msg;
        msg.Add<MyObject>(obj);

        agent->Send(other_rank, msg);
    }
    // o rank 1 recebe a mensagem
    else {
        auto msg = agent->Receive();
        MyObject obj = msg.Get<MyObject>();
        auto name = obj.getName();
        auto id = obj.getId();
        std::cout << "rank " << rank << ", name: " << name << ", id: " << id << std::endl;     
    }

    //std::cout << agent_idp << "\t" << rank << "\t" << clos_idp << "\t" << clos_size << "\t" << parent_idp << "\n";
}
