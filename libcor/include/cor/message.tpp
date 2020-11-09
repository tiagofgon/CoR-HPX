#ifdef COR_MESSAGE_HPP

#include <sstream>


namespace cor {

template <typename T>
T Message::Get(std::size_t index) const
{
    T obj;
    hpx::serialization::input_archive iarchive(_data.at(index));

    iarchive >> obj;

    return obj;

}

template <typename T>
void Message::Add(T const& obj)
{
    std::vector<char> buffer;
    hpx::serialization::output_archive oarchive(buffer);
    oarchive << obj;

    _data.push_back(buffer);
}

}

#endif
