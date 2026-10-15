// ==== ProcessSourceDamage @ 0x1813658c0 ====


undefined8
BattleEffectControllerDamageModule_ProcessSourceDamage
          (longlong param_1,longlong param_2,longlong param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  float fVar7;
  float fVar8;
  undefined8 uVar9;
  float fVar10;
  undefined4 uVar11;
  
  if (DAT_185a3b35b == '\0') {
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3b35b = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x30) == 0) ||
     (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar5 == 0)) goto LAB_181365b0e;
  if (*(char *)(lVar5 + 0x21) == '\0') {
    if (param_2 == 0) goto LAB_181365b0e;
    fVar10 = 0.0;
    uVar11 = 0;
  }
  else {
    if (param_2 == 0) goto LAB_181365b0e;
    uVar9 = BattleUnitController_GetStatValue(param_2,*(undefined1 *)(lVar5 + 0x21),0);
    uVar11 = (undefined4)((ulonglong)uVar9 >> 0x20);
    if ((*(longlong *)(param_1 + 0x30) == 0) ||
       (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar5 == 0))
    goto LAB_181365b0e;
    fVar10 = (float)uVar9 * *(float *)(lVar5 + 0x24) +
             *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38) + 0x28);
  }
  lVar5 = *(longlong *)(param_2 + 0x98);
  if (lVar5 != 0) {
    if (*(char *)(param_2 + 0x22d) == '\0') {
      if (((param_3 != 0) && (*(longlong *)(param_3 + 0x98) != 0)) &&
         (lVar6 = *(longlong *)(param_1 + 0x10), lVar6 != 0)) {
        iVar3 = *(int *)(lVar5 + 0x10);
        iVar4 = *(int *)(*(longlong *)(param_3 + 0x98) + 0x10);
        uVar11 = *(undefined4 *)(lVar6 + 0x2344);
        uVar1 = *(undefined4 *)(lVar6 + 0x2340);
        fVar8 = *(float *)(lVar6 + 0x2348);
        if (*(int *)(DAT_185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        fVar7 = (float)FUN_181377640((float)(iVar3 - iVar4) * fVar8,uVar11,uVar1);
        fVar8 = DAT_18529ef3c;
        fVar7 = fVar7 + DAT_18529ef3c;
        if ((*(longlong *)(param_1 + 0x38) != 0) &&
           (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x20), lVar5 != 0)) {
          uVar9 = BattleUnitsController_GetRelicLevelDeltaMod(lVar5,param_2,param_3,0);
          uVar11 = (undefined4)((ulonglong)uVar9 >> 0x20);
          fVar10 = ((float)uVar9 + fVar8) * fVar7 * fVar10;
          goto LAB_181365ae5;
        }
      }
    }
    else if (((param_3 != 0) && (*(longlong *)(param_3 + 0x98) != 0)) &&
            (lVar6 = *(longlong *)(param_1 + 0x10), lVar6 != 0)) {
      iVar3 = *(int *)(lVar5 + 0xcc);
      iVar4 = *(int *)(*(longlong *)(param_3 + 0x98) + 0xcc);
      uVar1 = *(undefined4 *)(lVar6 + 0x2338);
      uVar2 = *(undefined4 *)(lVar6 + 0x2334);
      fVar8 = *(float *)(lVar6 + 0x233c);
      if (*(int *)(DAT_185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      fVar8 = (float)FUN_181377640((float)(iVar3 - iVar4) * fVar8,uVar1,uVar2);
      fVar10 = fVar10 * (fVar8 + DAT_18529ef3c);
LAB_181365ae5:
      return CONCAT44(uVar11,fVar10);
    }
  }
LAB_181365b0e:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ProcessCritChance @ 0x181365b20 ====


ulonglong BattleEffectControllerDamageModule_ProcessCritChance
                    (longlong param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  longlong lVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  ulonglong uVar6;
  undefined8 uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  if (DAT_185a3b35c == '\0') {
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3b35c = '\x01';
  }
  uVar6 = *(ulonglong *)(param_1 + 0x30);
  if ((uVar6 == 0) || (*(longlong *)(uVar6 + 0x38) == 0)) goto LAB_181365f06;
  cVar1 = *(char *)(*(longlong *)(uVar6 + 0x38) + 0x1d);
  if (cVar1 == '\0') {
    return uVar6 & 0xffffffffffffff00;
  }
  if (cVar1 == '\x01') {
    if (param_2 == 0) goto LAB_181365f06;
    fVar10 = (float)BattleUnitController_GetStatValue(param_2,0xe,0);
    if (param_3 == 0) goto LAB_181365f06;
    fVar9 = (float)BattleUnitController_GetStatValue(param_3,0x16,0);
    fVar9 = (fVar10 + 0.0) - fVar9;
    fVar10 = (float)BattleUnitController_GetStatValue(param_2,0x1c,0);
    uVar7 = 0x28;
LAB_181365c3c:
    fVar11 = (float)BattleUnitController_GetStatValue(param_3,uVar7,0);
    fVar11 = (fVar10 + 0.0) - fVar11;
  }
  else {
    if (cVar1 == '\x02') {
      if (param_2 == 0) goto LAB_181365f06;
      fVar10 = (float)BattleUnitController_GetStatValue(param_2,0xf,0);
      if (param_3 == 0) goto LAB_181365f06;
      fVar9 = (float)BattleUnitController_GetStatValue(param_3,0x17,0);
      fVar9 = (fVar10 + 0.0) - fVar9;
      fVar10 = (float)BattleUnitController_GetStatValue(param_2,0x1d,0);
      uVar7 = 0x29;
      goto LAB_181365c3c;
    }
    fVar11 = 0.0;
    fVar9 = 0.0;
  }
  fVar10 = 0.0;
  if (((*(longlong *)(param_1 + 0x30) != 0) &&
      (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar2 != 0)) &&
     (*(longlong *)(param_1 + 0x10) != 0)) {
    lVar5 = 0x2308;
    if (*(char *)(lVar2 + 0x1d) != '\x01') {
      lVar5 = 0x22f0;
    }
    fVar8 = *(float *)(lVar5 + *(longlong *)(param_1 + 0x10));
    if (*(int *)(DAT_185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    fVar11 = fVar9 / fVar8 + fVar11;
    if ((DAT_18529ef3c <= fVar11) && ((uint)ABS(fVar11) < 0x7f800001)) {
      fVar11 = DAT_18529ef3c;
    }
    if (((param_2 != 0) && (*(longlong *)(param_2 + 0x98) != 0)) &&
       ((param_3 != 0 &&
        ((((*(longlong *)(param_3 + 0x98) != 0 && (*(longlong *)(param_1 + 0x30) != 0)) &&
          (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar2 != 0)) &&
         (lVar5 = *(longlong *)(param_1 + 0x10), lVar5 != 0)))))) {
      cVar1 = *(char *)(lVar2 + 0x1d);
      if (cVar1 == '\x01') {
        fVar9 = *(float *)(lVar5 + 0x2300);
      }
      else if (cVar1 == '\x02') {
        fVar9 = *(float *)(lVar5 + 0x22e8);
      }
      else {
        fVar9 = 0.0;
      }
      lVar2 = *(longlong *)(param_1 + 0x10);
      fVar9 = (float)(*(int *)(*(longlong *)(param_2 + 0x98) + 0x10) -
                     *(int *)(*(longlong *)(param_3 + 0x98) + 0x10)) * fVar9;
      cVar1 = *(char *)(*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38) + 0x1d);
      if (cVar1 == '\x01') {
        fVar8 = *(float *)(lVar2 + 0x22fc);
      }
      else if (cVar1 == '\x02') {
        fVar8 = *(float *)(lVar2 + 0x22e4);
      }
      else {
        fVar8 = 0.0;
      }
      if ((fVar8 < fVar9) || (0x7f800000 < (uint)ABS(fVar9))) {
        fVar8 = fVar9;
      }
      if (((*(longlong *)(param_1 + 0x30) != 0) &&
          (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar5 != 0)) && (lVar2 != 0)
         ) {
        cVar1 = *(char *)(lVar5 + 0x1d);
        if (cVar1 == '\x01') {
          fVar9 = *(float *)(lVar2 + 0x22f8);
        }
        else {
          fVar9 = DAT_18529ef3c;
          if (cVar1 == '\x02') {
            fVar9 = *(float *)(lVar2 + 0x22e0);
          }
        }
        if ((fVar8 < fVar9) || (0x7f800000 < (uint)ABS(fVar8))) {
          fVar9 = fVar8;
        }
        if (((*(longlong *)(param_1 + 0x30) != 0) &&
            (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar5 != 0)) &&
           (lVar2 != 0)) {
          cVar1 = *(char *)(lVar5 + 0x1d);
          if (cVar1 == '\x01') {
            fVar10 = *(float *)(lVar2 + 0x2314);
          }
          else if (cVar1 == '\x02') {
            fVar10 = *(float *)(lVar2 + 0x230c);
          }
          if ((*(longlong *)(param_1 + 0x38) != 0) &&
             (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x158), lVar2 != 0)) {
            uVar6 = *(ulonglong *)(lVar2 + 0x10);
            do {
              uVar6 = uVar6 * 0x5deece66d + 0xb & 0xffffffffffff;
              iVar3 = (int)((uVar6 >> 0x11) / 100);
              iVar4 = iVar3 * 100 + 99;
            } while (iVar4 < 0);
            fVar10 = (fVar11 + fVar9 + fVar10) * DAT_18529f324;
            *(ulonglong *)(lVar2 + 0x10) = uVar6;
            return (ulonglong)
                   CONCAT31((int3)((uint)iVar4 >> 8),
                            (float)((int)(uVar6 >> 0x11) + iVar3 * -100) < fVar10);
          }
        }
      }
    }
  }
LAB_181365f06:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ProcessCritDamage @ 0x181365f10 ====


undefined8 BattleEffectControllerDamageModule_ProcessCritDamage(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  float fVar2;
  undefined4 uVar3;
  
  if (DAT_185a3b35d == '\0') {
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3b35d = '\x01';
  }
  if (param_2 != 0) {
    uVar1 = BattleUnitController_GetStatValue(param_2,0x10,0);
    uVar3 = (undefined4)((ulonglong)uVar1 >> 0x20);
    if (*(longlong *)(param_1 + 0x10) != 0) {
      fVar2 = (float)uVar1 + *(float *)(*(longlong *)(param_1 + 0x10) + 0x231c);
      if (*(int *)(DAT_185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      if ((fVar2 < DAT_18529ef3c) || (0x7f800000 < (uint)ABS(DAT_18529ef3c))) {
        uVar3 = 0;
        fVar2 = DAT_18529ef3c;
      }
      return CONCAT44(uVar3,fVar2);
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ProcessDamageModification @ 0x181365fc0 ====


void BattleEffectControllerDamageModule_ProcessDamageModification
               (undefined8 param_1,longlong param_2,undefined8 param_3,undefined8 param_4)

{
  if (param_2 != 0) {
    BattleUnitController_GetModifyEffectAmount(param_2,param_3,param_4,0x1c,0xff,0);
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ProcessMitigation @ 0x181366000 ====


void BattleEffectControllerDamageModule_ProcessMitigation
               (longlong param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  if (DAT_185a3b35e == '\0') {
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3b35e = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x30) == 0) ||
     (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar2 == 0)) goto LAB_18136627f;
  cVar1 = *(char *)(lVar2 + 0x1d);
  if (cVar1 == '\0') {
    return;
  }
  if (cVar1 == '\x01') {
    if ((param_3 == 0) || (BattleUnitController_GetStatValue(param_3,8,0), param_2 == 0))
    goto LAB_18136627f;
    uVar3 = 10;
LAB_1813660d8:
    BattleUnitController_GetStatValue(param_2,uVar3,0);
  }
  else {
    if (cVar1 == '\x02') {
      if ((param_3 == 0) || (BattleUnitController_GetStatValue(param_3,9,0), param_2 == 0))
      goto LAB_18136627f;
      uVar3 = 0xb;
      goto LAB_1813660d8;
    }
    if (param_2 == 0) goto LAB_18136627f;
  }
  BattleUnitController_GetStatValue(param_2,0x38,0);
  if (*(int *)(DAT_185827178 + 0xe4) == 0) {
    FUN_180359940();
  }
  if ((((*(longlong *)(param_2 + 0x98) != 0) &&
       (*(longlong *)(*(longlong *)(param_2 + 0x98) + 0x40) != 0)) &&
      (*(longlong *)(param_1 + 0x10) != 0)) &&
     ((*(longlong *)(param_1 + 0x30) != 0 &&
      (*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38) != 0)))) {
    return;
  }
LAB_18136627f:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

