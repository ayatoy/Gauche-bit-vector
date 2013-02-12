#ifndef GAUCHE_BIT_VECTOR_H
#define GAUCHE_BIT_VECTOR_H

#include <gauche.h>
#include <gauche/extend.h>

SCM_DECL_BEGIN

typedef struct ScmBitVectorRec {
  SCM_HEADER;
  int length;
  unsigned int builded : 1;
  int numBlocks : (SIZEOF_INT*CHAR_BIT-1);
  ScmBits *blocks;
  int *rankTable;
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

SCM_EXTERN ScmBitVector *Scm_MakeBitVector(int length, int fill);
SCM_EXTERN ScmBitVector *Scm_BitVectorClear(ScmBitVector *bv);
SCM_EXTERN ScmBitVector *Scm_BitVectorBuild(ScmBitVector *bv);
SCM_EXTERN int Scm_BitVectorRef(ScmBitVector *bv, int i);
SCM_EXTERN ScmBitVector *Scm_BitVectorSet(ScmBitVector *bv, int i, int bit);
SCM_EXTERN ScmBitVector *Scm_BitVectorFill(ScmBitVector *bv,
                                           int fill, int start, int end);
SCM_EXTERN int Scm_BitVectorRank(ScmBitVector *bv, int i, int bit);
SCM_EXTERN int Scm_BitVectorRankAll(ScmBitVector *bv, int bit);
SCM_EXTERN int Scm_BitVectorSelect(ScmBitVector *bv, int i, int bit);

SCM_DECL_END

#endif  /* GAUCHE_BIT_VECTOR_H */
