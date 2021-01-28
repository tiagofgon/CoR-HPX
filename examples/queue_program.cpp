/*|────────────────────────────────────────────────────────|*/
/*|   Tiago Gonçalves - University of Minho - LIP, 2021    |*/
/*|────────────────────────────────────────────────────────|*/

/*
    To run: ./corhpx apps ctx 1 0 ../examples/libqueue_program.so --hpx:ini=hpx.component_paths=../examples/
*/

#include "cor/cor.hpp"
#include "queue_methods.hpp"

extern "C"
{
    void Main(int argc, char *argv[]);
}

typedef MyQueue<Object> myqueue_type;
REGISTER_DATA(myqueue_type); // need to register new Data type

void Main(int argc, char *argv[])
{
    // obter o domínio local
    auto domain = cor::GetDomain();

    // criar um Dado que irá conter a queue
    auto myqueue = domain->CreateLocal<cor::Data_Client<myqueue_type>>(domain->Idp(), "data");
    Object objA; Object objB; Object objC;


    auto f1 = myqueue->Run(Push<Object>, objA);
    auto f2 = myqueue->Run(Push<Object, Object>, objB, objC);

    auto f3 = hpx::dataflow([&myqueue](auto f1, auto f2){
        auto element = myqueue->Run(Pop); // irá retornar o objeto objA ou objB, depende se f1 foi computado primeiro que f2 ou não
    }, f1, f2);


    f3.then([&myqueue](auto f3){ 
        auto size = myqueue->Run(Size);
        std::cout << size.get() << std::endl; // irá imprimir 2
    }).get();

    return;
}
