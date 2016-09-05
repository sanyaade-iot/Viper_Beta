#include "board_mcu.h"
#include "viperlib.h"

#define SYM(x) (void*)x


#ifdef PLATFORM_ARMCMx

//See "Runtime ABI for ARM Architecture"

//Arithmetic
extern double __aeabi_dadd(double, double);
extern double __aeabi_ddiv(double, double);
extern double __aeabi_dmul(double, double);
extern double __aeabi_drsub(double, double);
extern double __aeabi_dsub(double, double);

extern float __aeabi_fadd(float, float);
extern float __aeabi_fdiv(float, float);
extern float __aeabi_fmul(float, float);
extern float __aeabi_frsub(float, float);
extern float __aeabi_fsub(float, float);

//comparison
extern void __aeabi_cdcmpeq(double, double);
extern void __aeabi_cdcmple(double, double);
extern void __aeabi_cdcmprle(double, double);
extern int __aeabi_dcmpeq(double, double);
extern int __aeabi_dcmple(double, double);
extern int __aeabi_dcmplt(double, double);
extern int __aeabi_dcmpge(double, double);
extern int __aeabi_dcmpgt(double, double);
extern int __aeabi_dcmpun(double, double);

extern void __aeabi_cfcmpeq(float, float);
extern void __aeabi_cfcmple(float, float);
extern void __aeabi_cfcmprle(float, float);
extern int __aeabi_fcmpeq(float, float);
extern int __aeabi_fcmple(float, float);
extern int __aeabi_fcmplt(float, float);
extern int __aeabi_fcmpge(float, float);
extern int __aeabi_fcmpgt(float, float);
extern int __aeabi_fcmpun(float, float);


//conversion to integer
extern int __aeabi_d2iz(double);
extern unsigned int __aeabi_d2uiz(double);
extern long long __aeabi_d2lz(double);
extern unsigned long long __aeabi_d2ulz(double);

extern int __aeabi_f2iz(float);
extern unsigned int __aeabi_f2uiz(float);
extern long long __aeabi_f2lz(float);
extern unsigned long long __aeabi_f2ulz(float);

//conversion between floating point

extern float __aeabi_d2f(double);
extern double __aeabi_f2d(float);
extern float __aeabi_h2f(short hf);
extern float __aeabi_h2f_alt(short hf);
extern short __aeabi_f2h(float f);
extern short __aeabi_f2h_alt(float f);
extern short __aeabi_d2h(double);
extern short __aeabi_d2h_alt(double);

//conversion from integer
extern double __aeabi_i2d(int);
extern double __aeabi_ui2d(unsigned);
extern double __aeabi_l2d(long long);
extern double __aeabi_ul2d(unsigned long long);
extern float __aeabi_i2f(int);
extern float __aeabi_ui2f(unsigned);
extern float __aeabi_l2f(long long);
extern float __aeabi_ul2f(unsigned long long);


//ABI TABLE

const void *const _abi_fns[] STORED = {
	SYM(__aeabi_dadd),
	SYM(__aeabi_ddiv),
	SYM(__aeabi_dmul),
	SYM(__aeabi_drsub),
	SYM(__aeabi_dsub),
	SYM(__aeabi_cdcmpeq),
	SYM(__aeabi_cdcmple),
	SYM(__aeabi_cdcmprle),
	SYM(__aeabi_dcmpeq),
	SYM(__aeabi_dcmplt),
	SYM(__aeabi_dcmple),
	SYM(__aeabi_dcmpge),
	SYM(__aeabi_dcmpgt),
	SYM(__aeabi_dcmpun),
	SYM(__aeabi_fadd),
	SYM(__aeabi_fdiv),
	SYM(__aeabi_fmul),
	SYM(__aeabi_frsub),
	SYM(__aeabi_fsub),
	SYM(__aeabi_cfcmpeq),
	SYM(__aeabi_cfcmple),
	SYM(__aeabi_cfcmprle),
	SYM(__aeabi_fcmpeq),
	SYM(__aeabi_fcmplt),
	SYM(__aeabi_fcmple),
	SYM(__aeabi_fcmpge),
	SYM(__aeabi_fcmpgt),
	SYM(__aeabi_fcmpun),
	SYM(__aeabi_d2iz),
	SYM(__aeabi_d2uiz),
	SYM(__aeabi_d2lz),
	SYM(__aeabi_d2ulz),
	SYM(__aeabi_f2iz),
	SYM(__aeabi_f2uiz),
	SYM(__aeabi_f2lz),
	SYM(__aeabi_f2ulz),
	SYM(__aeabi_d2f),
	SYM(__aeabi_f2d),
	SYM(__aeabi_h2f),
	SYM(__aeabi_h2f_alt),
	SYM(__aeabi_f2h),
	SYM(__aeabi_f2h_alt),
	SYM(__aeabi_d2h),
	SYM(__aeabi_d2h_alt),
	SYM(__aeabi_i2d),
	SYM(__aeabi_ui2d),
	SYM(__aeabi_l2d),
	SYM(__aeabi_ul2d),
	SYM(__aeabi_i2f),
	SYM(__aeabi_ui2f),
	SYM(__aeabi_l2f),
	SYM(__aeabi_ul2f)
};

const uint16_t _abi_fn_num = sizeof(_abi_fns) / sizeof(void *);

#else

#error Unsupported platform! (check board_mcu.h for PLATFORM_xxx)

#endif