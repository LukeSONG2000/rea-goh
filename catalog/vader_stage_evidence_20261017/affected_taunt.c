// affected_taunt VA 0x181a6f4d0 RVA 0x1a6f4d0

undefined8 FUN_181a6f4d0(undefined8 param_1,longlong param_2,undefined8 param_3)

{
  float fVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  longlong lVar5;
  uint uVar6;
  bool bVar7;
  undefined8 uVar8;
  float fVar9;
  undefined4 uVar10;
  uint uStack_30;
  
  uVar10 = (undefined4)((ulonglong)param_3 >> 0x20);
  if (cRam0000000185a3e09e == '\0') {
    FUN_18035da50(0x1858103f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810380);
    LOCK();
    UNLOCK();
    cRam0000000185a3e09e = '\x01';
  }
  if (((param_2 == 0) || (lVar4 = FUN_180035ea0(), lVar4 == 0)) ||
     (lVar4 = *(longlong *)(lVar4 + 0x10), lVar4 == 0)) {
    FUN_180352450();
code_r0x000181a6f697:
    FUN_180352460();
code_r0x000181a6f69d:
    FUN_180352450();
code_r0x000181a6f6a2:
    FUN_180352450();
code_r0x000181a6f6a7:
    FUN_180352450();
code_r0x000181a6f6ac:
    FUN_180352450();
code_r0x000181a6f6b1:
    FUN_183aed800(0);
  }
  else {
    uStack_30 = 0;
    fVar9 = (float)param_3;
code_r0x000181a6f5d0:
    if (lVar4 != 0) {
      if (*(int *)(lVar4 + 0x2c) == *(int *)(lVar4 + 0x2c)) {
        do {
          if (lVar4 == 0) goto code_r0x000181a6f6ac;
          if (*(uint *)(lVar4 + 0x20) <= uStack_30) {
            if (lVar4 != 0) {
              return CONCAT44(uVar10,fVar9);
            }
            goto code_r0x000181a6f6a7;
          }
          lVar2 = *(longlong *)(lVar4 + 0x18);
          lVar5 = (longlong)(int)uStack_30;
          uVar6 = uStack_30 + 1;
          if (lVar2 == 0) goto code_r0x000181a6f6a2;
          if (*(uint *)(lVar2 + 0x18) <= uStack_30) goto code_r0x000181a6f697;
          uStack_30 = uVar6;
        } while (*(int *)(lVar2 + 0x20 + lVar5 * 0x18) < 0);
        lVar2 = *(longlong *)(lVar2 + 0x30 + lVar5 * 0x18);
        if (lVar2 != 0) {
          fVar1 = *(float *)(lVar2 + 0x14);
          if (fVar1 <= 0.0) goto code_r0x000181a6f639;
          bVar7 = fVar1 < fVar9;
          goto code_r0x000181a6f646;
        }
        goto code_r0x000181a6f69d;
      }
      goto code_r0x000181a6f6b1;
    }
  }
  FUN_180352450();
  FUN_180352440();
  pcVar3 = (code *)swi(3);
  uVar8 = (*pcVar3)();
  return uVar8;
code_r0x000181a6f639:
  if (fVar9 <= 0.0) {
    bVar7 = fVar9 < fVar1;
code_r0x000181a6f646:
    if (!bVar7 && fVar9 != fVar1) {
      uVar10 = 0;
      fVar9 = fVar1;
    }
  }
  goto code_r0x000181a6f5d0;
}

