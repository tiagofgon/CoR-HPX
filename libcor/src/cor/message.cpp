#include "cor/message.hpp"

namespace cor {

Message::Message() = default;

Message::~Message() = default;

Message::Message(const Message&) = default;

Message& Message::operator=(const Message&) = default;

Message::Message(Message&&) noexcept = default;

Message& Message::operator=(Message&&) noexcept = default;

// std::size_t Message::Size() const
// {
//     std::ostringstream oss(std::stringstream::binary);
//     cereal::PortableBinaryOutputArchive oarchive(oss);
//     oarchive(*this);
//     return oss.str().size();
// }

void Message::Clear()
{
    _data.clear();
}

std::uint16_t Message::Type() const
{
    return _type;
}

void Message::SetType(std::uint16_t type)
{
    _type = type;
}

idp_t Message::Sender() const
{
    return _sender;
}

void Message::SetSender(idp_t sender)
{
    _sender = sender;
}

}
