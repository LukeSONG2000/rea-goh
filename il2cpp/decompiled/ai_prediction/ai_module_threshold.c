// ai_module_threshold VA 0x1816a6490 RVA 0x16a6490

bool FUN_1816a6490(undefined8 param_1,longlong param_2,longlong param_3,undefined8 param_4,
                  longlong *param_5)

{
  code *pcVar1;
  undefined1 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  
  if (cRam0000000185a3c861 == '\0') {
    FUN_18035da50(0x1857a4780);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c2450);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9648);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4778);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4788);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4798);
    LOCK();
    UNLOCK();
    cRam0000000185a3c861 = '\x01';
  }
  lVar3 = FUN_1803a51b0(uRam00000001857a4798);
  if ((lVar3 != 0) && (*(undefined8 *)(lVar3 + 0x10) = param_1, param_2 != 0)) {
    uVar6 = FUN_180fec9b0(param_2,0);
    *(undefined4 *)(lVar3 + 0x18) = uVar6;
    if (*(int *)(param_2 + 0x28) == 0) {
      uVar4 = FUN_1803a51b0(uRam00000001857a4780);
      uVar5 = uRam00000001857a4788;
    }
    else if (*(int *)(param_2 + 0x28) == 1) {
      uVar4 = FUN_1803a51b0(uRam00000001857a4780);
      uVar5 = uRam00000001857a4790;
    }
    else {
      uVar4 = FUN_1803a51b0(uRam00000001857a4780);
      uVar5 = uRam00000001857a47a0;
    }
    FUN_180e639d0(uVar4,lVar3,uVar5,0);
    *(undefined8 *)(lVar3 + 0x20) = uVar4;
    uVar5 = FUN_1803a51b0(uRam00000001857a9648);
    FUN_180e639d0(uVar5,lVar3,uRam00000001857a4778,0);
    if (param_3 != 0) {
      lVar3 = FUN_1836f55d0(param_3,uVar5,uRam00000001857c2450);
      *param_5 = lVar3;
      if (lVar3 != 0) {
        return 0 < *(int *)(lVar3 + 0x18);
      }
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar2 = (*pcVar1)();
  return (bool)uVar2;
}

