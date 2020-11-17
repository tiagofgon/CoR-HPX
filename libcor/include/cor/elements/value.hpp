#ifndef COR_VALUE_HPP
#define COR_VALUE_HPP

#include "cor/system/macros.hpp"


namespace cor {

template <typename T>
class Value
{

public:
    ~Value();
    Value();

    template <typename ... Args>
    explicit Value(idp_t idp, Args&&... args);

    // Value(const Value&) = delete;
    // Value& operator=(const Value&) = delete;

    // Value(Value&&) noexcept;
    // Value& operator=(Value&&) noexcept;

    T &operator*() 
    { 
        std::cout << "AQUI_1" << std::endl;
        return _value; 
    }

    T const &operator*() const 
    { 
        std::cout << "AQUI_2" << std::endl;
        return _value; 
    }

    T const *operator->() const 
    { 
        std::cout << "AQUI_3" << std::endl;
        return &_value; 
    }

    T *operator->() 
    { 
        std::cout << "AQUI_4" << std::endl;
        return &_value; 
    }

    T Fetch() const;
    T* Get();

    template <typename Archive>
    void serialize(Archive& ar, unsigned version)
    {
        ar & _idp;
        ar & _value;
    }

private:
    idp_t _idp;
    T _value;

};


}


#include "cor/elements/value.tpp"


#endif
