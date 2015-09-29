#include "divmod.h"

#define DEND "r16"
#define DSOR "r17"
#define QUOT "r18"
#define SHFT "r19"
#define DEND_SHFT "r20"
#define DSOR_SHFT "r21"


void divmod(uint8_t dend, uint8_t dsor, uint8_t *quot, uint8_t *rem)
{
	__asm__ __volatile__(
		"\n"
		/*
		 * All input registers are read-only so we need to move them to them
		 * into temporary ones.
		 */
		"\tmov "DEND", %[dend]\n"
		"\tmov "DSOR", %[dsor]\n"
		"\tclr "QUOT"\n"
		"\tclr "SHFT"\n"
		/*
		 * Now is the most boring part of the algorithm: nubmers has to be
		 * aligned. In order to do this we compute number of leading zeros
		 * in dividend and number of leading zeros in divisor. Difference
		 * between these two values is number of shits that has to be applied
		 * to the divisor to align numbers.
		 */
	"calc_dend_shift:\n"
		"\tclr "DEND_SHFT"\n"
		"\tmov __tmp_reg__, "DEND"\n"
	"calc_dend_shift_loop:\n"
		"\tsbrc __tmp_reg__, 7\n"
		"\trjmp calc_dsor_shift\n"
		"\tinc "DEND_SHFT"\n"
		"\tlsl __tmp_reg__\n"
		"\trjmp calc_dend_shift_loop\n"
	"calc_dsor_shift:\n"
		"\tclr "DSOR_SHFT"\n"
		"\tmov __tmp_reg__, "DSOR"\n"
	"calc_dsor_shift_loop:\n"
		"\tsbrc __tmp_reg__, 7\n"
		"\trjmp align\n"
		"\tinc "DSOR_SHFT"\n"
		"\tlsl __tmp_reg__\n"
		"\trjmp calc_dsor_shift_loop\n"
	"align:\n"
		"\tmov "SHFT", "DSOR_SHFT"\n"
		"\tsub "SHFT", "DEND_SHFT"\n"
		"\tmov __tmp_reg__, "SHFT"\n"
	"align_loop:\n"
		"\tcpi "SHFT", 0\n"
		"\tbreq divide\n"
		"\tlsl "DSOR"\n"
		"\tdec "SHFT"\n"
		"\trjmp align_loop\n"
		/*
		 * Now, having the numbers correctly aligned algorithm is relatively
		 * short. As long as the divisor has not been shiften again to its
		 * starting position we check whether we can subtract it from the
		 * divident or not. If we can then 1 is concatenated to the result,
		 * otherwise we concatenate it with 0.
		 */
	"divide:\n"
		"\tmov "SHFT", __tmp_reg__\n"
		"\tinc "SHFT"\n"
	"divide_loop:\n"
		"\tcpi "SHFT", 0\n"
		"\tbreq finish\n"
		"\tlsl "QUOT"\n"
		"\tcp "DEND", "DSOR"\n"
		"\tbrlo shift\n"
		"\tinc "QUOT"\n"
		"\tsub "DEND", "DSOR"\n"
	"shift:\n"
		"\tlsr "DSOR"\n"
		"\tdec "SHFT"\n"
		"\trjmp divide_loop\n"
		/*
		 * Once the division loop stops, we can just read the outcome. Value
		 * of the remainder can be simply copied form the dividend that left.
		 */
	"finish:\n"
		"\tmov %[quot], "QUOT"\n"
		"\tmov %[rem], "DEND"\n"
		: [quot] "=&r" (*quot), [rem] "=&r" (*rem)
		: [dend] "r" (dend), [dsor] "r" (dsor)
		: DEND, DSOR, QUOT, SHFT, DEND_SHFT, DSOR_SHFT
	);
}


#define DENDL "r16"
#define DENDH "r17"
#define DSORL "r18"
#define DSORH "r19"
#define SHFT "r20"
#define DEND_SHFT "r21"
#define DSOR_SHFT "r22"
#define TMPL "r24"
#define TMPH "r25"

void divmod16(uint16_t dend, uint16_t dsor, uint16_t *quot, uint16_t *rem)
{
	__asm__ __volatile__(
		"\n"
		"\tmov "DENDL", %A[dend]\n"
		"\tmov "DENDH", %B[dend]\n"
		"\tmov "DSORL", %A[dsor]\n"
		"\tmov "DSORH", %B[dsor]\n"
	"calc_dend_shift_%=:\n"
		"\tclr "DEND_SHFT"\n"
		"\tmovw "TMPL","DENDL"\n"
	"calc_dend_shift_loop_%=:\n"
		"\tsbrc "TMPH", 7\n"
		"\trjmp calc_dsor_shift_%=\n"
		"\tinc "DEND_SHFT"\n"
		"\tlsl "TMPL"\n"
		"\trol "TMPH"\n"
		"\trjmp calc_dend_shift_loop_%=\n"
	"calc_dsor_shift_%=:\n"
		"\tclr "DSOR_SHFT"\n"
		"\tmovw "TMPL", "DSORL"\n"
	"calc_dsor_shift_loop_%=:\n"
		"\tsbrc "TMPH", 7\n"
		"\trjmp align_%=\n"
		"\tinc "DSOR_SHFT"\n"
		"\tlsl "TMPL"\n"
		"\trol "TMPH"\n"
		"\trjmp calc_dsor_shift_loop_%=\n"
	"align_%=:\n"
		"\tmov "SHFT", "DSOR_SHFT"\n"
		"\tsub "SHFT", "DEND_SHFT"\n"
		"\tmov __tmp_reg__, "SHFT"\n"
	"align_loop_%=:\n"
		"\tcpi "SHFT", 0\n"
		"\tbreq divide_%=\n"
		"\tlsl "DSORL"\n"
		"\trol "DSORH"\n"
		"\tdec "SHFT"\n"
		"\trjmp align_loop_%=\n"
	"divide_%=:\n"
		"\tmov "SHFT", __tmp_reg__\n"
		"\tinc "SHFT"\n"
		"\tclr %A[quot]\n"
		"\tclr %B[quot]\n"
	"divide_loop_%=:\n"
		"\tcpi "SHFT", 0\n"
		"\tbreq finish_%=\n"
		"\tlsl %A[quot]\n"
		"\trol %B[quot]\n"
		"\tcp "DENDH", "DSORH"\n"
		"\tbrlo shift_%=\n"
		"\tbrne add_one_%=\n"
		"\tcp "DENDL", "DSORL"\n"
		"\tbrlo shift_%=\n"
	"add_one_%=:\n"
		"\tinc %A[quot]\n"
		"\tsub "DENDL", "DSORL"\n"
		"\tsbc "DENDH", "DSORH"\n"
	"shift_%=:\n"
		"\tlsr "DSORH"\n"
		"\tror "DSORL"\n"
		"\tdec "SHFT"\n"
		"\trjmp divide_loop_%=\n"
	"finish_%=:\n"
		"\tmov %A[rem], "DENDL"\n"
		"\tmov %B[rem], "DENDH"\n"
		: [quot] "=&r" (*quot), [rem] "=&r" (*rem)
		: [dend] "r" (dend), [dsor] "r" (dsor)
		: DENDL, DENDH, DSORL, DSORH, SHFT, DEND_SHFT, DSOR_SHFT
	);
}
