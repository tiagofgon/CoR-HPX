#include <hpx/hpx.hpp>

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

