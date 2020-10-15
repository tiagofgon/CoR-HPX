// #ifndef COR_MESSAGE_HPX_HPP
// #define COR_MESSAGE_HPX_HPP


// #include <hpx/hpx.hpp>

// #include <cstddef>
// #include <cstdint>
// #include <vector>

// #include "cor/system/macros.hpp"


// namespace cor {

// class Message_HPX
// {

// friend class hpx::serialization::access;

// public:
//     Message_HPX();
//     ~Message_HPX();

//     Message_HPX(const Message_HPX&);
//     Message_HPX& operator=(const Message_HPX&);

//     Message_HPX(Message_HPX&&) noexcept;
//     Message_HPX& operator=(Message_HPX&&) noexcept;

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
//     std::vector<std::vector<char>> _data;

// };

// }

// #include "cor/message_hpx.tpp"

// #endif
