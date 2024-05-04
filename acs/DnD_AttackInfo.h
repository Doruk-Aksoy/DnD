#ifndef DND_ATKINFO_IN
#define DND_ATKINFO_IN

#define ATK_CACHE_SHIFT 9
#define ATK_DTYPE_SHIFT (ATK_CACHE_SHIFT + 3)
#define ATK_DPCT_SHIFT (ATK_DTYPE_SHIFT + 5)

#define ATK_WID_MASK (0x1FF)
#define ATK_CACHE_MASK (0xE)
#define ATK_DTYPE_MASK (0x1F)
#define ATK_PCT_MASK (0x3FF)

#endif