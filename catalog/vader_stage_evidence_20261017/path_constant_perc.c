// path_constant_perc VA 0x1824ab4b0 RVA 0x24ab4b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_1824ab4b0(longlong param_1,float param_2)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  if (*(int *)(param_1 + 0x20) == 0) {
    return param_2;
  }
  if ((0.0 < param_2) && (param_2 < _UNK_18529ef3c)) {
    if (*(float *)(param_1 + 0x38) <= 0.0) {
      return param_2;
    }
    lVar1 = *(longlong *)(param_1 + 0x48);
    if (lVar1 == 0) {
code_r0x0001824ab5f3:
      FUN_180352450();
      pcVar3 = (code *)swi(3);
      fVar6 = (float)(*pcVar3)();
      return fVar6;
    }
    fVar6 = 0.0;
    param_2 = param_2 * *(float *)(param_1 + 0x38);
    fVar9 = 0.0;
    fVar7 = 0.0;
    fVar8 = 0.0;
    if (0 < *(int *)(lVar1 + 0x18)) {
      uVar5 = 0;
code_r0x0001824ab522:
      if (uVar5 < *(uint *)(lVar1 + 0x18)) {
        lVar2 = *(longlong *)(param_1 + 0x40);
        if (lVar2 == 0) goto code_r0x0001824ab5f3;
        if (*(float *)(lVar1 + 0x20 + (longlong)(int)uVar5 * 4) <= param_2) {
          if (*(uint *)(lVar2 + 0x18) <= uVar5) goto code_r0x0001824ab5ed;
          lVar4 = (longlong)(int)uVar5;
          uVar5 = uVar5 + 1;
          fVar6 = *(float *)(lVar2 + 0x20 + lVar4 * 4);
          if (*(int *)(lVar1 + 0x18) <= (int)uVar5) goto code_r0x0001824ab561;
          goto code_r0x0001824ab522;
        }
        if (uVar5 < *(uint *)(lVar2 + 0x18)) {
          fVar7 = *(float *)(lVar2 + 0x20 + (longlong)(int)uVar5 * 4);
          fVar8 = *(float *)(lVar1 + 0x20 + (longlong)(int)uVar5 * 4);
          if (0 < (int)uVar5) {
            if (*(uint *)(lVar1 + 0x18) <= (uint)((longlong)(int)uVar5 + -1))
            goto code_r0x0001824ab5ed;
            fVar9 = *(float *)(lVar1 + 0x20 + ((longlong)(int)uVar5 + -1) * 4);
          }
          goto code_r0x0001824ab561;
        }
      }
code_r0x0001824ab5ed:
      FUN_180352460();
      pcVar3 = (code *)swi(3);
      fVar6 = (float)(*pcVar3)();
      return fVar6;
    }
code_r0x0001824ab561:
    param_2 = ((param_2 - fVar9) / (fVar8 - fVar9)) * (fVar7 - fVar6) + fVar6;
  }
  fVar6 = _UNK_18529ef3c;
  if ((param_2 <= _UNK_18529ef3c) && (fVar6 = 0.0, 0.0 <= param_2)) {
    return param_2;
  }
  return fVar6;
}

