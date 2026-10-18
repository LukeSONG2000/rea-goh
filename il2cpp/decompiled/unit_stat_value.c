// unit_stat_value VA 0x181bbae00 RVA 0x1bbae00

/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float FUN_181bbae00(void)

{
  code *pcVar1;
  char cVar2;
  longlong in_RCX;
  undefined1 in_DL;
  float fVar3;
  float fVar4;
  float afStackX_8 [2];
  float afStackX_10 [2];
  
  if (cRam0000000185a3e8fc == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8fc = '\x01';
  }
  afStackX_10[0] = 0.0;
  afStackX_8[0] = 0.0;
  FUN_181bbaf90();
  if (*(longlong *)(in_RCX + 0x160) == 0) goto code_r0x000181bbaf7e;
  fVar3 = (float)FUN_180fde5c0(*(longlong *)(in_RCX + 0x160),in_DL);
  if (*(longlong *)(in_RCX + 0x60) == 0) goto code_r0x000181bbaf7e;
  cVar2 = FUN_181a73490(*(longlong *)(in_RCX + 0x60),fVar3,in_DL,0x16,0x22,afStackX_10,0);
  fVar4 = afStackX_10[0];
  if (cVar2 == '\0') {
code_r0x000181bbaefa:
    fVar4 = fVar3;
  }
  else {
    if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
      FUN_180359940();
    }
    if ((fVar3 < fVar4) || (0x7f800000 < (uint)ABS(fVar3))) goto code_r0x000181bbaefa;
  }
  if (*(longlong *)(in_RCX + 0x60) != 0) {
    cVar2 = FUN_181a73030(*(longlong *)(in_RCX + 0x60),fVar4,in_DL,0x16,0x21,afStackX_8,0);
    fVar3 = afStackX_8[0];
    if (cVar2 != '\0') {
      if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
        FUN_180359940();
      }
      if ((fVar4 <= fVar3) && ((uint)ABS(fVar4) < 0x7f800001)) {
        fVar4 = fVar3;
      }
    }
    return fVar4;
  }
code_r0x000181bbaf7e:
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  fVar3 = (float)(*pcVar1)();
  return fVar3;
}

