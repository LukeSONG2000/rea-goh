
ulonglong BattleUnitController_ProcessResistance(longlong param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  ulonglong uVar4;
  undefined8 uVar5;
  longlong lVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  
  if (DAT_185a3e941 == '\0') {
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1858316b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a93c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8fc0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a47d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9428);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3e941 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x88) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0xa8) != 0)) {
    uVar4 = FUN_1800084f0(0x13,DAT_1857a3910);
    if ((char)uVar4 == '\0') {
LAB_181bcafd5:
      return uVar4 & 0xffffffffffffff00;
    }
    if (*(longlong *)(param_1 + 0xe8) != 0) {
      uVar5 = FUN_1800084f0(8,DAT_1858316b0);
      lVar6 = FUN_1803a51b0(DAT_1857a9428);
      FUN_1836f4d00(lVar6,uVar5,DAT_1857a47d0);
      if ((*(longlong *)(param_1 + 0xe8) != 0) &&
         (uVar5 = FUN_1800084f0(0xd,DAT_1858316b0), lVar6 != 0)) {
        FUN_1836f5a70(lVar6,*(undefined4 *)(lVar6 + 0x18),uVar5,
                      *(undefined8 *)
                       (*(longlong *)(*(longlong *)(DAT_1857a93c0 + 0x20) + 0xc0) + 0x90));
        uVar4 = FUN_180014a60(lVar6,param_2);
        if ((char)uVar4 != '\0') goto LAB_181bcafd5;
        fVar7 = (float)BattleUnitController_GetStatValue(param_1,0x13,0);
        if (param_2 != 0) {
          fVar8 = (float)BattleUnitController_GetStatValue(param_2,0x12,0);
          if ((((*(longlong *)(param_1 + 0x98) != 0) && (*(longlong *)(param_2 + 0x98) != 0)) &&
              (*(longlong *)(param_1 + 0x88) != 0)) &&
             (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1e8), lVar6 != 0)) {
            fVar10 = (float)(*(int *)(*(longlong *)(param_1 + 0x98) + 0x10) -
                            *(int *)(*(longlong *)(param_2 + 0x98) + 0x10)) *
                     *(float *)(lVar6 + 0x2498);
            fVar9 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1e8) + 0x2494);
            if (*(int *)(DAT_185827178 + 0xe4) == 0) {
              FUN_180359940();
            }
            if ((fVar9 < fVar10) || (0x7f800000 < (uint)ABS(fVar10))) {
              fVar9 = fVar10;
            }
            if ((*(longlong *)(param_1 + 0x88) != 0) &&
               (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1e8), lVar6 != 0)) {
              fVar10 = *(float *)(lVar6 + 0x2490);
              if ((fVar9 < fVar10) || (0x7f800000 < (uint)ABS(fVar9))) {
                fVar10 = fVar9;
              }
              fVar10 = (fVar7 - fVar8) + fVar10;
              if (fVar10 < 0.0) {
                fVar10 = 0.0;
              }
              lVar6 = *(longlong *)(param_1 + 0x88);
              if (((lVar6 != 0) && (*(longlong *)(lVar6 + 0x1e8) != 0)) &&
                 (lVar1 = *(longlong *)(lVar6 + 0x158), lVar1 != 0)) {
                uVar4 = *(ulonglong *)(lVar1 + 0x10);
                do {
                  uVar4 = uVar4 * 0x5deece66d + 0xb & 0xffffffffffff;
                  iVar2 = (int)((uVar4 >> 0x11) / 100);
                  iVar3 = iVar2 * 100 + 99;
                } while (iVar3 < 0);
                fVar7 = (fVar10 + *(float *)(*(longlong *)(lVar6 + 0x1e8) + 0x2330)) * DAT_18529f324
                ;
                *(ulonglong *)(lVar1 + 0x10) = uVar4;
                return (ulonglong)
                       CONCAT31((int3)((uint)iVar3 >> 8),
                                (float)((int)(uVar4 >> 0x11) + iVar2 * -100) < fVar7);
              }
            }
          }
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

