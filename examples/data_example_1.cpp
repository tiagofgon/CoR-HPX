/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    Compile with --hpx:ini=hpx.component_paths= $location_of_the_example
    
    To run: 
        ./corhpx apps ctx 1 0 ../examples/libdata_example_1.so --hpx:ini=hpx.component_paths=../examples
*/

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}


class Object_1
{
public:
    friend class hpx::serialization::access;

    Object_1() = default;
    ~Object_1() = default;

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
REGISTER_DATA(Object_1); // need to register new Data type


// Functions to interact with the Data
struct Funcion_Object_1_1 {
    void operator()(Object_1 &obj, int i) {
        obj.setId(i);
    }
};
hpx::function<void(Object_1&, int)> setId = Funcion_Object_1_1();

struct Funcion_Object_1_2 {
    size_t operator()(Object_1 &obj) {
        return obj.getId();
    }
};
hpx::function<size_t(Object_1&)> getId = Funcion_Object_1_2();




void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);

    auto data = domain->CreateLocal<cor::Data_Client<Object_1>>(domain->Idp(), "data");

    // setId with 6 through an action
    data->Run(setId, 6).get();

    // getId through an action
    size_t res = data->Run(getId).get();
    std::cout << res << std::endl;

    // getId directly on the object
    data->AcquireWrite();
    auto obj = data->Get();
    std::cout << obj->getId() << std::endl;
    data->ReleaseWrite();

}