#include "bit_vector.h"

static void bitvector_print(ScmObj obj, ScmPort *port, ScmWriteContext *ctx)
{
    Scm_Printf(port, "#<bit-vector %p>", obj);
}

SCM_DEFINE_BUILTIN_CLASS_SIMPLE(Scm_BitVectorClass, bitvector_print);

static inline long rank1(u_long b, long i)
{
#if SIZEOF_LONG == 4
    b = i ? (b << (SCM_WORD_BITS - i)) : 0;
    b = ((b & 0xaaaaaaaaUL) >>  1)
       + (b & 0x55555555UL);
    b = ((b & 0xccccccccUL) >>  2)
       + (b & 0x33333333UL);
    b = ((b & 0xf0f0f0f0UL) >>  4)
       + (b & 0x0f0f0f0fUL);
    b = ((b & 0xff00ff00UL) >>  8)
       + (b & 0x00ff00ffUL);
    b = ((b & 0xffff0000UL) >> 16)
       + (b & 0x0000ffffUL);
    return b;
#else
    b = i ? (b << (SCM_WORD_BITS - i)) : 0;
    b = ((b & 0xaaaaaaaaaaaaaaaaUL) >> 1)
       + (b & 0x5555555555555555UL);
    b = ((b & 0xccccccccccccccccUL) >> 2)
       + (b & 0x3333333333333333UL);
    b = ((b & 0xf0f0f0f0f0f0f0f0UL) >> 4)
       + (b & 0x0f0f0f0f0f0f0f0fUL);
    b = ((b & 0xff00ff00ff00ff00UL) >> 8)
       + (b & 0x00ff00ff00ff00ffUL);
    b = ((b & 0xffff0000ffff0000UL) >> 16)
       + (b & 0x0000ffff0000ffffUL);
    b = ((b & 0xffffffff00000000UL) >> 32)
       + (b & 0x00000000ffffffffUL);
    return b;
#endif
}

static inline long select1(u_long b, long i)
{
#if SIZEOF_LONG == 4
    u_long b1 = ((b  & 0xaaaaaaaaUL) >> 1)
               + (b  & 0x55555555UL);
    u_long b2 = ((b1 & 0xccccccccUL) >> 2)
               + (b1 & 0x33333333UL);
    u_long b3 = ((b2 & 0xf0f0f0f0UL) >> 4)
               + (b2 & 0x0f0f0f0fUL);
    u_long b4 = ((b3 & 0xff00ff00UL) >> 8)
               + (b3 & 0x00ff00ffUL);
    u_long v0, v1, v2, v3, v4;
    long pos = 0;

    i++;
    v4 = (b4 >> pos) & 0xffffUL;
    if (i > v4) {
        i -= v4;
        pos += 16;
    }
    v3 = (b3 >> pos) & 0x00ffUL;
    if (i > v3) {
        i -= v3;
        pos += 8;
    }
    v2 = (b2 >> pos) & 0x000fUL;
    if (i > v2) {
        i -= v2;
        pos += 4;
    }
    v1 = (b1 >> pos) & 0x0003UL;
    if (i > v1) {
        i -= v1;
        pos += 2;
    }
    v0 = (b  >> pos) & 0x0001UL;
    if (i > v0) {
        i -= v0;
        pos += 1;
    }
    return i == 1 && (b & (0x1UL << pos)) ? pos : -1;
#else
    u_long b1 = ((b  & 0xaaaaaaaaaaaaaaaaUL) >> 1)
               + (b  & 0x5555555555555555UL);
    u_long b2 = ((b1 & 0xccccccccccccccccUL) >> 2)
               + (b1 & 0x3333333333333333UL);
    u_long b3 = ((b2 & 0xf0f0f0f0f0f0f0f0UL) >> 4)
               + (b2 & 0x0f0f0f0f0f0f0f0fUL);
    u_long b4 = ((b3 & 0xff00ff00ff00ff00UL) >> 8)
               + (b3 & 0x00ff00ff00ff00ffUL);
    u_long b5 = ((b4 & 0xffff0000ffff0000UL) >> 16)
               + (b4 & 0x0000ffff0000ffffUL);
    u_long v0, v1, v2, v3, v4, v5;
    long pos = 0;

    i++;
    v5 = (b5 >> pos) & 0xffffffffUL;
    if (i > v5) {
        i -= v5;
        pos += 32;
    }
    v4 = (b4 >> pos) & 0x0000ffffUL;
    if (i > v4) {
        i -= v4;
        pos += 16;
    }
    v3 = (b3 >> pos) & 0x000000ffUL;
    if (i > v3) {
        i -= v3;
        pos += 8;
    }
    v2 = (b2 >> pos) & 0x0000000fUL;
    if (i > v2) {
        i -= v2;
        pos += 4;
    }
    v1 = (b1 >> pos) & 0x00000003UL;
    if (i > v1) {
        i -= v1;
        pos += 2;
    }
    v0 = (b  >> pos) & 0x00000001UL;
    if (i > v0) {
        i -= v0;
        pos += 1;
    }
    return i == 1 && (b & (0x1UL << pos)) ? pos : -1;
#endif
}

ScmBitVector *Scm_MakeBitVector(long length, int fill)
{
    ScmBitVector *bv;

    if (length < 0)
        Scm_Error("invalid argument: %d", length);
    bv = SCM_NEW(ScmBitVector);
    SCM_SET_CLASS(bv, SCM_CLASS_BIT_VECTOR);
    bv->length    = length;
    bv->builded   = 0;
    bv->numBlocks = SCM_BITS_NUM_WORDS(length);
    bv->blocks    = (length == 0) ? NULL
        : SCM_NEW_ATOMIC_ARRAY(ScmBits, bv->numBlocks);
    bv->rankTable = (length == 0) ? NULL
        : SCM_NEW_ATOMIC_ARRAY(long, bv->numBlocks + 1);
    Scm_BitVectorClear(bv);
    if (fill && (0 < bv->length)) {
        Scm_BitVectorFill(bv, fill, 0, bv->length);
        Scm_BitVectorBuild(bv);
    }
    return bv;
}

ScmBitVector *Scm_BitVectorClear(ScmBitVector *bv)
{
    if (0 < bv->length) {
        long i;
        for (i = 0; i < bv->numBlocks; i++)
            bv->rankTable[i] = bv->blocks[i] = 0;
        bv->rankTable[i] = 0;
    }
    bv->builded = 1;
    return bv;
}

ScmBitVector *Scm_BitVectorBuild(ScmBitVector *bv)
{
    if (0 < bv->length) {
        long i, r = 0;
        for (i = 0; i < bv->numBlocks; i++) {
            bv->rankTable[i] = r;
            r += rank1(bv->blocks[i], SCM_WORD_BITS);
        }
        bv->rankTable[i] = r;
    }
    bv->builded = 1;
    return bv;
}

int Scm_BitVectorRef(ScmBitVector *bv, long i)
{
    long b;
    ScmBits m;

    if (i < 0 || bv->length <= i)
        Scm_Error("argument out of range: %d", i);
    b = i / SCM_WORD_BITS;
    m = 0x1UL << (i % SCM_WORD_BITS);
    return (bv->blocks[b] & m) ? 1 : 0;
}

ScmBitVector *Scm_BitVectorSet(ScmBitVector *bv, long i, int bit)
{
    long b;
    ScmBits m;

    if (i < 0 || bv->length <= i)
        Scm_Error("argument out of range: %d", i);
    b = i / SCM_WORD_BITS;
    m = 0x1UL << (i % SCM_WORD_BITS);
    if (bit)
        bv->blocks[b] |= m;
    else
        bv->blocks[b] &= ~m;
    bv->builded = 0;
    return bv;
}

ScmBitVector *Scm_BitVectorFill(ScmBitVector *bv,
                                int fill, long start, long end)
{
    long s, e, sb, eb;

    if (start < 0 || bv->length <= start)
        Scm_Error("argument out of range: %d", start);
    if (end < 0) {
        end = bv->length;
    } else if (end > bv->length) {
        Scm_Error("argument out of range: %d", end);
    } else if (end < start) {
        Scm_Error("invalid argument: %d, %d", start, end);
    } else if (start == end) {
        return bv;
    }
    s  = start / SCM_WORD_BITS;
    e  = (end - 1) / SCM_WORD_BITS;
    sb = start % SCM_WORD_BITS;
    eb = end   % SCM_WORD_BITS;
    if (fill) {
        if (s == e) {
            bv->blocks[s] |= SCM_BITS_MASK(sb, eb);
        } else {
            bv->blocks[s] |= SCM_BITS_MASK(sb, 0);
            for (s++; s < e; s++)
                bv->blocks[s] = ~0UL;
            bv->blocks[e] |= SCM_BITS_MASK(0, eb);
        }
    } else {
        if (s == e) {
            bv->blocks[s] &= ~SCM_BITS_MASK(sb, eb);
        } else {
            bv->blocks[s] &= ~SCM_BITS_MASK(sb, 0);
            for (s++; s < e; s++)
                bv->blocks[s] = 0UL;
            bv->blocks[e] &= ~SCM_BITS_MASK(0, eb);
        }
    }
    bv->builded = 0;
    return bv;
}

long Scm_BitVectorRank(ScmBitVector *bv, long i, int bit)
{
    long b, r;

    if (i < 0 || bv->length < i)
        Scm_Error("argument out of range: %d", i);
    if (bv->length == 0)
        return 0;
    if (!bv->builded)
        Scm_BitVectorBuild(bv);
    b = i / SCM_WORD_BITS;
    r = bv->rankTable[b] + rank1(bv->blocks[b], i % SCM_WORD_BITS);
    return bit ? r : i - r;
}

long Scm_BitVectorRankAll(ScmBitVector *bv, int bit)
{
    long r;

    if (bv->length == 0)
        return 0;
    if (!bv->builded)
        Scm_BitVectorBuild(bv);
    r = bv->rankTable[bv->numBlocks];
    return bit ? r : (bv->length - r);
}

long Scm_BitVectorSelect(ScmBitVector *bv, long i, int bit)
{
    long min, max, end;

    if (i < 0)
        Scm_Error("argument out of range: %d", i);
    if (bv->length == 0)
        return -1;
    if (!bv->builded)
        Scm_BitVectorBuild(bv);
    min = 0;
    max = bv->numBlocks;
    while (min < max) {
        long p = (min + max) / 2;
        long r = bv->rankTable[p];
        if (!bit)
            r = p * SCM_WORD_BITS - r;
        if (i < r)
            max = p;
        else
            min = p + 1;
    }
    max--;
    end = select1(bit ? bv->blocks[max] : ~bv->blocks[max],
                  i - bv->rankTable[max]);
    return (0 <= end) ? (max * SCM_WORD_BITS + end) : -1;
}

/*
 * Module initialization function.
 */
extern void Scm_Init_bit_vectorlib(ScmModule*);

void Scm_Init_bit_vector(void)
{
    ScmModule *mod;

    /* Register this DSO to Gauche */
    SCM_INIT_EXTENSION(bit_vector);

    /* Create the module if it doesn't exist yet. */
    mod = SCM_MODULE(SCM_FIND_MODULE("bit-vector", TRUE));

    Scm_InitStaticClass(SCM_CLASS_BIT_VECTOR, "<bit-vector>", mod, NULL, 0);

    /* Register stub-generated procedures */
    Scm_Init_bit_vectorlib(mod);
}
