// #ifdef COR_MESSAGE_HPP

// #include <sstream>

// #include "cereal/archives/portable_binary.hpp"

// namespace cor {

// template <typename T>
// T Message::Get(std::size_t index) const
// {
//     T obj;

//     std::istringstream iss(_data.at(index), std::istringstream::binary);
//     cereal::PortableBinaryInputArchive iarchive(iss);
//     iarchive(obj);

//     return obj;
// }

// template <typename T>
// void Message::Add(T const& obj)
// {
//     std::ostringstream oss(std::stringstream::binary);
//     cereal::PortableBinaryOutputArchive oarchive(oss);
//     oarchive(obj);

//     _data.push_back(oss.str());
// }

// }

// #endif
