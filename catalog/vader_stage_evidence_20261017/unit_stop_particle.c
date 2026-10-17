// unit_stop_particle VA 0x180fe0f50 RVA 0xfe0f50

void FUN_180fe0f50(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  
  if (cRam0000000185a39eff == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39eff = '\x01';
  }
  lVar1 = FUN_180fe1040(param_1,param_2,0);
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
  if (lVar1 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar1 + 0x10) != 0) {
      FUN_18453c070(lVar1,0);
    }
  }
  return;
}

