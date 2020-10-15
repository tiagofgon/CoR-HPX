// #ifdef COR_MESSAGE_HPP

// #include <hpx/serialization/serialize.hpp>
// #include <hpx/serialization/input_archive.hpp>
// #include <hpx/serialization/output_archive.hpp>

// namespace cor {

// template <typename T>
// T Message_HPX::Get(std::size_t index) const {
//     // T obj;

//     // std::istringstream iss(_data.at(index), std::istringstream::binary);
//     // cereal::PortableBinaryInputArchive iarchive(iss);
//     // iarchive(obj);

//     // return obj;

//     T obj;
//     std::vector<char> iss = _data.at(index);
//     hpx::serialization::input_archive iarchive(iss);
//     iarchive >> obj;

//     return obj;
// }

// template <typename T>
// void Message_HPX::Add(T const& obj) {
//     // std::ostringstream oss(std::stringstream::binary);
//     // cereal::PortableBinaryOutputArchive oarchive(oss);
//     // oarchive(obj);

//     // _data.push_back(oss.str());


//     std::vector<char> buffer;
//     hpx::serialization::output_archive oarchive(buffer);
//     oarchive << obj;
//     _data.push_back(buffer);

// }

// }

// #endif