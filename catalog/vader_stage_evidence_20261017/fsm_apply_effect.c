// fsm_apply_effect VA 0x180fd2940 RVA 0xfd2940

void FUN_180fd2940(longlong param_1)

{
  undefined1 uVar1;
  char cVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  longlong lVar6;
  undefined *puVar7;
  
  if (cRam0000000185a39ea7 == '\0') {
    FUN_18035da50(0x1857a9620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    cRam0000000185a39ea7 = '\x01';
  }
  lVar3 = *(longlong *)(param_1 + 0x38);
  uVar1 = *(undefined1 *)(param_1 + 0x83);
  lVar6 = FUN_1803a51b0(uRam000000018580d858);
  lVar5 = lRam00000001857a9620;
  puVar7 = *(undefined **)(lRam00000001857a9620 + 8);
  *(undefined **)(lVar6 + 0x10) = puVar7;
  *(longlong *)(lVar6 + 0x28) = lVar5;
  *(longlong *)(lVar6 + 0x20) = param_1;
  cVar2 = *(char *)(lVar5 + 0x52);
  *(longlong *)(lVar6 + 0x40) = lVar6;
  if (((*(byte *)(lVar5 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
    *(longlong *)(lVar6 + 0x40) = param_1;
  }
  else {
    puVar7 = &UNK_180006440;
  }
  *(undefined **)(lVar6 + 0x18) = puVar7;
  *(code **)(lVar6 + 0x38) = FUN_1800063e0;
  if (lVar3 != 0) {
    FUN_18184b450(lVar3,CONCAT71((int7)((ulonglong)puVar7 >> 8),1),uVar1,lVar6,0);
    return;
  }
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

