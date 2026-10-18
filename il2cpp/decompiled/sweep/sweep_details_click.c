// sweep_details_click VA 0x181acebf0 RVA 0x1acebf0

void FUN_181acebf0(longlong param_1,longlong param_2)

{
  char cVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  undefined1 uVar5;
  longlong lVar6;
  undefined *puVar7;
  longlong lVar8;
  undefined *puVar9;
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  if (cRam0000000185a3e32e == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858181f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858181f0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e32e = '\x01';
  }
  lVar2 = *(longlong *)(param_1 + 0x70);
  if (lVar2 != 0) {
    if (cRam0000000185a3ac3a == '\0') {
      FUN_18035da50(0x185788a00);
      LOCK();
      UNLOCK();
      cRam0000000185a3ac3a = '\x01';
    }
    uStack_28 = 0x88;
    uStack_20 = 0;
    FUN_18001f4d0(lVar2,&uStack_28);
    lVar2 = *(longlong *)(param_1 + 0xa8);
    lVar6 = FUN_1803a51b0(uRam000000018580d858);
    lVar4 = lRam00000001858181f0;
    puVar7 = *(undefined **)(lRam00000001858181f0 + 8);
    *(undefined **)(lVar6 + 0x10) = puVar7;
    *(longlong *)(lVar6 + 0x28) = lVar4;
    *(longlong *)(lVar6 + 0x20) = param_1;
    cVar1 = *(char *)(lVar4 + 0x52);
    *(longlong *)(lVar6 + 0x40) = lVar6;
    if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
      *(longlong *)(lVar6 + 0x40) = param_1;
    }
    else {
      puVar7 = &UNK_180006440;
    }
    *(undefined **)(lVar6 + 0x18) = puVar7;
    *(code **)(lVar6 + 0x38) = FUN_1800063e0;
    lVar8 = FUN_1803a51b0(uRam000000018580d858);
    lVar4 = lRam00000001858181f8;
    puVar7 = *(undefined **)(lRam00000001858181f8 + 8);
    *(undefined **)(lVar8 + 0x10) = puVar7;
    *(longlong *)(lVar8 + 0x28) = lVar4;
    *(longlong *)(lVar8 + 0x20) = param_1;
    cVar1 = *(char *)(lVar4 + 0x52);
    *(longlong *)(lVar8 + 0x40) = lVar8;
    if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) || (puVar9 = &UNK_180006440, cVar1 != '\0')) {
      *(longlong *)(lVar8 + 0x40) = param_1;
      puVar9 = puVar7;
    }
    *(undefined **)(lVar8 + 0x18) = puVar9;
    *(code **)(lVar8 + 0x38) = FUN_1800063e0;
    if (lVar2 != 0) {
      FUN_18164dbe0(lVar2,param_2,lVar6,lVar8,1,0);
      if (((param_2 != 0) && (*(longlong *)(param_2 + 0xe8) != 0)) &&
         (*(longlong *)(param_1 + 0x58) != 0)) {
        uVar5 = FUN_1813aaa30(*(longlong *)(param_1 + 0x58),
                              *(undefined8 *)(*(longlong *)(param_2 + 0xe8) + 0x10),
                              **(undefined8 **)(lRam0000000185a33e80 + 0xb8),0);
        if ((*(longlong *)(param_2 + 0xe8) != 0) && (*(longlong *)(param_1 + 0x50) != 0)) {
          FUN_1818dc0c0(*(longlong *)(param_1 + 0x50),*(undefined1 *)(param_2 + 0x21),uVar5,
                        *(undefined8 *)(*(longlong *)(param_2 + 0xe8) + 0x10),0);
          return;
        }
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

