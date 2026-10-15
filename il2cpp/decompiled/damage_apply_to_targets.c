// ==== BattleEffectControllerDamageModule.ApplyToTargets @ 0x181364ca0 ====

void BattleEffectControllerDamageModule_ApplyToTargets
               (longlong *param_1,longlong param_2,longlong param_3,longlong param_4,
               ulonglong param_5,longlong param_6,undefined1 param_7,undefined8 param_8,
               longlong param_9)

{
  longlong lVar1;
  undefined8 uVar2;
  float fVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  longlong lVar8;
  int iVar9;
  longlong lVar10;
  bool bVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  longlong lVar17;
  undefined4 uVar20;
  undefined8 uVar18;
  undefined8 uVar19;
  longlong lVar21;
  undefined4 uVar23;
  ulonglong uVar22;
  ulonglong uVar24;
  char local_108;
  int local_100;
  longlong local_f8;
  undefined8 uStack_f0;
  longlong local_e8;
  longlong local_e0;
  undefined8 local_d8;
  longlong *plStack_d0;
  undefined8 local_c8;
  longlong local_c0;
  longlong local_b8;
  
  if (DAT_185a3b35a == '\0') {
    FUN_18035da50(&DAT_18580d270);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580d2c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    DAT_185a3b35a = '\x01';
  }
  local_f8 = 0;
  uStack_f0 = 0;
  local_e8 = 0;
  fVar12 = (float)(**(code **)(*param_1 + 0x1f8))(param_1,param_4,*(undefined8 *)(*param_1 + 0x200))
  ;
  if ((param_1[6] == 0) || (lVar10 = *(longlong *)(param_1[6] + 0x38), lVar10 == 0))
  goto LAB_1813657f6;
  bVar11 = false;
  if ((*(ulonglong *)(lVar10 + 0xf0) & 0x1000000000) != 0) {
    bVar11 = *(longlong *)(param_1[6] + 0x30) != 0;
  }
  lVar10 = param_2;
  if (((*(ulonglong *)(*(longlong *)(param_1[6] + 0x38) + 0xf0) & 0x800000000) != 0) &&
     (*(longlong *)(param_1[6] + 0x30) != 0)) {
    lVar10 = *(longlong *)(param_1[6] + 0x30);
  }
  if (bVar11) {
    param_2 = *(longlong *)(param_1[6] + 0x30);
  }
  lVar8 = param_1[7];
  local_e0 = lVar10;
  if (lVar8 == 0) goto LAB_1813657f6;
  if (*(longlong *)(lVar8 + 0x240) == 0) {
LAB_181364f0a:
    bVar11 = false;
  }
  else {
    lVar8 = *(longlong *)(lVar8 + 0x240);
    if (DAT_185a3e8f5 == '\0') {
      FUN_18035da50(&DAT_1858316b0);
      LOCK();
      UNLOCK();
      DAT_185a3e8f5 = '\x01';
    }
    if (*(longlong *)(lVar8 + 0xe8) == 0) goto LAB_1813657f6;
    iVar5 = FUN_1800084f0(0xc,DAT_1858316b0);
    if (iVar5 != 0) goto LAB_181364f0a;
    if (lVar10 == 0) goto LAB_1813657f6;
    if (DAT_185a3e8f5 == '\0') {
      FUN_18035da50(&DAT_1858316b0);
      LOCK();
      UNLOCK();
      DAT_185a3e8f5 = '\x01';
    }
    if (*(longlong *)(lVar10 + 0xe8) == 0) goto LAB_1813657f6;
    iVar5 = FUN_1800084f0(0xc,DAT_1858316b0);
    bVar11 = iVar5 == 0;
  }
  if (param_4 != 0) {
    if (DAT_185a3b34c == '\0') {
      FUN_18035da50(&DAT_18580d398);
      LOCK();
      UNLOCK();
      DAT_185a3b34c = '\x01';
    }
    if (((*(longlong *)(param_4 + 0x10) != 0) &&
        (lVar8 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar8 != 0)) &&
       (lVar8 = FUN_1832a2dd0(lVar8,DAT_18580d398), fVar3 = DAT_18529ef3c, local_c0 = lVar8,
       param_3 != 0)) {
      local_100 = 0;
      local_c8 = 0;
      plStack_d0 = (longlong *)((ulonglong)*(uint *)(param_3 + 0x1c) << 0x20);
      uStack_f0 = (longlong)plStack_d0;
      local_e8 = 0;
      local_d8 = 0;
      plStack_d0 = &local_f8;
      local_f8 = param_3;
      local_b8 = lVar8;
      while( true ) {
        if (local_f8 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (uStack_f0._4_4_ != *(int *)(local_f8 + 0x1c)) break;
        if (*(uint *)(local_f8 + 0x18) <= (uint)uStack_f0) break;
        lVar1 = *(longlong *)(local_f8 + 0x10);
        if (lVar1 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(uint *)(lVar1 + 0x18) <= (uint)uStack_f0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        lVar1 = *(longlong *)(lVar1 + 0x20 + (longlong)(int)(uint)uStack_f0 * 8);
        uStack_f0 = CONCAT44(uStack_f0._4_4_,(uint)uStack_f0 + 1);
        local_e8 = lVar1;
        if (param_6 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        cVar4 = FUN_1833355c0(param_6,lVar1,DAT_18580d270);
        if (cVar4 == '\0') {
          BattleEffectControllerDamageModule_TriggerTagDirectedCondition
                    (param_1,lVar10,lVar1,0x46,param_5,0);
          fVar13 = (float)BattleEffectControllerDamageModule_ProcessSourceDamage
                                    (param_1,param_2,lVar1,0);
          if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[6] + 0x38);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          fVar16 = fVar12 * *(float *)(lVar8 + 0x2c) + fVar3;
          if ((*(byte *)(lVar8 + 0xf0) & 4) != 0) {
            if (lVar1 == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            fVar13 = (float)BattleUnitController_GetStatValue(lVar1,1,0);
            if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            lVar8 = *(longlong *)(param_1[6] + 0x38);
            if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            fVar16 = fVar12 * *(float *)(lVar8 + 0x2c) + *(float *)(lVar8 + 0x24);
            fVar13 = (float)(int)fVar13;
          }
          fVar16 = fVar16 * fVar13;
          if (param_1[7] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if (*(longlong *)(param_1[7] + 0xa8) == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          local_108 = '\0';
          cVar4 = FUN_1800084f0(0x13,DAT_1857a3910);
          if (cVar4 != '\0') {
            local_108 = BattleEffectControllerDamageModule_ProcessCritChance
                                  (param_1,param_2,lVar1,0);
            if (local_108 != '\0') {
              BattleEffectControllerDamageModule_TriggerTagDirectedCondition
                        (param_1,lVar10,lVar1,6,param_5,0);
              fVar13 = (float)BattleEffectControllerDamageModule_ProcessCritDamage
                                        (param_1,param_2,0);
              fVar16 = fVar16 * fVar13;
              if (bVar11) {
                if (lVar1 == 0) {
                    /* WARNING: Subroutine does not return */
                  FUN_180352450();
                }
                iVar5 = FUN_181bb9960(lVar1,0);
                if (iVar5 == 1) {
                  local_100 = local_100 + 1;
                }
              }
            }
          }
          if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          fVar14 = (float)BattleUnitController_GetModifyEffectAmount
                                    (param_2,lVar1,param_1[6],0x1c,0xff,0);
          fVar13 = (float)BattleEffectControllerDamageModule_ProcessMitigation
                                    (param_1,param_2,lVar1,0);
          fVar13 = fVar13 * fVar14 * fVar16;
          if (0.0 < fVar13) {
            if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            lVar8 = *(longlong *)(param_1[6] + 0x38);
            if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            cVar4 = FUN_180c75680(lVar8,0);
            if (cVar4 == '\0') {
              if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              lVar8 = *(longlong *)(param_1[6] + 0x38);
              if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              if (param_1[7] == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              lVar17 = *(longlong *)(param_1[7] + 0x158);
              if (lVar17 == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              iVar9 = (int)(fVar13 * *(float *)(lVar8 + 0x30));
              iVar5 = JavaRandom_nextInt(lVar17,iVar9 * 2 | 1,0);
              fVar13 = (float)(iVar5 - iVar9) + fVar13;
            }
          }
          if (*(int *)(DAT_185827178 + 0xe4) == 0) {
            FUN_180359940();
          }
          if ((fVar13 <= fVar3) && ((uint)ABS(fVar13) < 0x7f800001)) {
            fVar13 = fVar3;
          }
          fVar16 = (float)BattleUnitController_GetStatValue(param_2,0x22,0);
          if (fVar16 < 0.0) {
            fVar16 = 0.0;
          }
          else if (fVar3 < fVar16) {
            fVar16 = fVar3;
          }
          fVar16 = fVar16 * fVar13;
          if ((fVar13 <= fVar16) && ((uint)ABS(fVar16) < 0x7f800001)) {
            fVar16 = fVar13;
          }
          if (lVar1 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          fVar16 = fVar13 - fVar16;
          iVar5 = FUN_181bb9c20(lVar1,0);
          if ((fVar16 < (float)iVar5) || (fVar14 = (float)iVar5, 0x7f800000 < (uint)ABS(fVar16))) {
            fVar14 = fVar16;
          }
          if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[6] + 0x38);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          fVar16 = fVar13 - fVar14;
          if ((*(byte *)(lVar8 + 0xf0) & 1) == 0) {
LAB_1813653eb:
            fVar15 = fVar16;
          }
          else {
            iVar5 = 0;
            if (-1 < *(int *)(lVar1 + 0xc0)) {
              iVar5 = *(int *)(lVar1 + 0xc0);
            }
            fVar15 = (float)BattleUnitController_GetStatValue(lVar1,1,0);
            if ((int)fVar15 <= iVar5) {
              iVar5 = (int)fVar15;
            }
            if (*(int *)(DAT_185827178 + 0xe4) == 0) {
              FUN_180359940();
            }
            if ((fVar16 < (float)(iVar5 + -1)) ||
               (fVar15 = (float)(iVar5 + -1), 0x7f800000 < (uint)ABS(fVar16))) goto LAB_1813653eb;
          }
          if (fVar15 < 0.0) {
            fVar15 = 0.0;
          }
          iVar5 = 0;
          if (-1 < *(int *)(lVar1 + 0xc0)) {
            iVar5 = *(int *)(lVar1 + 0xc0);
          }
          fVar16 = (float)BattleUnitController_GetStatValue(lVar1,1,0);
          if ((int)fVar16 <= iVar5) {
            iVar5 = (int)fVar16;
          }
          if (*(int *)(DAT_185827178 + 0xe4) == 0) {
            FUN_180359940();
          }
          fVar16 = fVar15 - (float)iVar5;
          if (fVar16 < 0.0) {
            fVar16 = 0.0;
          }
          if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[6] + 0x38);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar17 = lVar10;
          lVar21 = lVar1;
          BattleUnitController_ApplyAttackDamage
                    (lVar1,(int)fVar15,(int)fVar14,local_108,lVar10,lVar1,(int)param_1[8],
                     *(undefined1 *)(lVar8 + 0x1d),param_7,(*(uint *)(lVar8 + 0xf0) & 0x10000) == 0,
                     param_1[6],0,0);
          uVar20 = (undefined4)((ulonglong)lVar17 >> 0x20);
          uVar23 = (undefined4)((ulonglong)lVar21 >> 0x20);
          if (param_1[7] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if (lVar10 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[7] + 0x58);
          uVar19 = *(undefined8 *)(lVar10 + 0x98);
          uVar6 = FUN_181bb9960(lVar10,0);
          uVar2 = *(undefined8 *)(lVar1 + 0x98);
          uVar7 = FUN_181bb9960(lVar1,0);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uVar18 = CONCAT44(uVar20,uVar7);
          uVar24 = param_5;
          BattleRecordController_LogDamageRecord
                    (lVar8,uVar19,uVar6,uVar2,uVar18,CONCAT44(uVar23,fVar13),fVar16,local_108,
                     param_5,*(undefined1 *)(lVar1 + 0x94),0);
          lVar10 = local_e0;
          if (param_1[7] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[7] + 0x60);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uVar22 = 0;
          uVar19 = CONCAT71((int7)((ulonglong)uVar18 >> 8),local_108);
          BattleStatController_DamageApplied(lVar8,local_e0,lVar1,fVar13,uVar19,0);
          uVar20 = (undefined4)((ulonglong)uVar19 >> 0x20);
          if (param_1[6] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar8 = *(longlong *)(param_1[6] + 0x38);
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if ((*(uint *)(lVar8 + 0xf0) & 0x20000) == 0) {
            fVar16 = (float)BattleUnitController_GetStatValue(param_2,0x18,0);
            if (0.0 < fVar16 * fVar13) {
              if (param_1[7] == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              lVar8 = *(longlong *)(param_1[7] + 0x1e8);
              if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              cVar4 = BattleUnitController_IsImmuneToEffect
                                (lVar10,*(undefined8 *)(lVar8 + 0x1120),0);
              if (cVar4 == '\0') {
                BattleUnitController_ApplyHeal
                          (lVar10,lVar10,param_1[6],(int)param_1[8],
                           CONCAT44(uVar20,(int)(fVar16 * fVar13)),uVar22 & 0xffffffff00000000,1,1,
                           uVar24 & 0xffffffffffffff00,1,0);
              }
            }
          }
          lVar8 = local_c0;
          if (local_c0 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          FUN_18335f400(local_c0,lVar1,fVar13,1,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(DAT_18580d2c8 + 0x20) + 0xc0) + 0x110));
        }
        else {
          if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          FUN_18335f400(lVar8,lVar1,0,1,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(DAT_18580d2c8 + 0x20) + 0xc0) + 0x110));
        }
      }
      if ((*(byte *)(*(longlong *)(DAT_185806eb8 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(DAT_185806eb8 + 0x20));
      }
      if (local_f8 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (uStack_f0._4_4_ != *(int *)(local_f8 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
      }
      uStack_f0 = CONCAT44(uStack_f0._4_4_,*(int *)(local_f8 + 0x18) + 1);
      local_e8 = 0;
      BattleEffectContext_AddEffectResult(param_4,lVar8,0);
      if (bVar11) {
        if ((param_1[7] == 0) || (lVar10 = *(longlong *)(param_1[7] + 0x68), lVar10 == 0))
        goto LAB_1813657f6;
        BattleStatTrackingController_PlayerDealtCriticalHits(lVar10,local_100,param_3,0);
      }
      if (param_9 != 0) {
        (**(code **)(param_9 + 0x18))
                  (*(undefined8 *)(param_9 + 0x40),*(undefined8 *)(param_9 + 0x28));
      }
      return;
    }
  }
LAB_1813657f6:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

