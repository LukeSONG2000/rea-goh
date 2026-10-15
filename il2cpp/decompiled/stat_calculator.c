// ==== GetCalculatedStatValue @ 0x180fde5c0 ====


ulonglong StatCalculator_GetCalculatedStatValue
                    (undefined8 param_1,char param_2,longlong *param_3,undefined1 param_4,
                    float param_5,undefined4 param_6)

{
  undefined1 uVar1;
  longlong lVar2;
  undefined8 *puVar3;
  ushort uVar4;
  float fVar6;
  float fVar7;
  float fVar8;
  undefined8 uVar9;
  undefined4 extraout_XMM0_Dc;
  undefined4 extraout_XMM0_Dd;
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  ulonglong in_stack_fffffffffffffec8;
  longlong local_118;
  undefined8 uStack_110;
  ulonglong local_108;
  undefined8 local_100;
  longlong *plStack_f8;
  undefined8 local_f0;
  float local_e8;
  undefined8 local_e0;
  longlong *local_d8;
  longlong local_d0;
  longlong *local_c8;
  ulonglong uVar5;
  
  if (DAT_185a39eee == '\0') {
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
    FUN_18035da50(&DAT_1857a8f48);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f40);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185827178);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f18);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a8f10);
    LOCK();
    UNLOCK();
    DAT_185a39eee = '\x01';
  }
  local_d0 = 0;
  local_118 = 0;
  uStack_110 = 0;
  local_108 = 0;
  if (param_3 != (longlong *)0x0) {
    fVar6 = (float)FUN_1800156b0(4,DAT_1857a8f48,param_3,param_2);
    if (*(int *)(DAT_1857a8f18 + 0xe4) == 0) {
      FUN_180359940(DAT_1857a8f18);
    }
    if ((**(longlong **)(DAT_1857a8f18 + 0xb8) != 0) &&
       (lVar2 = FUN_183006ef0(**(longlong **)(DAT_1857a8f18 + 0xb8),DAT_1857a8f10), lVar2 != 0)) {
      fVar6 = fVar6 + param_5;
      local_c8 = param_3;
      local_d0 = UnitStatLists_StartUsing(lVar2,0);
      local_e0 = 0;
      local_d8 = &local_d0;
      lVar2 = StatCalculator_GetAdditivesForStat(param_1,param_2,0);
      if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      uStack_110._4_4_ = *(int *)(lVar2 + 0x1c);
      local_f0 = 0;
      uStack_110._0_4_ = 0;
      local_108 = 0;
      local_100 = 0;
      plStack_f8 = &local_118;
      local_118 = lVar2;
      while( true ) {
        if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if ((uStack_110._4_4_ != *(int *)(local_118 + 0x1c)) ||
           (*(uint *)(local_118 + 0x18) <= (uint)uStack_110)) break;
        lVar2 = *(longlong *)(local_118 + 0x10);
        if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_110) {
                    /* WARNING: Subroutine does not return */
          FUN_180352460();
        }
        uVar1 = *(undefined1 *)((longlong)(int)(uint)uStack_110 + 0x20 + lVar2);
        local_108 = CONCAT71(local_108._1_7_,uVar1);
        uStack_110._0_4_ = (uint)uStack_110 + 1;
        lVar2 = *param_3;
        if (*(ushort *)(lVar2 + 0x12e) != 0) {
          uVar5 = 0;
          do {
            if (*(longlong *)(*(longlong *)(lVar2 + 0xb0) + uVar5 * 0x10) == DAT_1857a8f48) {
              puVar3 = (undefined8 *)
                       ((longlong)(*(int *)(*(longlong *)(lVar2 + 0xb0) + 8 + uVar5 * 0x10) + 4) *
                        0x10 + 0x138 + lVar2);
              goto LAB_180fde875;
            }
            uVar4 = (short)uVar5 + 1;
            uVar5 = (ulonglong)uVar4;
          } while (uVar4 < *(ushort *)(lVar2 + 0x12e));
        }
        puVar3 = (undefined8 *)FUN_18034f630(param_3,DAT_1857a8f48,4);
LAB_180fde875:
        fVar7 = (float)(*(code *)*puVar3)(param_3,uVar1,puVar3[1]);
        fVar6 = fVar6 + fVar7;
      }
      if ((*(byte *)(*(longlong *)(DAT_1857a8f30 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(DAT_1857a8f30 + 0x20));
      }
      if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_180352450();
      }
      if (uStack_110._4_4_ != *(int *)(local_118 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
        ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
      }
      uStack_110 = CONCAT44(uStack_110._4_4_,*(int *)(local_118 + 0x18) + 1);
      local_108 = local_108 & 0xffffffffffffff00;
      if (local_d0 != 0) {
        FUN_180005650(0,DAT_18580d620,local_d0);
      }
      if (param_2 != '=') {
        fVar7 = (float)StatCalculator_GetMasteryContribution(param_1,param_2,param_3,0);
        fVar8 = (float)StatCalculator_GetCalculatedStatValue
                                 (param_1,0x3d,param_3,param_4,
                                  in_stack_fffffffffffffec8 & 0xffffffff00000000,param_6,0);
        fVar6 = fVar6 + fVar8 * fVar7;
      }
      if (*(int *)(DAT_1857a8f18 + 0xe4) == 0) {
        FUN_180359940(DAT_1857a8f18);
      }
      if ((**(longlong **)(DAT_1857a8f18 + 0xb8) != 0) &&
         (lVar2 = FUN_183006ef0(**(longlong **)(DAT_1857a8f18 + 0xb8),DAT_1857a8f10), lVar2 != 0)) {
        fVar7 = 0.0;
        local_e8 = 0.0;
        local_d0 = UnitStatLists_StartUsing(lVar2,0);
        local_e0 = 0;
        local_d8 = &local_d0;
        lVar2 = StatCalculator_GetPercentAdditivesForStat(param_1,param_2,0);
        if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        uStack_110._4_4_ = *(int *)(lVar2 + 0x1c);
        local_f0 = 0;
        uStack_110._0_4_ = 0;
        local_108 = 0;
        local_100 = 0;
        plStack_f8 = &local_118;
        local_118 = lVar2;
        while( true ) {
          if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if ((uStack_110._4_4_ != *(int *)(local_118 + 0x1c)) ||
             (*(uint *)(local_118 + 0x18) <= (uint)uStack_110)) break;
          lVar2 = *(longlong *)(local_118 + 0x10);
          if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_110) {
                    /* WARNING: Subroutine does not return */
            FUN_180352460();
          }
          uVar1 = *(undefined1 *)((longlong)(int)(uint)uStack_110 + 0x20 + lVar2);
          local_108 = CONCAT71(local_108._1_7_,uVar1);
          uStack_110._0_4_ = (uint)uStack_110 + 1;
          lVar2 = *local_c8;
          if (*(ushort *)(lVar2 + 0x12e) != 0) {
            uVar5 = 0;
            do {
              if (*(longlong *)(*(longlong *)(lVar2 + 0xb0) + uVar5 * 0x10) == DAT_1857a8f48) {
                puVar3 = (undefined8 *)
                         ((longlong)(*(int *)(*(longlong *)(lVar2 + 0xb0) + 8 + uVar5 * 0x10) + 4) *
                          0x10 + 0x138 + lVar2);
                goto LAB_180fdeb69;
              }
              uVar4 = (short)uVar5 + 1;
              uVar5 = (ulonglong)uVar4;
            } while (uVar4 < *(ushort *)(lVar2 + 0x12e));
          }
          puVar3 = (undefined8 *)FUN_18034f630(param_3,DAT_1857a8f48,4);
LAB_180fdeb69:
          fVar8 = (float)(*(code *)*puVar3)(param_3,uVar1,puVar3[1]);
          fVar7 = fVar7 + fVar8;
          lVar2 = *local_c8;
          if (*(ushort *)(lVar2 + 0x12e) != 0) {
            uVar5 = 0;
            do {
              if (*(longlong *)(*(longlong *)(lVar2 + 0xb0) + uVar5 * 0x10) == DAT_1857a8f48) {
                puVar3 = (undefined8 *)
                         ((longlong)(*(int *)(*(longlong *)(lVar2 + 0xb0) + 8 + uVar5 * 0x10) + 5) *
                          0x10 + 0x138 + lVar2);
                local_e8 = fVar7;
                goto LAB_180fdebd5;
              }
              uVar4 = (short)uVar5 + 1;
              uVar5 = (ulonglong)uVar4;
            } while (uVar4 < *(ushort *)(lVar2 + 0x12e));
          }
          local_e8 = fVar7;
          puVar3 = (undefined8 *)FUN_18034f630(param_3,DAT_1857a8f48,5);
LAB_180fdebd5:
          fVar8 = (float)(*(code *)*puVar3)(param_3,uVar1,param_4,param_6,puVar3[1]);
          fVar7 = fVar7 + fVar8;
          local_e8 = fVar7;
        }
        if ((*(byte *)(*(longlong *)(DAT_1857a8f30 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(DAT_1857a8f30 + 0x20));
        }
        if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
          FUN_180352450();
        }
        if (uStack_110._4_4_ != *(int *)(local_118 + 0x1c)) {
                    /* WARNING: Subroutine does not return */
          ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
        }
        uStack_110 = CONCAT44(uStack_110._4_4_,*(int *)(local_118 + 0x18) + 1);
        local_108 = local_108 & 0xffffffffffffff00;
        if (local_d0 != 0) {
          FUN_180005650(0,DAT_18580d620,local_d0);
        }
        fVar8 = DAT_18529ef3c;
        fVar7 = fVar7 + DAT_18529ef3c;
        uVar9 = FUN_180015180();
        auVar10._8_4_ = extraout_XMM0_Dc;
        auVar10._0_8_ = uVar9;
        auVar10._12_4_ = extraout_XMM0_Dd;
        if (*(int *)(DAT_1857a8f18 + 0xe4) == 0) {
          FUN_180359940(DAT_1857a8f18);
        }
        if ((**(longlong **)(DAT_1857a8f18 + 0xb8) != 0) &&
           (lVar2 = FUN_183006ef0(**(longlong **)(DAT_1857a8f18 + 0xb8),DAT_1857a8f10), lVar2 != 0))
        {
          auVar11._4_12_ = auVar10._4_12_;
          auVar11._0_4_ = (float)uVar9 + fVar7 * fVar6;
          local_d0 = UnitStatLists_StartUsing(lVar2,0);
          local_e0 = 0;
          local_d8 = &local_d0;
          lVar2 = StatCalculator_GetAdditivesForStat(param_1,param_2,0);
          if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
            FUN_180352450();
          }
          uStack_110._4_4_ = *(int *)(lVar2 + 0x1c);
          local_f0 = 0;
          uStack_110._0_4_ = 0;
          local_108 = 0;
          local_100 = 0;
          plStack_f8 = &local_118;
          local_118 = lVar2;
          do {
            uVar5 = auVar11._0_8_;
            if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            if ((uStack_110._4_4_ != *(int *)(local_118 + 0x1c)) ||
               (*(uint *)(local_118 + 0x18) <= (uint)uStack_110)) {
              if ((*(byte *)(*(longlong *)(DAT_1857a8f30 + 0x20) + 0x135) & 1) == 0) {
                FUN_18034f1d0(*(longlong *)(DAT_1857a8f30 + 0x20));
              }
              if (local_118 == 0) {
                    /* WARNING: Subroutine does not return */
                FUN_180352450();
              }
              if (uStack_110._4_4_ == *(int *)(local_118 + 0x1c)) {
                uStack_110 = CONCAT44(uStack_110._4_4_,*(int *)(local_118 + 0x18) + 1);
                local_108 = local_108 & 0xffffffffffffff00;
                if (local_d0 != 0) {
                  FUN_180005650(0,DAT_18580d620,local_d0);
                }
                if (((byte)(param_2 - 6U) < 2) || (param_2 == '\x01')) {
                  if (*(int *)(DAT_185827178 + 0xe4) == 0) {
                    FUN_180359940();
                  }
                  if ((auVar11._0_4_ < fVar8) || (0x7f800000 < (uint)ABS(fVar8))) {
                    uVar5 = (ulonglong)(uint)fVar8;
                  }
                }
                if (param_2 == '\x19') {
                  if (*(int *)(DAT_185827178 + 0xe4) == 0) {
                    FUN_180359940();
                  }
                  if ((float)uVar5 < 0.0) {
                    uVar5 = 0;
                  }
                }
                switch(param_2) {
                case '\x10':
                case '\x12':
                case '\x13':
                case '\x18':
                case '\x1a':
                case '\x1b':
                case '\x1c':
                case '\x1d':
                case '\x1e':
                case '\x1f':
                case ' ':
                case '!':
                case '\"':
                case '#':
                case '$':
                case '%':
                case '&':
                case '\'':
                case '(':
                case ')':
                case '*':
                case '+':
                case ',':
                case '-':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case ':':
                case ';':
                case '<':
                case '=':
                case '>':
                  break;
                default:
                  uVar5 = (ulonglong)(uint)(float)(int)(float)uVar5;
                }
                return uVar5;
              }
                    /* WARNING: Subroutine does not return */
              ThrowHelper_ThrowInvalidOperationException_InvalidOperation_EnumFailedVersion(0);
            }
            lVar2 = *(longlong *)(local_118 + 0x10);
            if (lVar2 == 0) {
                    /* WARNING: Subroutine does not return */
              FUN_180352450();
            }
            if (*(uint *)(lVar2 + 0x18) <= (uint)uStack_110) {
                    /* WARNING: Subroutine does not return */
              FUN_180352460();
            }
            uVar1 = *(undefined1 *)((longlong)(int)(uint)uStack_110 + 0x20 + lVar2);
            local_108 = CONCAT71(local_108._1_7_,uVar1);
            uStack_110._0_4_ = (uint)uStack_110 + 1;
            lVar2 = *local_c8;
            if (*(ushort *)(lVar2 + 0x12e) != 0) {
              uVar5 = 0;
              do {
                if (*(longlong *)(*(longlong *)(lVar2 + 0xb0) + uVar5 * 0x10) == DAT_1857a8f48) {
                  puVar3 = (undefined8 *)
                           ((longlong)(*(int *)(*(longlong *)(lVar2 + 0xb0) + 8 + uVar5 * 0x10) + 5)
                            * 0x10 + 0x138 + lVar2);
                  goto LAB_180fdeec5;
                }
                uVar4 = (short)uVar5 + 1;
                uVar5 = (ulonglong)uVar4;
              } while (uVar4 < *(ushort *)(lVar2 + 0x12e));
            }
            puVar3 = (undefined8 *)FUN_18034f630(param_3,DAT_1857a8f48,5);
LAB_180fdeec5:
            fVar6 = (float)(*(code *)*puVar3)(param_3,uVar1,param_4,param_6,puVar3[1]);
            auVar11._0_4_ = auVar11._0_4_ + fVar6;
          } while( true );
        }
      }
    }
  }
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}



// ==== GetMitigationPercentFromRating @ 0x180fdfdf0 ====


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

