// ==== BattleUnitController.ApplyAttackDamage @ 0x181bc50b0 ====

void BattleUnitController_ApplyAttackDamage
               (longlong param_1,int param_2,int param_3,char param_4,undefined8 param_5,
               undefined8 param_6,undefined8 param_7,char param_8,undefined1 param_9,byte param_10,
               undefined8 param_11,char param_12)

{
  undefined1 uVar1;
  longlong lVar2;
  longlong lVar3;
  ulonglong uVar4;
  char cVar5;
  byte bVar6;
  undefined4 uVar7;
  int iVar8;
  uint uVar9;
  longlong lVar10;
  ulonglong uVar11;
  longlong lVar12;
  int iVar13;
  int iVar14;
  undefined4 uVar15;
  bool bVar16;
  float fVar17;
  undefined4 uVar18;
  ulonglong in_stack_ffffffffffffff98;
  undefined8 uVar19;
  undefined8 in_stack_ffffffffffffffa0;
  undefined8 *puVar20;
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar22;
  undefined8 uVar21;
  
  uVar18 = (undefined4)((ulonglong)in_stack_ffffffffffffffa0 >> 0x20);
  uVar22 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  if (DAT_185a3e927 == '\0') {
    FUN_18035da50(&DAT_185766520);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185820fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a9430);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185831418);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185831430);
    LOCK();
    UNLOCK();
    DAT_185a3e927 = '\x01';
  }
  if (DAT_185a3e95f == '\0') {
    FUN_18035da50(&DAT_185820fd8);
    LOCK();
    UNLOCK();
    DAT_185a3e95f = '\x01';
  }
  fVar17 = (float)param_5;
  lVar3 = CONCAT44(param_11._4_4_,(int)param_11);
  if (lVar3 == 0) {
LAB_181bc51fd:
    BattleUnitController_ConditionTriggered_BattleDirected
              (param_1,0x35,CONCAT44(param_5._4_4_,(float)param_5),0);
  }
  else {
    lVar12 = *(longlong *)(lVar3 + 0x38);
    if ((lVar12 == 0) || (lVar10 = *(longlong *)(lVar12 + 0xb8), lVar10 == 0)) goto LAB_181bc59d8;
    if (*(int *)(lVar10 + 0x18) < 1) goto LAB_181bc51fd;
    if (lVar12 == 0) goto LAB_181bc59d8;
    in_stack_ffffffffffffff98 = 0;
    BattleUnitController_ConditionTriggered_BattleDirectedTags
              (param_1,0x35,CONCAT44(param_5._4_4_,(float)param_5),lVar10,0);
  }
  lVar12 = CONCAT44(param_5._4_4_,fVar17);
  param_11._0_4_ = *(int *)(param_1 + 0xc0);
  *(undefined1 *)(param_1 + 0x1e9) = 0;
  if (param_4 == '\0') {
    if (param_8 == '\x01') {
      uVar15 = 3;
    }
    else {
      uVar15 = 0;
      if (param_8 == '\x02') {
        uVar15 = 4;
      }
    }
  }
  else {
    uVar15 = 2;
  }
  iVar14 = 0;
  if (-1 < *(int *)(param_1 + 0xc0)) {
    iVar14 = *(int *)(param_1 + 0xc0);
  }
  fVar17 = (float)BattleUnitController_GetStatValue(param_1,1,0);
  iVar13 = (int)fVar17;
  if (iVar14 < (int)fVar17) {
    iVar13 = iVar14;
  }
  if (param_2 < 0) {
    if (lVar3 != 0) {
      lVar10 = *(longlong *)(lVar3 + 0x38);
      if (lVar10 == 0) goto LAB_181bc59d8;
      uVar19 = *(undefined8 *)(lVar10 + 0xa0);
      uVar1 = *(undefined1 *)(lVar10 + 0x20);
      if (DAT_185a3e8f5 == '\0') {
        FUN_18035da50(&DAT_1858316b0);
        LOCK();
        UNLOCK();
        DAT_185a3e8f5 = '\x01';
      }
      if (*(longlong *)(param_1 + 0xe8) == 0) goto LAB_181bc59d8;
      uVar7 = FUN_1800084f0(0xc,DAT_1858316b0);
      uVar21 = CONCAT44(uVar18,uVar7);
      in_stack_ffffffffffffff98 = CONCAT71((int7)(in_stack_ffffffffffffff98 >> 8),uVar1);
      FUN_182b70a80(param_1,DAT_185831430,param_2,uVar19,in_stack_ffffffffffffff98,uVar21,
                    CONCAT44(uVar22,0xffffffff),DAT_185831418);
      uVar18 = (undefined4)((ulonglong)uVar21 >> 0x20);
    }
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x180) == 0)) goto LAB_181bc59d8;
    FUN_180005650(0x6d,DAT_1857a96a0);
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1c0), lVar10 == 0))
    goto LAB_181bc59d8;
    param_2 = 1;
    *(undefined1 *)(lVar10 + 0x162) = 1;
    if ((param_12 != '\0') &&
       (fVar17 = (float)BattleUnitController_GetStatValue(param_1,1,0), 0 < (int)fVar17)) {
      fVar17 = (float)BattleUnitController_GetStatValue(param_1,1,0);
      param_2 = (int)fVar17 + 1;
    }
  }
  BattleUnitController_UpdateHealth
            (param_1,-param_2,-param_3,param_9,in_stack_ffffffffffffff98 & 0xffffffffffffff00,
             CONCAT44(uVar18,uVar15),0);
  if (((*(longlong *)(param_1 + 0x88) == 0) ||
      (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1e8), lVar10 == 0)) || (lVar12 == 0)
     ) goto LAB_181bc59d8;
  uVar19 = 0;
  BattleUnitController_SetBlackboardValue(lVar12,*(undefined8 *)(lVar10 + 0x278),lVar3);
  if (lVar3 == 0) {
LAB_181bc547c:
    BattleUnitController_ConditionTriggered_BattleDirected(lVar12,0x3a,param_6,0,uVar19);
  }
  else {
    lVar10 = *(longlong *)(lVar3 + 0x38);
    if ((lVar10 == 0) || (lVar2 = *(longlong *)(lVar10 + 0xb8), lVar2 == 0)) goto LAB_181bc59d8;
    if (*(int *)(lVar2 + 0x18) < 1) goto LAB_181bc547c;
    if (lVar10 == 0) goto LAB_181bc59d8;
    BattleUnitController_ConditionTriggered_BattleDirectedTags(lVar12,0x3a,param_6,lVar2,0);
  }
  param_5._0_4_ = 0.0;
  lVar10 = *(longlong *)(param_1 + 0x60);
  BattleUnitController_GetStatValue(param_1,1,0);
  if (lVar10 == 0) goto LAB_181bc59d8;
  uVar21 = 0;
  puVar20 = &param_5;
  uVar19 = 0x18;
  cVar5 = BattlePersistentEffectsController_TryGetMinStatLimit(lVar10);
  if ((cVar5 != '\0') && (iVar14 = (int)(float)param_5, iVar14 < iVar13)) {
    iVar13 = 0;
    if (-1 < *(int *)(param_1 + 0xc0)) {
      iVar13 = *(int *)(param_1 + 0xc0);
    }
    fVar17 = (float)BattleUnitController_GetStatValue(param_1,1,0);
    iVar8 = (int)fVar17;
    if (iVar13 < (int)fVar17) {
      iVar8 = iVar13;
    }
    if (iVar8 == iVar14) {
      BattleUnitController_ConditionTriggered_2(param_1,0x3d,0);
    }
  }
  if ((*(longlong *)(param_1 + 0x88) == 0) ||
     (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x70), lVar10 == 0))
  goto LAB_181bc59d8;
  cVar5 = *(char *)(lVar10 + 0xa1);
  uVar18 = BattleUnitController_GetStatValue(lVar12,0x3e,0);
  bVar6 = StatUtil_IsStealthValue(uVar18,0);
  if (*(longlong *)(param_1 + 0x88) == 0) goto LAB_181bc59d8;
  lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x240);
  if (cVar5 == '\0') {
    bVar16 = false;
  }
  else {
    if (*(int *)(param_1 + 0xc0) < 1) {
LAB_181bc55c3:
      cVar5 = '\x01';
    }
    else {
      if (*(longlong *)(param_1 + 0x60) == 0) goto LAB_181bc59d8;
      cVar5 = BattlePersistentEffectsController_IsEffectActive
                        (*(longlong *)(param_1 + 0x60),0,0,0,uVar19,puVar20,uVar21);
      if (cVar5 != '\0') goto LAB_181bc55c3;
      cVar5 = *(char *)(param_1 + 0x1e8);
    }
    bVar16 = cVar5 == '\0';
  }
  if ((param_10 & bVar16) == 0) {
    bVar6 = 0;
  }
  else {
    bVar6 = bVar6 ^ 1;
  }
  if ((bVar6 & lVar12 == lVar10) != 0) {
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x158), lVar10 == 0))
    goto LAB_181bc59d8;
    uVar11 = *(ulonglong *)(lVar10 + 0x10);
    do {
      uVar11 = uVar11 * 0x5deece66d + 0xb & 0xffffffffffff;
      uVar4 = (uVar11 >> 0x11) / 100;
      iVar14 = (int)uVar4;
    } while (iVar14 * 100 + 99 < 0);
    *(ulonglong *)(lVar10 + 0x10) = uVar11;
    fVar17 = (float)BattleUnitController_GetStatValue(param_1,CONCAT71((int7)(uVar4 >> 8),0x2a),0);
    if ((float)((int)(uVar11 >> 0x11) + iVar14 * -100) < fVar17 * DAT_18529f324) {
      if (*(longlong *)(param_1 + 0xf0) == 0) goto LAB_181bc59d8;
      lVar10 = BattleTargetingController_GetValidTargets(*(longlong *)(param_1 + 0xf0),lVar12,0);
      cVar5 = FUN_182b0f850(lVar10,DAT_185766520);
      if (cVar5 != '\0') {
        if (((*(longlong *)(param_1 + 0x88) == 0) || (lVar10 == 0)) ||
           (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x158), lVar2 == 0))
        goto LAB_181bc59d8;
        uVar9 = JavaRandom_nextInt(lVar2,*(undefined4 *)(lVar10 + 0x18),0);
        if (*(uint *)(lVar10 + 0x18) <= uVar9) {
                    /* WARNING: Subroutine does not return */
          ThrowHelper_ThrowArgumentOutOfRange_IndexException(0);
        }
        lVar10 = *(longlong *)(lVar10 + 0x10);
        if (lVar10 == 0) goto LAB_181bc59d8;
        if (*(uint *)(lVar10 + 0x18) <= uVar9) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        if (*(longlong *)(param_1 + 0x150) == 0) goto LAB_181bc59d8;
        FUN_18000d200(*(longlong *)(param_1 + 0x150),
                      *(undefined8 *)(lVar10 + 0x20 + (longlong)(int)uVar9 * 8),DAT_1857a9420);
      }
    }
  }
  iVar14 = (int)param_11;
  if ((int)param_11 < 1) {
LAB_181bc5766:
    if (0 < *(int *)(param_1 + 0xc0)) goto LAB_181bc576f;
    if (DAT_185a3e95f == '\0') {
      FUN_18035da50(&DAT_185820fd8);
      LOCK();
      UNLOCK();
      DAT_185a3e95f = '\x01';
    }
    if (lVar3 == 0) {
LAB_181bc5814:
      BattleUnitController_ConditionTriggered_BattleDirected(lVar12,0xf,param_1,0);
    }
    else {
      lVar10 = *(longlong *)(lVar3 + 0x38);
      if ((lVar10 == 0) || (lVar2 = *(longlong *)(lVar10 + 0xb8), lVar2 == 0)) goto LAB_181bc59d8;
      if (*(int *)(lVar2 + 0x18) < 1) goto LAB_181bc5814;
      if (lVar10 == 0) goto LAB_181bc59d8;
      BattleUnitController_ConditionTriggered_BattleDirectedTags(lVar12,0xf,param_1,lVar2,0);
    }
    if (*(longlong *)(lVar12 + 0x10) != 0) {
      lVar10 = *(longlong *)(lVar12 + 0x10);
      (**(code **)(lVar10 + 0x18))
                (*(undefined8 *)(lVar10 + 0x40),lVar12,*(undefined8 *)(lVar10 + 0x28));
    }
    if (*(longlong *)(param_1 + 0x270) == 0) goto LAB_181bc59d8;
    if (*(int *)(*(longlong *)(param_1 + 0x270) + 0x18) < 2) {
      lVar10 = *(longlong *)(param_1 + 0xb8);
      if (lVar10 == 0) goto LAB_181bc59d8;
      if (DAT_185a39f65 == '\0') {
        FUN_18035da50(&DAT_1857ad6e8);
        LOCK();
        UNLOCK();
        DAT_185a39f65 = '\x01';
      }
      if (*(longlong *)(lVar10 + 0x18) != 0) {
        FUN_180005650(7,DAT_1857ad6e8,*(undefined8 *)(lVar10 + 0x18));
      }
    }
    else {
      if (lVar3 != 0) {
        if (*(longlong *)(lVar3 + 0x38) == 0) goto LAB_181bc59d8;
        if (*(char *)(*(longlong *)(lVar3 + 0x38) + 0x11) == '\0') {
          if (*(longlong *)(param_1 + 0xb8) == 0) goto LAB_181bc59d8;
          UnitFsmControllerNullable_PlayHit(*(longlong *)(param_1 + 0xb8),0);
        }
      }
      if ((0 < iVar14) && (*(longlong *)(param_1 + 0x38) != 0)) {
        lVar10 = *(longlong *)(param_1 + 0x38);
        (**(code **)(lVar10 + 0x18))
                  (*(undefined8 *)(lVar10 + 0x40),param_1,*(undefined8 *)(lVar10 + 0x28));
      }
    }
  }
  else {
    if (*(int *)(param_1 + 0xc0) < 1) {
      *(longlong *)(param_1 + 200) = lVar12;
      *(longlong *)(param_1 + 0xd0) = lVar3;
      *(undefined4 *)(param_1 + 0xd8) = 1;
      goto LAB_181bc5766;
    }
LAB_181bc576f:
    if (lVar3 != 0) {
      if (*(longlong *)(lVar3 + 0x38) == 0) goto LAB_181bc59d8;
      if (*(char *)(*(longlong *)(lVar3 + 0x38) + 0x11) == '\0') {
        if (*(longlong *)(param_1 + 0xb8) == 0) goto LAB_181bc59d8;
        UnitFsmControllerNullable_PlayHit(*(longlong *)(param_1 + 0xb8),0);
      }
    }
  }
  if (param_4 != '\0') {
    BattleUnitController_TriggerBattleDirectedTagCondition(param_1,lVar3,lVar12,7,0);
  }
  if ((*(longlong *)(param_1 + 0x88) != 0) &&
     (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x1e8), lVar10 != 0)) {
    uVar19 = 0;
    BattleUnitController_SetBlackboardValue(param_1,*(undefined8 *)(lVar10 + 0x278),lVar3);
    if (DAT_185a3e95f == '\0') {
      FUN_18035da50(&DAT_185820fd8);
      LOCK();
      UNLOCK();
      DAT_185a3e95f = '\x01';
    }
    if (lVar3 == 0) {
LAB_181bc59c6:
      BattleUnitController_ConditionTriggered_BattleDirected(param_1,0xb,lVar12,0,uVar19);
      return;
    }
    lVar3 = *(longlong *)(lVar3 + 0x38);
    if ((lVar3 != 0) && (lVar10 = *(longlong *)(lVar3 + 0xb8), lVar10 != 0)) {
      if (*(int *)(lVar10 + 0x18) < 1) goto LAB_181bc59c6;
      if (lVar3 != 0) {
        BattleUnitController_ConditionTriggered_BattleDirectedTags(param_1,0xb,lVar12,lVar10,0);
        return;
      }
    }
  }
LAB_181bc59d8:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

