// get_applied_effects VA 0x181a6eb30 RVA 0x1a6eb30

void FUN_181a6eb30(longlong param_1,longlong param_2)

{
  code *pcVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lStack_38;
  longlong lStack_30;
  longlong lStack_28;
  longlong lStack_20;
  undefined4 uStack_18;
  uint uStack_14;
  
  if (cRam0000000185a3e09b == '\0') {
    FUN_18035da50(0x18580d110);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d130);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d128);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ad2e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d120);
    LOCK();
    UNLOCK();
    cRam0000000185a3e09b = '\x01';
  }
  lStack_38 = 0;
  lStack_30 = 0;
  lStack_28 = 0;
  if ((*(longlong *)(param_1 + 0x58) == 0) ||
     (lVar3 = FUN_1819269a0(*(longlong *)(param_1 + 0x58),0), lVar3 == 0)) {
    FUN_180352450();
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
  lVar3 = FUN_1800084f0(0,uRam00000001858104b8,lVar3);
  do {
    if (lVar3 == 0) {
code_r0x000181a6edac:
      FUN_180352450();
      FUN_180352440();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
    cVar2 = FUN_1800084f0(0,uRam0000000185820d00);
    if (cVar2 == '\0') {
      if (lVar3 != 0) {
        FUN_180005650(0,uRam000000018580d620,lVar3);
      }
      return;
    }
    if (lVar3 == 0) {
code_r0x000181a6eda7:
      FUN_180352450();
      goto code_r0x000181a6edac;
    }
    lVar4 = FUN_180036260();
    if (lVar4 == 0) {
code_r0x000181a6eda2:
      FUN_180352450();
      goto code_r0x000181a6eda7;
    }
    uStack_18 = 0;
    uStack_14 = *(uint *)(lVar4 + 0x1c);
    lStack_30 = (ulonglong)uStack_14 << 0x20;
    lStack_28 = 0;
    lStack_38 = lVar4;
    lStack_20 = lVar4;
    while (cVar2 = FUN_1833f1b60(&lStack_38,uRam000000018580d130), cVar2 != '\0') {
      if (lStack_28 == 0) {
code_r0x000181a6ed97:
        FUN_180352450();
        FUN_180352440();
        goto code_r0x000181a6eda2;
      }
      if (param_2 == 0) {
        FUN_180352450();
        goto code_r0x000181a6ed97;
      }
      FUN_180016090(param_2,*(undefined8 *)(lStack_28 + 0x38));
    }
  } while( true );
}

