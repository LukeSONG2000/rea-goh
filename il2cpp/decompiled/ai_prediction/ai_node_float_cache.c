// ai_node_float_cache VA 0x180fec9b0 RVA 0xfec9b0

ulonglong FUN_180fec9b0(longlong param_1)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  ulonglong uVar4;
  
  if (cRam0000000185a39f7a == '\0') {
    FUN_18035da50(0x18582c7d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ad648);
    LOCK();
    UNLOCK();
    cRam0000000185a39f7a = '\x01';
  }
  if (*(char *)(param_1 + 0x45) == '\0') {
    lVar1 = *(longlong *)(param_1 + 0x20);
    if (lVar1 == 0) {
code_r0x000180feca5f:
      FUN_180352450();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
    if (*(int *)(lVar1 + 0x18) == 0) {
code_r0x000180feca65:
      FUN_180352460();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
    cVar3 = FUN_180cb1b80(*(undefined8 *)(lVar1 + 0x20),param_1 + 0x48,0);
    if (cVar3 == '\0') {
      lVar1 = *(longlong *)(param_1 + 0x20);
      if (lVar1 == 0) goto code_r0x000180feca5f;
      if (*(int *)(lVar1 + 0x18) == 0) goto code_r0x000180feca65;
      FUN_182b6d2e0(param_1,uRam00000001857ad648,*(undefined8 *)(lVar1 + 0x20),0xffffffff,
                    uRam000000018582c7d0);
      *(undefined4 *)(param_1 + 0x48) = 0;
    }
    *(undefined1 *)(param_1 + 0x45) = 1;
  }
  return (ulonglong)*(uint *)(param_1 + 0x48);
}

