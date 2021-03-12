#ifndef COR_SBARRIER_HPP
#define COR_SBARRIER_HPP

#include "cor/system/macros.hpp"

namespace cor {

class SBarrier
{

friend class Barrier;

public:
    ~SBarrier();

    // SBarrier(const SBarrier&) = delete;
    // SBarrier& operator=(const SBarrier&) = delete;

    // SBarrier(SBarrier&&) noexcept;
    // SBarrier& operator=(SBarrier&&) noexcept;

    void Synchronize();

    idp_t GetIdpClos() const;
    idp_t GetSBarrierIdp() const;

protected:
    SBarrier();
    explicit SBarrier(idp_t idp, idp_t clos);


private:
	template <typename Archive>
	void serialize(Archive& ar, unsigned) {
        ar & _idp;
        ar & _clos;
		// std::cout << "serialized\n";
	}

    idp_t _idp;
    idp_t _clos;

};

}

#endif
