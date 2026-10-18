// store_home_open VA 0x18173bfa0 RVA 0x173bfa0

void FUN_18173bfa0(longlong param_1,undefined4 param_2)

{
  longlong lVar1;
  code *pcVar2;
  longlong *plVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  
  if (cRam0000000185a3cc60 == '\0') {
    FUN_18035da50(0x1857b0be8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d3da0);
    LOCK();
    UNLOCK();
    cRam0000000185a3cc60 = '\x01';
  }
  lVar5 = *(longlong *)(param_1 + 0x268);
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056f == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056f = '\x01';
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5054a == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5054a = '\x01';
  }
  if (lVar5 == 0) {
code_r0x00018173c0ce:
    uVar4 = **(undefined8 **)(lRam0000000185a33e80 + 0xb8);
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar5 + 0x10) == 0) goto code_r0x00018173c0ce;
    if (*(longlong **)(param_1 + 0x268) == (longlong *)0x0) goto code_r0x00018173c153;
    plVar3 = (longlong *)FUN_1803a7d00(**(longlong **)(param_1 + 0x268) + 0x20);
    if (plVar3 == (longlong *)0x0) goto code_r0x00018173c153;
    uVar4 = (**(code **)(*plVar3 + 0x1b8))(plVar3,*(undefined8 *)(*plVar3 + 0x1c0));
  }
  lVar5 = FUN_1803a51b0(uRam00000001857d3da0);
  if (lVar5 != 0) {
    *(undefined4 *)(lVar5 + 0x1c) = param_2;
    *(undefined4 *)(lVar5 + 0x24) = 4;
    *(undefined8 *)(lVar5 + 0x30) = uVar4;
    lVar6 = lRam00000001857b0be8;
    if (*(longlong *)(lRam00000001857b0be8 + 0x38) == 0) {
      FUN_18034f250(lRam00000001857b0be8);
    }
    FUN_18173cb60(param_1,0);
    lVar1 = *(longlong *)(param_1 + 0x58);
    if (lVar1 != 0) {
      lVar6 = **(longlong **)(lVar6 + 0x38);
      if (*(longlong *)(lVar6 + 0x38) == 0) {
        FUN_18034f250(lVar6);
      }
      FUN_180fcd110(lVar1,1,0);
      lVar1 = *(longlong *)(lVar1 + 0x18);
      lVar6 = **(longlong **)(lVar6 + 0x38);
      if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
        FUN_180359940();
      }
      if (lVar6 == 0) {
        uVar4 = 0;
      }
      else {
        if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
          FUN_180359940();
        }
        lVar6 = FUN_1803ab790(lVar6,1);
        uVar4 = FUN_1803a7d00(lVar6 + 0x20);
      }
      if (lVar1 != 0) {
        FUN_1812a32d0(lVar1,uVar4,lVar5,1,0);
        return;
      }
      FUN_180352450();
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
  }
code_r0x00018173c153:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

