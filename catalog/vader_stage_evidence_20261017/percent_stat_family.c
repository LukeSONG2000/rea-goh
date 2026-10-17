// percent_stat_family VA 0x180fe0280 RVA 0xfe0280

longlong FUN_180fe0280(undefined8 param_1,byte param_2)

{
  code *pcVar1;
  int3 iVar2;
  longlong lVar3;
  int7 iVar5;
  undefined8 uVar4;
  uint uVar7;
  int7 iVar6;
  
  uVar7 = (uint)param_2;
  if (cRam0000000185a39ef4 == '\0') {
    FUN_18035da50(0x1857a8eb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f10);
    LOCK();
    UNLOCK();
    cRam0000000185a39ef4 = '\x01';
  }
  if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
    FUN_180359940(lRam00000001857a8f18);
  }
  if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
     (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
     lVar3 == 0)) goto code_r0x000180fe0441;
  lVar3 = FUN_180fe0b10(lVar3,0);
  if (uVar7 - 1 < 0xb) {
    iVar2 = (int3)(uVar7 - 1 >> 8);
    iVar5 = (int7)iVar2;
    iVar6 = (int7)iVar2;
    switch(uVar7 - 1) {
    case 1:
      if (lVar3 == 0) {
code_r0x000180fe0441:
        FUN_180352450();
        pcVar1 = (code *)swi(3);
        lVar3 = (*pcVar1)();
        return lVar3;
      }
      uVar4 = CONCAT71(iVar6,0x2b);
      break;
    default:
      goto code_r0x000180fe0433;
    case 5:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      uVar4 = CONCAT71(iVar6,0x2d);
      break;
    case 6:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar6,0x35));
      uVar4 = 0x24;
      break;
    case 7:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar6,0x35));
      uVar4 = 0x25;
      break;
    case 8:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar5,0x1e));
      uVar4 = 0x36;
      break;
    case 9:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar5,0x1f));
      uVar4 = 0x36;
      break;
    case 10:
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar5,0x20));
      uVar4 = 0x37;
      break;
    case 0xb:
                    /* WARNING: This code block may not be properly labeled as switch case */
      if (lVar3 == 0) goto code_r0x000180fe0441;
      FUN_180014ac0(lVar3,CONCAT71(iVar5,0x21));
      uVar4 = 0x37;
    }
  }
  else if (param_2 == 0x19) {
    if (lVar3 == 0) goto code_r0x000180fe0441;
    uVar4 = 0x2c;
  }
  else if (uVar7 == 0x2e) {
    if (lVar3 == 0) goto code_r0x000180fe0441;
    uVar4 = 0x35;
  }
  else if (uVar7 == 0x2f) {
    if (lVar3 == 0) goto code_r0x000180fe0441;
    uVar4 = 0x36;
  }
  else {
    if (uVar7 != 0x30) {
      return lVar3;
    }
    if (lVar3 == 0) goto code_r0x000180fe0441;
    uVar4 = 0x37;
  }
  FUN_180014ac0(lVar3,uVar4);
code_r0x000180fe0433:
  return lVar3;
}

