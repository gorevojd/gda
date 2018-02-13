#ifndef GOR_BIGNUM_H_INCLUDED
#define GOR_BIGNUM_H_INCLUDED

/*
	ABOUT:
		Big number library - arithmetic on multiple-precision unsigned integers.

		This library is an implementation of arithmetic on arbitrarily large integers.

		The difference between this and other implementations, is that the data structure
		has optimal memory utilization (i.e. a 1024 bit integer takes up 128 bytes RAM),
		and all memory is allocated statically: no dynamic allocation for better or worse.

		Primary goals are correctness, clarity of code and clean, portable implementation.
		Secondary goal is a memory footprint small enough to make it suitable for use in
		embedded applications.

		The current state is correct functionality and adequate performance.
		There may well be room for performance-optimizations and improvements.

	USAGE:
		

	LICENCE:
		This is free and unencumbered software released into the public domain.

		Anyone is free to copy, modify, publish, use, compile, sell, or
		distribute this software, either in source code form or as a compiled
		binary, for any purpose, commercial or non-commercial, and by any
		means.

		In jurisdictions that recognize copyright laws, the author or authors
		of this software dedicate any and all copyright interest in the
		software to the public domain. We make this dedication for the benefit
		of the public at large and to the detriment of our heirs and
		successors. We intend this dedication to be an overt act of
		relinquishment in perpetuity of all present and future rights to this
		software under copyright law.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
		MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
		IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
		OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
		ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.

		For more information, please refer to <https://unlicense.org>

	AUTHOR:
		Gorevoy Dmitry - github.com/gorevojd

	OTHER:
		Basic code was taken from https://github.com/kokke/tiny-bignum-c
		and then modified.
*/

#include <stdint.h>

#define GORBN_SZWORD 2
#define GORBN_SZARR (256 / GORBN_SZWORD)

#ifndef GORBN_SZWORD
#error GORBN_SZWORD must be defined
#elif (GORBN_SZWORD == 1)
#define gorbn_t uint8_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFF
#define GORBN_SZWORD_BITS 8
#define GORBN_SZWORD_BITS_MINUS_ONE 7

#elif (GORBN_SZWORD == 2)
#define gorbn_t uint16_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFFFF
#define GORBN_SZWORD_BITS 16
#define GORBN_SZWORD_BITS_MINUS_ONE 15

#elif (GORBN_SZWORD == 4)
#define gorbn_t uint32_t
#define gorbn_utmp_t uint64_t
#define gorbn_stmp_t int64_t
#define GORBN_MAX_VAL 0xFFFFFFFF
#define GORBN_SZWORD_BITS 32
#define GORBN_SZWORD_BITS_MINUS_ONE 31

#elif
#error GORBN_SZWORD must be defined to 1, 2 or 4
#endif

typedef struct gorbn_bignum{
	int sign;

	gorbn_t number[GORBN_SZARR];
} gorbn_bignum;

/* Custom macro for getting absolute value of the signed integer*/
#define GORBN_ABS(val) (((val) >= 0) ? (val) : (-(val)))

/*Custom macro for getting the biggest number from two numbers*/
#define GORBN_MAX(a, b) (((a) > (b)) ? (a) : (b))

#ifdef GOR_BIGNUM_SAFE
#ifndef GOR_BIGNUM_ENABLE_ASSERTS
#define GOR_BIGNUM_ENABLE_ASSERTS
#endif
#else
#define GOR_BIGNUM_FAST
#endif

#ifndef GOR_BIGNUM_FAST
#define GORBN_SAFE_CONDITION_BEGIN(cond) if(cond) {
#define GORBN_SAFE_CONDITION_END() }
#else
#define GORBN_SAFE_CONDITION_BEGIN(...)
#define GORBN_SAFE_CONDITION_END()
#endif

/* Custom assert macro */
#ifdef GOR_BIGNUM_ENABLE_ASSERTS
#define GORBN_ASSERT(cond) if(!(cond)){*(int*)0 = 0;}
#else
#define GORBN_ASSERT(...)
#endif

/*Tokens returned by gorbn_cmp() for value comparison*/
#define GORBN_CMP_LARGER 1
#define GORBN_CMP_SMALLER -1
#define GORBN_CMP_EQUAL 0

#ifdef GOR_BIGNUM_STATIC
#define GORBN_DEF static
#else
#define GORBN_DEF extern
#endif

#define _GORBN_INPUT
#define _GORBN_OUTPUT
#define _GORBN_OUTPUT_OPT

#ifdef __cplusplus
extern "C" {
#endif

/*Initialization functions*/
GORBN_DEF void gorbn_init(gorbn_bignum* n);
GORBN_DEF void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src);
GORBN_DEF void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i);
GORBN_DEF void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i);
GORBN_DEF void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size);
GORBN_DEF void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n);

/* Arithmetic operations: */
GORBN_DEF void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a + b */
GORBN_DEF void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a - b */
GORBN_DEF void gorbn_mul(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a * b */
GORBN_DEF void gorbn_mul_word(gorbn_bignum* r, gorbn_bignum* a, gorbn_t w);
GORBN_DEF void gorbn_sqr(gorbn_bignum* r, gorbn_bignum* a);                  /* r = a ^ 2 */
GORBN_DEF void gorbn_div(gorbn_bignum* q, gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* q = a / b; a = q * b + r*/
GORBN_DEF void gorbn_div_word(gorbn_bignum* q, gorbn_t* r, gorbn_bignum* x, gorbn_t w);
GORBN_DEF void gorbn_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a % b */
GORBN_DEF void gorbn_mod_word(gorbn_t* r, gorbn_bignum* n, gorbn_t w);
GORBN_DEF void gorbn_pow(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF void gorbn_mul_pow2(gorbn_bignum* r, gorbn_bignum* a, int k); /* r = a * (2 ^ k) */
GORBN_DEF void gorbn_div_pow2(gorbn_bignum* r, gorbn_bignum* a, int k); /* r = a / (2 ^ k) */
GORBN_DEF void gorbn_gcd(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b);

GORBN_DEF void gorbn_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* m);
GORBN_DEF void gorbn_mod_pow2(gorbn_bignum* r, gorbn_bignum* a, int k);
GORBN_DEF void gorbn_sub_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m);
GORBN_DEF void gorbn_add_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m);
GORBN_DEF void gorbn_mul_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m);

/* Bitwise operations: */
GORBN_DEF void gorbn_and(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a & b */
GORBN_DEF void gorbn_or(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a | b */
GORBN_DEF void gorbn_xor(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a ^ b */
GORBN_DEF void gorbn_lshift(gorbn_bignum* r, gorbn_bignum* a, int nbits); /* r = a << nbits */
GORBN_DEF void gorbn_rshift(gorbn_bignum* r, gorbn_bignum* b, int nbits); /* r = a >> nbits */

/* Special operators and comparison */
GORBN_DEF int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_zero(gorbn_bignum* a);
GORBN_DEF int gorbn_is_zero(gorbn_bignum* a);


#ifdef __cplusplus
}
#endif

#endif

#if defined(GOR_BIGNUM_IMPLEMENTATION) && !defined(GOR_BIGNUM_IMPLEMENTATION_DONE)
#define GOR_BIGNUM_IMPLEMENTATION_DONE

void gorbn_init(gorbn_bignum* n){
	GORBN_ASSERT(n);

	int i = 0;
	for(i = 0; i < GORBN_SZARR; i++){
		n->number[i] = 0;
	}
	n->sign = 1;
}

void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src){
	GORBN_ASSERT(dst);
	GORBN_ASSERT(src);

	int i;
	for(i = 0; i < GORBN_SZARR; i++){
		dst->number[i] = src->number[i];
	}
	dst->sign = src->sign;
}

void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif

	n->sign = 1;
}

void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

	n->sign = 1;
	if(i < 0){
		n->sign = -1;
	}

	i = GORBN_ABS(i);
	
#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif
}

void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size){
	GORBN_ASSERT(n);
	GORBN_ASSERT(data);

	int i;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n->number;

	gorbn_init(n);

	for(i = 0; i < data_size; i++){
		*to++ = *at++;
	}
}

void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n){
	GORBN_ASSERT(data);
	GORBN_ASSERT(n);
	GORBN_ASSERT((GORBN_SZARR * GORBN_SZWORD) <= data_size); /*data must be big enough to hold big number inside*/

	int i;

	gorbn_t* at = n->number;
	gorbn_t* to = (gorbn_t*)data;

	for(i = 0; i < GORBN_SZARR; i++){
		*to++ = *at++;
	}
}

static inline int _gorbn_is_even(gorbn_bignum* a) {
	int is_even = ((a->number[0] & 1) == 0);

	return(is_even);
}

static inline void _gorbn_lshift_one_bit(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int i;
	for (i = (GORBN_SZARR - 1); i > 0; --i) {
		a->number[i] = (a->number[i] << 1) | (a->number[i - 1] >> GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a->number[0] <<= 1;
}

static inline void _gorbn_rshift_one_bit(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int i;
	for (i = 0; i < (GORBN_SZARR - 1); i++) {
		a->number[i] = (a->number[i] >> 1) | (a->number[i + 1] << GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a->number[GORBN_SZARR - 1] >>= 1;
}

static inline void _gorbn_lshift_words(gorbn_bignum* a, int nwords) {
	GORBN_ASSERT(a);
	GORBN_ASSERT(nwords >= 0);

	int i;
	if (nwords > 0) {
		for (i = (GORBN_SZARR - 1); i >= nwords; i--) {
			a->number[i] = a->number[i - nwords];
		}

		for (; i >= 0; --i) {
			a->number[i] = 0;
		}
	}
}

static inline void _gorbn_rshift_words(gorbn_bignum* a, int nwords) {
	GORBN_ASSERT(a);
	GORBN_ASSERT(nwords >= 0);

	int i;
	if (nwords > 0) {
		for (i = 0; i < nwords; i++) {
			a->number[i] = a->number[i + nwords];
		}

		for (; i < GORBN_SZARR; i++) {
			a->number[i] = 0;
		}
	}
}

static inline int _gorbn_get_ndigits(gorbn_bignum* a) {
	int result = 0;

	for (int i = GORBN_SZARR - 1; i >= 0; i--) {
		if (a->number[i] != 0) {
			result = i + 1;
			break;
		}
	}

	return(result);
}

static inline void _gorbn_internal_addition(
	gorbn_bignum* dst,
	gorbn_bignum* a, int a_sign,
	gorbn_bignum* b, int b_sign)
{
	gorbn_utmp_t res;
	gorbn_utmp_t tmp1;
	gorbn_utmp_t tmp2;
	int borrow = 0;
	int carry = 0;
	int i;

	gorbn_bignum* big;
	gorbn_bignum* low;

	if(a_sign == b_sign){
		/* Just sum those 2 values */
		for(i = 0; i < GORBN_SZARR; i++){
			tmp1 = a->number[i] + b->number[i] + carry;
			carry = (tmp1 > GORBN_MAX_VAL);
			dst->number[i] = (tmp1 & GORBN_MAX_VAL);
		}

		/* Set the initial sign */
		dst->sign = a_sign;
	}
	else{
		/* Detecting which number is bigger */
		int comp_res = gorbn_cmp_mod(a, b);
		if(comp_res >= 0){
			big = a;
			low = b;
		}
		else{
			big = b;
			low = a;
		}

		/* Subtracting: dst = big - small */
		for (i = 0; i < GORBN_SZARR; ++i) {
			tmp1 = (gorbn_utmp_t)big->number[i] + ((gorbn_utmp_t)GORBN_MAX_VAL + 1);
			tmp2 = (gorbn_utmp_t)low->number[i] + borrow;
			res = (tmp1 - tmp2);
			dst->number[i] = (gorbn_t)(res & GORBN_MAX_VAL);
			borrow = (res <= GORBN_MAX_VAL);
		}

		/* Detecting the result sign */
		if(a_sign > 0 && comp_res >= 0){
			dst->sign = 1;
		}
		else{
			dst->sign = -1;
		}
	}
}

void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);
	
	gorbn_bignum buf;
	gorbn_init(&buf);
	_gorbn_internal_addition(&buf, a, a->sign, b, b->sign);
	gorbn_copy(r, &buf);
}

void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	gorbn_bignum buf;
	gorbn_init(&buf);
	_gorbn_internal_addition(&buf, a, a->sign, b, b->sign * -1);
	gorbn_copy(r, &buf);
}

//TODO(DIMA): Test
void gorbn_mul_word(gorbn_bignum* r, gorbn_bignum* a, gorbn_t w) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);

	gorbn_bignum buf;
	gorbn_init(&buf);

	gorbn_utmp_t uv;
	gorbn_utmp_t c = 0;
	int j, a_ndigits;

	if (w == 0) {
		gorbn_copy(r, &buf);
		return;
	}

	a_ndigits = _gorbn_get_ndigits(a);

	for (j = 0; j < a_ndigits; j++) {
		uv = (gorbn_utmp_t)buf.number[j] +
			(gorbn_utmp_t)a->number[j] * (gorbn_utmp_t)w + c;
		buf.number[j] = uv & GORBN_MAX_VAL;
		c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
	}

	GORBN_SAFE_CONDITION_BEGIN(a_ndigits < GORBN_SZARR);
	buf.number[a_ndigits] = c;
	buf.sign = a->sign;
	GORBN_SAFE_CONDITION_END();

	gorbn_copy(r, &buf);
}

void gorbn_mul(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i, j;
	int a_ndigits;
	int b_ndigits;

	a_ndigits = _gorbn_get_ndigits(a);
	b_ndigits = _gorbn_get_ndigits(b);

	gorbn_bignum buf;
	gorbn_init(&buf);
	
#if 0
	for (i = 0; i < a_ndigits; ++i) {
		gorbn_utmp_t carry_lo = 0;
		gorbn_utmp_t carry_hi = 0;
		gorbn_utmp_t sum_hi = 0;
		gorbn_utmp_t sum_lo;

		for (j = 0; j < b_ndigits; ++j) {
			sum_lo =
				(gorbn_utmp_t)a->number[i] *
				(gorbn_utmp_t)b->number[j] +
				carry_lo;

			sum_hi =
				(gorbn_utmp_t)buf.number[i + j] +
				(sum_lo & GORBN_MAX_VAL) +
				carry_hi;

			buf.number[i + j] = sum_hi & GORBN_MAX_VAL;

			carry_lo = (sum_lo >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
			carry_hi = (sum_hi >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		/* Continue processing carry values */
#if 0
		do {
			/*TODO: Should we increment here????*/
			sum_hi = (gorbn_utmp_t)buf.number[i + j] + carry_lo + carry_hi;
			buf.number[i + j] = sum_hi & GORBN_MAX_VAL;
			carry_hi = (sum_hi >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
			carry_lo = 0;
		} while (carry_hi != 0);
#else
		sum_hi = (gorbn_utmp_t)buf.number[i + j] + carry_lo + carry_hi;
		buf.number[i + j] = sum_hi & GORBN_MAX_VAL;
#endif
	}
#else
	for (i = 0; i < b_ndigits; i++) {
		gorbn_utmp_t uv;
		gorbn_utmp_t c = 0;

		for (j = 0; j < a_ndigits; j++) {
			GORBN_SAFE_CONDITION_BEGIN(i + j < GORBN_SZARR);
			uv = (gorbn_utmp_t)buf.number[i + j] + 
				(gorbn_utmp_t)a->number[j] * 
				(gorbn_utmp_t)b->number[i] + 
				c;
			buf.number[i + j] = uv & GORBN_MAX_VAL;
			c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
			GORBN_SAFE_CONDITION_END();
		}

		GORBN_SAFE_CONDITION_BEGIN(i + a_ndigits < GORBN_SZARR);
		buf.number[i + a_ndigits] = c;
		GORBN_SAFE_CONDITION_END();
	}

	buf.sign = a->sign * b->sign;
#endif

	gorbn_copy(r, &buf);
}

//TODO(DIMA): Test both variations
void gorbn_mul_pow2(gorbn_bignum* r, gorbn_bignum* a, int k) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(k >= 0);

	gorbn_lshift(r, a, k);
}

void gorbn_div_pow2(gorbn_bignum* r, gorbn_bignum* a, int k) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(k >= 0);

	gorbn_rshift(r, a, k);
}

//TODO(Dima): This funciton not working
#if 0
void gorbn_sqr(gorbn_bignum* r, gorbn_bignum* a) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);

	gorbn_bignum buf;
	gorbn_init(&buf);

	int t = _gorbn_get_ndigits(a);

	int i;
	for (i = 0; i < t; i++) {
		gorbn_utmp_t carry;
		gorbn_utmp_t temp_val = a->number[i];
		gorbn_utmp_t sum = (gorbn_utmp_t)buf.number[2 * i] + temp_val * temp_val;
		buf.number[2 * i] = sum & GORBN_MAX_VAL;
		carry = (sum >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;

		int j;
		for (j = i + 1; j < t; j++) {
			sum = 
				(gorbn_utmp_t)buf.number[i + j] + 2 * 
				(gorbn_utmp_t)a->number[i] * 
				(gorbn_utmp_t)a->number[j] + 
				carry;
			buf.number[i + j] = sum & GORBN_MAX_VAL;
			carry = (sum >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		buf.number[i + t] = carry;
	}

	gorbn_copy(r, &buf);
}
#endif

void gorbn_div_old(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	gorbn_bignum current, denom, tmp;

	gorbn_from_uint(&current, 1);
	gorbn_copy(&denom, b);
	gorbn_copy(&tmp, a);

	gorbn_utmp_t half_max = 1 + (GORBN_MAX_VAL >> 1);
	int overflow = 0;
	while (gorbn_cmp(&denom, a) != GORBN_CMP_LARGER) {
		if (denom.number[GORBN_SZARR - 1] >= half_max) {
			overflow = 1;
			break;
		}
		_gorbn_lshift_one_bit(&current);
		_gorbn_lshift_one_bit(&denom);
	}

	if (!overflow) {
		_gorbn_rshift_one_bit(&denom);
		_gorbn_rshift_one_bit(&current);
	}

	gorbn_init(r);

	while (!gorbn_is_zero(&current)) {
		if (gorbn_cmp(&tmp, &denom) != GORBN_CMP_SMALLER) {
			gorbn_sub(&tmp, &tmp, &denom);
			gorbn_or(r, r, &current);
		}

		_gorbn_rshift_one_bit(&current);
		_gorbn_rshift_one_bit(&denom);
	}
}

void gorbn_div_word(
	_GORBN_OUTPUT gorbn_bignum* q, 
	_GORBN_OUTPUT gorbn_t* r,
	_GORBN_INPUT gorbn_bignum* x,
	_GORBN_INPUT gorbn_t w)
{
	GORBN_ASSERT(q);
	GORBN_ASSERT(x);
	GORBN_ASSERT(r);
	GORBN_ASSERT(w > 0);

	gorbn_bignum buf;
	gorbn_init(&buf);

	int i;
	gorbn_t tmp_r = 0;
	gorbn_utmp_t divisor = 0;
	for (i = GORBN_SZARR - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= GORBN_SZWORD_BITS;
		divisor |= x->number[i];
		buf.number[i] = (gorbn_t)(divisor / w);
		tmp_r = (gorbn_t)(divisor % w);
	}
	
	buf.sign = x->sign;

	gorbn_copy(q, &buf);
	*r = tmp_r;
}

//TODO: make correct sign when n is negative
void gorbn_mod_word(
	_GORBN_OUTPUT gorbn_t* r, 
	_GORBN_INPUT gorbn_bignum* n, 
	_GORBN_INPUT gorbn_t w) 
{
	GORBN_ASSERT(r);
	GORBN_ASSERT(n);
	GORBN_ASSERT(w > 0);

	int i;
	gorbn_t tmp_r = 0;
	gorbn_utmp_t divisor = 0;
	for (i = GORBN_SZARR - 1; i >= 0; i--) {
		divisor = tmp_r;
		divisor <<= GORBN_SZWORD_BITS;
		divisor |= n->number[i];
		tmp_r = (gorbn_t)(divisor % w);
	}

	*r = tmp_r;
}


/*
	NOTE(Dima):
		q - the quotient output param. Can be NULL.
		r - the remainder output param. Can be NULL.
		x - the divident input param.
		y - the divisor input param.
*/
void gorbn_div(
	_GORBN_OUTPUT_OPT gorbn_bignum* q, 
	_GORBN_OUTPUT_OPT gorbn_bignum* r, 
	_GORBN_INPUT gorbn_bignum* x, 
	_GORBN_INPUT gorbn_bignum* y) 
{
	GORBN_ASSERT(x);
	GORBN_ASSERT(y);

#if 0
	gorbn_bignum q_buf;
	gorbn_bignum x_buf;
	gorbn_bignum r_buf;
	gorbn_bignum tmp_buf;
	gorbn_bignum tmp_base;
	gorbn_bignum tmp_3_1;
	gorbn_bignum tmp_3_2;
	gorbn_bignum tmp2_3_2;
	gorbn_bignum cmpr_3_2;
	gorbn_bignum tmp_3_3;

	int i;

	gorbn_init(&q_buf);
	gorbn_init(&tmp_3_1);
	gorbn_init(&tmp_3_2);
	gorbn_init(&r_buf);
	//gorbn_init(&tmp2_3_2);
	gorbn_init(&cmpr_3_2);
	gorbn_copy(&x_buf, x);

	int a_ndig = _gorbn_get_ndigits(x);
	int b_ndig = _gorbn_get_ndigits(y);

	int n = a_ndig - 1;
	int t = b_ndig - 1;
	int diff = n - t;
	int diff_pow2 = diff * GORBN_SZWORD_BITS;

	//tmp_3_2 = y[t]*b + y[t-1]
	tmp_3_2.number[1] = y->number[t];
	tmp_3_2.number[0] = y->number[t - 1];

	int begin_cmp_res = gorbn_cmp(x, y);
	if (begin_cmp_res == GORBN_CMP_EQUAL) {
		q_buf.number[0] = 1;
	}
	else if (begin_cmp_res == GORBN_CMP_LARGER) {
		/*(2) while x>=y*(b^(n-t)) do the following: q[n-t]=q[n-t]+1, x=x-y*(b^(n-t)) */
		gorbn_mul_pow2(&tmp_base, y, diff_pow2);

		while (gorbn_cmp(&x_buf, &tmp_base) >= 0) {
			q_buf.number[diff]++;
			gorbn_sub(&x_buf, &x_buf, &tmp_base);
		}

		/*(3) For i from n down to (t + 1) do the following: */
		for (i = n; i > t; i--) {
			int i_min_t_min_one = i - t - 1;

			/*(3.1) If x[i]=y[t] then set q[i-t-1]=b-1; else set q[i-t-q]*/
			if (x_buf.number[i] == y->number[t]) {
				q_buf.number[i_min_t_min_one] = GORBN_MAX_VAL;
			}
			else {
				tmp_3_1.number[1] = x_buf.number[i];
				tmp_3_1.number[0] = x_buf.number[i - 1];
				gorbn_t rst = 0;
				gorbn_div_word(&tmp_buf, &rst, &tmp_3_1, y->number[t]);
				//SUPER_IMPORTANT: What should I take from tmp_buf????
				q_buf.number[i_min_t_min_one] = tmp_buf.number[0];
			}

			/*(3.2) While (q[i-t-1]*(y[t]*b+y[t-1]) > x[i]*b*b + x[i-1]*b + x[i-2]) 
					do: q[i-t-1]=q[i-t-1]-1
			*/
			//NOTE(Dima): Possible overlap
			cmpr_3_2.number[2] = x_buf.number[i];
			cmpr_3_2.number[1] = x_buf.number[i - 1];
			cmpr_3_2.number[0] = x_buf.number[i - 2];

			gorbn_mul_word(&tmp2_3_2, &tmp_3_2, q_buf.number[i_min_t_min_one]);
			int loop_iterations = 0;
			while (gorbn_cmp(&tmp2_3_2, &cmpr_3_2) == GORBN_CMP_LARGER) {
				loop_iterations++;

				q_buf.number[i_min_t_min_one]--;

				gorbn_mul_word(&tmp2_3_2, &tmp_3_2, q_buf.number[i_min_t_min_one]);
			}

			/*(3.3) x = x-q[i-t-1] * y * b^[i-t-1] */
			gorbn_mul_pow2(&tmp_3_3, y, i_min_t_min_one * GORBN_SZWORD_BITS);
			gorbn_mul_word(&tmp_buf, &tmp_3_3, q_buf.number[i_min_t_min_one]);
			gorbn_sub(&x_buf, &x_buf, &tmp_buf);

			/*(3.4) If x < 0 then set x=x + y * b^(i-t-1) and q[i-t-1]-- */
			if (x_buf.sign < 0) {
				gorbn_add(&x_buf, &x_buf, &tmp_3_3);
				q_buf.number[i_min_t_min_one]--;
			}
		}

		/*(4) r = x */
		gorbn_copy(&r_buf, &x_buf);



	}
	else {
		/*a is smaller than b. Invalid case.*/
		//GORBN_ASSERT(0);
	}

	/*(5) Return(q, r) */
	gorbn_copy(r, &r_buf);
	gorbn_copy(q, &q_buf);
#else
	gorbn_bignum a_norm, b_norm;
	gorbn_bignum q_buf, r_buf;
	
	gorbn_copy(&a_norm, x);
	gorbn_copy(&b_norm, y);

	gorbn_init(&q_buf);
	gorbn_init(&r_buf);

	int a_ndig = _gorbn_get_ndigits(x);
	int b_ndig = _gorbn_get_ndigits(y);

	int i, j;
	gorbn_utmp_t mod_bn = (gorbn_utmp_t)GORBN_MAX_VAL + 1;
	gorbn_utmp_t p = 0;
	gorbn_stmp_t carry = 0;
	gorbn_stmp_t t;

	int begin_cmp_res = gorbn_cmp(x, y);
	if (begin_cmp_res == GORBN_CMP_EQUAL) {
		//NOTE(DIMA): If divisor is equal to divident than return 1
		q_buf.number[0] = 1;
	}
	else if (b_ndig == 1) {
		//NOTE(DIMA): If divisor is small number (== 1 word)
		
		for (j = a_ndig - 1; j >= 0; j--) {
			q_buf.number[j] = (gorbn_t)((carry * mod_bn + x->number[j]) / y->number[0]);
			carry = (carry * mod_bn + x->number[j]) - q_buf.number[j] * y->number[0];
		}

		r_buf.number[0] = (gorbn_t)carry;
	}
	else if (begin_cmp_res == GORBN_CMP_LARGER) {

		q_buf.sign = x->sign * y->sign;

		for (j = a_ndig - b_ndig; j >= 0; j--) {
			gorbn_utmp_t c_pred, r_pred;
			gorbn_utmp_t asd =
				((gorbn_utmp_t)a_norm.number[j + b_ndig] * mod_bn) +
				(gorbn_utmp_t)a_norm.number[j + b_ndig - 1];

			c_pred = asd / b_norm.number[b_ndig - 1];
			r_pred = asd - c_pred * b_norm.number[b_ndig - 1];

			do {
				p = 0;
				if (
					(c_pred >= mod_bn) ||
					((c_pred * (gorbn_utmp_t)b_norm.number[b_ndig - 2]) > (mod_bn * r_pred + (gorbn_utmp_t)a_norm.number[j + b_ndig - 2])))
				{
					c_pred--;
					r_pred += b_norm.number[b_ndig - 1];
					if (r_pred < mod_bn) {
						p = 1;
					}
				}
			} while (p);

			carry = 0;
			for (i = 0; i < b_ndig; i++) {
				p = c_pred * b_norm.number[i];
				t = a_norm.number[i + j] - carry - (p & GORBN_MAX_VAL);
				a_norm.number[i + j] = (gorbn_t)t;
				carry = (p >> GORBN_SZWORD_BITS) - (t >> GORBN_SZWORD_BITS);
			}

			t = a_norm.number[j + b_ndig] - carry;
			a_norm.number[j + b_ndig] = (gorbn_t)t;
			q_buf.number[j] = (gorbn_t)c_pred;

			if (t < 0) {
				q_buf.number[j]--;
				carry = 0;
				for (i = 0; i < b_ndig; i++) {
					t = a_norm.number[i + j] + b_norm.number[i] + carry;
					a_norm.number[i + j] = (gorbn_t)t;
					carry = t >> GORBN_SZWORD_BITS;
				}
				a_norm.number[j + b_ndig] = (gorbn_t)(a_norm.number[j + b_ndig] + carry);
			}
		}

		gorbn_copy(&r_buf, &a_norm);
	}
	else {
		//INVALID PATH
		//FIRST NUM SHOULD BE >= SECOND
	}

	if (q) {
		gorbn_copy(q, &q_buf);
	}

	if (r) {
		gorbn_copy(r, &r_buf);
	}
#endif
}

void gorbn_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* m) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(m);

	gorbn_div(0, r, a, m);

	if (a->sign == -1) {
		if (!gorbn_is_zero(r)) {
			r->sign = -1;
			int tmp_sign = m->sign;
			m->sign = 1;
			gorbn_add(r, r, m);
			m->sign = tmp_sign;
		}
	}
}

void gorbn_mod_pow2(gorbn_bignum* r, gorbn_bignum* a, int k) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(k >= 0);

	GORBN_ASSERT(a->sign > 0);

	gorbn_bignum r_buf;

	int i;
	int j;

	int nwords_masked = k >> GORBN_SZWORD_BITS;
	int nwords_rest = k & GORBN_SZWORD_BITS_MINUS_ONE;

	gorbn_t last_word_mask = 0;

	gorbn_init(&r_buf);

	for (i = 0; i < nwords_masked; i++) {
		r_buf.number[i] = a->number[i] & GORBN_MAX_VAL;
	}

	for (j = 0; j < nwords_rest; j++) {
		last_word_mask |= (1 << i);
	}
	
	r_buf.number[i] = a->number[i] & last_word_mask;

	gorbn_copy(r, &r_buf);
}

void gorbn_sub_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);
	GORBN_ASSERT(m);

	GORBN_ASSERT(a->sign > 0);
	GORBN_ASSERT(b->sign > 0);
	GORBN_ASSERT(m->sign > 0);

	gorbn_sub(r, a, b);
	gorbn_div(0, r, r, m);
}

void gorbn_add_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);
	GORBN_ASSERT(m);

	GORBN_ASSERT(a->sign > 0);
	GORBN_ASSERT(b->sign > 0);
	GORBN_ASSERT(m->sign > 0);

	gorbn_add(r, a, b);
	gorbn_div(0, r, r, m);
}

void gorbn_mul_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b, gorbn_bignum* m) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);
	GORBN_ASSERT(m);

	GORBN_ASSERT(a->sign > 0);
	GORBN_ASSERT(b->sign > 0);
	GORBN_ASSERT(m->sign > 0);

	gorbn_mul(r, a, b);
	gorbn_div(0, r, r, m);
}

void gorbn_pow(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	gorbn_bignum buf1;
	gorbn_bignum buf2;
	gorbn_bignum exp_buf;

	gorbn_init(&buf1);
	gorbn_init(&buf2);
	gorbn_init(&exp_buf);

	/*(1) A <- 1, S <- g*/
	gorbn_copy(&buf2, a);
	gorbn_copy(&exp_buf, b);

	buf1.number[0] = 1;

	/*(2) While e != 0 do the following: */
	while (!gorbn_is_zero(&exp_buf)) {
		int is_odd = exp_buf.number[0] & 1;

		/*(2.1) If e is odd then A <- A * S */
		if (is_odd) {
			gorbn_mul(&buf1, &buf1, &buf2);
		}
		
		/*(2.2) e <- e / 2 */
		_gorbn_rshift_one_bit(&exp_buf);

		/*(2.3) e != 0 then S <- S * S */
		if (!gorbn_is_zero(&exp_buf)) {
			gorbn_mul(&buf2, &buf2, &buf2);
		}
	}

	gorbn_copy(r, &buf1);
}

/*
	DESCRIPTION:
		Binary greatest common divisor algorithm

	SOURCE:
		Handbook of applied cryptography. 1996
*/
void gorbn_gcd(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	gorbn_bignum g_buf;
	gorbn_bignum t_buf;
	gorbn_bignum a_buf;
	gorbn_bignum b_buf;

	gorbn_init(&g_buf);
	gorbn_init(&t_buf);
	gorbn_copy(&a_buf, a);
	gorbn_copy(&b_buf, b);

	/*(1) g <- 1*/
	g_buf.number[0] = 1;

	/*(2)While both x and y are even do the following: x=x/2, y=y/2, g=2*g */
	while (
		_gorbn_is_even(&a_buf) &&
		_gorbn_is_even(&b_buf))
	{
		_gorbn_rshift_one_bit(&a_buf);
		_gorbn_rshift_one_bit(&b_buf);
		_gorbn_lshift_one_bit(&g_buf);
	}

	/*(3) While x != 0 do the follwing */
	while (!gorbn_is_zero(&a_buf)) {
		/*(3.1) While x is even do: x = x / 2 */
		while (_gorbn_is_even(&a_buf)) {
			_gorbn_rshift_one_bit(&a_buf);
		}

		/*(3.2) While y is even do: y = y / 2 */
		while (_gorbn_is_even(&b_buf)) {
			_gorbn_rshift_one_bit(&b_buf);
		}

		/*(3.3) t = |x-y|/2 */
		gorbn_sub(&t_buf, &a_buf, &b_buf);
		t_buf.sign = 1;
		_gorbn_rshift_one_bit(&t_buf);

		/*(3.4) If x>=y then x=t, otherwise, y=t*/
		int cmp_res = gorbn_cmp(&a_buf, &b_buf);
		if (cmp_res >= GORBN_CMP_EQUAL) {
			gorbn_copy(&a_buf, &t_buf);
		}
		else {
			gorbn_copy(&b_buf, &t_buf);
		}
	}

	/*(4) Return(g * y) */
	gorbn_mul(r, &b_buf, &g_buf);
}


/*
	DESCRIPTION:
		Binary extended gcd algorithm

		Given integers x and y, computes integers a and b such
		that a*x + b*y = v, where v=gcd(x, y).

	SOURCE:
		Handbook of applied cryptography. 1996

	PARAMS:
		r - output result - gcd(x, y)
		a - optional
		b - optional
		x - input number 1
		y - input number 2
*/
void gorbn_gcd_ext(
	_GORBN_OUTPUT gorbn_bignum* r,
	_GORBN_OUTPUT_OPT gorbn_bignum* a,
	_GORBN_OUTPUT_OPT gorbn_bignum* b,
	_GORBN_INPUT gorbn_bignum* x,
	_GORBN_INPUT gorbn_bignum* y)
{
	GORBN_ASSERT(r);
	GORBN_ASSERT(x);
	GORBN_ASSERT(y);

	int count_go_back = 0;

	gorbn_bignum g_buf;
	gorbn_bignum x_buf;
	gorbn_bignum y_buf;
	gorbn_bignum u_buf;
	gorbn_bignum v_buf;
	gorbn_bignum A_buf;
	gorbn_bignum B_buf;
	gorbn_bignum C_buf;
	gorbn_bignum D_buf;

	gorbn_init(&g_buf);
	gorbn_copy(&x_buf, x);
	gorbn_copy(&y_buf, y);
	gorbn_init(&u_buf);
	gorbn_init(&v_buf);
	gorbn_init(&A_buf);
	gorbn_init(&B_buf);
	gorbn_init(&C_buf);
	gorbn_init(&D_buf);

	/*(1) g=1 */
	g_buf.number[0] = 1;

	/*(2) while x and y are both even, do the following: x=x/2, y=y/2, g=2*g */
	while (
		_gorbn_is_even(&x_buf) &&
		_gorbn_is_even(&y_buf))
	{
		_gorbn_rshift_one_bit(&x_buf);
		_gorbn_rshift_one_bit(&y_buf);
		_gorbn_lshift_one_bit(&g_buf);
	}

	/*(3) u=x, v=y, A=1, B=0, C=0, D=1*/
	gorbn_copy(&u_buf, &x_buf);
	gorbn_copy(&v_buf, &y_buf);
	A_buf.number[0] = 1;
	D_buf.number[0] = 1;

	step_4:
	/*(4) while u is even do the following: */
	while (_gorbn_is_even(&u_buf) &&
		!gorbn_is_zero(&u_buf))
	{
		/*(4.1) u = u / 2 */
		_gorbn_rshift_one_bit(&u_buf);

		/*(4.2) if Amod2==Bmod2==0mod2 then A=A/2, B=B/2, else A=(A + y)/2, B=(B-x)/2 */
		if (_gorbn_is_even(&A_buf) &&
			_gorbn_is_even(&B_buf))
		{
			_gorbn_rshift_one_bit(&A_buf);
			_gorbn_rshift_one_bit(&B_buf);
		}
		else {
			gorbn_add(&A_buf, &A_buf, &y_buf);
			_gorbn_rshift_one_bit(&A_buf);

			gorbn_sub(&B_buf, &B_buf, &x_buf);
			_gorbn_rshift_one_bit(&B_buf);
		}
	}

	/*(5) While v is even do the following: */
	while (_gorbn_is_even(&v_buf)) 
	{
		/*(5.1) v = v / 2 */
		_gorbn_rshift_one_bit(&v_buf);

		/*(5.2) if Cmod2==Dmod2==0mod2 then C=C/2, D=D/2, else C=(C + y)/2, D=(D-x)/2 */
		if (_gorbn_is_even(&C_buf) &&
			_gorbn_is_even(&D_buf))
		{
			_gorbn_rshift_one_bit(&C_buf);
			_gorbn_rshift_one_bit(&D_buf);
		}
		else {
			gorbn_add(&C_buf, &C_buf, &y_buf);
			_gorbn_rshift_one_bit(&C_buf);

			gorbn_sub(&D_buf, &D_buf, &x_buf);
			_gorbn_rshift_one_bit(&D_buf);
		}

		if (gorbn_is_zero(&v_buf)) {
			break;
		}
	}

	/*(6) 
		if u>=v then u=u-v, A=A-C, B=B-D
		otherwise, v=v-u, C=C-A, D=D-B */
	int cmp_res = gorbn_cmp(&u_buf, &v_buf);
	if (cmp_res >= GORBN_CMP_EQUAL) {
		gorbn_sub(&u_buf, &u_buf, &v_buf);
		gorbn_sub(&A_buf, &A_buf, &C_buf);
		gorbn_sub(&B_buf, &B_buf, &D_buf);
	}
	else {
		gorbn_sub(&v_buf, &v_buf, &u_buf);
		gorbn_sub(&C_buf, &C_buf, &A_buf);
		gorbn_sub(&D_buf, &D_buf, &B_buf);
	}

	/*(7) If u=0, then a=C, b=D, and return(a, b, g*v); else go to step 4 */
	if (gorbn_is_zero(&u_buf)) {
		if (a) {
			gorbn_copy(a, &C_buf);
		}
		
		if (b) {
			gorbn_copy(b, &D_buf);
		}

		gorbn_mul(r, &g_buf, &v_buf);
	}
	else {
		count_go_back++;
		goto step_4;
	}
}

void gorbn_and(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b)
{
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] | b->number[i];
	}
}

void gorbn_or(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] ^ b->number[i];
	}
}

void gorbn_xor(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] & b->number[i];
	}
}

void gorbn_lshift(gorbn_bignum* r, gorbn_bignum* a, int nbits) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);

	int i;

	gorbn_copy(r, a);

	int words_count = nbits / GORBN_SZWORD_BITS;
	int bit_offset = nbits & GORBN_SZWORD_BITS_MINUS_ONE;

	_gorbn_lshift_words(r, words_count);
	for (i = (GORBN_SZARR - 1); i > 0; --i) {
		r->number[i] = (r->number[i] << bit_offset) | (r->number[i - 1] >> (GORBN_SZWORD_BITS - bit_offset));
	}
	r->number[0] <<= bit_offset;
}

void gorbn_rshift(gorbn_bignum* r, gorbn_bignum* b, int nbits) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(b);

	int i;

	gorbn_copy(r, b);
	
	int words_count = nbits / GORBN_SZWORD_BITS;
	int bit_offset = nbits & GORBN_SZWORD_BITS_MINUS_ONE;

	_gorbn_rshift_words(r, words_count);
	for (i = 0; i < (GORBN_SZARR - 1); i++) {
		r->number[i] = (r->number[i] >> bit_offset) | (r->number[i + 1] << (GORBN_SZWORD_BITS - bit_offset));
	}
	r->number[GORBN_SZARR - 1] >>= bit_offset;
}

int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i;
	int res = GORBN_CMP_EQUAL;
	int mod_res = 0;

	if(a->sign != b->sign){
		res = (a->sign > b->sign) ? GORBN_CMP_LARGER : GORBN_CMP_SMALLER;
	}
	else{
		i = GORBN_SZARR;
		do{
			i -= 1; /* Decrement first, to start with last array element */
			if (a->number[i] > b->number[i])
			{
				mod_res = GORBN_CMP_LARGER;
				break;
			}
			else if (a->number[i] < b->number[i])
			{
				mod_res = GORBN_CMP_SMALLER;
				break;
			}
		} while(i != 0);

		res = mod_res * a->sign;
	}

	return(res);
}

int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i = GORBN_SZARR;
	do{
		i -= 1; /* Decrement first, to start with last array element */
		if (a->number[i] > b->number[i])
		{
			return GORBN_CMP_LARGER;
		}
		else if (a->number[i] < b->number[i])
		{
			return GORBN_CMP_SMALLER;
		}
	} while(i != 0);

	return(GORBN_CMP_EQUAL);
}

int gorbn_is_zero(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int res = 1;

	int i;
	for (i = 0; i < GORBN_SZARR; i++) {
		if (a->number[i]) {
			res = 0;
			break;
		}
	}

	return(res);
}

#endif