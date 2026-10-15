// ==== stat_converted_pct @ 0x180fdff40 ====


undefined8 FUN_180fdff40(longlong param_1,undefined1 param_2,longlong param_3)

{
  longlong lVar1;
  code *pcVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  undefined8 uVar5;
  longlong *plVar6;
  
  if (cRam0000000185a39ef2 == '\0') {
    FUN_18035da50(0x1857a8f48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8ec0);
    LOCK();
    UNLOCK();
    cRam0000000185a39ef2 = '\x01';
  }
  if (cRam0000000185a39eed == '\0') {
    FUN_18035da50(0x1857a8f60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c0e8);
    LOCK();
    UNLOCK();
    cRam0000000185a39eed = '\x01';
  }
  if (*(longlong *)(param_1 + 0x18) == 0) {
    if (*(int *)(lRam000000018582c0e8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (cRam0000000185a38a62 == '\0') {
      FUN_18035da50(0x18582c0e8);
      LOCK();
      UNLOCK();
      cRam0000000185a38a62 = '\x01';
    }
    if (*(int *)(lRam000000018582c0e8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(*(longlong *)(lRam000000018582c0e8 + 0xb8) + 0x18) == 0)
    goto code_r0x000180fe00f5;
    uVar5 = FUN_180015220();
    *(undefined8 *)(param_1 + 0x18) = uVar5;
  }
  if (param_3 != 0) {
    lVar1 = *(longlong *)(param_1 + 0x18);
    uVar3 = FUN_1800084f0(0,uRam00000001857a8f48,param_3);
    uVar4 = FUN_1800084f0(1,uRam00000001857a8f48,param_3);
    if (lVar1 != 0) {
      plVar6 = (longlong *)FUN_181be74a0(lVar1,uVar3,uVar4,0);
      if (plVar6 != (longlong *)0x0) {
        if ((*(byte *)(lRam00000001857a8ec0 + 0x130) <= *(byte *)(*plVar6 + 0x130)) &&
           (*(longlong *)
             (*(longlong *)(*plVar6 + 200) + -8 +
             (ulonglong)*(byte *)(lRam00000001857a8ec0 + 0x130) * 8) == lRam00000001857a8ec0)) {
          uVar5 = FUN_182f7f730(plVar6,param_2,0,uRam00000001857a8ed0);
          return uVar5;
        }
      }
      return 0;
    }
  }
code_r0x000180fe00f5:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar5 = (*pcVar2)();
  return uVar5;
}

