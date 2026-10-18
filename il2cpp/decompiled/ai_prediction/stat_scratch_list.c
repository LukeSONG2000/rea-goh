// stat_scratch_list VA 0x180fe0b10 RVA 0xfe0b10

void FUN_180fe0b10(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  if (cRam0000000185a39ef9 == '\0') {
    FUN_18035da50(0x1857a8e68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8e50);
    LOCK();
    UNLOCK();
    cRam0000000185a39ef9 = '\x01';
  }
  if (*(int *)(param_1 + 0x18) < 0) {
    uVar4 = func_0x000180352160(0x18580de10);
    uVar4 = func_0x000180352400(uVar4);
    uVar5 = func_0x000180352160(0x1857a8e48);
    FUN_183b08790(uVar4,uVar5,0);
    uVar5 = func_0x000180352160(0x1857a8e58);
    FUN_180352410(uVar4,uVar5);
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
    return;
  }
  lVar2 = *(longlong *)(param_1 + 0x10);
  if (lVar2 != 0) {
    uVar1 = *(uint *)(param_1 + 0x18);
    if (*(uint *)(lVar2 + 0x18) <= uVar1) {
      FUN_183aeda10(0);
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    lVar2 = *(longlong *)(lVar2 + 0x10);
    if (lVar2 != 0) {
      if (*(uint *)(lVar2 + 0x18) <= uVar1) {
        FUN_180352460();
        pcVar3 = (code *)swi(3);
        (*pcVar3)();
        return;
      }
      lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)uVar1 * 8);
      if (lVar2 != 0) {
        *(int *)(lVar2 + 0x1c) = *(int *)(lVar2 + 0x1c) + 1;
        *(undefined4 *)(lVar2 + 0x18) = 0;
        return;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

