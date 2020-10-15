#include "cor/message_hpx.hpp"

namespace cor {

Message_HPX::Message_HPX() = default;

Message_HPX::~Message_HPX() = default;

Message_HPX::Message_HPX(const Message_HPX&) = default;

Message_HPX& Message_HPX::operator=(const Message_HPX&) = default;

Message_HPX::Message_HPX(Message_HPX&&) noexcept = default;

Message_HPX& Message_HPX::operator=(Message_HPX&&) noexcept = default;

// NECESSARIO ATUALIZAR FUNCAO
std::size_t Message_HPX::Size() const
{
    // std::ostringstream oss(std::stringstream::binary);
    // cereal::PortableBinaryOutputArchive oarchive(oss);
    // oarchive(*this);
    // return oss.str().size();
    return 1;
}

void Message_HPX::Clear()
{
    _data.clear();
}

std::uint16_t Message_HPX::Type() const
{
    return _type;
}

void Message_HPX::SetType(std::uint16_t type)
{
    _type = type;
}

idp_t Message_HPX::Sender() const
{
    return _sender;
}

void Message_HPX::SetSender(idp_t sender)
{
    _sender = sender;
}

}
