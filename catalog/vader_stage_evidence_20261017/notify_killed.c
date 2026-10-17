// notify_killed VA 0x181bc59f0 RVA 0x1bc59f0

void FUN_181bc59f0(longlong param_1,longlong param_2,undefined8 param_3)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  
  if (cRam0000000185a3e95f == '\0') {
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e95f = '\x01';
  }
  if (param_2 != 0) {
    lVar1 = *(longlong *)(param_2 + 0x38);
    if ((lVar1 == 0) || (lVar2 = *(longlong *)(lVar1 + 0xb8), lVar2 == 0)) {
code_r0x000181bc5aa2:
      FUN_180352450();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    if (0 < *(int *)(lVar2 + 0x18)) {
      if (lVar1 == 0) goto code_r0x000181bc5aa2;
      FUN_181bcd640(param_1,0xf,param_3,lVar2,0);
      goto code_r0x000181bc5a79;
    }
  }
  FUN_181bcd360(param_1,0xf,param_3,0);
code_r0x000181bc5a79:
  if (*(longlong *)(param_1 + 0x10) != 0) {
    lVar1 = *(longlong *)(param_1 + 0x10);
    (**(code **)(lVar1 + 0x18))(*(undefined8 *)(lVar1 + 0x40),param_1,*(undefined8 *)(lVar1 + 0x28))
    ;
  }
  return;
}

