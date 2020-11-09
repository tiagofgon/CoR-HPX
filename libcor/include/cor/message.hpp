#ifndef COR_MESSAGE_HPP
#define COR_MESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include "cor/system/macros.hpp"

#include <hpx/hpx.hpp>

namespace cor {

class Message
{

friend class hpx::serialization::access;

public:
    Message();
    ~Message();

    Message(const Message&);
    Message& operator=(const Message&);

    Message(Message&&) noexcept;
    Message& operator=(Message&&) noexcept;

    // std::size_t Size() const;
    void Clear();

    std::uint16_t Type() const;
    void SetType(std::uint16_t type);

    idp_t Sender() const;
    void SetSender(idp_t sender);
    
    template <typename T>
    T Get(std::size_t index = 0) const;

    template <typename T>
    void Add(T const& data);

private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _type;
        ar & _sender;
        ar & _data;
		// std::cout << "serialized\n";
	}

    std::uint16_t _type;
    idp_t _sender;
    std::vector<std::vector<char>> _data;

};

}

#include "cor/message.tpp"

#endif
