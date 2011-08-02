/*
 * A bitstr_t is an array of configurable size words.  The first two words
 * are for internal use.  Word 0 is a magic cookie used to validate that the
 * bitstr_t is properly initialized.  Word 1 is the number of valid bits in
 * the bitstr_t This limts the capacity of a bitstr_t to 4 gigabits if using
 * 32 bit words.
 *
 * bitstrings are zero origin
 *
 * bitstrings are always stored in a little-endian fashion.  In other words,
 * bit "1" is always in the byte of a word at the lowest memory address,
 * regardless of the native architecture endianness.
 */

#ifndef _BITSTRING_H_
#define	_BITSTRING_H_

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif  /* HAVE_INTTYPES_H */
#else	/* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif  /*  HAVE_CONFIG_H */

#define BITSTR_SHIFT_WORD8	3
#define BITSTR_SHIFT_WORD32	5
#define BITSTR_SHIFT_WORD64	6

#ifndef   __bitstr_datatypes_defined
#  define __bitstr_datatypes_defined

#ifdef USE_64BIT_BITSTR
typedef int64_t bitstr_t;
#define BITSTR_SHIFT 		BITSTR_SHIFT_WORD64
#else
typedef int32_t bitstr_t;
#define BITSTR_SHIFT 		BITSTR_SHIFT_WORD32
#endif

typedef bitstr_t bitoff_t;

#endif

/*
 * internal macros / defs
 */

/* 2 words used for magic cookie and size */
#define BITSTR_OVERHEAD 	2

/* bitstr_t signature in first word */
#define BITSTR_MAGIC 		0x42434445
#define BITSTR_MAGIC_STACK	0x42434446 /* signature if on stack */

/* max bit position in word */
#define BITSTR_MAXPOS		(sizeof(bitstr_t)*8 - 1)

/* word of the bitstring bit is in */
#define	_bit_word(bit) 		(((bit) >> BITSTR_SHIFT) + BITSTR_OVERHEAD)

/* address of the byte containing bit */
#define _bit_byteaddr(name, bit) \
	((char *)((name) + BITSTR_OVERHEAD) + ((bit) >> BITSTR_SHIFT_WORD8))

/* mask for the bit within its word */
#ifdef SLURM_BIGENDIAN
#define	_bit_mask(bit) ((bitstr_t)1 << (BITSTR_MAXPOS - ((bit)&BITSTR_MAXPOS)))
#else
#define	_bit_mask(bit) ((bitstr_t)1 << ((bit)&BITSTR_MAXPOS))
#endif

/* number of bits actually allocated to a bitstr */
#define _bitstr_bits(name) 	((name)[1])

/* magic cookie stored here */
#define _bitstr_magic(name) 	((name)[0])

/* words in a bitstring of nbits bits */
#define	_bitstr_words(nbits)	\
	((((nbits) + BITSTR_MAXPOS) >> BITSTR_SHIFT) + BITSTR_OVERHEAD)

/* check signature */
#define _assert_bitstr_valid(name) do { \
	assert((name) != NULL); \
	assert(_bitstr_magic(name) == BITSTR_MAGIC \
			    || _bitstr_magic(name) == BITSTR_MAGIC_STACK); \
} while (0)

/* check bit position */
#define _assert_bit_valid(name,bit) do { \
	assert((bit) >= 0); \
	assert((bit) < _bitstr_bits(name)); 	\
} while (0)

/*
 * external macros
 */

/* allocate a bitstring on the stack */
/* XXX bit_decl does not check if nbits overflows word 1 */
#define	bit_decl(name, nbits) \
	(name)[_bitstr_words(nbits)] = { BITSTR_MAGIC_STACK, (nbits) }

/* compat with Vixie macros */
bitstr_t *bit_alloc(bitoff_t nbits);
int bit_test(bitstr_t *b, bitoff_t bit);
void bit_set(bitstr_t *b, bitoff_t bit);
void bit_clear(bitstr_t *b, bitoff_t bit);
void bit_nclear(bitstr_t *b, bitoff_t start, bitoff_t stop);
void bit_nset(bitstr_t *b, bitoff_t start, bitoff_t stop);

/* changed interface from Vixie macros */
bitoff_t bit_ffc(bitstr_t *b);
bitoff_t bit_ffs(bitstr_t *b);

/* new */
bitoff_t bit_nffs(bitstr_t *b, int n);
bitoff_t bit_nffc(bitstr_t *b, int n);
bitoff_t bit_noc(bitstr_t *b, int n, int seed);
void	bit_free(bitstr_t *b);
bitstr_t *bit_realloc(bitstr_t *b, bitoff_t nbits);
bitoff_t bit_size(bitstr_t *b);
void	bit_and(bitstr_t *b1, bitstr_t *b2);
void	bit_not(bitstr_t *b);
void	bit_or(bitstr_t *b1, bitstr_t *b2);
int	bit_set_count(bitstr_t *b);
int	bit_clear_count(bitstr_t *b);
int	bit_nset_max_count(bitstr_t *b);
int	int_and_set_count(int *i1, int ilen, bitstr_t *b2);
bitstr_t *bit_rotate_copy(bitstr_t *b1, int n, bitoff_t nbits);
void	bit_rotate(bitstr_t *b1, int n);
char	*bit_fmt(char *str, int len, bitstr_t *b);
int 	bit_unfmt(bitstr_t *b, char *str);
int 	*bitfmt2int (char *bit_str_ptr);
char *  inx2bitfmt (int *inx);
int     inx2bitstr(bitstr_t *b, int *inx);
char	*bit_fmt_hexmask(bitstr_t *b);
int 	bit_unfmt_hexmask(bitstr_t *b, const char *str);
char	*bit_fmt_binmask(bitstr_t *b);
int 	bit_unfmt_binmask(bitstr_t *b, const char *str);
bitoff_t bit_fls(bitstr_t *b);
void	bit_fill_gaps(bitstr_t *b);
int	bit_super_set(bitstr_t *b1, bitstr_t *b2);
int     bit_overlap(bitstr_t *b1, bitstr_t *b2);
int     bit_equal(bitstr_t *b1, bitstr_t *b2);
void    bit_copybits(bitstr_t *dest, bitstr_t *src);
bitstr_t *bit_copy(bitstr_t *b);
bitstr_t *bit_pick_cnt(bitstr_t *b, bitoff_t nbits);
bitoff_t bit_get_bit_num(bitstr_t *b, int pos);
int      bit_get_pos_num(bitstr_t *b, bitoff_t pos);

#define FREE_NULL_BITMAP(_X)		\
	do {				\
		if (_X) bit_free (_X);	\
		_X	= NULL; 	\
	} while (0)


#endif /* !_BITSTRING_H_ */
