// ==== GetStatPercentageFromRating_2 @ 0x180fdf810 ====

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



// ==== GetStatPercentageFromRating @ 0x180fdf7a0 ====

void StatCalculator_GetStatPercentageFromRating
               (undefined8 param_1,ulonglong param_2,undefined8 param_3,undefined8 param_4,
               undefined4 param_5)

{
  undefined4 uVar1;
  ulonglong uVar2;
  
  uVar2 = 0;
  uVar1 = StatCalculator_GetCalculatedStatValue(0,param_2,param_3,param_4,0,param_5,0);
  StatCalculator_GetStatPercentageFromRating_2
            (param_1,param_2 & 0xff,uVar1,param_3,(char)param_4,param_5,uVar2 & 0xffffffffffffff00,0
            );
  return;
}

