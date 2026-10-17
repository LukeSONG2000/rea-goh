// recovery_round_helper VA 0x18000cb30 RVA 0xcb30

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

double FUN_18000cb30(undefined8 param_1)

{
  double dVar1;
  double dVar2;
  ulonglong uVar3;
  double dVar4;
  undefined1 in_XMM0 [16];
  undefined1 auVar5 [16];
  double adStackX_8 [4];
  
  auVar5._8_4_ = in_XMM0._8_4_;
  auVar5._12_4_ = in_XMM0._12_4_;
  if (in_XMM0._0_8_ < 0.0) {
    dVar4 = (double)func_0x000180b70f90(param_1,adStackX_8);
    if (dVar4 == _UNK_18529f4a0) {
      if (((longlong)adStackX_8[0] & 1U) != 0) {
        return adStackX_8[0] - _UNK_18529f018;
      }
    }
    else {
      adStackX_8[0] = (double)func_0x000180b9d990();
    }
  }
  else {
    dVar4 = (double)func_0x000180b70f90(param_1,adStackX_8);
    if (dVar4 != _UNK_18529efe0) {
      dVar4 = in_XMM0._0_8_ + _UNK_18529efe0;
      auVar5._0_8_ = dVar4;
      if (1 < iRam00000001855c8450) {
        auVar5 = roundsd(auVar5,auVar5,9);
        return auVar5._0_8_;
      }
      dVar1 = ABS(dVar4);
      uVar3 = (ulonglong)dVar1 >> 0x34;
      dVar2 = dVar4;
      if (uVar3 < 0x433) {
        if (uVar3 < 0x3ff) {
          if (dVar1 != 0.0) {
            if (dVar4 == dVar1) {
              return 0.0;
            }
            return _UNK_18529f4a8;
          }
        }
        else {
          dVar2 = (double)(~((1L << (0x33U - (char)uVar3 & 0x3f)) - 1U) & (ulonglong)dVar4);
          if ((dVar4 != dVar1) && (dVar2 != dVar4)) {
            dVar2 = dVar2 - _UNK_18529f018;
          }
        }
      }
      else if (0x7ff0000000000000 < (ulonglong)dVar1) {
        return (double)((ulonglong)dVar4 | 0x8000000000000);
      }
      return dVar2;
    }
    if (((longlong)adStackX_8[0] & 1U) != 0) {
      return adStackX_8[0] + _UNK_18529f018;
    }
  }
  return adStackX_8[0];
}

