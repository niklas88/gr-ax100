/**
 * Copyright 2016 Daniel Estevez <daniel@destevez.net>.
 *
 * This is free and unencumbered software released into the public domain.
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

static const uint32_t H[N] = { 0x8008ed, 0x4001db, 0x2003b5, 0x100769,
			       0x80ed1, 0x40da3, 0x20b47, 0x1068f,
			       0x8d1d, 0x4a3b, 0x2477, 0x1ffe };

#define B(i) (H[i] & 0xfff)

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

  // Step 2. if w(s) <= 3, then e = (s, 0) and go to step 8
  if (__builtin_popcount(s) <= 3) {
    e = s;
    e <<= N;
    goto step8;
  }

  // Step 3. if w(s + B[i]) <= 2, then e = (s + B[i], e_{i+1}) and go to step 8
  for (i = 0; i < N; i++) {
    if (__builtin_popcount(s ^ B(i)) <= 2) {
      e = s ^ B(i);
      e <<= N;
      e |= 1 << (N - i - 1);
      goto step8;
    }
  }

  // Step 4. compute q = B*s
  q = 0;
  for (i = 0; i < N; i++) {
    q <<= 1;
    q |= __builtin_parity(B(i) & s);
  }

  // Step 5. If w(q) <= 3, then e = (0, q) and go to step 8
  if (__builtin_popcount(q) <= 3) {
    e = q;
    goto step8;
  }
  
  // Step 6. If w(q + B[i]) <= 2, then e = (e_{i+1}, q + B[i]) and got to step 8
  for (i = 0; i < N; i++) {
    if (__builtin_popcount(q ^ B(i)) <= 2) {
      e = 1 << (2*N - i - 1);
      e |= q ^ B(i);
      goto step8;
    }
  }

  // Step 7. r is uncorrectable
  return -1;

 step8:
  // Step 8. c = r + e
  *data = r ^ e;

  return __builtin_popcount(e);
}
