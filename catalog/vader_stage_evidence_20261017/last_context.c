// last_context VA 0x181360420 RVA 0x1360420

undefined8 FUN_181360420(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  undefined8 uVar4;
  
  if (cRam0000000185a3b34e == '\0') {
    FUN_18035da50(0x1857b1978);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d2e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d370);
    LOCK();
    UNLOCK();
    cRam0000000185a3b34e = '\x01';
  }
  lVar1 = *(longlong *)(param_1 + 0x18);
  if (lVar1 != 0) {
    if (*(int *)(lVar1 + 0x18) < 1) {
      if (*(int *)(lRam00000001857b1978 + 0xe4) == 0) {
        FUN_180359940(lRam00000001857b1978);
      }
      return *(undefined8 *)(*(longlong *)(lRam00000001857b1978 + 0xb8) + 8);
    }
    if (lVar1 != 0) {
      if (*(uint *)(lVar1 + 0x18) <= *(int *)(lVar1 + 0x18) - 1U) {
        FUN_183aeda10(0);
        pcVar3 = (code *)swi(3);
        uVar4 = (*pcVar3)();
        return uVar4;
      }
      lVar2 = *(longlong *)(lVar1 + 0x10);
      if (lVar2 != 0) {
        lVar1 = (longlong)*(int *)(lVar1 + 0x18) + -1;
        if ((uint)lVar1 < *(uint *)(lVar2 + 0x18)) {
          return *(undefined8 *)(lVar2 + 0x20 + lVar1 * 8);
        }
        FUN_180352460();
        pcVar3 = (code *)swi(3);
        uVar4 = (*pcVar3)();
        return uVar4;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  uVar4 = (*pcVar3)();
  return uVar4;
}

