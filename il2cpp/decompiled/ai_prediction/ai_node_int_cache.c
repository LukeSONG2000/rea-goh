// ai_node_int_cache VA 0x180feca70 RVA 0xfeca70

ulonglong FUN_180feca70(longlong param_1)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  ulonglong uVar4;
  
  if (cRam0000000185a39f7b == '\0') {
    FUN_18035da50(0x18582c7d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ad640);
    LOCK();
    UNLOCK();
    cRam0000000185a39f7b = '\x01';
  }
  if (*(char *)(param_1 + 0x4c) == '\0') {
    lVar1 = *(longlong *)(param_1 + 0x20);
    if (lVar1 == 0) {
code_r0x000180fecb1d:
      FUN_180352450();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
    if (*(int *)(lVar1 + 0x18) == 0) {
code_r0x000180fecb23:
      FUN_180352460();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
    cVar3 = FUN_180cb21c0(*(undefined8 *)(lVar1 + 0x20),param_1 + 0x50,0);
    if (cVar3 == '\0') {
      lVar1 = *(longlong *)(param_1 + 0x20);
      if (lVar1 == 0) goto code_r0x000180fecb1d;
      if (*(int *)(lVar1 + 0x18) == 0) goto code_r0x000180fecb23;
      FUN_182b6d2e0(param_1,uRam00000001857ad640,*(undefined8 *)(lVar1 + 0x20),0xffffffff,
                    uRam000000018582c7d0);
      *(undefined4 *)(param_1 + 0x50) = 0;
    }
    *(undefined1 *)(param_1 + 0x4c) = 1;
  }
  return (ulonglong)*(uint *)(param_1 + 0x50);
}

