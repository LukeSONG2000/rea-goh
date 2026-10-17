// fsm_end_turn_condition VA 0x1819330f0 RVA 0x19330f0

void FUN_1819330f0(longlong param_1)

{
  code *pcVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined4 uStack_18;
  
  if (((*(longlong *)(param_1 + 0x78) == 0) ||
      (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240), lVar3 == 0)) ||
     ((lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x240),
      *(char *)(lVar3 + 0x22f) != '\0' && (lVar4 = *(longlong *)(lVar4 + 0x230), lVar4 == 0)))) {
    FUN_180352450();
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
  FUN_181bccd50(lVar4,CONCAT71((int7)((ulonglong)lVar3 >> 8),0x21),0);
  *(undefined1 *)(param_1 + 0xa3) = 1;
  if (*(char *)(param_1 + 0x80) == '\0') {
    if ((*(longlong *)(param_1 + 0x78) == 0) ||
       (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x1e8), lVar3 == 0)) {
DAT_18192a8f5:
      FUN_180352450();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
    if ((*(char *)(lVar3 + 0xb65) != '\0') && (*(char *)(param_1 + 0xa4) != '\0')) {
      if (cRam0000000185a3d8ae == '\0') {
        FUN_18035da50(0x185785328,0x26,0);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185785330);
        LOCK();
        UNLOCK();
        cRam0000000185a3d8ae = '\x01';
      }
      uStack_28 = uRam0000000185785330;
      uStack_20 = 0xffffffffffffffff;
      uStack_18 = 0x26;
      lVar3 = FUN_183b04fe0(&uStack_28,0);
      if (*(int *)(lRam0000000185785328 + 0xe4) == 0) {
        FUN_180359940(lRam0000000185785328);
      }
      if (lVar3 == 0) goto DAT_18192a8f5;
      cVar2 = FUN_1838d73b0(lVar3,**(undefined8 **)(lRam0000000185785328 + 0xb8),0);
      if (cVar2 == '\0') {
        return;
      }
    }
    *(undefined4 *)(param_1 + 0x100) = 0x26;
  }
  return;
}

