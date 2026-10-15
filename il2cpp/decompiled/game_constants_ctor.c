// DECOMPILE FAILED: GameConstants..ctor @ 0x1811ebf50: Exception while decompiling 1811ebf50: process: timeout


// ==== JavaRandom..ctor @ 0x180cac420 ====


void JavaRandom_SetSeed(longlong param_1,ulonglong param_2)

{
  *(ulonglong *)(param_1 + 0x10) = (param_2 ^ 0x5deece66d) & 0xffffffffffff;
  return;
}

