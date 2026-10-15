// ==== ProcessUnitsSpeed @ 0x181cffb50 ====


longlong BattleUnitsController_ProcessUnitsSpeed(longlong param_1,undefined8 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  char cVar4;
  uint uVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  code *pcVar9;
  longlong lVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  longlong local_f8;
  undefined8 uStack_f0;
  longlong local_e8;
  undefined8 local_e0;
  longlong *plStack_d8;
  undefined8 local_d0;
  longlong local_c8;
  longlong local_c0;
  undefined8 local_b8;
  longlong local_b0;
  
  if (DAT_185a3f0bf == '\0') {
    FUN_18035da50(&DAT_1857f5d50);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d58);
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
    FUN_18035da50(&DAT_1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a4808);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1858107d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d40);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b3b70);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9410);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9430);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9428);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cd8);
    LOCK();
    UNLOCK();
    DAT_185a3f0bf = '\x01';
  }
  local_f8 = 0;
  uStack_f0 = 0;
  local_e8 = 0;
  lVar6 = FUN_1803a51b0(DAT_1857f5cd8);
  local_c0 = lVar6;
  FUN_182d09b30(lVar6,DAT_1857f5d48);
  fVar11 = (float)BattleUnitsController_DetermineTicksToNextAttack(param_1,lVar6,0);
  lVar7 = FUN_1803a51b0(DAT_1857a9428);
  local_c8 = lVar7;
  FUN_182d09b30(lVar7,DAT_1857a9410);
  if (*(longlong *)(param_1 + 0xb8) != 0) {
    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x18);
    lVar8 = FUN_1803a51b0(DAT_1857a9428);
    FUN_182d09bb0(lVar8,uVar1,DAT_1857b3b70);
    if (lVar7 == 0) goto LAB_181d00630;
    lVar10 = 0;
    if (*(int *)(lVar7 + 0x18) == 0) {
      BattleUnitsController_PopulateNextUnitCandidateList(param_1,lVar6,fVar11,lVar7,lVar8,0);
    }
    if (lVar8 == 0) goto LAB_181d00630;
    local_b8 = 0;
    iVar2 = *(int *)(lVar8 + 0x18);
    if (1 < iVar2) {
      lVar6 = FUN_1803a51b0(DAT_1857a9428);
      FUN_182d09bb0(lVar6,iVar2,DAT_1857b3b70);
      fVar14 = 0.0;
      local_d0 = 0;
      uStack_f0._4_4_ = *(int *)(lVar8 + 0x1c);
      uStack_f0._0_4_ = 0;
      local_e8 = 0;
      local_e0 = 0;
      plStack_d8 = &local_f8;
      local_f8 = lVar8;
      local_b0 = lVar6;
LAB_181cffe60:
      if (local_f8 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if ((uStack_f0._4_4_ == *(int *)(local_f8 + 0x1c)) &&
         ((uint)uStack_f0 < *(uint *)(local_f8 + 0x18))) {
        lVar8 = *(longlong *)(local_f8 + 0x10);
        if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(uint *)(lVar8 + 0x18) <= (uint)uStack_f0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        lVar8 = *(longlong *)(lVar8 + 0x20 + (longlong)(int)(uint)uStack_f0 * 8);
        uStack_f0._0_4_ = (uint)uStack_f0 + 1;
        local_e8 = lVar8;
        if (lVar8 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        fVar13 = (float)BattleUnitController_GetStatValue
                                  (lVar8,CONCAT71((uint7)(uint3)((uint)uStack_f0._4_4_ >> 8),5),0);
        if (fVar13 < 0.0) {
          fVar13 = 0.0;
        }
        if (fVar13 <= fVar14) goto code_r0x000181cffed6;
        if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        *(int *)(lVar6 + 0x1c) = *(int *)(lVar6 + 0x1c) + 1;
        iVar2 = *(int *)(lVar6 + 0x18);
        *(undefined4 *)(lVar6 + 0x18) = 0;
        fVar14 = fVar13;
        if (0 < iVar2) {
          Array_Clear(*(undefined8 *)(lVar6 + 0x10),0,iVar2,0);
        }
        goto LAB_181cfff20;
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
      if (lVar6 == 0) goto LAB_181d00630;
      if (*(int *)(lVar6 + 0x18) < 2) {
        if (*(int *)(lVar6 + 0x18) != 1) goto LAB_181d00065;
        lVar6 = *(longlong *)(lVar6 + 0x10);
        if (lVar6 == 0) goto LAB_181d00630;
        if (*(int *)(lVar6 + 0x18) == 0) goto LAB_181d006d7;
        lVar8 = *(longlong *)(param_1 + 0xb8);
        if (lVar8 == 0) goto LAB_181d00630;
        lVar10 = *(longlong *)(lVar6 + 0x20);
      }
      else {
        if ((*(longlong *)(param_1 + 0x68) == 0) ||
           (lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x158), lVar8 == 0))
        goto LAB_181d00630;
        uVar5 = JavaRandom_nextInt(lVar8,*(undefined4 *)(lVar6 + 0x18),0);
        if (*(uint *)(lVar6 + 0x18) <= uVar5) goto LAB_181d006dd;
        lVar6 = *(longlong *)(lVar6 + 0x10);
        if (lVar6 == 0) goto LAB_181d00630;
        if (*(uint *)(lVar6 + 0x18) <= uVar5) goto LAB_181d006d7;
        lVar8 = *(longlong *)(param_1 + 0xb8);
        if (lVar8 == 0) goto LAB_181d00630;
        lVar10 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar5 * 8);
      }
      FUN_1836f5f00(lVar8,lVar10,DAT_1858107d8);
      goto LAB_181d00065;
    }
    if (iVar2 == 1) {
      lVar6 = *(longlong *)(lVar8 + 0x10);
      if (lVar6 == 0) goto LAB_181d00630;
      if (*(int *)(lVar6 + 0x18) == 0) goto LAB_181d006d7;
      if (*(longlong *)(param_1 + 0xb8) == 0) goto LAB_181d00630;
      lVar10 = *(longlong *)(lVar6 + 0x20);
      FUN_1836f5f00(*(longlong *)(param_1 + 0xb8),lVar10,DAT_1858107d8);
    }
LAB_181d00065:
    if (*(int *)(lVar7 + 0x18) == 1) {
      lVar6 = *(longlong *)(lVar7 + 0x10);
      if (lVar6 == 0) goto LAB_181d00630;
      if (*(int *)(lVar6 + 0x18) == 0) {
LAB_181d006d7:
                    /* WARNING: Subroutine does not return */
        FUN_180352460();
      }
      lVar10 = *(longlong *)(lVar6 + 0x20);
    }
    if (lVar10 == 0) {
      uVar1 = *(undefined4 *)(lVar7 + 0x18);
      lVar6 = FUN_1803a51b0(DAT_1857a9428);
      FUN_182d09bb0(lVar6,uVar1,DAT_1857b3b70);
      local_d0 = 0;
      uStack_f0._4_4_ = *(int *)(lVar7 + 0x1c);
      uStack_f0._0_4_ = 0;
      local_e8 = 0;
      local_e0 = 0;
      plStack_d8 = &local_f8;
      fVar14 = 0.0;
      local_f8 = lVar7;
      local_c8 = lVar6;
LAB_181d00100:
      if (local_f8 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if ((uStack_f0._4_4_ == *(int *)(local_f8 + 0x1c)) &&
         ((uint)uStack_f0 < *(uint *)(local_f8 + 0x18))) {
        lVar7 = *(longlong *)(local_f8 + 0x10);
        if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(uint *)(lVar7 + 0x18) <= (uint)uStack_f0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        lVar7 = *(longlong *)(lVar7 + 0x20 + (longlong)(int)(uint)uStack_f0 * 8);
        uStack_f0._0_4_ = (uint)uStack_f0 + 1;
        local_e8 = lVar7;
        if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        fVar13 = *(float *)(lVar7 + 0x20c);
        fVar12 = (float)BattleUnitController_GetStatValue
                                  (lVar7,CONCAT71((uint7)(uint3)((uint)uStack_f0._4_4_ >> 8),5),0);
        if (fVar12 < 0.0) {
          fVar12 = 0.0;
        }
        fVar13 = fVar12 * fVar11 + fVar13;
        if (fVar13 <= fVar14) goto code_r0x000181d00189;
        if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        *(int *)(lVar6 + 0x1c) = *(int *)(lVar6 + 0x1c) + 1;
        iVar2 = *(int *)(lVar6 + 0x18);
        *(undefined4 *)(lVar6 + 0x18) = 0;
        if (0 < iVar2) {
          Array_Clear(*(undefined8 *)(lVar6 + 0x10),0);
        }
        goto LAB_181d001d3;
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
      if (lVar6 == 0) goto LAB_181d00630;
      if (1 < *(int *)(lVar6 + 0x18)) {
        uVar1 = *(undefined4 *)(lVar6 + 0x18);
        lVar7 = FUN_1803a51b0(DAT_1857a9428);
        FUN_182d09bb0(lVar7,uVar1,DAT_1857b3b70);
        local_d0 = 0;
        uStack_f0._4_4_ = *(int *)(lVar6 + 0x1c);
        uStack_f0._0_4_ = 0;
        local_e8 = 0;
        local_e0 = 0;
        plStack_d8 = &local_f8;
        fVar14 = 0.0;
        local_f8 = lVar6;
        local_c8 = lVar7;
        goto LAB_181d002f0;
      }
      if (*(int *)(lVar6 + 0x18) == 0) goto LAB_181d006dd;
      lVar6 = *(longlong *)(lVar6 + 0x10);
      goto LAB_181d00456;
    }
LAB_181d004bd:
    if ((*(longlong *)(param_1 + 0x68) != 0) &&
       (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x228), lVar6 != 0)) {
      cVar4 = FeatureLockController_IsFeatureLocked(lVar6,0x4b,0,0);
      if (cVar4 == '\0') {
        lVar7 = FUN_1803a51b0(DAT_1857f5d58);
        lVar6 = DAT_1857f5d50;
        pcVar9 = *(code **)(DAT_1857f5d50 + 8);
        *(code **)(lVar7 + 0x10) = pcVar9;
        *(longlong *)(lVar7 + 0x28) = lVar6;
        *(longlong *)(lVar7 + 0x20) = param_1;
        cVar4 = *(char *)(lVar6 + 0x52);
        *(longlong *)(lVar7 + 0x40) = lVar7;
        if (((*(byte *)(lVar6 + 0x4c) & 0x10) == 0) || (cVar4 != '\x02')) {
          *(longlong *)(lVar7 + 0x40) = param_1;
        }
        else {
          pcVar9 = FUN_180132840;
        }
        *(code **)(lVar7 + 0x18) = pcVar9;
        *(code **)(lVar7 + 0x38) = FUN_180168940;
        lVar6 = DAT_1857f5d40;
        if (local_c0 == 0) goto LAB_181d00630;
        if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
          ThrowHelper_ThrowArgumentNullException_2(0x22,0);
        }
        if (1 < *(int *)(local_c0 + 0x18)) {
          uVar3 = *(undefined8 *)(local_c0 + 0x10);
          uVar1 = *(undefined4 *)(local_c0 + 0x18);
          lVar8 = *(longlong *)(*(longlong *)(*(longlong *)(DAT_1857f5d40 + 0x20) + 0xc0) + 0x1c8);
          if ((*(byte *)(lVar8 + 0x135) & 1) == 0) {
            lVar8 = FUN_18034f1d0(lVar8);
          }
          if (*(int *)(lVar8 + 0xe4) == 0) {
            FUN_180359940(lVar8);
          }
          FUN_182cfa370(uVar3,0,uVar1,lVar7,
                        *(undefined8 *)(*(longlong *)(*(longlong *)(lVar6 + 0x20) + 0xc0) + 0x1c0));
        }
        *(int *)(local_c0 + 0x1c) = *(int *)(local_c0 + 0x1c) + 1;
        BattleUnitsController_SetVisualTurnOrder(param_1,param_2,lVar10,local_c0,0);
      }
      BattleUnitsController_AdvanceUnitSpeedFill(param_1,fVar11,0);
      return lVar10;
    }
  }
LAB_181d00630:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
code_r0x000181cffed6:
  cVar4 = FUN_180c75680(fVar13,fVar14);
  if (cVar4 != '\0') {
    if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
LAB_181cfff20:
    FUN_18000d200(lVar6,lVar8);
  }
  goto LAB_181cffe60;
code_r0x000181d00189:
  cVar4 = FUN_180c75680(fVar13,fVar14);
  if (cVar4 != '\0') {
    fVar13 = fVar14;
    if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
LAB_181d001d3:
    FUN_18000d200(lVar6,lVar7);
    fVar14 = fVar13;
  }
  goto LAB_181d00100;
LAB_181d002f0:
  if (local_f8 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  if ((uStack_f0._4_4_ == *(int *)(local_f8 + 0x1c)) &&
     ((uint)uStack_f0 < *(uint *)(local_f8 + 0x18))) {
    lVar6 = *(longlong *)(local_f8 + 0x10);
    if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(uint *)(lVar6 + 0x18) <= (uint)uStack_f0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352460();
    }
    lVar6 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)(uint)uStack_f0 * 8);
    uStack_f0._0_4_ = (uint)uStack_f0 + 1;
    local_e8 = lVar6;
    if (lVar6 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    fVar13 = (float)BattleUnitController_GetStatValue
                              (lVar6,CONCAT71((uint7)(uint3)((uint)uStack_f0._4_4_ >> 8),5),0);
    if (fVar13 < 0.0) {
      fVar13 = 0.0;
    }
    if (fVar13 <= fVar14) goto code_r0x000181d00366;
    if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    *(int *)(lVar7 + 0x1c) = *(int *)(lVar7 + 0x1c) + 1;
    iVar2 = *(int *)(lVar7 + 0x18);
    *(undefined4 *)(lVar7 + 0x18) = 0;
    if (0 < iVar2) {
      Array_Clear(*(undefined8 *)(lVar7 + 0x10),0,iVar2,0);
    }
    goto LAB_181d003b0;
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
  if (lVar7 == 0) goto LAB_181d00630;
  if (*(int *)(lVar7 + 0x18) < 2) {
    if (*(int *)(lVar7 + 0x18) != 0) {
      lVar6 = *(longlong *)(lVar7 + 0x10);
LAB_181d00456:
      local_e8 = 0;
      if (lVar6 == 0) goto LAB_181d00630;
      if (*(int *)(lVar6 + 0x18) == 0) goto LAB_181d006d7;
      lVar10 = *(longlong *)(lVar6 + 0x20);
      goto LAB_181d004bd;
    }
    goto LAB_181d006dd;
  }
  if ((*(longlong *)(param_1 + 0x68) == 0) ||
     (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x158), lVar6 == 0)) goto LAB_181d00630;
  uVar5 = JavaRandom_nextInt(lVar6,*(undefined4 *)(lVar7 + 0x18),0);
  if (*(uint *)(lVar7 + 0x18) <= uVar5) {
LAB_181d006dd:
                    /* WARNING: Subroutine does not return */
    ThrowHelper_ThrowArgumentOutOfRange_IndexException(0);
  }
  lVar6 = *(longlong *)(lVar7 + 0x10);
  if (lVar6 == 0) goto LAB_181d00630;
  if (*(uint *)(lVar6 + 0x18) <= uVar5) goto LAB_181d006d7;
  lVar10 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar5 * 8);
  goto LAB_181d004bd;
code_r0x000181d00366:
  cVar4 = FUN_180c75680(fVar13,fVar14);
  if (cVar4 != '\0') {
    fVar13 = fVar14;
    if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
LAB_181d003b0:
    FUN_18000d200(lVar7,lVar6);
    fVar14 = fVar13;
  }
  goto LAB_181d002f0;
}



// ==== DetermineTicksToNextAttack @ 0x181d006f0 ====


float BattleUnitsController_DetermineTicksToNextAttack(longlong param_1,longlong param_2)

{
  uint uVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  longlong *plVar7;
  undefined8 uVar8;
  longlong lVar9;
  uint uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined4 uStackX_c;
  undefined4 local_88;
  undefined4 local_84;
  float local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined8 local_70;
  undefined1 local_68;
  
  if (DAT_185a3f0c0 == '\0') {
    FUN_18035da50(&DAT_1857a38e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857fdd48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582cbd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9430);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cc0);
    LOCK();
    UNLOCK();
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a3f0c0 = '\x01';
  }
  lVar3 = *(longlong *)(param_1 + 0x98);
  if (lVar3 != 0) {
    uVar10 = 0;
    fVar13 = DAT_18529f3f4;
    do {
      if (*(int *)(lVar3 + 0x18) <= (int)uVar10) {
        return fVar13;
      }
      lVar3 = *(longlong *)(param_1 + 0x98);
      if (lVar3 == 0) break;
      if (*(uint *)(lVar3 + 0x18) <= uVar10) {
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowArgumentOutOfRange_IndexException(0);
      }
      lVar3 = *(longlong *)(lVar3 + 0x10);
      if (lVar3 == 0) break;
      if (*(uint *)(lVar3 + 0x18) <= uVar10) {
LAB_181d00c20:
                    /* WARNING: Subroutine does not return */
        FUN_180352460();
      }
      lVar3 = *(longlong *)(lVar3 + 0x20 + (longlong)(int)uVar10 * 8);
      if (lVar3 == 0) break;
      fVar11 = (float)BattleUnitController_GetStatValue(lVar3,5,0);
      if (((0.0 < fVar11) && (*(char *)(lVar3 + 0x1e8) == '\0')) && (0 < *(int *)(lVar3 + 0xc0))) {
        if (*(int *)(lVar3 + 0xd8) == 1) {
          if ((*(longlong *)(param_1 + 0x68) == 0) ||
             (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x1e8), lVar4 == 0)) break;
          lVar4 = GameConstants_GetBattleDeadUnitLoggingIgnoreList(lVar4,0);
          if ((*(longlong *)(lVar3 + 0x98) == 0) ||
             ((lVar9 = *(longlong *)(*(longlong *)(lVar3 + 0x98) + 0x40), lVar9 == 0 || (lVar4 == 0)
              ))) break;
          cVar2 = FUN_1835eadf0(lVar4,*(undefined8 *)(lVar9 + 0x58),DAT_18582cbd8);
          if (cVar2 == '\0') {
            uVar5 = BattleUnitsController_GetUnitIdLogString(*(undefined8 *)(param_1 + 0x88),0);
            uVar6 = BattleUnitsController_GetUnitIdLogString(*(undefined8 *)(param_1 + 0x90),0);
            if ((*(longlong *)(param_1 + 0x68) == 0) ||
               (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x10) == 0)) break;
            plVar7 = (longlong *)FUN_180014310();
            if (*(int *)(DAT_1857fdd48 + 0xe4) == 0) {
              FUN_180359940();
            }
            if (DAT_185a3b4d5 == '\0') {
              FUN_18035da50(&DAT_1857fdd48);
              LOCK();
              UNLOCK();
              DAT_185a3b4d5 = '\x01';
            }
            if (*(int *)(DAT_1857fdd48 + 0xe4) == 0) {
              FUN_180359940();
            }
            if (plVar7 == (longlong *)0x0) {
              if ((*(longlong *)(param_1 + 0x68) == 0) ||
                 (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x10) == 0)) break;
              local_68 = FUN_1800084f0(0x17,DAT_1857a3928);
              local_78 = DAT_1857a38e0;
              local_70 = 0xffffffffffffffff;
              uVar8 = Enum_ToString_3(&local_78,0);
            }
            else {
              uVar8 = (**(code **)(*plVar7 + 0x168))(plVar7,*(undefined8 *)(*plVar7 + 0x170));
            }
            if ((*(longlong *)(lVar3 + 0x98) == 0) ||
               (lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x98) + 0x40), lVar4 == 0)) break;
            FUN_182b71b90(param_1,DAT_1857f5cb0,*(undefined8 *)(lVar4 + 0x58),uVar5,uVar6,uVar8,
                          0xffffffff,DAT_1857f5cc0);
            if ((*(longlong *)(param_1 + 0x68) == 0) ||
               (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x180) == 0)) break;
            FUN_180005650(0x6d,DAT_1857a96a0);
            if ((*(longlong *)(param_1 + 0x68) == 0) ||
               (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x1c0), lVar4 == 0)) break;
            *(undefined1 *)(lVar4 + 0x162) = 1;
          }
        }
        if ((*(longlong *)(param_1 + 0x68) == 0) ||
           (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x1e8), lVar4 == 0)) break;
        fVar11 = *(float *)(lVar4 + 0x24b4) - *(float *)(lVar3 + 0x208);
        if (fVar11 < 0.0) {
          fVar11 = 0.0;
        }
        fVar12 = (float)BattleUnitController_GetStatValue(lVar3,5,0);
        if (fVar12 < 0.0) {
          fVar12 = 0.0;
        }
        fVar11 = fVar11 / fVar12;
        if (fVar11 < fVar13) {
          fVar13 = fVar11;
        }
        if (param_2 == 0) break;
        *(int *)(param_2 + 0x1c) = *(int *)(param_2 + 0x1c) + 1;
        lVar4 = *(longlong *)(param_2 + 0x10);
        if (lVar4 == 0) break;
        uVar1 = *(uint *)(param_2 + 0x18);
        lVar9 = (longlong)(int)uVar1;
        uStackX_c = (undefined4)((ulonglong)lVar3 >> 0x20);
        if (uVar1 < *(uint *)(lVar4 + 0x18)) {
          *(uint *)(param_2 + 0x18) = uVar1 + 1;
          if (*(uint *)(lVar4 + 0x18) <= uVar1) goto LAB_181d00c20;
          *(undefined4 *)(lVar4 + 0x24 + lVar9 * 0x10) = uStackX_c;
          *(float *)(lVar4 + 0x28 + lVar9 * 0x10) = fVar11;
          *(undefined4 *)(lVar4 + 0x2c + lVar9 * 0x10) = 0;
          *(int *)(lVar4 + 0x20 + lVar9 * 0x10) = (int)lVar3;
        }
        else {
          local_84 = uStackX_c;
          local_7c = 0;
          local_88 = (int)lVar3;
          local_80 = fVar11;
          FUN_182d0a280(param_2,&local_88);
        }
      }
      lVar3 = *(longlong *)(param_1 + 0x98);
      uVar10 = uVar10 + 1;
    } while (lVar3 != 0);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== PopulateNextUnitCandidateList @ 0x181d00ee0 ====


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void BattleUnitsController_PopulateNextUnitCandidateList
               (longlong param_1,longlong param_2,float param_3,longlong param_4,longlong param_5)

{
  longlong lVar1;
  undefined8 uVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  uint uVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  uint uStack_98;
  float fStack_88;
  
  if (DAT_185a3f0c2 == '\0') {
    FUN_18035da50(&DAT_1857f5d10);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d08);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580ce58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580cea0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a4808);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8fc0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5cf8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580cf58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9430);
    LOCK();
    UNLOCK();
    DAT_185a3f0c2 = '\x01';
  }
  uVar6 = _DAT_18529fe30;
  fVar8 = DAT_18529f218;
  fVar11 = DAT_18529ec30;
  if (param_2 != 0) {
    iVar4 = *(int *)(param_2 + 0x1c);
    uStack_98 = 0;
    while( true ) {
      if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if ((iVar4 != *(int *)(param_2 + 0x1c)) || (*(uint *)(param_2 + 0x18) <= uStack_98)) break;
      lVar1 = *(longlong *)(param_2 + 0x10);
      if (lVar1 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      lVar5 = (longlong)(int)uStack_98;
      if (*(uint *)(lVar1 + 0x18) <= uStack_98) {
                    /* WARNING: Subroutine does not return */
        FUN_180352460();
      }
      uVar2 = *(undefined8 *)(lVar1 + (lVar5 + 2) * 0x10);
      uStack_98 = uStack_98 + 1;
      fStack_88 = (float)*(undefined8 *)(lVar1 + 8 + (lVar5 + 2) * 0x10);
      if (DAT_185a33caf == '\0') {
        FUN_18035da50(&DAT_185827160);
        LOCK();
        UNLOCK();
        DAT_185a33caf = '\x01';
      }
      fVar7 = (float)((uint)fStack_88 & uVar6);
      if ((float)((uint)fStack_88 & uVar6) <= (float)((uint)param_3 & uVar6)) {
        fVar7 = (float)((uint)param_3 & uVar6);
      }
      fVar9 = **(float **)(DAT_185827160 + 0xb8) * fVar8;
      fVar10 = fVar7 * fVar11;
      if (fVar7 * fVar11 <= fVar9) {
        fVar10 = fVar9;
      }
      if ((float)((uint)(param_3 - fStack_88) & uVar6) < fVar10) {
        if (param_4 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        FUN_18000d200(param_4,uVar2);
      }
    }
    if ((*(byte *)(*(longlong *)(DAT_1857f5d08 + 0x20) + 0x135) & 1) == 0) {
      FUN_18034f1d0();
    }
    if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (iVar4 != *(int *)(param_2 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
      ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
    }
    if (*(longlong *)(param_1 + 0xb8) != 0) {
      uVar6 = *(int *)(*(longlong *)(param_1 + 0xb8) + 0x18) - 1;
      if (-1 < (int)uVar6) {
        fVar11 = 0.0;
        do {
          lVar1 = *(longlong *)(param_1 + 0xb8);
          if (lVar1 == 0) goto LAB_181d013a2;
          if (*(uint *)(lVar1 + 0x18) <= uVar6) {
                    /* WARNING: Subroutine does not return */
            ThrowHelper_ThrowArgumentOutOfRange_IndexException(0);
          }
          lVar1 = *(longlong *)(lVar1 + 0x10);
          if (lVar1 == 0) goto LAB_181d013a2;
          if (*(uint *)(lVar1 + 0x18) <= uVar6) {
                    /* WARNING: Subroutine does not return */
            FUN_180352460();
          }
          if (param_4 == 0) goto LAB_181d013a2;
          if (*(int *)(param_4 + 0x18) == 0) {
LAB_181d0131e:
            if (*(longlong *)(param_1 + 0xb8) == 0) goto LAB_181d013a2;
            FUN_1836f61e0(*(longlong *)(param_1 + 0xb8),(ulonglong)uVar6);
          }
          else {
            lVar1 = *(longlong *)(lVar1 + 0x20 + (ulonglong)uVar6 * 8);
            iVar4 = FUN_182c408a0(*(undefined8 *)(param_4 + 0x10),lVar1,0,
                                  *(undefined4 *)(param_4 + 0x18),
                                  *(undefined8 *)
                                   (*(longlong *)
                                     (*(longlong *)
                                       (*(longlong *)
                                         (*(longlong *)(*(longlong *)(DAT_1857a8fc0 + 0x20) + 0xc0)
                                         + 0xd0) + 0x20) + 0xc0) + 0x158));
            if (iVar4 == -1) goto LAB_181d0131e;
            if (lVar1 == 0) goto LAB_181d013a2;
            fVar8 = *(float *)(lVar1 + 0x20c);
            fVar7 = (float)BattleUnitController_GetStatValue(lVar1,5);
            if (fVar7 < 0.0) {
              fVar7 = 0.0;
            }
            fVar8 = fVar7 * param_3 + fVar8;
            if (fVar11 < fVar8) {
              if (param_5 == 0) goto LAB_181d013a2;
              *(int *)(param_5 + 0x1c) = *(int *)(param_5 + 0x1c) + 1;
              iVar4 = *(int *)(param_5 + 0x18);
              *(undefined4 *)(param_5 + 0x18) = 0;
              fVar11 = fVar8;
              if (0 < iVar4) {
                Array_Clear(*(undefined8 *)(param_5 + 0x10),0,iVar4,0);
              }
LAB_181d0130a:
              FUN_18000d200(param_5,lVar1);
            }
            else {
              cVar3 = FUN_180c75680(fVar8,fVar11);
              if (cVar3 != '\0') {
                if (param_5 != 0) goto LAB_181d0130a;
                goto LAB_181d013a2;
              }
            }
          }
          uVar6 = uVar6 - 1;
        } while (-1 < (int)uVar6);
      }
      return;
    }
  }
LAB_181d013a2:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== AdvanceUnitSpeedFill @ 0x181d013b0 ====


void BattleUnitsController_AdvanceUnitSpeedFill(longlong param_1,float param_2)

{
  short sVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  float fVar8;
  float fVar9;
  longlong local_e8;
  undefined8 uStack_e0;
  longlong local_d8;
  longlong local_d0;
  longlong lStack_c8;
  longlong local_c0;
  longlong local_b8;
  undefined4 uStack_b0;
  uint uStack_ac;
  undefined8 local_a0;
  longlong *plStack_98;
  undefined8 local_90;
  undefined8 local_88;
  longlong *local_80;
  undefined8 local_78;
  longlong *local_70;
  
  if (DAT_185a3f0c3 == '\0') {
    FUN_18035da50(&DAT_185806bf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806bc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806bb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806ba8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806e58);
    LOCK();
    UNLOCK();
    DAT_185a3f0c3 = '\x01';
  }
  local_e8 = 0;
  uStack_e0 = 0;
  local_d8 = 0;
  local_d0 = 0;
  lStack_c8 = 0;
  local_c0 = 0;
  lVar2 = *(longlong *)(param_1 + 0x98);
  if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  local_90 = 0;
  uStack_e0._4_4_ = *(int *)(lVar2 + 0x1c);
  uStack_e0._0_4_ = 0;
  local_d8 = 0;
  local_a0 = 0;
  plStack_98 = &local_e8;
  local_e8 = lVar2;
  while( true ) {
    if (local_e8 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if ((uStack_e0._4_4_ != *(int *)(local_e8 + 0x1c)) ||
       (*(uint *)(local_e8 + 0x18) <= (uint)uStack_e0)) {
      if ((*(byte *)(*(longlong *)(DAT_185806eb8 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(DAT_185806eb8 + 0x20));
      }
      if (local_e8 != 0) {
        if (uStack_e0._4_4_ == *(int *)(local_e8 + 0x1c)) {
          return;
        }
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
      }
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    lVar2 = *(longlong *)(local_e8 + 0x10);
    if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_e0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352460();
    }
    lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)(uint)uStack_e0 * 8);
    uStack_e0._0_4_ = (uint)uStack_e0 + 1;
    local_d8 = lVar2;
    if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if ((*(char *)(lVar2 + 0x1e8) == '\0') && (0 < *(int *)(lVar2 + 0xc0))) {
      BattleUnitController_GetStatValue
                (lVar2,CONCAT71((uint7)(uint3)((uint)uStack_e0._4_4_ >> 8),5),0);
      BattleUnitController_SetSpeedFill(lVar2);
      fVar9 = *(float *)(lVar2 + 0x20c);
      fVar8 = (float)BattleUnitController_GetStatValue(lVar2,5,0);
      if (fVar8 < 0.0) {
        fVar8 = 0.0;
      }
      *(float *)(lVar2 + 0x20c) = fVar8 * param_2 + fVar9;
    }
    lVar3 = *(longlong *)(lVar2 + 0x100);
    if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    uStack_b0 = 0;
    uStack_ac = *(uint *)(lVar3 + 0x1c);
    lStack_c8 = (ulonglong)uStack_ac << 0x20;
    local_c0 = 0;
    local_88 = 0;
    local_80 = &local_d0;
    local_d0 = lVar3;
    local_b8 = lVar3;
    while (cVar5 = FUN_1833f1b60(&local_d0,DAT_185806bc8), lVar3 = local_c0, cVar5 != '\0') {
      if (local_c0 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      lVar4 = *(longlong *)(local_c0 + 0x38);
      if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (*(longlong *)(lVar4 + 0x70) == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (*(char *)(*(longlong *)(lVar4 + 0x70) + 0x13) == '\x01') {
        if (*(longlong *)(lVar2 + 0x60) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        sVar1 = *(short *)(*(longlong *)(lVar4 + 0x70) + 0x1a);
        fVar9 = (float)BattlePersistentEffectsController_GetModifyCooldownAmount
                                 (*(longlong *)(lVar2 + 0x60),local_c0,1,0);
        if (*(longlong *)(lVar3 + 0x38) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x38) + 0x70);
        if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(char *)(lVar4 + 0x13) == '\x01') {
          iVar7 = *(int *)(lVar3 + 0x14) - (int)(((float)(int)sVar1 + fVar9) * param_2);
          if (((iVar7 < 0) ||
              (iVar6 = (int)*(short *)(*(longlong *)(lVar3 + 0x38) + 0x18), iVar7 <= iVar6)) &&
             (iVar6 = iVar7, iVar7 < 0)) {
            iVar6 = 0;
          }
          *(int *)(lVar3 + 0x14) = iVar6;
        }
      }
    }
    if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    lVar3 = *(longlong *)(lVar2 + 0x110);
    if (lVar3 == 0) break;
    uStack_b0 = 0;
    uStack_ac = *(uint *)(lVar3 + 0x1c);
    lStack_c8 = (ulonglong)uStack_ac << 0x20;
    local_c0 = 0;
    local_78 = 0;
    local_70 = &local_d0;
    local_d0 = lVar3;
    local_b8 = lVar3;
    while (cVar5 = FUN_1833f1b60(&local_d0,DAT_185806bc8), lVar3 = local_c0, cVar5 != '\0') {
      if (local_c0 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      lVar4 = *(longlong *)(local_c0 + 0x38);
      if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (*(longlong *)(lVar4 + 0x70) == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (*(char *)(*(longlong *)(lVar4 + 0x70) + 0x13) == '\x01') {
        if (*(longlong *)(lVar2 + 0x60) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        sVar1 = *(short *)(*(longlong *)(lVar4 + 0x70) + 0x1a);
        fVar9 = (float)BattlePersistentEffectsController_GetModifyCooldownAmount
                                 (*(longlong *)(lVar2 + 0x60),local_c0,1,0);
        if (*(longlong *)(lVar3 + 0x38) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x38) + 0x70);
        if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(char *)(lVar4 + 0x13) == '\x01') {
          iVar7 = *(int *)(lVar3 + 0x14) - (int)(((float)(int)sVar1 + fVar9) * param_2);
          if (((iVar7 < 0) ||
              (iVar6 = (int)*(short *)(*(longlong *)(lVar3 + 0x38) + 0x18), iVar7 <= iVar6)) &&
             (iVar6 = iVar7, iVar7 < 0)) {
            iVar6 = 0;
          }
          *(int *)(lVar3 + 0x14) = iVar6;
        }
      }
    }
    BattleUnitController_OnUnitCooldownsChanged(lVar2,0);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== SortTurnOrder @ 0x181cff960 ====


int BattleUnitsController_SortTurnOrder(undefined8 param_1,longlong *param_2,longlong *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  
  if (DAT_185a3f0be == '\0') {
    FUN_18035da50(&DAT_18580ce58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580cea0);
    LOCK();
    UNLOCK();
    DAT_185a3f0be = '\x01';
  }
  fVar1 = *(float *)(param_2 + 1);
  fVar3 = *(float *)(param_3 + 1);
  if (fVar1 < fVar3) {
    return -1;
  }
  if (fVar3 < fVar1) {
    return 1;
  }
  if (fVar1 != fVar3) {
    if ((uint)ABS(fVar1) < 0x7f800001) {
      return 1;
    }
    if ((uint)ABS(fVar3) < 0x7f800001) {
      return -1;
    }
  }
  if ((*param_3 != 0) && (*param_2 != 0)) {
    fVar1 = *(float *)(*param_3 + 0x20c);
    fVar3 = *(float *)(*param_2 + 0x20c);
    if (fVar1 < fVar3) {
      return -1;
    }
    if (fVar3 < fVar1) {
      return 1;
    }
    if (fVar1 != fVar3) {
      if ((uint)ABS(fVar1) < 0x7f800001) {
        return 1;
      }
      if ((uint)ABS(fVar3) < 0x7f800001) {
        return -1;
      }
    }
    if (*param_3 != 0) {
      fVar1 = (float)BattleUnitController_GetStatValue(*param_3,5,0);
      if (*param_2 != 0) {
        fVar3 = 0.0;
        if (0.0 <= fVar1) {
          fVar3 = fVar1;
        }
        fVar2 = (float)BattleUnitController_GetStatValue(*param_2,5,0);
        fVar1 = 0.0;
        if (0.0 <= fVar2) {
          fVar1 = fVar2;
        }
        if (fVar3 < fVar1) {
          return -1;
        }
        if (fVar3 <= fVar1) {
          if (fVar3 == fVar1) {
            return 0;
          }
          if (0x7f800000 < (uint)ABS(fVar3)) {
            return (0x7f800000 < (uint)ABS(fVar1)) - 1;
          }
        }
        return 1;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== GiveUnitTurnPriority @ 0x181cff800 ====


void BattleUnitsController_GiveUnitTurnPriority(longlong param_1,undefined8 param_2)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  if (DAT_185a3f0bd == '\0') {
    FUN_18035da50(&DAT_1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185810980);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9648);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d20);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857f5d28);
    LOCK();
    UNLOCK();
    DAT_185a3f0bd = '\x01';
  }
  lVar4 = FUN_1803a51b0(DAT_1857f5d28);
  if (lVar4 != 0) {
    *(undefined8 *)(lVar4 + 0x10) = param_2;
    lVar2 = *(longlong *)(param_1 + 0xb8);
    uVar5 = FUN_1803a51b0(DAT_1857a9648);
    FUN_180e639d0(uVar5,lVar4,DAT_1857f5d20,0);
    if (lVar2 != 0) {
      FUN_1836f60b0(lVar2,uVar5,DAT_185810980);
      lVar3 = DAT_1857a9420;
      lVar2 = *(longlong *)(param_1 + 0xb8);
      if (lVar2 != 0) {
        uVar5 = *(undefined8 *)(lVar4 + 0x10);
        *(int *)(lVar2 + 0x1c) = *(int *)(lVar2 + 0x1c) + 1;
        lVar4 = *(longlong *)(lVar2 + 0x10);
        if (lVar4 != 0) {
          uVar1 = *(uint *)(lVar2 + 0x18);
          if (uVar1 < *(uint *)(lVar4 + 0x18)) {
            *(uint *)(lVar2 + 0x18) = uVar1 + 1;
            if (*(uint *)(lVar4 + 0x18) <= uVar1) {
                    /* WARNING: Subroutine does not return */
              FUN_180352460();
            }
            *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)uVar1 * 8) = uVar5;
          }
          else {
            FUN_1836f50c0(lVar2,uVar5,
                          *(undefined8 *)(*(longlong *)(*(longlong *)(lVar3 + 0x20) + 0xc0) + 0x70))
            ;
          }
          return;
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

