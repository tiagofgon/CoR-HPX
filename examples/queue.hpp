#include <hpx/hpx.hpp>

template <typename T>
class MyQueue
{
public:
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

