#ifndef DND_BITSET_IN
#define DND_BITSET_IN

typedef struct {
    int bit_count;              // how many bits is the bitset responsible of (not size of the bits array in ints)
    int elem_count;             // how many int elements are in bits array
    int[]? bits;
} bitset_T;

// given N amount of bits in size, create the array to accommodate that many bits with 32 bit ints, gives zero'd out memory
bitset_T* CreateBitset(int size) {
    bitset_T* res = bcs::malloc(SIZEOF_INT * 2);

    res.bit_count = size;

    res.elem_count = (size + 31) / 32;
    res.bits = bcs::arrNew(res.elem_count);

    for(int i = 0; i < res.elem_count; ++i)
        res.bits[i] = 0;

    return res;
}

#endif