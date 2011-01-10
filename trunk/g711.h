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

