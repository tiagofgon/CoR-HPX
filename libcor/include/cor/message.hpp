// #ifndef COR_MESSAGE_HPP
// #define COR_MESSAGE_HPP

// #include <cstddef>
// #include <cstdint>
// #include <vector>

// #include <cereal/types/vector.hpp>
// #include <cereal/types/string.hpp>

// #include "cor/system/macros.hpp"

// namespace cor {

// class Message
// {

// friend class cereal::access;

// public:
//     Message();
//     ~Message();

//     Message(const Message&);
//     Message& operator=(const Message&);

//     Message(Message&&) noexcept;
//     Message& operator=(Message&&) noexcept;

//     std::size_t Size() const;
//     void Clear();

//     std::uint16_t Type() const;
//     void SetType(std::uint16_t type);

//     idp_t Sender() const;
//     void SetSender(idp_t sender);
    
//     template <typename T>
//     T Get(std::size_t index = 0) const;

//     template <typename T>
//     void Add(T const& data);

// private:
//     template <typename Archive>
//     void serialize(Archive& ar)
//     {
//         ar(_type, _sender, _data);
//     }

//     std::uint16_t _type;
//     idp_t _sender;
//     std::vector<std::string> _data;

// };

// }

// #include "cor/message.tpp"

// #endif
