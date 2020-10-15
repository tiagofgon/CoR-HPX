// #ifndef COR_RWMUTEX_HPP
// #define COR_RWMUTEX_HPP

// #include "cor/resources/resource.hpp"
// #include "cor/elements/srwmutex.hpp"

// #include "cereal/types/polymorphic.hpp"

// namespace cor {

// class RWMutex: public Resource, public SRWMutex
// {

// friend class ResourceManager;
// friend class cereal::access;

// public:
//     ~RWMutex();

//     RWMutex(const RWMutex&) = delete;
//     RWMutex& operator=(const RWMutex&) = delete;

//     RWMutex(RWMutex&&) noexcept;
//     RWMutex& operator=(RWMutex&&) noexcept;

// protected:
//     RWMutex();
//     explicit RWMutex(idp_t idp);

// private:
//     template <typename Archive>
//     void serialize(Archive& ar)
//     {
//         ar(cereal::base_class<Resource>(this), cereal::base_class<SRWMutex>(this));
//     }

// };

// }

// #include <cereal/archives/binary.hpp>
// #include <cereal/archives/portable_binary.hpp>

// CEREAL_REGISTER_TYPE(cor::RWMutex);

// #endif
