#include <hpx/hpx.hpp>

class Container
{

public:
    Container() = default;
    ~Container() = default;

    Container(int _id);

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


class Object: public Container {
public:
    Object() = default;
    void setA(int a) {
        this->a = a;
    }
    void setB(double b) {
        this->b = b;
    }
    void setC(std::string c) {
        this->c = c;
    }

    int getA(){
        return a;
    }
    double getB() {
        return b;
    }
    std::string getC(){
        return c;
    }
    friend class hpx::serialization::access;
    template <typename Archive>
    void serialize(Archive& ar, unsigned) {   
        ar& a;
        ar& b;
        ar& c;
    }

private:
    int a = 1;
    double b = 1.1;
    std::string c = "1.2";
};