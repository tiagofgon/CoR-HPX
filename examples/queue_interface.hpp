#include <hpx/hpx.hpp>
#include "queue.hpp"
#include "object.hpp"


typedef Queue<Object> _queue_type;

// Functions to interact with the Data
struct Funcion_object1 {
    template <typename ... Args>
    void operator()(_queue_type& obj, Args ... args) {
        obj.Push(args...);
    }
};
template <typename ... Args>
hpx::function<void(_queue_type&, Args ... args)> Push = Funcion_object1();


struct Funcion_object2 {
    Object operator()(_queue_type& obj) {
        return obj.Pop();
    }
};
hpx::function<Object(_queue_type&)> Pop = Funcion_object2();


struct Funcion_object3 {
    size_t operator()(_queue_type& obj) {
        return obj.Size();
    }
};
hpx::function<size_t(_queue_type&)> Size = Funcion_object3();


struct Funcion_object4 {
    int operator()(_queue_type& obj) {
        return obj.GetId();
    }
};
hpx::function<int(_queue_type&)> GetId = Funcion_object4();