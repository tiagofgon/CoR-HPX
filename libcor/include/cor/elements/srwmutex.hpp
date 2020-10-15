// #ifndef COR_SRWMUTEX_HPP
// #define COR_SRWMUTEX_HPP

// #include "cereal/access.hpp"

// #include "cor/system/macros.hpp"

// namespace cor {

// class SRWMutex
// {

// friend class cereal::access;

// public:
//     ~SRWMutex();

//     SRWMutex(const SRWMutex&) = delete;
//     SRWMutex& operator=(const SRWMutex&) = delete;

//     SRWMutex(SRWMutex&&) noexcept;
//     SRWMutex& operator=(SRWMutex&&) noexcept;

//     void AcquireRead();
//     void ReleaseRead();

//     void AcquireWrite();
//     void ReleaseWrite();

// protected:
//     SRWMutex();
//     explicit SRWMutex(idp_t idp);

// private:
//     template <typename Archive>
//     void serialize(Archive& ar)
//     {
//         ar(_idp);
//     }

//     idp_t _idp;

// };

// }

// #endif
