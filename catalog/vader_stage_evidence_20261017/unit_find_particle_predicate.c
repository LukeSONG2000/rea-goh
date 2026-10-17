// unit_find_particle_predicate VA 0x180fe1b40 RVA 0xfe1b40

bool FUN_180fe1b40(longlong param_1,longlong param_2)

{
  longlong lVar1;
  ulonglong uVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  if (cRam0000000185a39f07 == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f07 = '\x01';
  }
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
  if (param_2 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(param_2 + 0x10) != 0) {
      lVar3 = FUN_1844aedc0(param_2,0);
      lVar1 = *(longlong *)(param_1 + 0x10);
      if (lVar3 != lVar1) {
        if (lVar3 == 0) {
          return false;
        }
        if (lVar1 == 0) {
          return false;
        }
        if (*(int *)(lVar3 + 0x10) != *(int *)(lVar1 + 0x10)) {
          return false;
        }
        uVar4 = 0;
        uVar2 = (longlong)*(int *)(lVar3 + 0x10) * 2;
        if (7 < uVar2) {
          if (uVar2 != 8) {
            do {
              if (*(longlong *)(lVar3 + 0x14 + uVar4) != *(longlong *)(lVar1 + 0x14 + uVar4)) {
                return false;
              }
              uVar4 = uVar4 + 8;
            } while (uVar4 < uVar2 - 8);
          }
          return *(longlong *)(uVar2 + 0xc + lVar3) == *(longlong *)(uVar2 + 0xc + lVar1);
        }
        if (uVar2 != 0) {
          do {
            if (*(char *)(lVar3 + 0x14 + uVar4) != *(char *)(lVar1 + 0x14 + uVar4)) {
              return false;
            }
            uVar4 = uVar4 + 1;
          } while (uVar4 < uVar2);
        }
      }
      return true;
    }
  }
  return false;
}

