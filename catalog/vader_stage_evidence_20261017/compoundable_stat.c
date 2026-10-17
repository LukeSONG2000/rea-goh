// compoundable_stat VA 0x181bcea70 RVA 0x1bcea70

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float FUN_181bcea70(void)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  longlong in_RCX;
  ulonglong in_RDX;
  uint uVar7;
  undefined4 uVar8;
  float fVar9;
  uint uStack_38;
  
  lVar2 = *(longlong *)(in_RCX + 0x60);
  if (lVar2 == 0) {
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    fVar9 = (float)(*pcVar3)();
    return fVar9;
  }
  if (cRam0000000185a3e09d == '\0') {
    FUN_18035da50(0x185810420,in_RDX,0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810408);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810400);
    LOCK();
    UNLOCK();
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
    FUN_18035da50(0x185810370);
    LOCK();
    UNLOCK();
    cRam0000000185a3e09d = '\x01';
  }
  if (*(char *)(lVar2 + 0x88) != '\0') {
    return 0.0;
  }
  lVar5 = *(longlong *)(lVar2 + 0x58);
  uVar4 = FUN_1803a51b0(uRam0000000185810420);
  FUN_183313140(uVar4,lVar2,uRam0000000185810408,0);
  if ((lVar5 != 0) &&
     (FUN_181926ce0(lVar5,4,in_RDX & 0xff,uVar4,0), *(longlong *)(lVar2 + 0x90) != 0)) {
    fVar9 = 0.0;
    lVar5 = FUN_180035ea0();
    if ((char)in_RDX == '>') {
      if ((lVar5 != 0) && (lVar5 = *(longlong *)(lVar5 + 0x10), lVar5 != 0)) {
        if (0 < *(int *)(lVar5 + 0x20) - *(int *)(lVar5 + 0x28)) {
          uVar8 = FUN_181a6f4d0(lVar2,*(undefined8 *)(lVar2 + 0x90),0,0);
          fVar9 = (float)FUN_181a6f4d0(lVar2,*(undefined8 *)(lVar2 + 0x98),uVar8,0);
        }
code_r0x000181a6f41c:
        if (*(longlong *)(lVar2 + 0x90) != 0) {
          FUN_1832a7f70(*(longlong *)(lVar2 + 0x90),uRam0000000185810400);
          if (*(longlong *)(lVar2 + 0x98) != 0) {
            FUN_1832a7f70(*(longlong *)(lVar2 + 0x98),uRam0000000185810400);
            return fVar9;
          }
        }
      }
    }
    else if ((lVar5 != 0) && (lVar5 = *(longlong *)(lVar5 + 0x10), lVar5 != 0)) {
      uStack_38 = 0;
      while (lVar5 != 0) {
        if (*(int *)(lVar5 + 0x2c) != *(int *)(lVar5 + 0x2c)) goto code_r0x000181a6f48d;
        do {
          if (lVar5 == 0) goto code_r0x000181a6f488;
          if (*(uint *)(lVar5 + 0x20) <= uStack_38) {
            if (lVar5 == 0) goto code_r0x000181a6f483;
            if (((*(longlong *)(lVar2 + 0x98) == 0) || (lVar5 = FUN_180035ea0(), lVar5 == 0)) ||
               (lVar5 = *(longlong *)(lVar5 + 0x10), lVar5 == 0)) goto code_r0x000181a6f46d;
            uStack_38 = 0;
            goto code_r0x000181a6f320;
          }
          lVar1 = *(longlong *)(lVar5 + 0x18);
          lVar6 = (longlong)(int)uStack_38;
          uVar7 = uStack_38 + 1;
          if (lVar1 == 0) goto code_r0x000181a6f47e;
          if (*(uint *)(lVar1 + 0x18) <= uStack_38) goto code_r0x000181a6f473;
          uStack_38 = uVar7;
        } while (*(int *)(lVar1 + 0x20 + lVar6 * 0x18) < 0);
        lVar1 = *(longlong *)(lVar1 + 0x30 + lVar6 * 0x18);
        if (lVar1 == 0) goto code_r0x000181a6f479;
        fVar9 = fVar9 + *(float *)(lVar1 + 0x14);
      }
      goto code_r0x000181a6f494;
    }
  }
code_r0x000181a6f46d:
  FUN_180352450();
code_r0x000181a6f473:
  FUN_180352460();
code_r0x000181a6f479:
  FUN_180352450();
code_r0x000181a6f47e:
  FUN_180352450();
code_r0x000181a6f483:
  FUN_180352450();
code_r0x000181a6f488:
  FUN_180352450();
code_r0x000181a6f48d:
  FUN_183aed800(0);
code_r0x000181a6f494:
  FUN_180352450();
  FUN_180352440();
code_r0x000181a6f4a0:
  FUN_180352460();
code_r0x000181a6f4a5:
  FUN_180352450();
code_r0x000181a6f4aa:
  FUN_180352450();
code_r0x000181a6f4af:
  FUN_180352450();
code_r0x000181a6f4b4:
  FUN_180352450();
code_r0x000181a6f4b9:
  FUN_183aed800(0);
code_r0x000181a6f4c0:
  FUN_180352450();
  FUN_180352440();
  pcVar3 = (code *)swi(3);
  fVar9 = (float)(*pcVar3)();
  return fVar9;
code_r0x000181a6f320:
  if (lVar5 == 0) goto code_r0x000181a6f4c0;
  if (*(int *)(lVar5 + 0x2c) != *(int *)(lVar5 + 0x2c)) goto code_r0x000181a6f4b9;
  do {
    if (lVar5 == 0) goto code_r0x000181a6f4b4;
    if (*(uint *)(lVar5 + 0x20) <= uStack_38) {
      if (lVar5 == 0) goto code_r0x000181a6f4af;
      goto code_r0x000181a6f41c;
    }
    lVar1 = *(longlong *)(lVar5 + 0x18);
    lVar6 = (longlong)(int)uStack_38;
    uVar7 = uStack_38 + 1;
    if (lVar1 == 0) goto code_r0x000181a6f4aa;
    if (*(uint *)(lVar1 + 0x18) <= uStack_38) goto code_r0x000181a6f4a0;
    uStack_38 = uVar7;
  } while (*(int *)(lVar1 + 0x20 + lVar6 * 0x18) < 0);
  lVar1 = *(longlong *)(lVar1 + 0x30 + lVar6 * 0x18);
  if (lVar1 == 0) goto code_r0x000181a6f4a5;
  fVar9 = fVar9 + *(float *)(lVar1 + 0x14);
  goto code_r0x000181a6f320;
}

