#ifndef DND_ATKINFO_IN
#define DND_ATKINFO_IN

#define ATK_CACHE_SHIFT 9
#define ATK_DTYPE_SHIFT 6
#define ATK_DPCT_SHIFT 5

#define CACHE_SHIFT 9
#define DTYPE_SHIFT 15
#define DPCT_SHIFT 20
#define BITMASK_NOFACTOR (0xFFFFF) // depends on DPCT_SHIFT, 17 1s

#define ATK_WID_MASK (0x1FF) // 9
#define ATK_CACHE_MASK (0x3F) // 6
#define ATK_DTYPE_MASK (0x1F) // 5
#define ATK_DPCT_MASK (0x3FF)

#define SPELL_DMG_SHIFT 16
#define SPELL_DTYPE_SHIFT 5

#define SPELLDMG_MASK (0xFFFF)
#define SPELLDTYPE_MASK (0x1F)
#define SPELLID_MASK (0x3FF)

#endif