#include <rex/ppc/context.h>
#include <rex/ppc/function.h>
#include <rex/cvar.h>
#include "generated/gh2test_init.h"

REXCVAR_DEFINE_DOUBLE(gh2_audiooffset, 0.0, "Guitar Hero 2", "Offset song audio to sync with notes");

void GuitarHook(PPCRegister& r11) {
    r11.u64 = 7;
}

void AudioOffsetHook(PPCRegister& f1) {
	double offset = static_cast<double>(REXCVAR_GET(gh2_audiooffset));
	f1.f64 += offset;
}

void AudioOffsetHookPractice(PPCRegister& f1) {
	double offset = static_cast<double>(REXCVAR_GET(gh2_audiooffset));
	double newpos = f1.f64 + offset;
	if (newpos < 0) return;
	else f1.f64 = newpos;
}