/* This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

/*
 This algorithm is based on 
 R.H. Morelos-Zaragoza, The Art of Error Correcting Coding, Wiley, 2002; Section 2.2.3
*/

#include <stdint.h>

#include "golay24.h"

#define N 12

static const uint32_t H[N] = { 0x7ff800, 0xee2400, 0xdc5200, 0xb8b100, 0xf16080, 0xe2d040,
			       0xc5b020, 0x8b7010, 0x96e008, 0xadc004, 0xdb8002, 0xb71001 };
static const uint16_t B[N] = { 0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d,
			       0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71 };

int decode_golay24(uint32_t *data) {
  register uint32_t r = *data;
  register uint16_t s; /* syndrome */
  register uint16_t q; /* modified syndrome */
  register uint32_t e; /* estimated error vector */
  register int i;

  // Step 1. s = H*r
  s = 0;
  for (i = 0; i < N; i++) {
    s <<= 1;
    s |= __builtin_parity(H[i] & r);
  }

  // Step 2. if w(s) <= 3, then e = (0,s) and go to step 8
  if (__builtin_popcount(s) > 3) {
    // Step 3. if w(s + B[i]) <= 2, then e = (e_{i+1} , s + B[i]) and go to step 8
    for (i = 0; i < N; i++) {
      if (__builtin_popcount(s ^ B[i]) <= 2) {
	e = 1 << (2*N - i - 1);
	e |= s ^ B[i];
	break;
      }
    }
    if (i == N) {
      // Step 4. compute q = B*s
      q = 0;
      for (i = 0; i < N; i++) {
	q <<= 1;
	q |= __builtin_parity(B[i] & s);
      }

      // Step 5. If w(q) <= 3, then e = (q, 0) and go to step 8
      if (__builtin_popcount(q) > 3) {
	// Step 6. If w(q + B[i]) <= 2, then e = (q + B[i], e_{i+1}) and got to step 8
	for (i = 0; i < N; i++) {
	  if (__builtin_popcount(q ^ B[i]) <= 2) {
	    e = q ^ B[i];
	    e <<= N;
	    e |= 1 << (N - i - 1);
	    break;
	  }
	}
	if (i == N) {
	  // Step 7. r is uncorrectable
	  return -1;
	}
      }
      else {
	e = q;
	e <<= N;
      }
    }
  }
  else {
    e = s;
  }
  
  // Step 8. c = r + e
  *data = r ^ e;

  return __builtin_popcount(e);
}
