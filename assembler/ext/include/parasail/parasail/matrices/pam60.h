/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 *
 * This file was converted to C code from the raw file found at
 * ftp://ftp.cbi.pku.edu.cn/pub/software/blast/matrices/PAM60, the
 * Center for Bioinformatics, Peking University, China.
 */
#ifndef _PARASAIL_PAM60_H_
#define _PARASAIL_PAM60_H_

#include "parasail/parasail.h"
#include "pam_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/* # */
/* # This matrix was produced by "pam" Version 1.0.6 [28-Jul-93] */
/* # */
/* # PAM 60 substitution matrix, scale = ln(2)/2 = 0.346574 */
/* # */
/* # Expected score = -3.21, Entropy = 1.79 bits */
/* # */
/* # Lowest score = -12, Highest score = 13 */
/* # */

static const int parasail_pam60_[] = {
/*        A   R   N   D   C   Q   E   G   H   I   L   K   M   F   P   S   T   W   Y   V   B   Z   X   * */
/* A */   5, -5, -2, -2, -5, -3, -1,  0, -5, -3, -4, -5, -3, -6,  0,  1,  1,-10, -6, -1, -2, -2, -2,-12,
/* R */  -5,  8, -3, -6, -6,  0, -6, -7,  0, -4, -6,  2, -2, -7, -2, -2, -4,  0, -8, -5, -5, -2, -4,-12,
/* N */  -2, -3,  6,  2, -7, -2,  0, -1,  1, -4, -5,  0, -6, -6, -4,  1, -1, -6, -3, -5,  5, -1, -2,-12,
/* D */  -2, -6,  2,  7,-10, -1,  3, -2, -2, -5, -9, -2, -7,-11, -5, -2, -3,-11, -8, -6,  5,  2, -3,-12,
/* C */  -5, -6, -7,-10,  9,-10,-10, -7, -6, -4,-11,-10,-10, -9, -6, -1, -5,-12, -2, -4, -9,-10, -6,-12,
/* Q */  -3,  0, -2, -1,-10,  7,  2, -5,  2, -5, -3, -1, -2, -9, -1, -3, -4, -9, -8, -5, -1,  6, -3,-12,
/* E */  -1, -6,  0,  3,-10,  2,  7, -2, -3, -4, -7, -3, -5,-10, -3, -2, -4,-12, -7, -4,  2,  5, -3,-12,
/* G */   0, -7, -1, -2, -7, -5, -2,  6, -6, -7, -8, -5, -6, -7, -4,  0, -3,-11,-10, -4, -2, -3, -3,-12,
/* H */  -5,  0,  1, -2, -6,  2, -3, -6,  8, -6, -4, -4, -7, -4, -2, -4, -5, -5, -2, -5,  0,  0, -3,-12,
/* I */  -3, -4, -4, -5, -4, -5, -4, -7, -6,  7,  0, -4,  1, -1, -6, -4, -1,-10, -4,  3, -4, -4, -3,-12,
/* L */  -4, -6, -5, -9,-11, -3, -7, -8, -4,  0,  6, -6,  2, -1, -5, -6, -5, -4, -5, -1, -7, -5, -4,-12,
/* K */  -5,  2,  0, -2,-10, -1, -3, -5, -4, -4, -6,  6,  0,-10, -4, -2, -2, -8, -7, -6, -1, -2, -3,-12,
/* M */  -3, -2, -6, -7,-10, -2, -5, -6, -7,  1,  2,  0, 10, -2, -6, -4, -2, -9, -7,  0, -6, -4, -3,-12,
/* F */  -6, -7, -6,-11, -9, -9,-10, -7, -4, -1, -1,-10, -2,  8, -7, -5, -6, -3,  3, -5, -8,-10, -5,-12,
/* P */   0, -2, -4, -5, -6, -1, -3, -4, -2, -6, -5, -4, -6, -7,  7,  0, -2,-10,-10, -4, -4, -2, -3,-12,
/* S */   1, -2,  1, -2, -1, -3, -2,  0, -4, -4, -6, -2, -4, -5,  0,  5,  1, -4, -5, -4,  0, -3, -2,-12,
/* T */   1, -4, -1, -3, -5, -4, -4, -3, -5, -1, -5, -2, -2, -6, -2,  1,  6, -9, -5, -1, -2, -4, -2,-12,
/* W */ -10,  0, -6,-11,-12, -9,-12,-11, -5,-10, -4, -8, -9, -3,-10, -4, -9, 13, -3,-11, -8,-11, -8,-12,
/* Y */  -6, -8, -3, -8, -2, -8, -7,-10, -2, -4, -5, -7, -7,  3,-10, -5, -5, -3,  9, -5, -5, -7, -5,-12,
/* V */  -1, -5, -5, -6, -4, -5, -4, -4, -5,  3, -1, -6,  0, -5, -4, -4, -1,-11, -5,  6, -5, -5, -3,-12,
/* B */  -2, -5,  5,  5, -9, -1,  2, -2,  0, -4, -7, -1, -6, -8, -4,  0, -2, -8, -5, -5,  5,  1, -3,-12,
/* Z */  -2, -2, -1,  2,-10,  6,  5, -3,  0, -4, -5, -2, -4,-10, -2, -3, -4,-11, -7, -5,  1,  5, -3,-12,
/* X */  -2, -4, -2, -3, -6, -3, -3, -3, -3, -3, -4, -3, -3, -5, -3, -2, -2, -8, -5, -3, -3, -3, -3,-12,
/* * */ -12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,-12,  1
};

static const parasail_matrix_t parasail_pam60 = {
    "pam60",
    parasail_pam60_,
    parasail_pam_map,
    24,
    13,
    -12,
    NULL
};

#ifdef __cplusplus
}
#endif

#endif /* _PARASAIL_PAM60_H_ */

