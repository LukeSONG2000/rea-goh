// set_speed_fill VA 0x181bcdb50 RVA 0x1bcdb50

void FUN_181bcdb50(longlong param_1,undefined4 param_2)

{
  code *pcVar1;
  
  if (cRam0000000185a3e958 == '\0') {
    FUN_18035da50(0x1857a96a0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e958 = '\x01';
  }
  *(undefined4 *)(param_1 + 0x208) = param_2;
  if ((*(longlong *)(param_1 + 0x88) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x180) != 0)) {
    FUN_180037e10();
    return;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

