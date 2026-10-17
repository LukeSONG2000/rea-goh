// blackboard_get_value VA 0x1816a8bb0 RVA 0x16a8bb0

ulonglong FUN_1816a8bb0(longlong param_1,undefined8 param_2)

{
  code *pcVar1;
  int iVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  if (cRam0000000185a3c875 == '\0') {
    FUN_18035da50(0x1857a45a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a45b0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c875 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    iVar2 = FUN_18332e340(*(longlong *)(param_1 + 0x10),param_2,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam00000001857a45a0 + 0x20) + 0xc0) + 0x108
                           ));
    if (iVar2 < 0) {
      return 0;
    }
    if (*(longlong *)(param_1 + 0x10) != 0) {
      lVar3 = FUN_18332db60(*(longlong *)(param_1 + 0x10),param_2,uRam00000001857a45b0);
      if (lVar3 != 0) {
        return (ulonglong)*(uint *)(lVar3 + 0x10);
      }
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar4 = (*pcVar1)();
  return uVar4;
}

