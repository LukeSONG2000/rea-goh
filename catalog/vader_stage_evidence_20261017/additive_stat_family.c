// additive_stat_family VA 0x180fe0480 RVA 0xfe0480

longlong FUN_180fe0480(undefined8 param_1,uint param_2)

{
  code *pcVar1;
  longlong lVar2;
  undefined7 uVar4;
  undefined8 uVar3;
  
  if (cRam0000000185a39ef5 == '\0') {
    FUN_18035da50(0x1857a8eb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f10);
    LOCK();
    UNLOCK();
    cRam0000000185a39ef5 = '\x01';
  }
  if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
    FUN_180359940(lRam00000001857a8f18);
  }
  if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
     (lVar2 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
     lVar2 == 0)) {
code_r0x000180fe05ec:
    FUN_180352450();
    pcVar1 = (code *)swi(3);
    lVar2 = (*pcVar1)();
    return lVar2;
  }
  lVar2 = FUN_180fe0b10(lVar2,0);
  uVar4 = (undefined7)
          ((ulonglong)
           *(uint *)(&UNK_180fe05f4 +
                    (ulonglong)(byte)(&UNK_180fe0630)[(int)((param_2 & 0xff) - 6)] * 4) +
           0x180000000 >> 8);
  switch(param_2 & 0xff) {
  case 0xc:
  case 0xd:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x2e);
    break;
  case 0xe:
  case 0xf:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x2f);
    break;
  case 0x10:
  case 0x11:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x30);
    break;
  case 0x12:
  case 0x13:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x31);
    break;
  case 0x14:
  case 0x15:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x32);
    break;
  default:
    goto code_r0x000180fe05de;
  case 0x1a:
  case 0x1b:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x33);
    break;
  case 0x1c:
  case 0x1d:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x34);
    break;
  case 0x20:
  case 0x21:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x39);
    break;
  case 0x22:
  case 0x23:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x3b);
    break;
  case 0x24:
  case 0x25:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x36);
    break;
  case 0x26:
  case 0x27:
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x37);
    break;
  case 0x2a:
  case 0x2b:
                    /* WARNING: This code block may not be properly labeled as switch case */
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x35);
    break;
  case 0x2c:
  case 0x2d:
                    /* WARNING: This code block may not be properly labeled as switch case */
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x3a);
    break;
  case 0x2e:
  case 0x2f:
                    /* WARNING: This code block may not be properly labeled as switch case */
    if (lVar2 == 0) goto code_r0x000180fe05ec;
    uVar3 = CONCAT71(uVar4,0x3c);
  }
  FUN_180014ac0(lVar2,uVar3);
code_r0x000180fe05de:
  return lVar2;
}

