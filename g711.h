/**
 * Swift Text-To-Speech Extension for PHP5      
 *
 * Copyright (c) 2011, Mike Pultz <mike@mikepultz.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *   * Neither the name of Mike Pultz nor the names of his contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRIC
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author      Mike Pultz <mike@mikepultz.com>
 * @copyright   2011 Mike Pultz <mike@mikepultz.com>
 * @license     http://www.opensource.org/licenses/bsd-license.php  BSD License
 * @version     SVN $id$
 *
 */

#ifndef __CG711_H__
#define __CG711_H__

#include <stdint.h>

class cg711
{
	public:
		static void ulaw_decode(const uint8_t *_in, int16_t *_out, const int _size);
		static void ulaw_decode(const uint8_t *_in, float *_out, const int _size);
		static void ulaw_encode(const int16_t *_in, uint8_t *_out, const int _size);
		static void ulaw_encode(const float *_in, uint8_t *_out, const int _size);

		static void alaw_decode(const uint8_t *_in, int16_t *_out, const int _size);
		static void alaw_decode(const uint8_t *_in, float *_out, const int _size);
		static void alaw_encode(const int16_t *_in, uint8_t *_out, const int _size);
		static void alaw_encode(const float *_in, uint8_t *_out, const int _size);

		static void alaw_to_ulaw(const uint8_t *_in, uint8_t *_out, const int _size);
		static void ulaw_to_alaw(const uint8_t *_in, uint8_t *_out, const int _size);
};

extern const uint8_t linear_to_ulaw_table[16384];
extern const uint8_t linear_to_alaw_table[16384];
extern const int16_t ulaw_to_linear_table[256];
extern const int16_t alaw_to_linear_table[256];

#define __u_round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

/**
 * Convert 16-bit linear PCM value to 8-bit A-Law.
 *
 * @param pcm_val   16-bit linear PCM value.
 * @return	    8-bit A-Law value.
 */
#define __linear_to_alaw(pcm_val)	\
	linear_to_alaw_table[(((int16_t)pcm_val) >> 2) & 0x3fff]

/**
 * Convert 8-bit A-Law value to 16-bit linear PCM value.
 *
 * @param chara_val 8-bit A-Law value.
 * @return	    16-bit linear PCM value.
 */
#define __alaw_to_linear(chara_val) \
	alaw_to_linear_table[chara_val]

/**
 * Convert 16-bit linear PCM value to 8-bit U-Law.
 *
 * @param pcm_val   16-bit linear PCM value.
 * @return	    8-bit U-Law value.
 */
#define __linear_to_ulaw(pcm_val)	\
	linear_to_ulaw_table[(((int16_t)pcm_val) >> 2) & 0x3fff]

/**
 * Convert 8-bit U-Law value to 16-bit linear PCM value.
 *
 * @param u_val	    8-bit U-Law value.
 * @return	    16-bit linear PCM value.
 */
#define __ulaw_to_linear(u_val) \
	ulaw_to_linear_table[u_val]

#endif // __CULAW_H__

