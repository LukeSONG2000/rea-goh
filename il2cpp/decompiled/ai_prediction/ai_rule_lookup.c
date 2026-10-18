// ai_rule_lookup VA 0x181be6e80 RVA 0x1be6e80

undefined8 FUN_181be6e80(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  undefined8 uVar4;
  uint auStackX_8 [2];
  
  if (cRam0000000185a3ea15 == '\0') {
    FUN_18035da50(0x185835bd8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ea15 = '\x01';
  }
  lVar1 = *(longlong *)(param_1 + 0x170);
  if (lVar1 != 0) {
    auStackX_8[0] = 0;
    cVar3 = FUN_182d56dd0(lVar1,param_2,auStackX_8,
                          *(undefined8 *)
                           (*(longlong *)
                             (*(longlong *)
                               (*(longlong *)
                                 (*(longlong *)(*(longlong *)(lRam0000000185835bd8 + 0x20) + 0xc0) +
                                 0x128) + 0x20) + 0xc0) + 0xa0));
    if (cVar3 == '\0') {
      return 0;
    }
    if (*(uint *)(lVar1 + 0x18) <= auStackX_8[0]) {
      FUN_183aeda10(0);
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
    lVar1 = *(longlong *)(lVar1 + 0x10);
    if (lVar1 != 0) {
      if (auStackX_8[0] < *(uint *)(lVar1 + 0x18)) {
        return *(undefined8 *)(lVar1 + 0x20 + (longlong)(int)auStackX_8[0] * 8);
      }
      FUN_180352460();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar4 = (*pcVar2)();
  return uVar4;
}

