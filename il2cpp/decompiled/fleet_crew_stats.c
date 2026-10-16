// ==== UnitDO.GetEquippedStatContribution @ 0x181011c90 ====


void UnitDO_GetEquippedStatContribution
               (longlong param_1,char param_2,byte param_3,undefined4 param_4)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 *puVar7;
  longlong lVar8;
  ushort uVar9;
  int iVar10;
  float fVar11;
  float fVar12;
  double dVar13;
  float fVar14;
  float fVar15;
  
  if (DAT_185a3a07c == '\0') {
    FUN_18035da50(&DAT_1857b1760);
    LOCK();
    UNLOCK();
    DAT_185a3a07c = '\x01';
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    if ((*(char *)(*(longlong *)(param_1 + 0x40) + 0x121) == '\0') && ((param_3 & 1) == 0)) {
      fVar11 = (float)UnitDO_GetEquipmentStatBonus(param_1,param_2,0);
      fVar11 = fVar11 + 0.0;
    }
    else {
      fVar11 = 0.0;
    }
    fVar14 = 0.0;
    if (*(longlong *)(param_1 + 0x80) != 0) {
      cVar4 = FUN_18000a350(1,DAT_1857b1760,*(longlong *)(param_1 + 0x80),param_3);
      if (cVar4 == '\0') {
        plVar2 = *(longlong **)(param_1 + 0x80);
        if (plVar2 == (longlong *)0x0) goto LAB_181012057;
        lVar3 = *plVar2;
        if (*(ushort *)(lVar3 + 0x12e) != 0) {
          uVar9 = 0;
          do {
            if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + (ulonglong)uVar9 * 0x10) ==
                DAT_1857b1760) {
              puVar7 = (undefined8 *)
                       ((longlong)
                        *(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + (ulonglong)uVar9 * 0x10) * 0x10 +
                        0x138 + lVar3);
              goto LAB_181011db2;
            }
            uVar9 = uVar9 + 1;
          } while (uVar9 < *(ushort *)(lVar3 + 0x12e));
        }
        puVar7 = (undefined8 *)FUN_18034f630(plVar2,DAT_1857b1760,0);
LAB_181011db2:
        fVar12 = (float)(*(code *)*puVar7)(plVar2,param_2,param_4,puVar7[1]);
        fVar11 = fVar12 + fVar11;
      }
      fVar12 = 0.0;
      if (*(longlong *)(param_1 + 0x108) == 0) {
LAB_181011f21:
        if (*(longlong *)(param_1 + 0x40) != 0) {
          cVar4 = FUN_18101a900(*(longlong *)(param_1 + 0x40),0);
          if (cVar4 != '\0') {
            if (DAT_185a3a07d == '\0') {
              FUN_18035da50(&DAT_185830790);
              LOCK();
              UNLOCK();
              DAT_185a3a07d = '\x01';
            }
            if (*(longlong *)(param_1 + 0xf8) != 0) {
              lVar3 = *(longlong *)(*(longlong *)(param_1 + 0xf8) + 0x10);
              if (lVar3 == 0) goto LAB_181012057;
              fVar12 = (float)StatDefinitionDO_GetStat(lVar3,param_2,0);
            }
            if (*(int *)(DAT_185830790 + 0xe4) == 0) {
              FUN_180359940();
            }
            cVar4 = MathUtils_Approximately(0,fVar12,0);
            if (cVar4 == '\0') {
              if (*(longlong *)(param_1 + 0x40) == 0) goto LAB_181012057;
              cVar4 = FUN_18101a900(*(longlong *)(param_1 + 0x40),0);
              if ((cVar4 != '\0') && (*(int *)(param_1 + 0xf0) == 0)) {
                if (*(longlong *)(param_1 + 0xe8) == 0) goto LAB_181012057;
                uVar6 = CrewRatingCalculator_GetCrewRating(*(longlong *)(param_1 + 0xe8),param_1,0);
                *(undefined4 *)(param_1 + 0xf0) = uVar6;
              }
              fVar14 = (float)*(int *)(param_1 + 0xf0) * *(float *)(param_1 + 0x100) * fVar12;
            }
            fVar11 = fVar11 + fVar14;
          }
          UnitStatEnumHelper_Round(param_2,fVar11,0);
          return;
        }
      }
      else {
        lVar3 = *(longlong *)(param_1 + 0x108);
        iVar1 = *(int *)(param_1 + 0x110);
        if (DAT_185a3bf55 == '\0') {
          FUN_18035da50(&DAT_1857daeb0);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_185815ad0);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_1857daee8);
          LOCK();
          UNLOCK();
          FUN_18035da50(&DAT_185827178);
          LOCK();
          UNLOCK();
          DAT_185a3bf55 = '\x01';
        }
        iVar10 = 0;
        lVar8 = *(longlong *)(lVar3 + 0x30);
        fVar15 = fVar12;
        while (lVar8 != 0) {
          iVar5 = FUN_1800084f0(0,DAT_185815ad0,lVar8);
          if ((iVar5 <= iVar10) || (iVar1 < iVar10)) {
            if (*(int *)(DAT_1857daeb0 + 0xe4) == 0) {
              FUN_180359940(DAT_1857daeb0);
            }
            uVar6 = **(undefined4 **)(DAT_1857daeb0 + 0xb8);
            if (*(int *)(DAT_185827178 + 0xe4) == 0) {
              FUN_180359940();
            }
            dVar13 = (double)FUN_181561010((double)fVar15,uVar6);
            fVar11 = fVar11 + (float)dVar13;
            goto LAB_181011f21;
          }
          if (*(longlong *)(lVar3 + 0x30) == 0) break;
          lVar8 = FUN_18002dc10();
          if (*(char *)(lVar8 + 10) == param_2) {
            fVar15 = fVar15 + *(float *)(lVar8 + 0xc);
          }
          iVar10 = iVar10 + 1;
          lVar8 = *(longlong *)(lVar3 + 0x30);
        }
      }
    }
  }
LAB_181012057:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== UnitDO.GetSimulatedCrewStatContribution @ 0x181012060 ====


float UnitDO_GetSimulatedCrewStatContribution(longlong param_1,undefined1 param_2)

{
  longlong lVar1;
  char cVar2;
  undefined4 uVar3;
  float fVar4;
  
  if (DAT_185a3a07d == '\0') {
    FUN_18035da50(&DAT_185830790);
    LOCK();
    UNLOCK();
    DAT_185a3a07d = '\x01';
  }
  if (*(longlong *)(param_1 + 0xf8) == 0) {
    fVar4 = 0.0;
  }
  else {
    lVar1 = *(longlong *)(*(longlong *)(param_1 + 0xf8) + 0x10);
    if (lVar1 == 0) goto LAB_181012163;
    fVar4 = (float)StatDefinitionDO_GetStat(lVar1,param_2,0);
  }
  if (*(int *)(DAT_185830790 + 0xe4) == 0) {
    FUN_180359940();
  }
  cVar2 = MathUtils_Approximately();
  if (cVar2 != '\0') {
    return 0.0;
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    cVar2 = FUN_18101a900(*(longlong *)(param_1 + 0x40),0);
    if ((cVar2 != '\0') && (*(int *)(param_1 + 0xf0) == 0)) {
      if (*(longlong *)(param_1 + 0xe8) == 0) goto LAB_181012163;
      uVar3 = CrewRatingCalculator_GetCrewRating(*(longlong *)(param_1 + 0xe8),param_1,0);
      *(undefined4 *)(param_1 + 0xf0) = uVar3;
    }
    return (float)*(int *)(param_1 + 0xf0) * *(float *)(param_1 + 0x100) * fVar4;
  }
LAB_181012163:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== UnitDO.Factory.EquipCrewMembers @ 0x181018c30 ====


void Factory_EquipCrewMembers(longlong param_1,longlong param_2,longlong param_3)

{
  char cVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong *plVar5;
  undefined4 extraout_XMM0_Da;
  longlong local_98 [3];
  undefined8 local_80;
  longlong *local_78;
  undefined8 local_70;
  longlong *plStack_68;
  undefined8 local_60;
  longlong local_58;
  undefined8 uStack_50;
  longlong *local_48;
  
  if (DAT_185a3a098 == '\0') {
    FUN_18035da50(&DAT_1857a90c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857accd0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857accd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a90b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1538);
    LOCK();
    UNLOCK();
    DAT_185a3a098 = '\x01';
  }
  local_58 = 0;
  uStack_50 = 0;
  local_48 = (longlong *)0x0;
  local_98[0] = 0;
  if (param_2 != 0) {
    lVar3 = FUN_180014a00(param_2);
    if (lVar3 == 0) {
      return;
    }
    lVar3 = *(longlong *)(lVar3 + 0x10);
    if (lVar3 != 0) {
      local_60 = 0;
      uStack_50._4_4_ = *(int *)(lVar3 + 0x1c);
      uStack_50._0_4_ = 0;
      local_48 = (longlong *)0x0;
      local_70 = 0;
      plStack_68 = &local_58;
      local_58 = lVar3;
      while( true ) {
        if (local_58 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if ((uStack_50._4_4_ != *(int *)(local_58 + 0x1c)) ||
           (*(uint *)(local_58 + 0x18) <= (uint)uStack_50)) break;
        lVar3 = *(longlong *)(local_58 + 0x10);
        if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(uint *)(lVar3 + 0x18) <= (uint)uStack_50) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        local_48 = *(longlong **)(lVar3 + 0x20 + (longlong)(int)(uint)uStack_50 * 8);
        uStack_50._0_4_ = (uint)uStack_50 + 1;
        plVar5 = (longlong *)0x0;
        if (local_48 != (longlong *)0x0) {
          if (((*(byte *)(*local_48 + 0x130) < *(byte *)(DAT_1857a90c0 + 0x130)) ||
              (*(longlong *)
                (*(longlong *)(*local_48 + 200) + -8 +
                (ulonglong)*(byte *)(DAT_1857a90c0 + 0x130) * 8) != DAT_1857a90c0)) ||
             (plVar5 = local_48, local_48 == (longlong *)0x0)) {
                    /* WARNING: Subroutine does not return */
            FUN_180352e70(local_48);
          }
        }
        if (param_3 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        local_98[0] = FUN_1800084f0(0,DAT_1857accd0,param_3);
        local_80 = 0;
        local_78 = local_98;
        do {
          if (local_98[0] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          cVar1 = FUN_1800084f0(0,DAT_185820d00);
          if (cVar1 == '\0') goto LAB_181018f08;
          if (local_98[0] == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uVar4 = FUN_1800169c0(extraout_XMM0_Da,DAT_1857accd8);
          if (plVar5 == (longlong *)0x0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          cVar1 = CrewSlotDO_CanEquip_2(plVar5,uVar4,0);
        } while (cVar1 == '\0');
        if (*(longlong *)(param_1 + 0x38) == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        uVar4 = Factory_Get_170(*(longlong *)(param_1 + 0x38),uVar4,0);
        FUN_18308d610(plVar5,uVar4,DAT_1857b1538);
LAB_181018f08:
        if (local_98[0] != 0) {
          FUN_180005650(0,DAT_18580d620,local_98[0]);
        }
      }
      if ((*(byte *)(*(longlong *)(DAT_1857a90d0 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(DAT_1857a90d0 + 0x20));
      }
      if (local_58 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (uStack_50._4_4_ != *(int *)(local_58 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
      }
      uStack_50 = CONCAT44(uStack_50._4_4_,*(int *)(local_58 + 0x18) + 1);
      local_48 = (longlong *)0x0;
      if (*(longlong *)(param_2 + 0x30) != 0) {
        uVar2 = PowerCalculator_GetPower(*(longlong *)(param_2 + 0x30),param_2,0);
        *(undefined4 *)(param_2 + 0x24) = uVar2;
        uVar2 = UnitDO_GetFullUnitStatusHashCode(param_2,0);
        *(undefined4 *)(param_2 + 0x38) = uVar2;
        return;
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== UnitDO.Factory.SetupCrewlessShip @ 0x181019060 ====


void Factory_SetupCrewlessShip(longlong param_1,longlong param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong *plVar4;
  
  if (DAT_185a3a099 == '\0') {
    FUN_18035da50(&DAT_1857a92d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a91a8);
    LOCK();
    UNLOCK();
    DAT_185a3a099 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x50) != 0) {
    lVar2 = Factory_Get_23(*(longlong *)(param_1 + 0x50),0);
    if ((param_2 != 0) && (*(longlong *)(param_2 + 0xe8) = lVar2, lVar2 != 0)) {
      uVar1 = CrewRatingCalculator_GetCrewRating(lVar2,param_2,0);
      *(undefined4 *)(param_2 + 0xf0) = uVar1;
      if (*(longlong *)(param_2 + 0x40) != 0) {
        lVar2 = *(longlong *)(*(longlong *)(param_2 + 0x40) + 0x160);
        if ((lVar2 != 0) && (*(int *)(lVar2 + 0x10) != 0)) {
          if (*(longlong *)(param_1 + 0x10) == 0) goto LAB_1810191a7;
          uVar3 = GameDataDomainController_GetStatProgression
                            (*(longlong *)(param_1 + 0x10),
                             *(undefined8 *)(*(longlong *)(param_2 + 0x40) + 0x160),0);
          *(undefined8 *)(param_2 + 0xf8) = uVar3;
        }
        if ((*(longlong *)(param_1 + 0x28) != 0) && (*(longlong *)(param_1 + 0x10) != 0)) {
          plVar4 = (longlong *)
                   GameDataDomainController_GetTable
                             (*(longlong *)(param_1 + 0x10),
                              *(undefined8 *)(*(longlong *)(param_1 + 0x28) + 0x6c0),0);
          if (plVar4 != (longlong *)0x0) {
            if ((*(byte *)(DAT_1857a92d8 + 0x130) <= *(byte *)(*plVar4 + 0x130)) &&
               (*(longlong *)
                 (*(longlong *)(*plVar4 + 200) + -8 +
                 (ulonglong)*(byte *)(DAT_1857a92d8 + 0x130) * 8) == DAT_1857a92d8)) {
              uVar1 = FUN_182f7f730(plVar4,*(undefined1 *)(param_2 + 200),0,DAT_1857a91a8);
              *(undefined4 *)(param_2 + 0x100) = uVar1;
            }
          }
          return;
        }
      }
    }
  }
LAB_1810191a7:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== CrewRatingCalculator.GetCrewRating @ 0x180fd62a0 ====


int CrewRatingCalculator_GetCrewRating(undefined8 *param_1,longlong param_2)

{
  undefined1 uVar1;
  char cVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ulonglong uVar7;
  undefined8 *puVar8;
  uint uVar9;
  int iVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float local_res8 [2];
  undefined4 local_res20 [2];
  
  puVar6 = param_1;
  if (DAT_185a39ec0 == '\0') {
    puVar6 = &DAT_1857a93b8;
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a39ec0 = '\x01';
  }
  fVar15 = 0.0;
  fVar14 = 0.0;
  if (param_1[4] != 0) {
    if (param_2 == 0) goto LAB_180fd6775;
    puVar6 = (undefined8 *)param_1[4];
    lVar4 = XpTableDO_GetRowForLevel(puVar6,*(undefined4 *)(param_2 + 0x10),0);
    if (lVar4 != 0) {
      puVar6 = *(undefined8 **)(param_2 + 0x40);
      if (puVar6 == (undefined8 *)0x0) goto LAB_180fd6775;
      fVar14 = (float)*(int *)(lVar4 + 0x14);
      cVar2 = FUN_18101a900(puVar6,0);
      if (cVar2 != '\0') {
        if (param_1[2] == 0) goto LAB_180fd6775;
        fVar14 = fVar14 * *(float *)(param_1[2] + 0xfc0);
      }
      fVar14 = fVar14 + 0.0;
    }
  }
  if (DAT_185a39ec5 == '\0') {
    puVar6 = &DAT_1857a9310;
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a39ec5 = '\x01';
  }
  if (param_1[9] == 0) {
    fVar11 = 0.0;
  }
  else {
    if (param_2 == 0) goto LAB_180fd6775;
    puVar8 = (undefined8 *)param_1[9];
    if (*(longlong *)(param_2 + 0xc0) == 0) {
      uVar7 = (ulonglong)puVar6 & 0xffffffffffffff00;
    }
    else {
      uVar7 = (ulonglong)*(byte *)(*(longlong *)(param_2 + 0xc0) + 0x18);
    }
    uVar3 = RelicTierEnumHelper_GetRelicTierNumber(uVar7,0);
    fVar11 = (float)FUN_180014980(puVar8,uVar3,0);
    fVar11 = (float)*(int *)(param_2 + 0x10) * fVar11;
    puVar6 = puVar8;
  }
  if (DAT_185a39ec6 == '\0') {
    puVar6 = &DAT_1857a9310;
    FUN_18035da50();
    LOCK();
    UNLOCK();
    DAT_185a39ec6 = '\x01';
  }
  if (param_1[10] == 0) {
    fVar12 = 0.0;
  }
  else {
    if (param_2 == 0) goto LAB_180fd6775;
    uVar5 = param_1[10];
    if (*(longlong *)(param_2 + 0xc0) == 0) {
      uVar7 = (ulonglong)puVar6 & 0xffffffffffffff00;
    }
    else {
      uVar7 = (ulonglong)*(byte *)(*(longlong *)(param_2 + 0xc0) + 0x18);
    }
    uVar3 = RelicTierEnumHelper_GetRelicTierNumber(uVar7,0);
    fVar12 = (float)FUN_180014980(uVar5,uVar3,0);
  }
  fVar12 = fVar12 + fVar11 + fVar14;
  if (param_1[3] == 0) {
    if (param_2 == 0) goto LAB_180fd6775;
  }
  else {
    if (param_2 == 0) goto LAB_180fd6775;
    fVar14 = (float)FUN_182f7f6b0(param_1[3],*(undefined1 *)(param_2 + 200),DAT_1857a93b8);
    fVar12 = fVar14 + fVar12;
  }
  fVar14 = DAT_18529f1c0;
  uVar9 = 0;
  fVar11 = 0.0;
  while ((int)uVar9 < (int)(uint)*(byte *)(param_2 + 0xc9)) {
    if (DAT_185a39ec1 == '\0') {
      FUN_18035da50(&DAT_1857a9398);
      LOCK();
      UNLOCK();
      DAT_185a39ec1 = '\x01';
    }
    if (param_1[5] == 0) {
      uVar9 = uVar9 + 1;
      fVar11 = fVar11 + fVar14 * 0.0;
    }
    else {
      fVar13 = (float)FUN_182f7f6b0(param_1[5],uVar9 & 0xff);
      uVar9 = uVar9 + 1;
      fVar11 = fVar11 + fVar13 * fVar14;
    }
  }
  if (DAT_185a3a082 == '\0') {
    FUN_18035da50(&DAT_1857b17c8);
    LOCK();
    UNLOCK();
    DAT_185a3a082 = '\x01';
  }
  if (*(longlong *)(param_2 + 0x78) == 0) {
    iVar10 = 0;
  }
  else {
    iVar10 = *(int *)(*(longlong *)(param_2 + 0x78) + 0x18);
  }
  uVar1 = *(undefined1 *)(param_2 + 0xc9);
  if (DAT_185a39ec1 == '\0') {
    FUN_18035da50(&DAT_1857a9398);
    LOCK();
    UNLOCK();
    DAT_185a39ec1 = '\x01';
  }
  if (param_1[5] != 0) {
    fVar15 = (float)FUN_182f7f6b0(param_1[5],uVar1,DAT_1857a9398);
  }
  fVar14 = (float)CrewRatingCalculator_GetCrewRatingPerAbilityLevel(param_1,param_2,0);
  if (*(longlong *)(param_2 + 0x40) == 0) goto LAB_180fd6775;
  cVar2 = FUN_18101a900(*(longlong *)(param_2 + 0x40),0);
  if (cVar2 != '\0') {
    if (DAT_185a39ec4 == '\0') {
      FUN_18035da50(&DAT_1857a3438);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857a9320);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857a9318);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857a9330);
      LOCK();
      UNLOCK();
      DAT_185a39ec4 = '\x01';
    }
    local_res8[0] = 0.0;
    if (param_1[7] != 0) {
      if (*(longlong *)(param_2 + 0x40) == 0) {
LAB_180fd6775:
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      lVar4 = param_1[7];
      uVar3 = UnitDefinitionDO_GetAbilityCount(*(longlong *)(param_2 + 0x40),1,0);
      if (*(longlong *)(lVar4 + 0x18) == 0) goto LAB_180fd6775;
      cVar2 = FUN_18000b350(*(longlong *)(lVar4 + 0x18),uVar3,local_res8,
                            *(undefined8 *)
                             (*(longlong *)(*(longlong *)(DAT_1857a9320 + 0x20) + 0xc0) + 0x48));
      if (cVar2 == '\0') {
        if (*(longlong *)(param_2 + 0x40) == 0) goto LAB_180fd6775;
        local_res20[0] = UnitDefinitionDO_GetAbilityCount(*(longlong *)(param_2 + 0x40),1,0);
        uVar5 = FUN_1803a4e80(DAT_185a33e38,local_res20);
        uVar5 = String_Format(DAT_1857a9318,uVar5,0);
        if ((param_1[2] == 0) || (*(longlong *)(param_2 + 0x40) == 0)) goto LAB_180fd6775;
        FUN_182b70250(param_1,DAT_1857a9330,uVar5,*(undefined8 *)(param_1[2] + 0x6b8),
                      *(undefined8 *)(*(longlong *)(param_2 + 0x40) + 0x58),0xffffffff,DAT_1857a3438
                     );
      }
    }
    fVar14 = local_res8[0] * fVar14;
  }
  fVar13 = (float)CrewRatingCalculator_GetCrewRatingPerMod(param_1,param_2,0);
  return (int)(fVar13 + fVar14 + (float)iVar10 * fVar15 + fVar11 + fVar12);
}



// ==== SquadDO.PrepareShipSquad @ 0x181007ea0 ====


void SquadDO_PrepareShipSquad(undefined8 param_1,longlong param_2,int param_3,char param_4)

{
  undefined4 uVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  int iVar5;
  int iVar6;
  uint uStack_40;
  
  if (DAT_185a3a038 == '\0') {
    FUN_18035da50(&DAT_1857a3970);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3940);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3918);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1fb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a38e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1ec8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857b1e98);
    LOCK();
    UNLOCK();
    DAT_185a3a038 = '\x01';
  }
  if (*(int *)(DAT_1857b1e98 + 0xe4) == 0) {
    FUN_180359940(DAT_1857b1e98);
  }
  if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  iVar5 = 0;
  iVar6 = **(int **)(DAT_1857b1e98 + 0xb8);
  uVar1 = *(undefined4 *)(param_2 + 0x18);
  lVar4 = FUN_1803a51b0(DAT_1857b1fd8);
  FUN_182d09bb0(lVar4,uVar1,DAT_1857b1ec8);
  iVar2 = *(int *)(param_2 + 0x1c);
  uStack_40 = 0;
  while( true ) {
    if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if ((iVar2 != *(int *)(param_2 + 0x1c)) || (*(uint *)(param_2 + 0x18) <= uStack_40)) {
      if ((*(byte *)(*(longlong *)(DAT_1857a3940 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0();
      }
      if (param_2 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (iVar2 != *(int *)(param_2 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
      }
      SquadDO_UpdateSquad(param_1,lVar4,0);
      return;
    }
    lVar3 = *(longlong *)(param_2 + 0x10);
    if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(uint *)(lVar3 + 0x18) <= uStack_40) break;
    lVar3 = *(longlong *)(lVar3 + 0x20 + (longlong)(int)uStack_40 * 8);
    uStack_40 = uStack_40 + 1;
    if (lVar3 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352450();
    }
    if (*(char *)(lVar3 + 0x10) == '\x04') {
      if (param_4 != '\0') {
        *(int *)(lVar3 + 0x18) = iVar6;
        iVar6 = iVar6 + 1;
      }
      if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      FUN_180016b30(lVar4);
    }
    else if (iVar5 < param_3) {
      *(int *)(lVar3 + 0x18) = iVar5;
      if (lVar4 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      iVar5 = iVar5 + 1;
      FUN_180016b30(lVar4);
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352460();
}

