#ifndef GAUCHE_BIT_VECTOR_H
#define GAUCHE_BIT_VECTOR_H

#include <sys/types.h>
#include <gauche.h>
#include <gauche/extend.h>

SCM_DECL_BEGIN

typedef struct ScmBitVectorRec {
  SCM_HEADER;
  long length;
  u_long builded : 1;
  long numBlocks : (SIZEOF_INT*CHAR_BIT-1);
  ScmBits *blocks;
  long *rankTable;
} ScmBitVector;

SCM_CLASS_DECL(Scm_BitVectorClass);
#define SCM_CLASS_BIT_VECTOR           (&Scm_BitVectorClass)
#define SCM_BIT_VECTOR(obj)            ((ScmBitVector*)(obj))
#define SCM_BIT_VECTOR_P(obj)          SCM_XTYPEP(obj, SCM_CLASS_BIT_VECTOR)
#define SCM_BIT_VECTOR_LENGTH(obj)     (SCM_BIT_VECTOR(obj)->length)
#define SCM_BIT_VECTOR_BUILDED(obj)    (SCM_BIT_VECTOR(obj)->builded)
#define SCM_BIT_VECTOR_NUM_BLOCKS(obj) (SCM_BIT_VECTOR(obj)->numBlocks)
#define SCM_BIT_VECTOR_BLOCKS(obj)     (SCM_BIT_VECTOR(obj)->blocks)
#define SCM_BIT_VECTOR_RANK_TABLE(obj) (SCM_BIT_VECTOR(obj)->rankTable)

SCM_EXTERN ScmBitVector *Scm_MakeBitVector(long length, int fill);
SCM_EXTERN ScmBitVector *Scm_BitVectorClear(ScmBitVector *bv);
SCM_EXTERN ScmBitVector *Scm_BitVectorBuild(ScmBitVector *bv);
SCM_EXTERN int Scm_BitVectorRef(ScmBitVector *bv, long i);
SCM_EXTERN ScmBitVector *Scm_BitVectorSet(ScmBitVector *bv, long i, int bit);
SCM_EXTERN ScmBitVector *Scm_BitVectorFill(ScmBitVector *bv,
                                           int fill, long start, long end);
SCM_EXTERN long Scm_BitVectorRank(ScmBitVector *bv, long i, int bit);
SCM_EXTERN long Scm_BitVectorRankAll(ScmBitVector *bv, int bit);
SCM_EXTERN long Scm_BitVectorSelect(ScmBitVector *bv, long i, int bit);

SCM_DECL_END

#endif  /* GAUCHE_BIT_VECTOR_H */
