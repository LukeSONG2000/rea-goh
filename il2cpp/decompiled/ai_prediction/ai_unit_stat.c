// ai_unit_stat VA 0x181bbae00 RVA 0x1bbae00

ulonglong FUN_181bbae00(longlong param_1,undefined1 param_2)

{
  code *pcVar1;
  ulonglong uVar2;
  float fVar3;
  char cVar4;
  undefined4 uVar5;
  ulonglong uVar6;
  float fVar7;
  float afStackX_8 [2];
  float afStackX_10 [2];
  undefined8 in_stack_ffffffffffffffc0;
  undefined4 uVar8;
  
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffc0 >> 0x20);
  if (cRam0000000185a3e8fc == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8fc = '\x01';
  }
  afStackX_10[0] = 0.0;
  afStackX_8[0] = 0.0;
  uVar5 = FUN_181bbaf90(param_1,param_2,0);
  if (*(longlong *)(param_1 + 0x160) == 0) goto code_r0x000181bbaf7e;
  uVar6 = FUN_180fde5c0(*(longlong *)(param_1 + 0x160),param_2,param_1,0,uVar5,
                        CONCAT44(uVar8,0xffffffff),0);
  fVar7 = (float)uVar6;
  if (*(longlong *)(param_1 + 0x60) == 0) goto code_r0x000181bbaf7e;
  cVar4 = FUN_181a73490(*(longlong *)(param_1 + 0x60),fVar7,param_2,0x16,0x22,afStackX_10,0);
  fVar3 = afStackX_10[0];
  if (cVar4 == '\0') {
code_r0x000181bbaefa:
    uVar2 = uVar6;
  }
  else {
    uVar2 = (ulonglong)(uint)afStackX_10[0];
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    if ((fVar7 < fVar3) || (0x7f800000 < (uint)ABS(fVar7))) goto code_r0x000181bbaefa;
  }
  fVar7 = (float)uVar2;
  if (*(longlong *)(param_1 + 0x60) != 0) {
    cVar4 = FUN_181a73030(*(longlong *)(param_1 + 0x60),fVar7,param_2,0x16,0x21,afStackX_8,0);
    fVar3 = afStackX_8[0];
    if (cVar4 != '\0') {
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      if ((fVar7 <= fVar3) && ((uint)ABS(fVar7) < 0x7f800001)) {
        uVar2 = (ulonglong)(uint)fVar3;
      }
    }
    return uVar2;
  }
code_r0x000181bbaf7e:
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar6 = (*pcVar1)();
  return uVar6;
}

