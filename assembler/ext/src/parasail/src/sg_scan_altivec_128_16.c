/**
 * @file
 *
 * @author jeff.daily@pnnl.gov
 *
 * Copyright (c) 2015 Battelle Memorial Institute.
 */
#include "config.h"

#include <stdint.h>
#include <stdlib.h>



#include "parasail/parasail.h"
#include "parasail/parasail/memory.h"
#include "parasail/parasail/internal_altivec.h"



#ifdef PARASAIL_TABLE
static inline void arr_store_si128(
        int *array,
        vec128i vH,
        int32_t t,
        int32_t seglen,
        int32_t d,
        int32_t dlen)
{
    array[1LL*(0*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 0);
    array[1LL*(1*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 1);
    array[1LL*(2*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 2);
    array[1LL*(3*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 3);
    array[1LL*(4*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 4);
    array[1LL*(5*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 5);
    array[1LL*(6*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 6);
    array[1LL*(7*seglen+t)*dlen + d] = (int16_t)_mm_extract_epi16(vH, 7);
}
#endif

#ifdef PARASAIL_ROWCOL
static inline void arr_store_col(
        int *col,
        vec128i vH,
        int32_t t,
        int32_t seglen)
{
    col[0*seglen+t] = (int16_t)_mm_extract_epi16(vH, 0);
    col[1*seglen+t] = (int16_t)_mm_extract_epi16(vH, 1);
    col[2*seglen+t] = (int16_t)_mm_extract_epi16(vH, 2);
    col[3*seglen+t] = (int16_t)_mm_extract_epi16(vH, 3);
    col[4*seglen+t] = (int16_t)_mm_extract_epi16(vH, 4);
    col[5*seglen+t] = (int16_t)_mm_extract_epi16(vH, 5);
    col[6*seglen+t] = (int16_t)_mm_extract_epi16(vH, 6);
    col[7*seglen+t] = (int16_t)_mm_extract_epi16(vH, 7);
}
#endif

#ifdef PARASAIL_TABLE
#define FNAME parasail_sg_table_scan_altivec_128_16
#define PNAME parasail_sg_table_scan_profile_altivec_128_16
#else
#ifdef PARASAIL_ROWCOL
#define FNAME parasail_sg_rowcol_scan_altivec_128_16
#define PNAME parasail_sg_rowcol_scan_profile_altivec_128_16
#else
#define FNAME parasail_sg_scan_altivec_128_16
#define PNAME parasail_sg_scan_profile_altivec_128_16
#endif
#endif

parasail_result_t* FNAME(
        const char * const restrict s1, const int s1Len,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap, const parasail_matrix_t *matrix)
{
    parasail_profile_t *profile = parasail_profile_create_altivec_128_16(s1, s1Len, matrix);
    parasail_result_t *result = PNAME(profile, s2, s2Len, open, gap);
    parasail_profile_free(profile);
    return result;
}

parasail_result_t* PNAME(
        const parasail_profile_t * const restrict profile,
        const char * const restrict s2, const int s2Len,
        const int open, const int gap)
{
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t end_query = 0;
    int32_t end_ref = 0;
    const int s1Len = profile->s1Len;
    const parasail_matrix_t *matrix = profile->matrix;
    const int32_t segWidth = 8; /* number of values in vector unit */
    const int32_t segLen = (s1Len + segWidth - 1) / segWidth;
    const int32_t offset = (s1Len - 1) % segLen;
    const int32_t position = (segWidth - 1) - (s1Len - 1) / segLen;
    vec128i* const restrict pvP = (vec128i*)profile->profile16.score;
    vec128i* const restrict pvE = parasail_memalign_vec128i(16, segLen);
    vec128i* const restrict pvHt= parasail_memalign_vec128i(16, segLen);
    vec128i* const restrict pvH = parasail_memalign_vec128i(16, segLen);
    vec128i* const restrict pvGapper = parasail_memalign_vec128i(16, segLen);
    vec128i vGapO = _mm_set1_epi16(open);
    vec128i vGapE = _mm_set1_epi16(gap);
    const int16_t NEG_LIMIT = (-open < matrix->min ?
        INT16_MIN + open : INT16_MIN - matrix->min) + 1;
    const int16_t POS_LIMIT = INT16_MAX - matrix->max - 1;
    vec128i vZero = _mm_setzero_si128();
    int16_t score = NEG_LIMIT;
    vec128i vNegLimit = _mm_set1_epi16(NEG_LIMIT);
    vec128i vPosLimit = _mm_set1_epi16(POS_LIMIT);
    vec128i vSaturationCheckMin = vPosLimit;
    vec128i vSaturationCheckMax = vNegLimit;
    vec128i vMaxH = vNegLimit;
    vec128i vPosMask = _mm_cmpeq_epi16(_mm_set1_epi16(position),
            _mm_set_epi16(0,1,2,3,4,5,6,7));
    vec128i vNegInfFront = vZero;
    vec128i vSegLenXgap;
#ifdef PARASAIL_TABLE
    parasail_result_t *result = parasail_result_new_table1(segLen*segWidth, s2Len);
#else
#ifdef PARASAIL_ROWCOL
    parasail_result_t *result = parasail_result_new_rowcol1(segLen*segWidth, s2Len);
#else
    parasail_result_t *result = parasail_result_new();
#endif
#endif

    vNegInfFront = _mm_insert_epi16(vNegInfFront, NEG_LIMIT, 0);
    vSegLenXgap = _mm_add_epi16(vNegInfFront,
            _mm_slli_si128(_mm_set1_epi16(-segLen*gap), 2));

    /* initialize H and E */
    parasail_memset_vec128i(pvH, vZero, segLen);
    parasail_memset_vec128i(pvE, vNegLimit, segLen);
    {
        vec128i vGapper = _mm_sub_epi16(vZero,vGapO);
        for (i=segLen-1; i>=0; --i) {
            _mm_store_si128(pvGapper+i, vGapper);
            vGapper = _mm_sub_epi16(vGapper, vGapE);
        }
    }

    /* outer loop over database sequence */
    for (j=0; j<s2Len; ++j) {
        vec128i vE;
        vec128i vHt;
        vec128i vF;
        vec128i vH;
        vec128i vHp;
        vec128i *pvW;
        vec128i vW;

        /* calculate E */
        /* calculate Ht */
        /* calculate F and H first pass */
        vHp = _mm_load_si128(pvH+(segLen-1));
        vHp = _mm_slli_si128(vHp, 2);
        pvW = pvP + matrix->mapper[(unsigned char)s2[j]]*segLen;
        vHt = _mm_sub_epi16(vNegLimit, pvGapper[0]);
        vF = vNegLimit;
        for (i=0; i<segLen; ++i) {
            vH = _mm_load_si128(pvH+i);
            vE = _mm_load_si128(pvE+i);
            vW = _mm_load_si128(pvW+i);
            vE = _mm_max_epi16(
                    _mm_sub_epi16(vE, vGapE),
                    _mm_sub_epi16(vH, vGapO));
            vHp = _mm_add_epi16(vHp, vW);
            vF = _mm_max_epi16(vF, _mm_add_epi16(vHt, pvGapper[i]));
            vHt = _mm_max_epi16(vE, vHp);
            _mm_store_si128(pvE+i, vE);
            _mm_store_si128(pvHt+i, vHt);
            vHp = vH;
        }

        /* pseudo prefix scan on F and H */
        vHt = _mm_slli_si128(vHt, 2);
        vF = _mm_max_epi16(vF, _mm_add_epi16(vHt, pvGapper[0]));
        for (i=0; i<segWidth-2; ++i) {
            vec128i vFt = _mm_slli_si128(vF, 2);
            vFt = _mm_add_epi16(vFt, vSegLenXgap);
            vF = _mm_max_epi16(vF, vFt);
        }

        /* calculate final H */
        vF = _mm_slli_si128(vF, 2);
        vF = _mm_add_epi16(vF, vNegInfFront);
        vH = _mm_max_epi16(vHt, vF);
        for (i=0; i<segLen; ++i) {
            vHt = _mm_load_si128(pvHt+i);
            vF = _mm_max_epi16(
                    _mm_sub_epi16(vF, vGapE),
                    _mm_sub_epi16(vH, vGapO));
            vH = _mm_max_epi16(vHt, vF);
            _mm_store_si128(pvH+i, vH);
            vSaturationCheckMin = _mm_min_epi16(vSaturationCheckMin, vH);
            vSaturationCheckMax = _mm_max_epi16(vSaturationCheckMax, vH);
#ifdef PARASAIL_TABLE
            arr_store_si128(result->tables->score_table, vH, i, segLen, j, s2Len);
#endif
        } 

        /* extract vector containing last value from column */
        {
            vec128i vCompare;
            vH = _mm_load_si128(pvH + offset);
            vCompare = _mm_and_si128(vPosMask, _mm_cmpgt_epi16(vH, vMaxH));
            vMaxH = _mm_max_epi16(vH, vMaxH);
            if (_mm_movemask_epi8(vCompare)) {
                end_ref = j;
                end_query = s1Len - 1;
            }
#ifdef PARASAIL_ROWCOL
            for (k=0; k<position; ++k) {
                vH = _mm_slli_si128(vH, 2);
            }
            result->rowcols->score_row[j] = (int16_t) _mm_extract_epi16 (vH, 7);
#endif
        }
    }

    /* max last value from all columns */
    {
        int16_t value;
        for (k=0; k<position; ++k) {
            vMaxH = _mm_slli_si128(vMaxH, 2);
        }
        value = (int16_t) _mm_extract_epi16(vMaxH, 7);
        if (value > score) {
            score = value;
        }
    }

    /* max of last column */
    {
        int16_t score_last;
        vMaxH = vNegLimit;

        for (i=0; i<segLen; ++i) {
            vec128i vH = _mm_load_si128(pvH + i);
            vMaxH = _mm_max_epi16(vH, vMaxH);
#ifdef PARASAIL_ROWCOL
            arr_store_col(result->rowcols->score_col, vH, i, segLen);
#endif
        }

        /* max in vec */
        score_last = _mm_hmax_epi16(vMaxH);
        if (score_last > score || (score_last == score && end_ref == s2Len - 1)) {
            score = score_last;
            end_ref = s2Len - 1;
            end_query = s1Len;
            /* Trace the alignment ending position on read. */
            {
                int16_t *t = (int16_t*)pvH;
                int32_t column_len = segLen * segWidth;
                for (i = 0; i<column_len; ++i, ++t) {
                    if (*t == score) {
                        int32_t temp = i / segWidth + i % segWidth * segLen;
                        if (temp < end_query) {
                            end_query = temp;
                        }
                    }
                }
            }
        }
    }

    if (_mm_movemask_epi8(_mm_or_si128(
            _mm_cmplt_epi16(vSaturationCheckMin, vNegLimit),
            _mm_cmpgt_epi16(vSaturationCheckMax, vPosLimit)))) {
        result->flag |= PARASAIL_FLAG_SATURATED;
        score = 0;
        end_query = 0;
        end_ref = 0;
    }

    result->score = score;
    result->end_query = end_query;
    result->end_ref = end_ref;
    result->flag |= PARASAIL_FLAG_SG | PARASAIL_FLAG_SCAN
        | PARASAIL_FLAG_BITS_16 | PARASAIL_FLAG_LANES_8;
#ifdef PARASAIL_TABLE
    result->flag |= PARASAIL_FLAG_TABLE;
#endif
#ifdef PARASAIL_ROWCOL
    result->flag |= PARASAIL_FLAG_ROWCOL;
#endif

    parasail_free(pvGapper);
    parasail_free(pvH);
    parasail_free(pvHt);
    parasail_free(pvE);

    return result;
}


