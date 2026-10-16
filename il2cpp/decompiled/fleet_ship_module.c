// ==== ShipModule.GetMaxSquadSize @ 0x1817ec050 ====


undefined4 BattleControllerCombatTypeShipModule_GetMaxSquadSize(longlong param_1)

{
  longlong lVar1;
  
  if (DAT_185a3d09e == '\0') {
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    DAT_185a3d09e = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0xa8) != 0)) {
    lVar1 = FUN_180031fd0();
    if (lVar1 != 0) {
      return *(undefined4 *)(lVar1 + 0x14);
    }
    if ((*(longlong *)(param_1 + 0x18) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 0x1e8), lVar1 != 0)) {
      return *(undefined4 *)(lVar1 + 0xab8);
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ShipModule.GetMaxStartingSquadSize @ 0x1817ec0d0 ====


undefined4 BattleControllerCombatTypeShipModule_GetMaxStartingSquadSize(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  if (DAT_185a3d09f == '\0') {
    FUN_18035da50(&DAT_1857fdd48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    DAT_185a3d09f = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x10) != 0)) {
    lVar2 = FUN_180014310();
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
    lVar1 = *(longlong *)(param_1 + 0x18);
    if (lVar2 == 0) {
      if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1e8) != 0)) {
        return *(undefined4 *)(*(longlong *)(lVar1 + 0x1e8) + 0xab8);
      }
    }
    else if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1a8) != 0)) {
      lVar2 = FUN_180014430();
      if (lVar2 != 0) {
        lVar2 = FUN_180031f40();
        if (lVar2 != 0) {
          return *(undefined4 *)(lVar2 + 0x14);
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ShipModule.GetMaxDeployedBattleSquadSize @ 0x1817ec200 ====


undefined4 BattleControllerCombatTypeShipModule_GetMaxDeployedBattleSquadSize(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  if (DAT_185a3d0a0 == '\0') {
    FUN_18035da50(&DAT_1857fdd48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    DAT_185a3d0a0 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x10) != 0)) {
    lVar2 = FUN_180014310();
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
    lVar1 = *(longlong *)(param_1 + 0x18);
    if (lVar2 == 0) {
      if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1e8) != 0)) {
        return *(undefined4 *)(*(longlong *)(lVar1 + 0x1e8) + 0xac4);
      }
    }
    else if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1a8) != 0)) {
      lVar2 = FUN_180014430();
      if (lVar2 != 0) {
        lVar2 = FUN_180031f40();
        if (lVar2 != 0) {
          return *(undefined4 *)(lVar2 + 0x14);
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== ShipModule.SetupOpponentSquadUnitTypes @ 0x1817ec680 ====


void BattleControllerCombatTypeShipModule_SetupOpponentSquadUnitTypes(longlong param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  char cVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong local_80;
  ulonglong uStack_78;
  undefined8 local_70;
  longlong lStack_68;
  undefined8 local_60;
  longlong local_58;
  uint uStack_50;
  undefined4 uStack_4c;
  undefined8 local_38;
  
  if (DAT_185a3d0a3 == '\0') {
    FUN_18035da50(&DAT_1857cb460);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857cb478);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857cb458);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857cb470);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857cb468);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1ed8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1fd8);
    LOCK();
    UNLOCK();
    DAT_185a3d0a3 = '\x01';
  }
  local_80 = 0;
  uStack_78 = 0;
  local_70 = 0;
  lStack_68 = 0;
  local_60 = 0;
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x1a8) != 0)) {
    lVar5 = FUN_180014430();
    if ((lVar5 != 0) && (local_58 = *(longlong *)(lVar5 + 0x28), local_58 != 0)) {
      local_38 = 2;
      uStack_50 = *(uint *)(local_58 + 0x2c);
      uStack_4c = 0;
      uStack_78 = (ulonglong)uStack_50;
      local_70 = 0;
      lStack_68 = 0;
      local_60 = 2;
      local_80 = local_58;
      do {
        cVar4 = FUN_183423760(&local_80,DAT_1857cb458);
        lVar5 = lStack_68;
        if (cVar4 == '\0') {
          return;
        }
        if (lStack_68 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        uVar2 = *(undefined8 *)(lStack_68 + 0x40);
        uVar6 = FUN_1803a51b0(DAT_1857b1fd8);
        FUN_1836f4d00(uVar6,uVar2,DAT_1857b1ed8);
        if (DAT_185a3d09f == '\0') {
          FUN_18035da50(&DAT_1857fdd48);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_1857a3928);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_1857a3920);
          LOCK();
          UNLOCK();
          DAT_185a3d09f = '\x01';
        }
        if (*(longlong *)(param_1 + 0x18) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x10) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        lVar7 = FUN_180014310();
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
        lVar3 = *(longlong *)(param_1 + 0x18);
        if (lVar7 == 0) {
          if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if (*(longlong *)(lVar3 + 0x1e8) == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uVar1 = *(undefined4 *)(*(longlong *)(lVar3 + 0x1e8) + 0xab8);
        }
        else {
          if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if (*(longlong *)(lVar3 + 0x1a8) == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar7 = FUN_180014430();
          if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          lVar7 = FUN_180031f40();
          if (lVar7 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uVar1 = *(undefined4 *)(lVar7 + 0x14);
        }
        SquadDO_PrepareShipSquad(lVar5,uVar6,uVar1,0,0);
      } while( true );
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== CharacterModule.GetMaxSquadSize @ 0x1817eb3a0 ====


undefined4 BattleControllerCombatTypeCharacterModule_GetMaxSquadSize(longlong param_1)

{
  longlong lVar1;
  
  if (DAT_185a3d092 == '\0') {
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    DAT_185a3d092 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0xa8) != 0)) {
    lVar1 = FUN_180031fd0();
    if (lVar1 != 0) {
      return *(undefined4 *)(lVar1 + 0x14);
    }
    if ((*(longlong *)(param_1 + 0x18) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 0x1e8), lVar1 != 0)) {
      return *(undefined4 *)(lVar1 + 0xa18);
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== CharacterModule.GetMaxDeployedBattleSquadSize @ 0x1817eb420 ====


undefined4 BattleControllerCombatTypeCharacterModule_GetMaxDeployedBattleSquadSize(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  if (DAT_185a3d093 == '\0') {
    FUN_18035da50(&DAT_1857fdd48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    DAT_185a3d093 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0x10) != 0)) {
    lVar2 = FUN_180014310();
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
    lVar1 = *(longlong *)(param_1 + 0x18);
    if (lVar2 == 0) {
      if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1e8) != 0)) {
        return *(undefined4 *)(*(longlong *)(lVar1 + 0x1e8) + 0xa18);
      }
    }
    else if ((lVar1 != 0) && (*(longlong *)(lVar1 + 0x1a8) != 0)) {
      lVar2 = FUN_180014430();
      if (lVar2 != 0) {
        lVar2 = FUN_180031f40();
        if (lVar2 != 0) {
          return *(undefined4 *)(lVar2 + 0x14);
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== PreBattleShipModule.get_DefaultMaxUnitsAllowed @ 0x1819654d0 ====


undefined4 FUN_1819654d0(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x10) != 0) {
    return *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xab8);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== PreBattleShipModule.get_DefaultMinUnitsAllowed @ 0x1819654f0 ====


undefined4 FUN_1819654f0(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x10) != 0) {
    return *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xabc);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== PreBattleShipModule.get_DefaultMaxReinforcementUnitsAllowed @ 0x181965510 ====


undefined4 FUN_181965510(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x10) != 0) {
    return *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa38);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== PreBattleControllerCharacterModule_DfltMax @ 0x181965690 ====


void PreBattleControllerShipModule_LoadCombatTypeViews(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  if (DAT_185a3da29 == '\0') {
    FUN_18035da50(&DAT_18578b9c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18578b9d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18578b9d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18578b990);
    LOCK();
    UNLOCK();
    DAT_185a3da29 = '\x01';
  }
  lVar2 = FUN_1803a51b0(DAT_18578b9d8);
  if (lVar2 != 0) {
    *(longlong *)(lVar2 + 0x10) = param_1;
    *(undefined8 *)(lVar2 + 0x18) = param_2;
    lVar1 = *(longlong *)(param_1 + 0x40);
    uVar3 = FUN_1803a51b0(DAT_18578b9c8);
    FUN_180c84ee0(uVar3,lVar2,DAT_18578b9d0,0);
    if (lVar1 != 0) {
      FUN_182c81d20(lVar1,uVar3,0,0,0,DAT_18578b990);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BattleUnitController.get_IsCommander @ 0x181bba230 ====


undefined8 FUN_181bba230(longlong param_1)

{
  longlong lVar1;
  
  if ((*(longlong *)(param_1 + 0x98) != 0) &&
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x40), lVar1 != 0)) {
    return CONCAT71((int7)((ulonglong)lVar1 >> 8),*(char *)(lVar1 + 0xc4) == '\x01');
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// NOT FOUND: SquadCellDO.get_IsCommander @ 0x181004d20

// ==== UnitDO.get_CombatType @ 0x18100f5a0 ====


undefined1 FUN_18100f5a0(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x40) != 0) {
    return *(undefined1 *)(*(longlong *)(param_1 + 0x40) + 0x121);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== UnitDO.get_SquadPositionPriority @ 0x18100f5c0 ====


undefined4 FUN_18100f5c0(longlong param_1)

{
  if (*(longlong *)(param_1 + 0x40) != 0) {
    return *(undefined4 *)(*(longlong *)(param_1 + 0x40) + 0x170);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== BattleUnitController.get_CombatType @ 0x181bba320 ====


undefined1 FUN_181bba320(longlong param_1)

{
  longlong lVar1;
  
  if ((*(longlong *)(param_1 + 0x98) != 0) &&
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x40), lVar1 != 0)) {
    return *(undefined1 *)(lVar1 + 0x121);
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== StatCalculator.GetMitigationPercentFromRating @ 0x180fdfdf0 ====


void StatCalculator_GetMitigationPercentFromRating(undefined8 param_1,longlong param_2)

{
  longlong lVar1;
  
  if (DAT_185a39ef1 == '\0') {
    FUN_18035da50(&DAT_1857a8f48);
    LOCK();
    UNLOCK();
    DAT_185a39ef1 = '\x01';
  }
  if (param_2 != 0) {
    FUN_1800084f0(2,DAT_1857a8f48,param_2);
    lVar1 = FUN_180fde440(param_1,0);
    FUN_1800084f0(3,DAT_1857a8f48,param_2);
    if (lVar1 != 0) {
      lVar1 = FUN_180fde440(param_1,0);
      FUN_1800084f0(3,DAT_1857a8f48,param_2);
      if (lVar1 != 0) {
        return;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== StatCalculator.GetStatPercentageFromRating_2 @ 0x180fdf810 ====


float StatCalculator_GetStatPercentageFromRating_2
                (undefined8 param_1,char param_2,float param_3,undefined8 param_4,undefined1 param_5
                ,undefined4 param_6,char param_7)

{
  undefined1 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  float fVar4;
  float fVar5;
  ulonglong in_stack_ffffffffffffff28;
  longlong local_b8 [3];
  undefined8 local_a0;
  longlong *local_98;
  undefined8 local_90;
  longlong *plStack_88;
  undefined8 local_80;
  longlong local_78;
  undefined8 uStack_70;
  ulonglong local_68;
  
  if (DAT_185a39ef0 == '\0') {
    FUN_18035da50(&DAT_1857a8f50);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f30);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f28);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f40);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f18);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f10);
    LOCK();
    UNLOCK();
    DAT_185a39ef0 = '\x01';
  }
  local_b8[0] = 0;
  local_78 = 0;
  uStack_70 = 0;
  local_68 = 0;
  if (param_2 == '\b') {
    uVar3 = 1;
LAB_180fdf8f5:
    fVar4 = (float)StatCalculator_GetMitigationPercentFromRating(param_1,param_4,param_3,uVar3,0);
    return fVar4;
  }
  if (param_2 == '\t') {
    uVar3 = 2;
    goto LAB_180fdf8f5;
  }
  lVar2 = FUN_180fde440(param_1,0);
  if (lVar2 == 0) goto LAB_180fdfd51;
  switch(param_2) {
  case '\x18':
                    /* WARNING: This code block may not be properly labeled as switch case */
    fVar4 = *(float *)(lVar2 + 0x2380);
    break;
  case '\x19':
                    /* WARNING: This code block may not be properly labeled as switch case */
    fVar4 = *(float *)(lVar2 + 0x235c);
    break;
  case '\x1a':
                    /* WARNING: This code block may not be properly labeled as switch case */
    fVar4 = *(float *)(lVar2 + 0x2308);
    break;
  case '\x1b':
                    /* WARNING: This code block may not be properly labeled as switch case */
    fVar4 = *(float *)(lVar2 + 0x22f0);
    break;
  default:
switchD_180fdf963_caseD_1c:
                    /* WARNING: This code block may not be properly labeled as switch case */
    fVar4 = DAT_18529ef3c;
    break;
  case ' ':
  case '!':
  case '\"':
  case '#':
                    /* WARNING: This code block may not be properly labeled as switch case */
    if (param_2 == '\x14') {
      fVar4 = *(float *)(lVar2 + 0x237c);
    }
    else if (param_2 == '\x15') {
      fVar4 = *(float *)(lVar2 + 0x2358);
    }
    else if (param_2 == '\x16') {
      fVar4 = *(float *)(lVar2 + 0x2304);
    }
    else {
      if (param_2 != '\x17') goto switchD_180fdf963_caseD_1c;
      fVar4 = *(float *)(lVar2 + 0x22ec);
    }
  }
  param_3 = param_3 / fVar4;
  if (*(int *)(DAT_1857a8f18 + 0xe4) == 0) {
    FUN_180359940(DAT_1857a8f18);
  }
  if ((**(longlong **)(DAT_1857a8f18 + 0xb8) == 0) ||
     (lVar2 = FUN_183006ef0(**(longlong **)(DAT_1857a8f18 + 0xb8),DAT_1857a8f10), lVar2 == 0)) {
LAB_180fdfd51:
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  local_b8[0] = UnitStatLists_StartUsing(lVar2,0);
  local_a0 = 0;
  local_98 = local_b8;
  lVar2 = StatCalculator_GetConvertedRatingPercentAdditivesForStat(param_1,param_2,0);
  if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  uStack_70._4_4_ = *(int *)(lVar2 + 0x1c);
  local_80 = 0;
  uStack_70._0_4_ = 0;
  local_68 = 0;
  local_90 = 0;
  plStack_88 = &local_78;
  fVar4 = 0.0;
  local_78 = lVar2;
  while( true ) {
    if (local_78 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if ((uStack_70._4_4_ != *(int *)(local_78 + 0x1c)) ||
       (*(uint *)(local_78 + 0x18) <= (uint)uStack_70)) break;
    lVar2 = *(longlong *)(local_78 + 0x10);
    if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_70) {
                    /* WARNING: Subroutine does not return */
      FUN_180352460();
    }
    uVar1 = *(undefined1 *)((longlong)(int)(uint)uStack_70 + 0x20 + lVar2);
    local_68 = CONCAT71(local_68._1_7_,uVar1);
    uStack_70._0_4_ = (uint)uStack_70 + 1;
    in_stack_ffffffffffffff28 = in_stack_ffffffffffffff28 & 0xffffffff00000000;
    fVar5 = (float)StatCalculator_GetCalculatedStatValue
                             (param_1,uVar1,param_4,param_5,in_stack_ffffffffffffff28,param_6,0);
    param_3 = param_3 + fVar5;
  }
  if ((*(byte *)(*(longlong *)(DAT_1857a8f30 + 0x20) + 0x135) & 1) == 0) {
    FUN_18034f1d0(*(longlong *)(DAT_1857a8f30 + 0x20));
  }
  if (local_78 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  if (uStack_70._4_4_ != *(int *)(local_78 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
    ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
  }
  uStack_70 = CONCAT44(uStack_70._4_4_,*(int *)(local_78 + 0x18) + 1);
  local_68 = local_68 & 0xffffffffffffff00;
  if (local_b8[0] != 0) {
    FUN_180005650(0,DAT_18580d620,local_b8[0]);
  }
  if (param_7 == '\0') {
    lVar2 = FUN_180fde440(param_1,0);
    if (lVar2 == 0) goto LAB_180fdfd51;
    switch(param_2) {
    case '\f':
      param_3 = (param_3 + *(float *)(lVar2 + 9000)) * DAT_18529f324;
      break;
    case '\r':
      param_3 = (param_3 + *(float *)(lVar2 + 0x2320)) * DAT_18529f324;
      break;
    case '\x0e':
      param_3 = (param_3 + *(float *)(lVar2 + 0x2314)) * DAT_18529f324;
      break;
    case '\x0f':
      param_3 = (param_3 + *(float *)(lVar2 + 0x230c)) * DAT_18529f324;
      break;
    case '\x14':
    case '\x15':
    case '\x16':
    case '\x17':
      if (param_2 == '\x14') {
        fVar4 = *(float *)(lVar2 + 0x232c);
      }
      else {
        if (param_2 == '\x15') {
          return (param_3 + *(float *)(lVar2 + 0x2324)) * DAT_18529f324;
        }
        if (param_2 == '\x16') {
          return (param_3 + *(float *)(lVar2 + 0x2318)) * DAT_18529f324;
        }
        if (param_2 == '\x17') {
          return (param_3 + *(float *)(lVar2 + 0x2310)) * DAT_18529f324;
        }
      }
    default:
      param_3 = param_3 + fVar4;
      goto LAB_180fdfd41;
    }
  }
  else {
LAB_180fdfd41:
    param_3 = param_3 * DAT_18529f324;
  }
  return param_3;
}



// ==== BattleFsmController.TriggerTurnStarted @ 0x18193b880 ====


void BattleFsmController_TriggerTurnStarted(longlong param_1)

{
  longlong lVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  
  if (DAT_185a3d90c == '\0') {
    FUN_18035da50(&DAT_1857c2ef8);
    LOCK();
    UNLOCK();
    DAT_185a3d90c = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x78) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x110) != 0)) {
    iVar4 = FUN_1800084f0(0,DAT_1857c2ef8);
    if ((*(longlong *)(param_1 + 0x78) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x20), lVar1 != 0)) {
      if (DAT_185a3f0c6 == '\0') {
        FUN_18035da50(&DAT_1857f5f78);
        LOCK();
        UNLOCK();
        DAT_185a3f0c6 = '\x01';
      }
      if (*(longlong *)(lVar1 + 0x80) != 0) {
        bVar2 = FUN_1800084f0(6,DAT_1857f5f78);
        bVar2 = bVar2 ^ 1;
        if (iVar4 != 0) {
          bVar2 = 1;
        }
        if (bVar2 == 0) {
          if ((*(longlong *)(param_1 + 0x78) == 0) ||
             (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 == 0))
          goto LAB_18193ba0d;
          if (0 < *(int *)(lVar1 + 0xc0)) {
            if (*(longlong *)(lVar1 + 0x60) == 0) goto LAB_18193ba0d;
            cVar3 = BattlePersistentEffectsController_IsEffectActive
                              (*(longlong *)(lVar1 + 0x60),0,0,0);
            if ((cVar3 == '\0') && (*(char *)(lVar1 + 0x1e8) == '\0')) {
              BattleFsmController_StartAbilitySelectPhase(param_1,0);
              return;
            }
          }
        }
        if ((*(longlong *)(param_1 + 0x78) != 0) &&
           (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
          BattleUnitController_SetSpeedFill(lVar1,0,0);
          if ((*(longlong *)(param_1 + 0x78) != 0) &&
             (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar1 != 0)) {
            *(undefined4 *)(lVar1 + 0x20c) = 0;
            BattleFsmController_StartPlayerUnitMove(param_1,0);
            return;
          }
        }
      }
    }
  }
LAB_18193ba0d:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

