/*  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2017-2018 Daniel Baston <dbaston@gmail.com>
 */

#include <R.h>
#include <Rinternals.h>
#include <inttypes.h>

#define CLAMP(x, a, b) (x > b ? b : (x < a ? a : x))

typedef double(*bitmanipfn_t)(double, int);

const char bit_rep[16][5] = {
    "0000", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111",
};

/* Private functions */
static uint8_t bits_for_precision(int32_t significant_digits) {
    significant_digits = CLAMP(significant_digits, 1, 16);

    int32_t bits_needed = ceil(significant_digits / log10(2));

    return CLAMP(bits_needed, 1, 52);
}

/* Use a mask of significant bits to set insignificant bits to 0 */
static double trim_double(double d, uint64_t mask) {
    uint64_t* double_bits = (uint64_t*) (&d);

    (*double_bits) &= mask;

    return *((double*) double_bits);
}

/* Use a mask of significant bits to set insignificant bits to 1 */
static double pad_double(double d, uint64_t mask) {
    uint64_t* double_bits = (uint64_t*) (&d);

    (*double_bits) |= (~mask);

    return *((double*) double_bits);
}

/* Trim values while maintaining a certain number of significant digits. */
static double trim_preserve_significant_digits(double d, int32_t significant_digits) {
    uint8_t bits_needed = bits_for_precision(significant_digits);

    return trim_double(d, 0xffffffffffffffff << (52 - bits_needed));
}

/* Pad values while maintaining a certain number of significant digits. */
static double pad_preserve_significant_digits(double d, int32_t significant_digits) {
    uint8_t bits_needed = bits_for_precision(significant_digits);

    return pad_double(d, 0xffffffffffffffff << (52 - bits_needed));
}

/* Find out how many significant digits are needed to maintain a certain number of
 * digits after the decimal point.
 */
static double significant_digits(double d, int32_t decimal_digits) {
  return decimal_digits + ((int) (1 + log10(fabs(d))));
}

/* Trim values while maintaining a certain number of decimal digits. */
static double trim_preserve_decimal_digits(double d, int32_t decimal_digits) {
    return trim_preserve_significant_digits(d, significant_digits(d, decimal_digits));
}

/* Pad values while maintaining a certain number of decimal digits. */
static double pad_preserve_decimal_digits(double d, int32_t decimal_digits) {
    return pad_preserve_significant_digits(d, significant_digits(d, decimal_digits));
}

/* Process finite, nonzero elements of v from with fn(v[i], p),
   beginning at 'from' and stepping with 'by' */
static void process_bits(SEXP v, SEXP p, SEXP out, bitmanipfn_t fn, int from, int by) {
    int n = length(v);

    if (TYPEOF(v) == REALSXP) {
        for (int i=from; i < n; i += by) {
            if (R_FINITE(REAL(v)[i]) && REAL(v)[i] != 0) {
              REAL(out)[i] = fn(REAL(v)[i], INTEGER(p)[0]);
            } else {
              REAL(out)[i] = REAL(v)[i];
            }
        }
    }
}

/* Public functions */
SEXP C_double_bits(SEXP dbl) {
  double d = REAL(dbl)[0];
  uint64_t bits = *((uint64_t*) &d);

  char bitstring[65] = { '\0' };
  for (int i = 0; i < 16; i++) {
    memcpy(&(bitstring[60 - i*4]), &(bit_rep[bits & 0x0f]), 4*sizeof(char));
    bits >>= 4;
  }

  return mkString(bitstring);
}

SEXP C_trim_bits_sd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, trim_preserve_significant_digits, 0, 1);
  UNPROTECT(1);
  return out;
}

SEXP C_trim_bits_dd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, trim_preserve_decimal_digits, 0, 1);
  UNPROTECT(1);
  return out;
}

SEXP C_pad_bits_sd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, pad_preserve_significant_digits, 0, 1);
  UNPROTECT(1);
  return out;
}

SEXP C_pad_bits_dd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, pad_preserve_decimal_digits, 0, 1);
  UNPROTECT(1);
  return out;
}

SEXP C_groom_bits_dd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, trim_preserve_decimal_digits, 0, 2);
  process_bits(v, p, out, pad_preserve_decimal_digits, 1, 2);
  UNPROTECT(1);
  return out;
}

SEXP C_groom_bits_sd(SEXP v, SEXP p) {
  SEXP out = PROTECT(allocVector(REALSXP, length(v)));
  process_bits(v, p, out, trim_preserve_significant_digits, 0, 2);
  process_bits(v, p, out, pad_preserve_significant_digits, 1, 2);
  UNPROTECT(1);
  return out;
}

static const R_CallMethodDef CallEntries[] = {
    {"C_trim_bits_sd",  (DL_FUNC) &C_trim_bits_sd,  2},
    {"C_pad_bits_sd",   (DL_FUNC) &C_pad_bits_sd,   2},
    {"C_groom_bits_sd", (DL_FUNC) &C_groom_bits_sd, 2},
    {"C_trim_bits_dd",  (DL_FUNC) &C_trim_bits_dd,  2},
    {"C_pad_bits_dd",   (DL_FUNC) &C_pad_bits_dd,   2},
    {"C_groom_bits_dd", (DL_FUNC) &C_groom_bits_dd, 2},
    {"C_double_bits",   (DL_FUNC) &C_double_bits,   1},
    {NULL, NULL, 0}
};

void R_init_bitsqueezr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_forceSymbols(dll, TRUE);
    R_useDynamicSymbols(dll, TRUE);
}
