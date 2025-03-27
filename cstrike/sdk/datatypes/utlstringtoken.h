#pragma once
#include "../../utilities/murmyrhash2.h"

class cUltStringToken
{
public:
    std::uint32_t m_nHashCode;

    cUltStringToken(char const* szString)
    {
        this->SetHashCode(this->MakeStringToken(szString));
    }

    bool operator==(cUltStringToken const& other) const
    {
        return (other.m_nHashCode == m_nHashCode);
    }

    bool operator!=(cUltStringToken const& other) const
    {
        return (other.m_nHashCode != m_nHashCode);
    }

    bool operator<(cUltStringToken const& other) const
    {
        return (m_nHashCode < other.m_nHashCode);
    }

    uint32_t GetHashCode(void) const
    {
        return m_nHashCode;
    }

    void SetHashCode(uint32_t nCode)
    {
        m_nHashCode = nCode;
    }

    __forceinline std::uint32_t MakeStringToken(char const* szString, int nLen)
    {
        std::uint32_t nHashCode = MurmurHash2LowerCaseA(szString, nLen, STRINGTOKEN_MURMURHASH_SEED);
        return nHashCode;
    }

    __forceinline std::uint32_t MakeStringToken(char const* szString)
    {
        return MakeStringToken(szString, (int)strlen(szString));
    }

    cUltStringToken() { m_nHashCode = 0; }
};
