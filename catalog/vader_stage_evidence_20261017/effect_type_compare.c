// effect_type_compare VA 0x180ffb210 RVA 0xffb210

undefined8 FUN_180ffb210(char *param_1,longlong *param_2)

{
  byte bVar1;
  byte bVar2;
  code *pcVar3;
  undefined8 uVar4;
  char acStackX_10 [24];
  undefined8 uStack_28;
  undefined8 uStack_20;
  char cStack_18;
  
  if (cRam0000000185a39fcf == '\0') {
    FUN_18035da50(0x1857acee0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3958);
    LOCK();
    UNLOCK();
    cRam0000000185a39fcf = '\x01';
  }
  if (param_2 == (longlong *)0x0) {
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    uVar4 = (*pcVar3)();
    return uVar4;
  }
  if (*(longlong *)(*param_2 + 0x40) != *(longlong *)(lRam00000001857acee0 + 0x40)) {
    FUN_180352e70(param_2);
    pcVar3 = (code *)swi(3);
    uVar4 = (*pcVar3)();
    return uVar4;
  }
  acStackX_10[0] = (char)param_2[2];
  if (*param_1 == acStackX_10[0]) {
    bVar1 = *(byte *)((longlong)param_2 + 0x11);
    bVar2 = param_1[1];
    if (bVar2 == 0) {
      if (bVar1 == 0) {
        return 0;
      }
      return 1;
    }
    if (bVar1 != 0) {
      if (bVar2 == bVar1) {
        return 0;
      }
      if (bVar1 < bVar2) {
        return 1;
      }
    }
    return 0xffffffff;
  }
  uVar4 = FUN_1803a4e80(uRam00000001857a3958,acStackX_10);
  uStack_28 = uRam00000001857a3958;
  cStack_18 = *param_1;
  uStack_20 = 0xffffffffffffffff;
  uVar4 = FUN_183b050e0(&uStack_28,uVar4,0);
  return uVar4;
}

