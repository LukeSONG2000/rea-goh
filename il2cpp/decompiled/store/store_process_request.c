// store_process_request VA 0x181c41630 RVA 0x1c41630

void FUN_181c41630(longlong param_1,longlong param_2,undefined4 param_3,undefined1 param_4,
                  undefined8 param_5,longlong param_6,undefined8 param_7)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  longlong lVar4;
  code *pcVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  longlong lVar11;
  undefined *puVar12;
  undefined *puVar13;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined4 uStack_30;
  
  if (cRam0000000185a3ec45 == '\0') {
    FUN_18035da50(0x18582adf8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582adf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582ade0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582add8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857bcff8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec45 = '\x01';
  }
  if (param_6 != 0) {
    (**(code **)(param_6 + 0x18))(*(undefined8 *)(param_6 + 0x40),*(undefined8 *)(param_6 + 0x28));
  }
  if (*(longlong *)(param_1 + 0x58) != 0) {
    uVar7 = FUN_180fcd800(*(longlong *)(param_1 + 0x58),2,param_1,0);
    *(undefined8 *)(param_1 + 0xf8) = uVar7;
    if (param_2 != 0) {
      uVar7 = *(undefined8 *)(param_2 + 0x108);
      lVar4 = *(longlong *)(param_1 + 0xb0);
      uVar2 = *(undefined4 *)(param_1 + 0x100);
      uStack_40 = uRam00000001857bcff8;
      uStack_30 = *(undefined4 *)(param_1 + 0x104);
      uStack_38 = 0xffffffffffffffff;
      uVar8 = FUN_183b04fe0(&uStack_40,0);
      uVar9 = FUN_1803a51b0(uRam000000018582adf8);
      FUN_1820f2fe0(uVar9,param_1,uRam000000018582ade0,0);
      lVar10 = FUN_1803a51b0(uRam000000018580d858);
      lVar6 = lRam000000018582adf0;
      puVar12 = *(undefined **)(lRam000000018582adf0 + 8);
      *(undefined **)(lVar10 + 0x10) = puVar12;
      *(longlong *)(lVar10 + 0x28) = lVar6;
      *(longlong *)(lVar10 + 0x20) = param_1;
      cVar1 = *(char *)(lVar6 + 0x52);
      *(longlong *)(lVar10 + 0x40) = lVar10;
      if (((*(byte *)(lVar6 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
        *(longlong *)(lVar10 + 0x40) = param_1;
      }
      else {
        puVar12 = &UNK_180006440;
      }
      *(undefined **)(lVar10 + 0x18) = puVar12;
      *(code **)(lVar10 + 0x38) = FUN_1800063e0;
      uVar3 = *(undefined4 *)(param_1 + 0x108);
      lVar11 = FUN_1803a51b0(uRam000000018580d858);
      lVar6 = lRam000000018582add8;
      puVar12 = *(undefined **)(lRam000000018582add8 + 8);
      *(undefined **)(lVar11 + 0x10) = puVar12;
      *(longlong *)(lVar11 + 0x28) = lVar6;
      *(longlong *)(lVar11 + 0x20) = param_1;
      cVar1 = *(char *)(lVar6 + 0x52);
      *(longlong *)(lVar11 + 0x40) = lVar11;
      if (((*(byte *)(lVar6 + 0x4c) & 0x10) == 0) || (puVar13 = &UNK_180006440, cVar1 != '\0')) {
        *(longlong *)(lVar11 + 0x40) = param_1;
        puVar13 = puVar12;
      }
      *(undefined **)(lVar11 + 0x18) = puVar13;
      *(code **)(lVar11 + 0x38) = FUN_1800063e0;
      if (lVar4 != 0) {
        FUN_18002c4d0(lVar6,lVar4,param_2,uVar7,param_3,uVar2,uVar8,uVar9,lVar10,param_4,param_5,
                      uVar3,lVar11,param_7);
        return;
      }
    }
  }
  FUN_180352450();
  pcVar5 = (code *)swi(3);
  (*pcVar5)();
  return;
}

