// ==== ConditionStatus.IsMatch @ 0x1817dc710 ====


/* WARNING: Type propagation algorithm not settling */

bool ConditionStatus_IsMatch(longlong param_1,longlong *param_2,longlong param_3)

{
  int iVar1;
  ulonglong uVar2;
  longlong *plVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  ulonglong uVar7;
  longlong *plVar8;
  bool bVar9;
  bool bVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float local_res10 [2];
  float local_98 [2];
  longlong local_90;
  longlong *local_88;
  longlong local_80;
  longlong local_78;
  longlong *local_70;
  
  if (DAT_185a3d042 == '\0') {
    FUN_18035da50(&DAT_1857a4008);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3d88);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3da8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c34b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c34c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582cba0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582cb90);
    LOCK();
    UNLOCK();
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a3d042 = '\x01';
  }
  local_98[0] = 0.0;
  local_res10[0] = 0.0;
  local_90 = 0;
  if (param_2 != (longlong *)0x0) {
    cVar4 = FUN_1800084f0(0,DAT_1857a3d88,param_2);
    lVar5 = *(longlong *)(param_1 + 0x10);
    if (lVar5 != 0) {
      if (*(char *)(lVar5 + 0x10) == '6') {
        if (cVar4 == '\x17') {
          *(undefined1 *)(param_1 + 0x1a) = 1;
        }
        else if ((cVar4 == '!') && (*(char *)(param_1 + 0x1a) != '\0')) {
          *(undefined1 *)(param_1 + 0x1a) = 0;
          return true;
        }
      }
      if (lVar5 != 0) {
        if (cVar4 != *(char *)(lVar5 + 0x10)) {
          return false;
        }
        if (*(char *)(param_1 + 0x24) != '\0') {
          return true;
        }
        if ((*(byte *)(*param_2 + 0x130) < *(byte *)(DAT_1857a4008 + 0x130)) ||
           (*(longlong *)
             (*(longlong *)(*param_2 + 200) + -8 + (ulonglong)*(byte *)(DAT_1857a4008 + 0x130) * 8)
            != DAT_1857a4008)) {
          bVar10 = false;
        }
        else {
          bVar10 = true;
        }
        bVar9 = true;
        local_88 = (longlong *)0x0;
        if (bVar10) {
          local_88 = param_2;
        }
        lVar5 = FUN_1803a50f0(param_2,DAT_1857a3da8);
        local_78 = FUN_1803a50f0(param_2,DAT_1857c34b0);
        local_80 = FUN_1803a50f0(param_2,DAT_1857c34c8);
        plVar3 = local_88;
        if ((*(byte *)(*param_2 + 0x130) < *(byte *)(DAT_1857a3fd8 + 0x130)) ||
           (*(longlong *)
             (*(longlong *)(*param_2 + 200) + -8 + (ulonglong)*(byte *)(DAT_1857a3fd8 + 0x130) * 8)
            != DAT_1857a3fd8)) {
          bVar9 = false;
        }
        plVar8 = (longlong *)0x0;
        if (bVar9) {
          plVar8 = param_2;
        }
        if (local_88 == (longlong *)0x0) {
          if (plVar8 == (longlong *)0x0) {
            if (lVar5 != 0) {
              if (*(longlong *)(param_1 + 0x10) == 0) goto LAB_1817dce09;
              lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x18);
              if (lVar6 == 0) {
                return true;
              }
              if (*(int *)(lVar6 + 0x10) == 0) {
                return true;
              }
              lVar6 = FUN_1800084f0(0,DAT_1857a3da8,lVar5);
              if ((*(longlong *)(param_1 + 0x10) == 0) || (lVar6 == 0)) goto LAB_1817dce09;
              cVar4 = BattleUnitController_HasCategory
                                (lVar6,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x18),0);
              if (cVar4 != '\0') {
                return true;
              }
            }
            if (*(longlong *)(param_1 + 0x10) != 0) {
              lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x18);
              if (local_78 != 0) {
                if (lVar6 == 0) {
                  return true;
                }
                if (*(int *)(lVar6 + 0x10) == 0) {
                  return true;
                }
                lVar5 = FUN_1800084f0(0,DAT_1857c34b0);
                if (lVar6 == lVar5) {
                  return true;
                }
                if (lVar6 == 0) {
                  return false;
                }
                if (lVar5 == 0) {
                  return false;
                }
                if (*(int *)(lVar6 + 0x10) != *(int *)(lVar5 + 0x10)) {
                  return false;
                }
                uVar7 = 0;
                uVar2 = (longlong)*(int *)(lVar6 + 0x10) * 2;
                if (uVar2 < 8) {
                  if (uVar2 == 0) {
                    return true;
                  }
                  do {
                    if (*(char *)(lVar6 + 0x14 + uVar7) != *(char *)(lVar5 + 0x14 + uVar7)) {
                      return false;
                    }
                    uVar7 = uVar7 + 1;
                  } while (uVar7 < uVar2);
                  return true;
                }
                if (uVar2 != 8) {
                  do {
                    if (*(longlong *)(lVar6 + 0x14 + uVar7) != *(longlong *)(lVar5 + 0x14 + uVar7))
                    {
                      return false;
                    }
                    uVar7 = uVar7 + 8;
                  } while (uVar7 < uVar2 - 8);
                }
                lVar5 = *(longlong *)(lVar6 + 0xc + uVar2) - *(longlong *)(uVar2 + 0xc + lVar5);
LAB_1817dca1e:
                return lVar5 == 0;
              }
              if (local_80 == 0) {
                if (lVar6 == 0) {
                  return true;
                }
                if (*(int *)(lVar6 + 0x10) == 0) {
                  return true;
                }
                if (param_3 != 0) {
                  cVar4 = BattleUnitController_HasCategory(param_3,lVar6);
                  if (cVar4 == '\0') {
                    return false;
                  }
                  goto LAB_1817dca1e;
                }
              }
              else {
                if (lVar6 == 0) {
                  return true;
                }
                if (*(int *)(lVar6 + 0x10) == 0) {
                  return true;
                }
                lVar5 = FUN_1800084f0(0,DAT_1857c34c8,local_80);
                if (lVar5 != 0) {
                  local_res10[0] = (float)((uint)local_res10[0] & 0xffffff00);
                  local_90 = FUN_1800084f0(0,DAT_18582cba0,lVar5);
                  local_78 = 0;
                  local_70 = &local_90;
LAB_1817dcab0:
                  while( true ) {
                    if (local_90 == 0) {
                    /* WARNING: Subroutine does not return */
                      FUN_180352450();
                    }
                    cVar4 = FUN_1800084f0(0,DAT_185820d00);
                    if (cVar4 == '\0') {
                      if (local_90 != 0) {
                        FUN_180005650(0,DAT_18580d620,local_90);
                        return false;
                      }
                      return false;
                    }
                    if (local_90 == 0) {
                    /* WARNING: Subroutine does not return */
                      FUN_180352450();
                    }
                    lVar5 = FUN_1800084f0(0,DAT_18582cb90);
                    if (*(longlong *)(param_1 + 0x10) == 0) {
                    /* WARNING: Subroutine does not return */
                      FUN_180352450();
                    }
                    lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x18);
                    if (lVar6 == lVar5) break;
                    if (((lVar6 != 0) && (lVar5 != 0)) &&
                       (*(int *)(lVar6 + 0x10) == *(int *)(lVar5 + 0x10))) {
                      uVar7 = 0;
                      uVar2 = (longlong)*(int *)(lVar6 + 0x10) * 2;
                      if (uVar2 < 8) {
                        if (uVar2 != 0) {
                          do {
                            if (*(char *)(lVar6 + 0x14 + uVar7) != *(char *)(lVar5 + 0x14 + uVar7))
                            goto LAB_1817dcb9e;
                            uVar7 = uVar7 + 1;
                          } while (uVar7 < uVar2);
                        }
                        bVar10 = true;
                      }
                      else {
                        if (uVar2 != 8) {
                          do {
                            if (*(longlong *)(lVar6 + 0x14 + uVar7) !=
                                *(longlong *)(lVar5 + 0x14 + uVar7)) goto LAB_1817dcb9e;
                            uVar7 = uVar7 + 8;
                          } while (uVar7 < uVar2 - 8);
                        }
                        bVar10 = *(longlong *)(uVar2 + 0xc + lVar6) ==
                                 *(longlong *)(uVar2 + 0xc + lVar5);
                      }
                      goto LAB_1817dcb72;
                    }
                  }
                  goto LAB_1817dcb7a;
                }
              }
            }
            goto LAB_1817dce09;
          }
          if (*(longlong *)(param_1 + 0x10) == 0) goto LAB_1817dce09;
          NumberUtils_TryParseFloatInvariant
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x18),local_res10,0);
          fVar11 = *(float *)((longlong)plVar8 + 0x1c) / DAT_18529f324;
          if (cVar4 == 'L') {
            if (local_res10[0] <= fVar11) {
              return false;
            }
            if (local_res10[0] <= *(float *)(plVar8 + 4) / DAT_18529f324) {
              return true;
            }
            return false;
          }
          if (cVar4 != 'M') {
            return false;
          }
          if (local_res10[0] < *(float *)(plVar8 + 4) / DAT_18529f324) {
            return false;
          }
          bVar10 = fVar11 == local_res10[0];
          bVar9 = fVar11 < local_res10[0];
        }
        else {
          if ((*(longlong *)(param_1 + 0x10) == 0) ||
             (NumberUtils_TryParseFloatInvariant
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x18),local_98,0),
             param_3 == 0)) goto LAB_1817dce09;
          iVar1 = *(int *)((longlong)plVar3 + 0x1c);
          fVar11 = (float)BattleUnitController_GetStatValue(param_3,1,0);
          fVar12 = (float)iVar1 / (float)(int)fVar11;
          lVar5 = plVar3[4];
          fVar11 = (float)BattleUnitController_GetStatValue(param_3,1,0);
          fVar13 = (float)(int)lVar5 / (float)(int)fVar11;
          cVar4 = FUN_1800084f0(0,DAT_1857a3d88,param_2);
          fVar11 = local_98[0];
          if ((cVar4 == '\x19') && ((fVar12 < local_98[0] && (local_98[0] < fVar13)))) {
            return true;
          }
          cVar4 = FUN_1800084f0(0,DAT_1857a3d88,param_2);
          if (cVar4 != '\x18') {
            return false;
          }
          if (fVar11 <= fVar13) {
            return false;
          }
          bVar10 = fVar12 == fVar11;
          bVar9 = fVar12 < fVar11;
        }
        if (!bVar9 && !bVar10) {
          return true;
        }
        return false;
      }
    }
  }
LAB_1817dce09:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
LAB_1817dcb9e:
  bVar10 = false;
LAB_1817dcb72:
  if (bVar10) {
LAB_1817dcb7a:
    local_res10[0] = (float)CONCAT31(local_res10[0]._1_3_,1);
    if (local_90 != 0) {
      FUN_180005650(0,DAT_18580d620,local_90);
      return true;
    }
    return true;
  }
  goto LAB_1817dcab0;
}

