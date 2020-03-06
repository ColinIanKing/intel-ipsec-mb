/*****************************************************************************
 Copyright (c) 2019-2020, Intel Corporation

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <intel-ipsec-mb.h>

#include "gcm_ctr_vectors_test.h"
#include "utils.h"

int ecb_test(const enum arch_type arch, struct IMB_MGR *mb_mgr);

struct ecb_vector {
	const uint8_t *K;          /* key */
	const uint8_t *P;          /* plain text */
	uint64_t       Plen;       /* plain text length */
        const uint8_t *C;          /* cipher text - same length as plain text */
        uint32_t       Klen;       /* key length */
};

/*  128-bit */
static const uint8_t K1[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static const uint8_t P1[] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
static const uint8_t C1[] = {
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60,
        0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d,
        0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23,
        0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f,
        0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4
};

/*  192-bit */
static const uint8_t K2[] = {
        0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b
};
static const uint8_t P2[] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
static const uint8_t C2[] = {
        0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f,
        0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc,
        0x97, 0x41, 0x04, 0x84, 0x6d, 0x0a, 0xd3, 0xad,
        0x77, 0x34, 0xec, 0xb3, 0xec, 0xee, 0x4e, 0xef,
        0xef, 0x7a, 0xfd, 0x22, 0x70, 0xe2, 0xe6, 0x0a,
        0xdc, 0xe0, 0xba, 0x2f, 0xac, 0xe6, 0x44, 0x4e,
        0x9a, 0x4b, 0x41, 0xba, 0x73, 0x8d, 0x6c, 0x72,
        0xfb, 0x16, 0x69, 0x16, 0x03, 0xc1, 0x8e, 0x0e
};

/*  256-bit */
static const uint8_t K3[] = {
        0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
};
static const uint8_t P3[] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c,
        0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11,
        0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
static const uint8_t C3[] = {
        0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c,
        0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8,
        0x59, 0x1c, 0xcb, 0x10, 0xd4, 0x10, 0xed, 0x26,
        0xdc, 0x5b, 0xa7, 0x4a, 0x31, 0x36, 0x28, 0x70,
        0xb6, 0xed, 0x21, 0xb9, 0x9c, 0xa6, 0xf4, 0xf9,
        0xf1, 0x53, 0xe7, 0xb1, 0xbe, 0xaf, 0xed, 0x1d,
        0x23, 0x30, 0x4b, 0x7a, 0x39, 0xf9, 0xf3, 0xff,
        0x06, 0x7d, 0x8d, 0x8f, 0x9e, 0x24, 0xec, 0xc7
};


/* Extra AES test vectors */

/*  128-bit */
static const uint8_t K4[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static const uint8_t P4[] = {
        0xf7, 0xcd, 0x12, 0xfb, 0x4f, 0x8e, 0x50, 0xab,
        0x35, 0x8e, 0x56, 0xf9, 0x83, 0x53, 0x9a, 0x1a,
        0xfc, 0x47, 0x3c, 0x96, 0x01, 0xfe, 0x01, 0x87,
        0xd5, 0xde, 0x46, 0x24, 0x5c, 0x62, 0x8f, 0xba,
        0xba, 0x91, 0x17, 0x8d, 0xba, 0x5a, 0x79, 0xb1,
        0x57, 0x05, 0x4d, 0x08, 0xba, 0x1f, 0x30, 0xd3,
        0x80, 0x40, 0xe9, 0x37, 0xb0, 0xd6, 0x34, 0x87,
        0x33, 0xdd, 0xc0, 0x5b, 0x2d, 0x58, 0x1d, 0x2a,
        0x7b, 0xb6, 0xe3, 0xd0, 0xc8, 0xa0, 0x7a, 0x69,
        0xc8, 0x5d, 0x10, 0xa2, 0xc3, 0x39, 0xca, 0xaf,
        0x40, 0xdc, 0xc7, 0xcb, 0xff, 0x18, 0x7d, 0x51,
        0x06, 0x28, 0x28, 0x1f, 0x3a, 0x9c, 0x18, 0x7d,
        0x5b, 0xb5, 0xe9, 0x20, 0xc2, 0xae, 0x17, 0x7f,
        0xd1, 0x65, 0x7a, 0x75, 0xcf, 0x21, 0xa0, 0x1e,
        0x17, 0x1b, 0xf7, 0xe8, 0x62, 0x5f, 0xaf, 0x34,
        0x7f, 0xd8, 0x18, 0x4a, 0x94, 0xf2, 0x33, 0x90
};
static const uint8_t C4[] = {
        0x48, 0xa0, 0xe8, 0x0a, 0x89, 0x99, 0xab, 0xb5,
        0x66, 0x6d, 0x68, 0x23, 0x43, 0x40, 0x1f, 0x26,
        0xac, 0x52, 0xc4, 0x7b, 0x09, 0x0a, 0x8f, 0xc0,
        0x38, 0x00, 0xf5, 0x48, 0x3a, 0xfd, 0xcd, 0x7e,
        0x21, 0xe7, 0xf8, 0xf6, 0xc2, 0xa7, 0x4c, 0x1c,
        0x6e, 0x83, 0x57, 0xf4, 0xa4, 0xb0, 0xc0, 0x5f,
        0x36, 0x73, 0x22, 0xff, 0x33, 0x44, 0xab, 0xeb,
        0x96, 0xa8, 0xe0, 0x37, 0x65, 0x81, 0x6b, 0x82,
        0x89, 0xcd, 0xcc, 0xac, 0x33, 0x18, 0x7d, 0x43,
        0x0e, 0x79, 0x53, 0x30, 0x21, 0x4c, 0x95, 0x18,
        0xb6, 0xc9, 0xea, 0x5c, 0x6f, 0xa1, 0x10, 0xa3,
        0x51, 0x0e, 0x67, 0x8c, 0x1c, 0x9d, 0xf1, 0x57,
        0xeb, 0xf6, 0xad, 0x4f, 0xf2, 0x55, 0xe8, 0x11,
        0x6f, 0xaa, 0x4d, 0xe5, 0x18, 0x3d, 0xc3, 0x14,
        0xf9, 0x40, 0xfa, 0x86, 0x9d, 0xaf, 0xff, 0xfc,
        0x78, 0xba, 0xbe, 0x61, 0xf8, 0xd1, 0x00, 0x8d
};

/*  192-bit */
static const uint8_t K5[] = {
        0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52,
        0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
        0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b
};
static const uint8_t P5[] = {
        0x19, 0x08, 0xa3, 0x58, 0x17, 0x14, 0x70, 0x5a,
        0xb8, 0xab, 0x4f, 0x5f, 0xa4, 0x25, 0x2b, 0xec,
        0xb6, 0x74, 0x0b, 0x9d, 0x56, 0x3b, 0xaf, 0xa3,
        0xa4, 0x2d, 0x3e, 0x1f, 0x18, 0x84, 0x3b, 0x4f,
        0x48, 0xd9, 0xa3, 0xfe, 0x59, 0x1e, 0x80, 0x67,
        0x44, 0x35, 0x26, 0x00, 0x78, 0xda, 0x68, 0xfa,
        0x61, 0x9c, 0xd8, 0x8e, 0x5c, 0xc1, 0xff, 0xeb,
        0x9c, 0x7d, 0xe7, 0xa9, 0x38, 0xeb, 0x66, 0xf8,
        0x6a, 0x46, 0x71, 0x51, 0x02, 0xba, 0x8d, 0x70,
        0x55, 0x5b, 0x60, 0xc6, 0x4c, 0xae, 0xda, 0x2e,
        0x17, 0xbb, 0x65, 0xef, 0x60, 0x85, 0x9e, 0x77,
        0xe5, 0x83, 0xef, 0x30, 0x08, 0x3a, 0xba, 0x80,
        0x28, 0xc0, 0xa1, 0x93, 0x4c, 0x2a, 0x0b, 0xe1,
        0xcb, 0xd0, 0xac, 0x72, 0x72, 0x1d, 0x96, 0x76,
        0x0e, 0xc0, 0xec, 0x7d, 0x84, 0xfd, 0xee, 0x08,
        0xa1, 0x11, 0x20, 0x0d, 0x59, 0x5c, 0x06, 0x3f,
        0xa3, 0xf1, 0xd7, 0xa3, 0x1d, 0x29, 0xc3, 0xaa,
        0x05, 0x2b, 0x74, 0x8c, 0x73, 0x60, 0x65, 0x43,
        0x76, 0xd4, 0xd7, 0x7b, 0x5f, 0x40, 0xf4, 0x77,
        0xe1, 0xcc, 0x85, 0x37, 0x1c, 0xd8, 0xda, 0x91,
        0xf0, 0x40, 0xb2, 0x43, 0x2d, 0x87, 0x51, 0xd0,
        0xce, 0x27, 0xa6, 0x60, 0xac, 0x67, 0xea, 0x8b,
        0xae, 0x46, 0x2e, 0x78, 0x06, 0x09, 0x8a, 0x82,
        0xb0, 0x0d, 0x57, 0x56, 0x82, 0xfe, 0x89, 0xd2
};
static const uint8_t C5[] = {
        0xcc, 0xe2, 0x3f, 0xc3, 0x12, 0x41, 0x31, 0x63,
        0x03, 0x3a, 0x3c, 0xfe, 0x76, 0x55, 0xd2, 0x26,
        0xf0, 0xc9, 0xb5, 0xc6, 0xf0, 0x1e, 0xc3, 0x72,
        0xfb, 0x64, 0x94, 0x7d, 0xf1, 0x5e, 0x2a, 0x9e,
        0x0d, 0x9a, 0x7a, 0xe0, 0xbc, 0x7b, 0xa6, 0x65,
        0x41, 0xc0, 0xa0, 0x9d, 0xb1, 0xb1, 0x09, 0x99,
        0x6e, 0xe7, 0x25, 0x5e, 0x64, 0x2b, 0x74, 0xfa,
        0xa1, 0x9a, 0x03, 0x33, 0x88, 0x81, 0x27, 0x48,
        0xdd, 0x53, 0x77, 0x0b, 0xef, 0xd9, 0x2f, 0xfa,
        0xc8, 0x50, 0x0e, 0x08, 0xa1, 0x45, 0x12, 0x82,
        0x2b, 0xfb, 0x85, 0x5a, 0x39, 0x8c, 0x71, 0x32,
        0x59, 0x27, 0x37, 0x53, 0xce, 0x3e, 0xae, 0x00,
        0x45, 0x53, 0xfd, 0xaf, 0xa5, 0xd1, 0x1a, 0xe9,
        0xa4, 0x1b, 0xe3, 0x99, 0xde, 0xcd, 0x03, 0x36,
        0x6b, 0x72, 0x43, 0x76, 0x04, 0xa8, 0xf9, 0x83,
        0xef, 0x9e, 0x57, 0x75, 0x36, 0x0e, 0x99, 0xe1,
        0x79, 0x2b, 0x2b, 0x96, 0x01, 0x10, 0xb8, 0xf6,
        0x4a, 0xa6, 0x13, 0xab, 0x7f, 0x55, 0x60, 0xf0,
        0xc9, 0x5c, 0x81, 0xa7, 0x96, 0x99, 0xb4, 0x55,
        0x41, 0x48, 0xf1, 0xd4, 0xa1, 0xb4, 0x76, 0xb5,
        0x35, 0xe1, 0x02, 0x8e, 0x09, 0xb2, 0x6c, 0x11,
        0x3f, 0xfb, 0x04, 0x47, 0x98, 0xab, 0x9b, 0x55,
        0xc3, 0xa9, 0x2a, 0x64, 0x32, 0x5a, 0x69, 0x96,
        0x28, 0x8c, 0x5b, 0xe3, 0xb2, 0x60, 0x82, 0xec
};

/*  256-bit */
static const uint8_t K6[] = {
        0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
};
static const uint8_t P6[] = {
        0x0b, 0xe5, 0x48, 0xa6, 0xa1, 0xbc, 0xac, 0x81,
        0x80, 0x06, 0x5f, 0xae, 0x1e, 0x3f, 0x55, 0x73,
        0x6d, 0x36, 0x7f, 0x57, 0x3d, 0xa4, 0x4a, 0x6b,
        0xb6, 0x65, 0x2f, 0xb7, 0xe8, 0x85, 0x47, 0xe2,
        0x41, 0x42, 0xc2, 0x4e, 0x58, 0xf1, 0xde, 0x42,
        0x9f, 0x15, 0x4c, 0xaf, 0xea, 0x04, 0x20, 0xd0,
        0x1a, 0x19, 0x36, 0x74, 0x71, 0x12, 0x72, 0x1b,
        0xdb, 0x18, 0xf9, 0x0b, 0xb3, 0xf3, 0x63, 0xd4,
        0x62, 0x52, 0x8b, 0x63, 0x0f, 0x6b, 0x4d, 0xb9,
        0x70, 0xd6, 0x91, 0xa0, 0x43, 0x3f, 0x46, 0xfe,
        0x43, 0xbb, 0xb8, 0xdc, 0x5e, 0xdb, 0xd4, 0x1f,
        0xf0, 0x17, 0x94, 0x25, 0xee, 0x55, 0x67, 0xbf,
        0x4d, 0xda, 0x9d, 0xe7, 0x4b, 0xc6, 0x7a, 0xcf,
        0x8f, 0xd7, 0xbb, 0x29, 0x6e, 0x26, 0xd4, 0xc3,
        0x08, 0x9b, 0x67, 0x15, 0xe9, 0x2d, 0x9f, 0x2d,
        0x3c, 0x76, 0x26, 0xd3, 0xda, 0xfe, 0x6e, 0x73,
        0x9d, 0x09, 0x60, 0x4b, 0x35, 0x60, 0xdb, 0x77,
        0xb6, 0xc0, 0x45, 0x91, 0xf9, 0x14, 0x8a, 0x7a,
        0xdd, 0xe2, 0xf1, 0xdf, 0x8f, 0x12, 0x4f, 0xd7,
        0x75, 0xd6, 0x9a, 0x17, 0xda, 0x76, 0x88, 0xf0,
        0xfa, 0x44, 0x27, 0xbe, 0x61, 0xaf, 0x55, 0x9f,
        0xc7, 0xf0, 0x76, 0x77, 0xde, 0xca, 0xd1, 0x47,
        0x51, 0x55, 0xb1, 0xbf, 0xfa, 0x1e, 0xca, 0x28,
        0x17, 0x70, 0xf3, 0xb5, 0xd4, 0x32, 0x47, 0x04,
        0xe0, 0x92, 0xd8, 0xa5, 0x03, 0x69, 0x46, 0x99,
        0x7f, 0x1e, 0x3f, 0xb2, 0x93, 0x36, 0xa3, 0x88,
        0x75, 0x07, 0x68, 0xb8, 0x33, 0xce, 0x17, 0x3f,
        0x5c, 0xb7, 0x1e, 0x93, 0x38, 0xc5, 0x1d, 0x79,
        0x86, 0x7c, 0x9d, 0x9e, 0x2f, 0x69, 0x38, 0x0f,
        0x97, 0x5c, 0x67, 0xbf, 0xa0, 0x8d, 0x37, 0x0b,
        0xd3, 0xb1, 0x04, 0x87, 0x1d, 0x74, 0xfe, 0x30,
        0xfb, 0xd0, 0x22, 0x92, 0xf9, 0xf3, 0x23, 0xc9
};
static const uint8_t C6[] = {
        0x4b, 0xc0, 0x1f, 0x80, 0xf5, 0xc7, 0xe8, 0xf5,
        0xc9, 0xd0, 0x3c, 0x86, 0x50, 0x78, 0x21, 0xce,
        0x01, 0xec, 0x91, 0x00, 0xc9, 0xf8, 0x73, 0x43,
        0x2f, 0x73, 0x8a, 0x6d, 0xee, 0xed, 0x2d, 0x40,
        0x17, 0x16, 0x93, 0x15, 0xac, 0xed, 0x28, 0x61,
        0xb0, 0x0f, 0xa2, 0xe1, 0xd3, 0x80, 0x51, 0xdf,
        0x73, 0xce, 0x48, 0x4c, 0x1c, 0xc1, 0x8b, 0xc9,
        0x9e, 0x5c, 0x48, 0x07, 0xa0, 0xf6, 0x29, 0xf8,
        0x63, 0x87, 0xe4, 0xe7, 0x8b, 0xf8, 0xcf, 0x58,
        0xda, 0x57, 0x62, 0x11, 0x2e, 0x6e, 0x91, 0x7e,
        0xc7, 0x73, 0xdb, 0x27, 0x3c, 0x64, 0x72, 0x52,
        0xe3, 0x27, 0x84, 0x1f, 0x73, 0x3f, 0xf4, 0x94,
        0xd2, 0xdd, 0x93, 0x33, 0x65, 0x91, 0x98, 0x89,
        0x13, 0xa9, 0x2b, 0x0d, 0x6f, 0x56, 0x51, 0x15,
        0x07, 0xc6, 0xa7, 0x36, 0x8f, 0x0c, 0xd6, 0xc2,
        0x07, 0x06, 0x65, 0x7a, 0xf8, 0x94, 0xa6, 0x75,
        0x48, 0x4c, 0xcc, 0xa5, 0xa9, 0x91, 0x04, 0x2f,
        0x7b, 0x89, 0x46, 0xd2, 0x87, 0xcb, 0xd6, 0x1b,
        0xf3, 0x1e, 0xa7, 0xe5, 0x09, 0xcf, 0x75, 0x05,
        0x9f, 0xc9, 0xac, 0xcc, 0x61, 0x15, 0x2d, 0x2e,
        0x2c, 0x0a, 0x57, 0x4d, 0x33, 0x17, 0x6b, 0x22,
        0x9e, 0x92, 0xc5, 0x81, 0xce, 0x9d, 0x52, 0x68,
        0x7d, 0x98, 0xe1, 0x23, 0x70, 0xc5, 0x19, 0x3e,
        0x91, 0xfc, 0xc6, 0xd7, 0x67, 0x5f, 0xbb, 0x57,
        0x20, 0x96, 0x3f, 0x1f, 0x9f, 0x64, 0xe9, 0xb1,
        0x51, 0xfd, 0x8c, 0xc1, 0x0f, 0x50, 0xbe, 0x43,
        0x5f, 0x90, 0xb4, 0xd1, 0xb6, 0x41, 0x7c, 0x37,
        0x92, 0x71, 0xda, 0x9d, 0xfd, 0xee, 0x69, 0x8c,
        0x24, 0x18, 0xe8, 0x81, 0x60, 0xe2, 0x89, 0x33,
        0x42, 0xd4, 0x1b, 0x6a, 0xcb, 0x4a, 0x5b, 0x00,
        0x01, 0x4f, 0x11, 0x47, 0x0f, 0x57, 0xb0, 0x90,
        0xf0, 0xed, 0xb0, 0x34, 0x2e, 0x9f, 0x81, 0x6c
};

/*  128-bit */
static const uint8_t K7[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
static const uint8_t P7[] = {
        0xdd, 0x14, 0xde, 0x30, 0xe0, 0xfd, 0x7b, 0x2a,
        0x94, 0x8e, 0x28, 0xa0, 0xf6, 0x93, 0x6e, 0xf5,
        0x92, 0x65, 0x1d, 0x5e, 0x78, 0x2a, 0x9d, 0x39,
        0xfc, 0xb8, 0x6d, 0x8b, 0xa5, 0xf4, 0x4b, 0x21,
        0xdd, 0x4e, 0xe9, 0xeb, 0xd7, 0xa7, 0xa1, 0x59,
        0xdc, 0x4c, 0x5e, 0xcc, 0x83, 0xab, 0xd3, 0x45,
        0xfe, 0x2c, 0x73, 0x23, 0xea, 0x45, 0xcb, 0x0c,
        0x12, 0x67, 0x28, 0xcd, 0xef, 0x4e, 0xca, 0xe2,
        0x1d, 0x92, 0x82, 0xd8, 0x0f, 0xa9, 0x36, 0x23,
        0x6d, 0x38, 0x68, 0xac, 0xa0, 0xeb, 0xdc, 0xcc,
        0xdf, 0xb8, 0x3a, 0x53, 0x04, 0x1a, 0x55, 0x27,
        0x8e, 0x22, 0x86, 0x8c, 0xbd, 0xdc, 0x6b, 0x12,
        0x9c, 0x69, 0xd2, 0x7a, 0x4b, 0x52, 0x5d, 0x76,
        0x34, 0xb9, 0x5e, 0x30, 0x0a, 0x8d, 0x1e, 0xf1,
        0x27, 0xda, 0x5b, 0xb9, 0x5e, 0xbf, 0x65, 0x34,
        0x00, 0xb6, 0xd2, 0xb0, 0x89, 0x12, 0xb6, 0x35,
        0xae, 0x27, 0x7f, 0x11, 0xe9, 0xf9, 0x1c, 0x71,
        0xc9, 0x50, 0xfe, 0xd4, 0x76, 0x50, 0x95, 0xf7,
        0xe1, 0x1c, 0x14, 0xcd, 0x67, 0x0f, 0xf0, 0x6d,
        0xa2, 0x93, 0x7b, 0x2c, 0x8d, 0x83, 0x5c, 0xff,
        0xe4, 0x95, 0xf3, 0xa1, 0xfd, 0x00, 0x77, 0x68,
        0x41, 0xb4, 0xfb, 0x81, 0xf4, 0x61, 0x1a, 0x84,
        0x5a, 0x53, 0xc3, 0xdc, 0xba, 0x0d, 0x67, 0x2e,
        0xcf, 0xf2, 0x30, 0xf5, 0x1d, 0xe9, 0xc4, 0x2c,
        0xac, 0x1f, 0xa7, 0x9c, 0x64, 0xfd, 0x45, 0x30,
        0x1b, 0xa1, 0x3b, 0x3d, 0xc7, 0xf5, 0xf9, 0xbb,
        0xba, 0x99, 0xa4, 0x12, 0x6e, 0x4e, 0xea, 0x0b,
        0x29, 0x7f, 0xcd, 0x84, 0x64, 0x50, 0x40, 0xb7,
        0x6a, 0x24, 0x29, 0xa4, 0xa7, 0xa1, 0xef, 0xa9,
        0xcf, 0xdf, 0x09, 0xff, 0xaa, 0x17, 0x5d, 0x82,
        0x74, 0xf5, 0xae, 0xd0, 0xe9, 0xec, 0xad, 0x5e,
        0xa7, 0x84, 0xda, 0xe7, 0x33, 0x58, 0x7e, 0x00,
        0x45, 0x5f, 0xbb, 0x15, 0xa3, 0x65, 0x0e, 0xf5,
        0x7e, 0x27, 0xe7, 0x04, 0x52, 0x58, 0x81, 0xd0,
        0xee, 0x8f, 0xaf, 0xe2, 0x3c, 0xbe, 0x08, 0x97,
        0x8a, 0x97, 0x12, 0xb0, 0x09, 0xfe, 0xa5, 0xeb,
        0xd1, 0x9c, 0x30, 0xe8, 0x9a, 0x3f, 0xe0, 0x38,
        0x34, 0x2b, 0xad, 0xb7, 0xc4, 0xda, 0x54, 0xab,
        0x97, 0x9c, 0x46, 0x2b, 0x2c, 0x0b, 0xb3, 0x49,
        0xcd, 0x9d, 0x32, 0x38, 0x3c, 0x1a, 0x49, 0xdc,
        0x2f, 0xe7, 0xcd, 0x8a, 0xb0, 0x76, 0xcf, 0x30,
        0xea, 0x0b, 0xb0, 0xb7, 0x63, 0xed, 0xb2, 0x8c,
        0xc9, 0x2c, 0xb7, 0x75, 0xa8, 0xf6, 0x63, 0xb6,
        0xcd, 0xb5, 0x63, 0xfb, 0x5f, 0x89, 0xae, 0x3d,
        0x33, 0x73, 0xaf, 0xde, 0xcb, 0x37, 0x0a, 0x50,
        0x6f, 0xae, 0xf3, 0xa6, 0x79, 0x85, 0xdd, 0xc5,
        0x24, 0xc5, 0x29, 0x23, 0x64, 0xef, 0x43, 0xd7,
        0xc4, 0xab, 0xd8, 0xb0, 0x84, 0x26, 0x6b, 0xe8,
        0xb1, 0x5d, 0xb5, 0x69, 0xfb, 0x97, 0x0e, 0x20,
        0xb3, 0xc1, 0x60, 0xad, 0x1a, 0xd2, 0xd6, 0x3a,
        0x73, 0x08, 0xf0, 0x47, 0x5f, 0xcf, 0x15, 0xf7,
        0x7b, 0xf3, 0x69, 0x08, 0x5a, 0x6b, 0x9f, 0xc7,
        0x12, 0xa1, 0xf0, 0xfb, 0x91, 0xc9, 0x07, 0x61,
        0x21, 0xa0, 0x30, 0x4c, 0x16, 0x81, 0xcd, 0x3c,
        0x61, 0xe8, 0x96, 0x91, 0x30, 0xdd, 0x0c, 0x0e,
        0x0b, 0xa1, 0x33, 0x95, 0x67, 0x99, 0xd6, 0x1e,
        0x1a, 0xb3, 0x12, 0xfd, 0xad, 0x46, 0x48, 0x87,
        0x5e, 0xe8, 0xd4, 0xf5, 0xac, 0xdf, 0xa7, 0x37,
        0xb8, 0xa1, 0x62, 0x8c, 0xb8, 0xb6, 0xb0, 0x69,
        0x63, 0x29, 0x60, 0x64, 0x26, 0xc3, 0xf8, 0x18,
        0x8e, 0x46, 0xa0, 0xc5, 0x45, 0x5c, 0x08, 0x2a,
        0xed, 0x29, 0x84, 0x11, 0xea, 0x59, 0xc0, 0x16,
        0xe2, 0x04, 0x30, 0x63, 0x22, 0x87, 0xb6, 0xc7,
        0x81, 0xa6, 0x58, 0xc0, 0xb2, 0xb0, 0x7d, 0xbc,
        0x16, 0x44, 0x6e, 0x5d, 0x6d, 0xce, 0x2a, 0xe0,
        0x20, 0x69, 0x35, 0xa1, 0x5d, 0x17, 0x48, 0x55,
        0x88, 0xfe, 0xde, 0x34, 0xe7, 0x18, 0xbf, 0x7e,
        0x0a, 0x1c, 0x32, 0x88, 0xab, 0xde, 0xe1, 0x02,
        0x61, 0x09, 0x58, 0x96, 0xef, 0x16, 0x73, 0xac,
        0xc0, 0x5c, 0x15, 0xca, 0x9b, 0xea, 0x0e, 0x05,
        0x97, 0x88, 0x09, 0xc5, 0xd0, 0x95, 0x90, 0xae,
        0xa5, 0xb5, 0x28, 0xc6, 0x5a, 0x7b, 0xb3, 0xcc,
        0xae, 0x57, 0x71, 0x83, 0x56, 0x57, 0xca, 0xe8,
        0x8b, 0x21, 0x0c, 0x37, 0x1d, 0xde, 0x85, 0xe2,
        0x1b, 0xa2, 0x38, 0xa0, 0xc5, 0xc7, 0x98, 0x7b,
        0xf9, 0x5e, 0x6a, 0x68, 0xb3, 0xed, 0x49, 0x5e,
        0x46, 0xb9, 0xc9, 0xf6, 0x34, 0xa6, 0x0e, 0xac,
        0x90, 0x72, 0xcf, 0xf8, 0x5b, 0x48, 0x13, 0x40,
        0x7a, 0xce, 0xfd, 0x3c, 0x16, 0xff, 0xb5, 0xea,
        0xb2, 0x56, 0x47, 0xcc, 0x9f, 0xbc, 0xae, 0x4a,
        0xc8, 0xa5, 0x59, 0x57, 0x01, 0xd7, 0x9f, 0xd7,
        0xbf, 0x13, 0xb1, 0xbf, 0xb7, 0x9a, 0xa0, 0xa1,
        0xc6, 0x66, 0x61, 0x96, 0xf2, 0xcd, 0x8c, 0xcb,
        0x3c, 0x67, 0xb5, 0xed, 0xb7, 0xa2, 0x54, 0x84,
        0x3c, 0xcb, 0x7e, 0xb3, 0x97, 0x05, 0xcb, 0x8f,
        0xa9, 0xc6, 0x3c, 0xa2, 0xbd, 0xbf, 0x3a, 0xb8,
        0x92, 0x08, 0x01, 0xea, 0xfd, 0x55, 0x2f, 0x27,
        0x2a, 0x82, 0x38, 0x26, 0x1d, 0x81, 0x19, 0x33,
        0x75, 0x3c, 0xa2, 0x13, 0x1e, 0x58, 0x9f, 0x0b,
        0x08, 0x5d, 0x7a, 0x2c, 0x9a, 0xd1, 0xa5, 0x4c,
        0x41, 0xb4, 0x1d, 0xf8, 0x42, 0x08, 0x87, 0xdd,
        0x8e, 0xc9, 0x05, 0xd2, 0x8c, 0xba, 0x93, 0x28,
        0xbe, 0x4a, 0x14, 0x13, 0x2a, 0x58, 0xf0, 0x1c,
        0xac, 0xc1, 0xc4, 0x49, 0xbc, 0xe1, 0xda, 0xb6,
        0x2d, 0x06, 0x98, 0x32, 0xea, 0xa3, 0x89, 0x11,
        0xca, 0x5f, 0x3e, 0xda, 0x24, 0xe2, 0xdb, 0x1e,
        0xca, 0xf3, 0xc0, 0xc7, 0x64, 0xee, 0x4b, 0x3d,
        0xa2, 0xee, 0x69, 0xb0, 0x3f, 0x2c, 0xd5, 0x49,
        0xba, 0x2d, 0x45, 0x7d, 0xdd, 0xb0, 0x0d, 0xc5,
        0xe0, 0x57, 0x95, 0xbe, 0xf8, 0x4a, 0x11, 0x46,
        0x4c, 0xbb, 0xdf, 0xa8, 0x5a, 0xf9, 0xff, 0x0e,
        0x31, 0xa9, 0x50, 0x5d, 0xc4, 0xb3, 0x3d, 0x09,
        0x46, 0x33, 0x39, 0x31, 0xd5, 0xb3, 0xe5, 0x91,
        0xcf, 0xca, 0x8a, 0xe0, 0xc2, 0x8e, 0xea, 0xbe,
        0x54, 0x64, 0x78, 0x0c, 0x25, 0x1c, 0x17, 0xbc,
        0x49, 0xf9, 0xc0, 0x30, 0x5f, 0x08, 0x04, 0x9d,
        0xb5, 0xe4, 0xeb, 0x9e, 0xe5, 0x1e, 0x6d, 0xbc,
        0x7b, 0xe7, 0xf0, 0xd1, 0xa0, 0x01, 0x18, 0x51,
        0x4f, 0x64, 0xc3, 0x9c, 0x70, 0x25, 0x4f, 0xed,
        0xc7, 0xbc, 0x19, 0x00, 0x09, 0x22, 0x97, 0x5d,
        0x6f, 0xe4, 0x47, 0x98, 0x05, 0xcd, 0xcc, 0xde,
        0xd5, 0xe3, 0xaf, 0xa3, 0xde, 0x69, 0x99, 0x2a,
        0xd1, 0x28, 0x4d, 0x7c, 0x89, 0xa0, 0xdb, 0xae,
        0xf9, 0xf1, 0x4a, 0x46, 0xdf, 0xbe, 0x1d, 0x37,
        0xf2, 0xd5, 0x36, 0x4a, 0x54, 0xe8, 0xc4, 0xfb,
        0x57, 0x77, 0x09, 0x05, 0x31, 0x99, 0xaf, 0x9a,
        0x17, 0xd1, 0x20, 0x93, 0x31, 0x89, 0xff, 0xed,
        0x0f, 0xf8, 0xed, 0xb3, 0xcf, 0x4c, 0x9a, 0x74,
        0xbb, 0x00, 0x36, 0x41, 0xd1, 0x13, 0x68, 0x73,
        0x78, 0x63, 0x42, 0xdd, 0x99, 0x15, 0x9a, 0xf4,
        0xe1, 0xad, 0x6d, 0xf6, 0x5e, 0xca, 0x20, 0x24,
        0xd7, 0x9d, 0x2f, 0x58, 0x97, 0xf7, 0xde, 0x31,
        0x51, 0xa3, 0x1c, 0xe2, 0x66, 0x24, 0x4b, 0xa1,
        0x56, 0x02, 0x32, 0xf4, 0x89, 0xf3, 0x86, 0x9a,
        0x85, 0xda, 0x95, 0xa8, 0x7f, 0x6a, 0x77, 0x02,
        0x3a, 0xba, 0xe0, 0xbe, 0x34, 0x5c, 0x9a, 0x1a
};
static const uint8_t C7[] = {
        0x62, 0xa1, 0xcc, 0x1e, 0x1b, 0xc3, 0xb1, 0x11,
        0xb5, 0x11, 0x4c, 0x37, 0xbf, 0xd0, 0x0c, 0xef,
        0x36, 0x9f, 0x99, 0x49, 0x38, 0xc2, 0x62, 0xbd,
        0x3e, 0x03, 0xd1, 0x02, 0xa2, 0x18, 0xdc, 0x58,
        0x9c, 0x01, 0x99, 0xd8, 0x47, 0xeb, 0x27, 0xce,
        0x76, 0x84, 0xa5, 0xab, 0xb7, 0x9b, 0xbb, 0x98,
        0xc9, 0x84, 0x02, 0x6e, 0x32, 0x65, 0xc9, 0xcb,
        0xca, 0xc7, 0xa5, 0x95, 0x11, 0xcc, 0x0a, 0x9d,
        0x5e, 0xea, 0xba, 0x59, 0xef, 0x25, 0xc0, 0x2d,
        0x8b, 0xa2, 0xec, 0x2f, 0x34, 0xea, 0x7c, 0xef,
        0xee, 0x2a, 0x57, 0x80, 0xc4, 0xca, 0x5e, 0x08,
        0x8c, 0x12, 0x13, 0x39, 0xd1, 0xc7, 0x96, 0x93,
        0x41, 0x22, 0x97, 0x1c, 0x7d, 0xe0, 0x47, 0xab,
        0xfa, 0xd7, 0xc6, 0x38, 0x5a, 0x39, 0xdb, 0x4c,
        0xd4, 0x6d, 0x50, 0x2b, 0x8f, 0xb1, 0x92, 0x06,
        0x01, 0xbf, 0xdc, 0x14, 0x5c, 0x32, 0xee, 0xb0,
        0x6a, 0x36, 0xe8, 0xe9, 0xf3, 0x12, 0x9f, 0x1f,
        0x00, 0xe5, 0x25, 0x3b, 0x52, 0x74, 0xba, 0x50,
        0x17, 0x81, 0x60, 0x5c, 0x15, 0xec, 0x4d, 0xb0,
        0x6a, 0xa1, 0xdd, 0xb4, 0xa2, 0x71, 0x01, 0xb8,
        0x8b, 0x59, 0x93, 0x58, 0x23, 0xd6, 0x38, 0xbf,
        0x49, 0x94, 0xb7, 0x6e, 0x22, 0x75, 0x68, 0x1f,
        0x15, 0x2c, 0xc4, 0x46, 0x44, 0x35, 0xc8, 0x7a,
        0x40, 0x2e, 0x55, 0x3f, 0x67, 0x4d, 0x12, 0x21,
        0xf6, 0xb1, 0x20, 0x47, 0x4f, 0x35, 0xe4, 0x96,
        0xf9, 0xa2, 0xdc, 0x4c, 0xe3, 0xa2, 0x13, 0x41,
        0xed, 0x6d, 0x86, 0x80, 0x23, 0xe5, 0x2a, 0xd1,
        0xa0, 0x69, 0x8f, 0x7e, 0x22, 0x3f, 0xf1, 0x65,
        0x9f, 0xd7, 0x86, 0xa8, 0x78, 0x57, 0x49, 0x74,
        0x91, 0x52, 0x91, 0xe7, 0x1e, 0xe2, 0x14, 0xe9,
        0x88, 0xe1, 0x67, 0x12, 0x3d, 0x0a, 0x22, 0x31,
        0x56, 0x2e, 0x36, 0xd4, 0x45, 0xc9, 0x9b, 0x7b,
        0x09, 0x53, 0x55, 0x36, 0xed, 0xa3, 0xc2, 0x22,
        0xac, 0x00, 0x5e, 0x57, 0xc8, 0x40, 0x65, 0xd2,
        0x62, 0x61, 0x35, 0xf2, 0xe8, 0x4f, 0xb3, 0x9d,
        0x2c, 0xb2, 0x12, 0x5e, 0x15, 0x47, 0xd6, 0x1c,
        0x99, 0x80, 0xe0, 0x1c, 0x09, 0x28, 0xa0, 0x7e,
        0x6c, 0x96, 0xc9, 0x62, 0x33, 0xd3, 0xbe, 0x53,
        0x16, 0xa0, 0xf2, 0xa9, 0x42, 0x1c, 0x81, 0xa3,
        0x35, 0x9b, 0x93, 0x9e, 0xc6, 0xc0, 0x83, 0x03,
        0xb7, 0x39, 0x66, 0xc9, 0x86, 0xf8, 0x8d, 0xc0,
        0xe2, 0x88, 0xb4, 0x1f, 0x5d, 0x15, 0x80, 0x60,
        0x2d, 0x53, 0x1d, 0x60, 0x07, 0xbc, 0x72, 0x11,
        0xd0, 0x0e, 0xcb, 0x70, 0x9c, 0xa0, 0x48, 0x56,
        0x21, 0x5f, 0x18, 0xdd, 0xa3, 0x1d, 0xdb, 0xe0,
        0x41, 0x0c, 0x9e, 0xb9, 0xa2, 0x7e, 0x32, 0xb3,
        0x3e, 0x91, 0x9d, 0xf2, 0xa6, 0x0d, 0x8c, 0xea,
        0xae, 0x44, 0xb2, 0x0f, 0x11, 0x35, 0x27, 0x2e,
        0xb6, 0x3d, 0xe9, 0x63, 0x86, 0x2e, 0x81, 0xdc,
        0xfa, 0xb4, 0x52, 0x1d, 0x9c, 0xd5, 0x44, 0x95,
        0xc8, 0xd0, 0x66, 0x8a, 0xbd, 0xf6, 0xd1, 0xff,
        0xeb, 0x82, 0x68, 0x58, 0x7b, 0xec, 0x0e, 0x92,
        0x0e, 0x48, 0xd6, 0xff, 0x8d, 0xac, 0xc1, 0x41,
        0x84, 0x9e, 0x56, 0x54, 0xf9, 0xb5, 0x1c, 0xb0,
        0x9f, 0xde, 0xfe, 0x14, 0x42, 0x0d, 0x22, 0x12,
        0xf2, 0x7d, 0x7b, 0xc3, 0x2e, 0x72, 0x27, 0x76,
        0x12, 0xdf, 0x57, 0x2f, 0x97, 0x82, 0x9b, 0xcf,
        0x75, 0x1a, 0x4a, 0x0c, 0xad, 0x29, 0x56, 0x4c,
        0x74, 0xaf, 0x95, 0x03, 0xff, 0x9f, 0x9d, 0xc3,
        0x2e, 0x9c, 0x1a, 0x42, 0x75, 0xe1, 0x59, 0xc9,
        0x05, 0x12, 0x6c, 0xea, 0x2b, 0x2f, 0x89, 0xfc,
        0xa4, 0x73, 0xc8, 0xdc, 0xf6, 0xd5, 0x50, 0x19,
        0x22, 0x80, 0xbc, 0x08, 0x48, 0xb4, 0x45, 0x47,
        0x25, 0x01, 0xa9, 0x55, 0x7b, 0x66, 0xbd, 0x84,
        0x0f, 0x16, 0xfa, 0x44, 0x23, 0x51, 0x6f, 0xed,
        0x35, 0x0e, 0x88, 0x4d, 0xda, 0xe8, 0x27, 0x94,
        0xbd, 0x68, 0x46, 0x28, 0x79, 0x8c, 0x03, 0x03,
        0xf0, 0x81, 0xac, 0xbc, 0xc2, 0xdd, 0xa8, 0x98,
        0xdf, 0xe3, 0x1c, 0x1c, 0x4b, 0x43, 0x9e, 0x7b,
        0x26, 0x3c, 0xe9, 0xff, 0x3b, 0xee, 0x35, 0xe6,
        0x2a, 0xcf, 0xdc, 0x17, 0x85, 0x99, 0x9e, 0x88,
        0x5c, 0x38, 0x4c, 0x56, 0x4a, 0x06, 0xeb, 0x28,
        0xf7, 0xb5, 0x97, 0x04, 0xd4, 0x05, 0x85, 0xee,
        0x90, 0xd7, 0xe2, 0x10, 0x8a, 0x86, 0xb2, 0x3f,
        0xbf, 0x3f, 0x6a, 0xe6, 0xeb, 0xc1, 0x42, 0x97,
        0xcb, 0x30, 0x41, 0x44, 0x79, 0x44, 0x7e, 0x1e,
        0x3e, 0x55, 0xe5, 0xc8, 0xd5, 0xec, 0x64, 0x3d,
        0x09, 0x69, 0xea, 0xdb, 0xe5, 0x08, 0x33, 0x00,
        0x79, 0x1b, 0x31, 0xf2, 0x3d, 0xbd, 0x73, 0xe6,
        0x0e, 0xc1, 0xb9, 0x45, 0xbf, 0xa5, 0x52, 0x5a,
        0xcd, 0x71, 0x7a, 0x2e, 0x20, 0x1e, 0xbf, 0xff,
        0x42, 0x0a, 0x6a, 0x1b, 0xa4, 0xad, 0x79, 0x3d,
        0x34, 0x54, 0x73, 0xe2, 0xd6, 0x6f, 0xb0, 0xcc,
        0xc0, 0x8a, 0x56, 0x3d, 0x4d, 0x90, 0x35, 0xe3,
        0x4b, 0xcc, 0x40, 0x40, 0xbc, 0xcf, 0x93, 0xa0,
        0xbd, 0x5c, 0xed, 0x22, 0x57, 0x92, 0x5c, 0x8d,
        0xfb, 0x67, 0x9e, 0xab, 0x40, 0xc9, 0xed, 0x7c,
        0xa1, 0xb6, 0x36, 0xb2, 0xcb, 0xbc, 0xf2, 0x1a,
        0x46, 0x6c, 0x1f, 0xb3, 0xe4, 0xf6, 0x4c, 0x7a,
        0x10, 0x81, 0x16, 0x93, 0x77, 0xa3, 0xa1, 0x07,
        0xec, 0xc8, 0x01, 0x76, 0xf8, 0xe3, 0xe6, 0xae,
        0xaf, 0x90, 0x98, 0x3a, 0xbd, 0x7d, 0x28, 0x57,
        0xb4, 0xc5, 0xfe, 0x13, 0xab, 0x6c, 0x77, 0xc1,
        0xc3, 0x47, 0x1d, 0x34, 0x2f, 0xdd, 0xe1, 0x7b,
        0x8b, 0x65, 0xc4, 0xe3, 0x45, 0xda, 0x6e, 0xba,
        0x37, 0xb1, 0x37, 0xbf, 0x63, 0x1d, 0x39, 0x77,
        0xf0, 0xa8, 0xf8, 0xda, 0x91, 0xd3, 0x27, 0xb9,
        0x29, 0x70, 0xf7, 0xae, 0x11, 0x6d, 0x8a, 0x8f,
        0x2f, 0x3a, 0xe1, 0xb8, 0x9b, 0xb5, 0x2a, 0xa8,
        0x7b, 0x86, 0x49, 0xca, 0x0c, 0x95, 0x17, 0x1e,
        0xaf, 0x9c, 0x52, 0x6b, 0x68, 0xae, 0xe3, 0xc3,
        0xc9, 0x8c, 0x89, 0x4b, 0xf2, 0xfb, 0xb1, 0xae,
        0x2f, 0x80, 0xf9, 0xa3, 0xf4, 0x10, 0x09, 0x36,
        0x81, 0x27, 0x06, 0x6d, 0xe9, 0x79, 0x8e, 0xa4,
        0x8e, 0x12, 0xfa, 0x03, 0x8e, 0x69, 0x4c, 0x7e,
        0xc5, 0x10, 0xd5, 0x00, 0x64, 0x87, 0xf8, 0x10,
        0x8a, 0x8e, 0x96, 0x9e, 0xc8, 0xac, 0x42, 0x75,
        0x97, 0x6d, 0x62, 0x3f, 0xa3, 0x29, 0x11, 0xd2,
        0x73, 0xd3, 0x95, 0xef, 0xb4, 0x64, 0xa4, 0x37,
        0x09, 0x15, 0x42, 0x7f, 0xc4, 0x46, 0x8b, 0x80,
        0xa8, 0xd9, 0x2a, 0xfc, 0x38, 0x8f, 0xf9, 0xc1,
        0xc5, 0x95, 0xad, 0x62, 0xc9, 0x6c, 0x60, 0x0b,
        0x30, 0x04, 0x8c, 0x88, 0xb5, 0x0b, 0x73, 0x23,
        0xa4, 0xe0, 0xb7, 0x6e, 0x4c, 0x78, 0xe5, 0x0a,
        0xfb, 0xe1, 0xc4, 0xeb, 0x1a, 0xb4, 0xd8, 0x3c,
        0x06, 0xb0, 0x00, 0x23, 0x86, 0xb0, 0xb4, 0x9d,
        0x33, 0xe4, 0x21, 0xca, 0xf2, 0xad, 0x14, 0x07,
        0x82, 0x25, 0xde, 0x85, 0xe4, 0x58, 0x56, 0x93,
        0x09, 0x3a, 0xeb, 0xde, 0x46, 0x77, 0x76, 0xa2,
        0x35, 0x39, 0xd0, 0xf6, 0x10, 0x81, 0x73, 0x3f,
        0x22, 0x3b, 0xeb, 0xca, 0x00, 0x19, 0x38, 0x89,
        0x26, 0x29, 0x7d, 0x6f, 0x70, 0xa6, 0xbb, 0x52,
        0x58, 0xb1, 0x0a, 0x85, 0xe9, 0x0b, 0x74, 0x2f,
        0x08, 0xe8, 0xa4, 0x4d, 0xa1, 0xcf, 0xf2, 0x75,
        0xed, 0x05, 0xae, 0x7f, 0x10, 0xb1, 0x71, 0x26,
        0xc5, 0xc7, 0xdc, 0xb0, 0x2d, 0x26, 0xf1, 0xb4
};

static const struct ecb_vector ecb_vectors[] = {
        {K1, P1, sizeof(P1), C1, sizeof(K1)},
        {K2, P2, sizeof(P2), C2, sizeof(K2)},
        {K3, P3, sizeof(P3), C3, sizeof(K3)},
        {K4, P4, sizeof(P4), C4, sizeof(K4)},
        {K5, P5, sizeof(P5), C5, sizeof(K5)},
        {K6, P6, sizeof(P6), C6, sizeof(K6)},
        {K7, P7, sizeof(P7), C7, sizeof(K7)},
};

static int
ecb_job_ok(const struct JOB_AES_HMAC *job,
           const uint8_t *out_text,
           const uint8_t *target,
           const uint8_t *padding,
           const size_t sizeof_padding,
           const unsigned text_len)
{
        const int num = (const int)((uint64_t)job->user_data2);

        if (job->status != STS_COMPLETED) {
                printf("%d error status:%d, job %d",
                       __LINE__, job->status, num);
                return 0;
        }
        if (memcmp(out_text, target + sizeof_padding,
                   text_len)) {
                printf("%d mismatched\n", num);
                return 0;
        }
        if (memcmp(padding, target, sizeof_padding)) {
                printf("%d overwrite head\n", num);
                return 0;
        }
        if (memcmp(padding,
                   target + sizeof_padding + text_len,
                   sizeof_padding)) {
                printf("%d overwrite tail\n", num);
                return 0;
        }
        return 1;
}

static int
test_ecb_many(struct IMB_MGR *mb_mgr,
              void *enc_keys,
              void *dec_keys,
              const uint8_t *in_text,
              const uint8_t *out_text,
              unsigned text_len,
              int dir,
              int order,
              JOB_CIPHER_MODE cipher,
              const int in_place,
              const int key_len,
              const int num_jobs)
{
        struct JOB_AES_HMAC *job;
        uint8_t padding[16];
        uint8_t **targets = malloc(num_jobs * sizeof(void *));
        int i, jobs_rx = 0, ret = -1;

        assert(targets != NULL);

        memset(padding, -1, sizeof(padding));

        for (i = 0; i < num_jobs; i++) {
                targets[i] = malloc(text_len + (sizeof(padding) * 2));
                memset(targets[i], -1, text_len + (sizeof(padding) * 2));
                if (in_place) {
                        /* copy input text to the allocated buffer */
                        memcpy(targets[i] + sizeof(padding), in_text, text_len);
                }
        }

        /* flush the scheduler */
        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL)
                ;

        for (i = 0; i < num_jobs; i++) {
                job = IMB_GET_NEXT_JOB(mb_mgr);
                job->cipher_direction = dir;
                job->chain_order = order;
                if (!in_place) {
                        job->dst = targets[i] + sizeof(padding);
                        job->src = in_text;
                } else {
                        job->dst = targets[i] + sizeof(padding);
                        job->src = targets[i] + sizeof(padding);
                }
                job->cipher_mode = cipher;
                job->aes_enc_key_expanded = enc_keys;
                job->aes_dec_key_expanded = dec_keys;
                job->aes_key_len_in_bytes = key_len;

                job->iv_len_in_bytes = 0;
                job->cipher_start_src_offset_in_bytes = 0;
                job->msg_len_to_cipher_in_bytes = text_len;
                job->user_data = targets[i];
                job->user_data2 = (void *)((uint64_t)i);

                job->hash_alg = IMB_AUTH_NULL;

                job = IMB_SUBMIT_JOB(mb_mgr);
                if (job != NULL) {
                        jobs_rx++;
                        if (!ecb_job_ok(job, out_text, job->user_data, padding,
                                       sizeof(padding), text_len))
                                goto end;
                }
        }

        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL) {
                jobs_rx++;
                if (!ecb_job_ok(job, out_text, job->user_data, padding,
                               sizeof(padding), text_len))
                        goto end;
        }

        if (jobs_rx != num_jobs) {
                printf("Expected %d jobs, received %d\n", num_jobs, jobs_rx);
                goto end;
        }
        ret = 0;

 end:
        while ((job = IMB_FLUSH_JOB(mb_mgr)) != NULL)
                ;

        for (i = 0; i < num_jobs; i++)
                free(targets[i]);
        free(targets);
        return ret;
}

static int
test_ecb_vectors(struct IMB_MGR *mb_mgr, const int vec_cnt,
                 const struct ecb_vector *vec_tab, const char *banner,
                 const JOB_CIPHER_MODE cipher, const int num_jobs)
{
	int vect, errors = 0;
        DECLARE_ALIGNED(uint32_t enc_keys[15*4], 16);
        DECLARE_ALIGNED(uint32_t dec_keys[15*4], 16);

	printf("%s (N jobs = %d):\n", banner, num_jobs);
	for (vect = 0; vect < vec_cnt; vect++) {
#ifdef DEBUG
		printf("[%d/%d] Standard vector key_len:%d\n",
                       vect + 1, vec_cnt,
                       (int) vec_tab[vect].Klen);
#else
		printf(".");
#endif
                switch (vec_tab[vect].Klen) {
                case 16:
                        IMB_AES_KEYEXP_128(mb_mgr, vec_tab[vect].K, enc_keys,
                                           dec_keys);
                        break;
                case 24:
                        IMB_AES_KEYEXP_192(mb_mgr, vec_tab[vect].K, enc_keys,
                                           dec_keys);
                        break;
                case 32:
                default:
                        IMB_AES_KEYEXP_256(mb_mgr, vec_tab[vect].K, enc_keys,
                                           dec_keys);
                        break;
                }

                if (test_ecb_many(mb_mgr, enc_keys, dec_keys,
                             vec_tab[vect].P, vec_tab[vect].C,
                             (unsigned) vec_tab[vect].Plen,
                             IMB_DIR_ENCRYPT, IMB_ORDER_CIPHER_HASH, cipher, 0,
                             vec_tab[vect].Klen, num_jobs)) {
                        printf("error #%d encrypt\n", vect + 1);
                        errors++;
                }

                if (test_ecb_many(mb_mgr, enc_keys, dec_keys,
                             vec_tab[vect].C, vec_tab[vect].P,
                             (unsigned) vec_tab[vect].Plen,
                             IMB_DIR_DECRYPT, IMB_ORDER_HASH_CIPHER, cipher, 0,
                             vec_tab[vect].Klen, num_jobs)) {
                        printf("error #%d decrypt\n", vect + 1);
                        errors++;
                }

                if (test_ecb_many(mb_mgr, enc_keys, dec_keys,
                             vec_tab[vect].P, vec_tab[vect].C,
                             (unsigned) vec_tab[vect].Plen,
                             IMB_DIR_ENCRYPT, IMB_ORDER_CIPHER_HASH, cipher, 1,
                             vec_tab[vect].Klen, num_jobs)) {
                        printf("error #%d encrypt in-place\n", vect + 1);
                        errors++;
                }

                if (test_ecb_many(mb_mgr, enc_keys, dec_keys,
                             vec_tab[vect].C, vec_tab[vect].P,
                             (unsigned) vec_tab[vect].Plen,
                             IMB_DIR_DECRYPT, IMB_ORDER_HASH_CIPHER, cipher, 1,
                             vec_tab[vect].Klen, num_jobs)) {
                        printf("error #%d decrypt in-place\n", vect + 1);
                        errors++;
                }
	}
	printf("\n");
	return errors;
}

int
ecb_test(const enum arch_type arch,
         struct IMB_MGR *mb_mgr)
{
        const int num_jobs_tab[] = {
                1, 3, 4, 5, 7, 8, 9, 15, 16, 17
        };
        unsigned i;
        int errors = 0;

        (void) arch; /* unused */

        for (i = 0; i < DIM(num_jobs_tab); i++)
                errors += test_ecb_vectors(mb_mgr, DIM(ecb_vectors),
                                           ecb_vectors,
                                           "AES-ECB standard test vectors",
                                           IMB_CIPHER_ECB,
                                           num_jobs_tab[i]);
	if (0 == errors)
		printf("...Pass\n");
	else
		printf("...Fail\n");

	return errors;
}
