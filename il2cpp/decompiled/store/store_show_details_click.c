// store_show_details_click VA 0x181c4b950 RVA 0x1c4b950

void FUN_181c4b950(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  longlong lVar2;
  code *pcVar3;
  undefined4 uVar4;
  
  if (cRam0000000185a3ec83 == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec83 = '\x01';
  }
  if (*(char *)(param_1 + 0x209) == '\0') {
    lVar1 = *(longlong *)(param_1 + 0x200);
    if (lVar1 == 0) goto DAT_181c4ba9e;
    *(undefined4 *)(lVar1 + 0x20) = *(undefined4 *)(lVar1 + 0x1c);
    lVar1 = *(longlong *)(param_1 + 0x1e8);
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
        if ((*(longlong *)(param_1 + 0x1e8) == 0) ||
           (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x1e8) + 0x380), lVar1 == 0))
        goto DAT_181c4ba9e;
        lVar2 = *(longlong *)(param_1 + 0x200);
        uVar4 = FUN_181b644b0(lVar1,0);
        if (lVar2 == 0) goto DAT_181c4ba9e;
        *(undefined4 *)(lVar2 + 0x2c) = uVar4;
      }
    }
    if (*(longlong *)(param_1 + 0x200) == 0) {
DAT_181c4ba9e:
      FUN_180352450();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    *(undefined1 *)(*(longlong *)(param_1 + 0x200) + 0x14) = 0;
    FUN_181c4bab0(param_1,param_2,0);
  }
  return;
}

