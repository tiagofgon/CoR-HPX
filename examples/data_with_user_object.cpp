/*|────────────────────────────────────────────────────────|*/
/*|   Tiago Gonçalves - University of Minho - LIP, 2021    |*/
/*|────────────────────────────────────────────────────────|*/

/*
    To run: ./corhpx apps ctx 1 0 ../examples/libdata_with_user_object.so --hpx:ini=hpx.component_paths=../examples
*/

#include "cor/cor.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}


class MyObject
{
public:
    friend class hpx::serialization::access;

    MyObject() = default;
    ~MyObject() = default;

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
REGISTER_DATA(MyObject); // need to register new Data type


// Functions to interact with the Data
struct Funcion_object1 {
    void operator()(MyObject &obj, int i) {
        obj.setId(i);
    }
};
hpx::function<void(MyObject&, int)> setId = Funcion_object1();

struct Funcion_object2 {
    size_t operator()(MyObject &obj) {
        return obj.getId();
    }
};
hpx::function<size_t(MyObject&)> getId = Funcion_object2();


void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::Agent_Client<void(int,char**)>>(agent_idp);

    // criar um recurso Data do tipo MyObject
    auto data = domain->CreateLocal<cor::Data_Client<MyObject>>(domain->Idp(), "data");

    // setId with 6 through an action
    data->Run(setId, 6).get();

    // getId through an action
    size_t res = data->Run(getId).get();
    std::cout << res << std::endl;

    // getId directly through the object pointer
    data->AcquireWrite();
    auto obj = data->Get();
    std::cout << obj->getId() << std::endl;
    data->ReleaseWrite();

}