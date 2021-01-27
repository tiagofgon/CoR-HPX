#include <hpx/hpx.hpp>
#include "queue.hpp"
#include "queue_type_object.hpp"


typedef MyQueue<Object> myqueue_type;

// Functions to interact with the Data
struct Funcion_object1 {
    template <typename ... Args>
    void operator()(myqueue_type* obj, Args ... args) {
        obj->Push(args...);
    }
};
template <typename ... Args>
hpx::function<void(myqueue_type*, Args ... args)> Push = Funcion_object1();


struct Funcion_object2 {
    Object operator()(myqueue_type* obj) {
        return obj->Pop();
    }
};
hpx::function<Object(myqueue_type*)> Pop = Funcion_object2();


struct Funcion_object3 {
    size_t operator()(myqueue_type* obj) {
        return obj->Size();
    }
};
hpx::function<size_t(myqueue_type*)> Size = Funcion_object3();