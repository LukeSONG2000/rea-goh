// ai_rule_filter VA 0x181bb0ec0 RVA 0x1bb0ec0

undefined8 FUN_181bb0ec0(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  longlong lVar1;
  code *pcVar2;
  undefined8 uVar3;
  
  if (cRam0000000185a3e8bb == '\0') {
    FUN_18035da50(0x1857a96f0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8bb = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x10) != 0) &&
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x308), lVar1 != 0)) {
    uVar3 = FUN_18366e0e0(lVar1,uRam00000001857a96f0);
    FUN_181bb2710(param_1,param_2,param_3,uVar3,0);
    return uVar3;
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar3 = (*pcVar2)();
  return uVar3;
}

