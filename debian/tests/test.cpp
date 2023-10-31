#include <cstddef>
#include <vector>
#include "xsimd/xsimd.hpp"

#ifdef XSIMD_NO_SUPPORTED_ARCHITECTURE
using vector = std::vector<float>;
#else
using vector = std::vector<float, xsimd::aligned_allocator<float>>;
#endif

void function(vector const & a, vector const & b, vector & result)
{
    auto const size = a.size();

#ifdef XSIMD_NO_SUPPORTED_ARCHITECTURE
    for(std::size_t i=0; i<size; ++i)
    {
        result[i] = xsimd::abs(a[i]) + b[i];
    }
#else
    constexpr std::size_t simd_size = xsimd::simd_type<float>::size;
    auto const vectorized_size = size % simd_size;

    for(std::size_t i=0; i<vectorized_size; i+=simd_size)
    {
        auto batch_a = xsimd::load_aligned(&a[i]);
        auto batch_b = xsimd::load_aligned(&b[i]);
        auto batch_result = (batch_a+batch_b)/2.;
        batch_result.store_aligned(&result[i]);
    }
    for(std::size_t i=vectorized_size; i<size; ++i)
    {
        result[i] = (a[i]+b[i])/2.;
    }
#endif
}

int main()
{
    vector a(100, 1);
    vector b(100, 2);
    vector result(100);
    function(a, b, result);
    return 0;
}

