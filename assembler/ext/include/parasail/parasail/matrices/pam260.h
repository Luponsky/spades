/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 *
 * This file was converted to C code from the raw file found at
 * ftp://ftp.cbi.pku.edu.cn/pub/software/blast/matrices/PAM260, the
 * Center for Bioinformatics, Peking University, China.
 */
#ifndef _PARASAIL_PAM260_H_
#define _PARASAIL_PAM260_H_

#include "parasail/parasail.h"
#include "pam_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/* # */
/* # This matrix was produced by "pam" Version 1.0.6 [28-Jul-93] */
/* # */
/* # PAM 260 substitution matrix, scale = ln(2)/3 = 0.231049 */
/* # */
/* # Expected score = -0.794, Entropy = 0.330 bits */
/* # */
/* # Lowest score = -7, Highest score = 17 */
/* # */

static const int parasail_pam260_[] = {
/*        A   R   N   D   C   Q   E   G   H   I   L   K   M   F   P   S   T   W   Y   V   B   Z   X   * */
/* A */   2, -1,  0,  0, -2,  0,  0,  1, -1,  0, -2, -1, -1, -3,  1,  1,  1, -6, -3,  0,  0,  0,  0, -7,
/* R */  -1,  6,  0, -1, -4,  1, -1, -2,  2, -2, -3,  3,  0, -4,  0,  0, -1,  2, -4, -2, -1,  0, -1, -7,
/* N */   0,  0,  2,  2, -3,  1,  1,  0,  2, -2, -3,  1, -2, -3,  0,  1,  0, -4, -2, -2,  2,  1,  0, -7,
/* D */   0, -1,  2,  4, -5,  2,  3,  1,  1, -2, -4,  0, -2, -5, -1,  0,  0, -6, -4, -2,  3,  3, -1, -7,
/* C */  -2, -4, -3, -5, 12, -5, -5, -3, -3, -2, -6, -5, -5, -4, -3,  0, -2, -7,  0, -2, -4, -5, -3, -7,
/* Q */   0,  1,  1,  2, -5,  4,  2, -1,  3, -2, -2,  1, -1, -4,  0,  0, -1, -5, -4, -2,  1,  3,  0, -7,
/* E */   0, -1,  1,  3, -5,  2,  4,  0,  1, -2, -3,  0, -2, -5,  0,  0,  0, -7, -4, -2,  3,  3, -1, -7,
/* G */   1, -2,  0,  1, -3, -1,  0,  5, -2, -2, -4, -2, -3, -5,  0,  1,  0, -7, -5, -1,  1,  0, -1, -7,
/* H */  -1,  2,  2,  1, -3,  3,  1, -2,  6, -2, -2,  0, -2, -2,  0, -1, -1, -3,  0, -2,  1,  2, -1, -7,
/* I */   0, -2, -2, -2, -2, -2, -2, -2, -2,  4,  2, -2,  2,  1, -2, -1,  0, -5, -1,  4, -2, -2, -1, -7,
/* L */  -2, -3, -3, -4, -6, -2, -3, -4, -2,  2,  6, -3,  4,  2, -2, -3, -2, -2, -1,  2, -3, -2, -1, -7,
/* K */  -1,  3,  1,  0, -5,  1,  0, -2,  0, -2, -3,  4,  0, -5, -1,  0,  0, -3, -4, -2,  1,  0, -1, -7,
/* M */  -1,  0, -2, -2, -5, -1, -2, -3, -2,  2,  4,  0,  6,  0, -2, -1, -1, -4, -2,  2, -2, -2, -1, -7,
/* F */  -3, -4, -3, -5, -4, -4, -5, -5, -2,  1,  2, -5,  0,  9, -4, -3, -3,  0,  7, -1, -4, -5, -2, -7,
/* P */   1,  0,  0, -1, -3,  0,  0,  0,  0, -2, -2, -1, -2, -4,  6,  1,  0, -5, -5, -1, -1,  0, -1, -7,
/* S */   1,  0,  1,  0,  0,  0,  0,  1, -1, -1, -3,  0, -1, -3,  1,  1,  1, -2, -3, -1,  0,  0,  0, -7,
/* T */   1, -1,  0,  0, -2, -1,  0,  0, -1,  0, -2,  0, -1, -3,  0,  1,  2, -5, -3,  0,  0,  0,  0, -7,
/* W */  -6,  2, -4, -6, -7, -5, -7, -7, -3, -5, -2, -3, -4,  0, -5, -2, -5, 17,  0, -6, -5, -6, -4, -7,
/* Y */  -3, -4, -2, -4,  0, -4, -4, -5,  0, -1, -1, -4, -2,  7, -5, -3, -3,  0, 10, -2, -3, -4, -2, -7,
/* V */   0, -2, -2, -2, -2, -2, -2, -1, -2,  4,  2, -2,  2, -1, -1, -1,  0, -6, -2,  4, -2, -2, -1, -7,
/* B */   0, -1,  2,  3, -4,  1,  3,  1,  1, -2, -3,  1, -2, -4, -1,  0,  0, -5, -3, -2,  3,  2,  0, -7,
/* Z */   0,  0,  1,  3, -5,  3,  3,  0,  2, -2, -2,  0, -2, -5,  0,  0,  0, -6, -4, -2,  2,  3, -1, -7,
/* X */   0, -1,  0, -1, -3,  0, -1, -1, -1, -1, -1, -1, -1, -2, -1,  0,  0, -4, -2, -1,  0, -1, -1, -7,
/* * */  -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7,  1
};

static const parasail_matrix_t parasail_pam260 = {
    "pam260",
    parasail_pam260_,
    parasail_pam_map,
    24,
    17,
    -7,
    NULL
};

#ifdef __cplusplus
}
#endif

#endif /* _PARASAIL_PAM260_H_ */

