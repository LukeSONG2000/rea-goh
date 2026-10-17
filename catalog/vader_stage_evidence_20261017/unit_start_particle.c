// unit_start_particle VA 0x180fe0e40 RVA 0xfe0e40

void FUN_180fe0e40(undefined8 param_1,undefined8 param_2)

{
  longlong lVar1;
  
  if (cRam0000000185a39efe == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39efe = '\x01';
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
      FUN_18453bc40(lVar1,0,1,1,1,0);
      FUN_18453bd90(lVar1,1,0);
    }
  }
  return;
}

