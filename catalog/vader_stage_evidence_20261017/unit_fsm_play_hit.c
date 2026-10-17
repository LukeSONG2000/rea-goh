// unit_fsm_play_hit VA 0x180fe84d0 RVA 0xfe84d0

void FUN_180fe84d0(longlong param_1)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  longlong lVar4;
  
  if (cRam0000000185a39f47 == '\0') {
    FUN_18035da50(0x1857a8a88);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8a48);
    LOCK();
    UNLOCK();
    cRam0000000185a39f47 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x40) != 0) {
    cVar3 = FUN_180015050(8,uRam00000001857a8a88,*(longlong *)(param_1 + 0x40),0,0);
    if (cVar3 == '\0') {
      *(undefined1 *)(param_1 + 0x5a) = 1;
      lVar4 = lRam00000001857a8a48;
      if ((*(longlong *)(param_1 + 0x20) == 0) ||
         (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x20), lVar1 == 0))
      goto code_r0x000180fe85cf;
      if (cRam0000000185a4a365 == '\0') {
        FUN_18035da50(0x1858366d0);
        LOCK();
        UNLOCK();
        cRam0000000185a4a365 = '\x01';
      }
      if ((lVar4 != 0) && (*(int *)(lVar4 + 0x10) != 0)) {
        if (*(int *)(lRam00000001858366d0 + 0xe4) == 0) {
          FUN_180359940();
        }
        lVar4 = FUN_183ccd510(lVar4,0);
        if (lVar4 != 0) {
          FUN_183cc51e0(lVar1,*(undefined8 *)(lVar1 + 0x120),lVar4,0);
        }
      }
    }
    return;
  }
code_r0x000180fe85cf:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

