// store_success_callback VA 0x181c41900 RVA 0x1c41900

void FUN_181c41900(longlong *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  undefined8 uVar2;
  longlong *plVar3;
  undefined8 uVar4;
  
  if (cRam0000000185a3ec46 == '\0') {
    FUN_18035da50(0x1857d7720);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec46 = '\x01';
  }
  uVar2 = uRam00000001857d7720;
  if (cRam0000000185a38afe == '\0') {
    FUN_18035da50(0x18582cc68);
    LOCK();
    UNLOCK();
    cRam0000000185a38afe = '\x01';
  }
  if (param_1 != (longlong *)0x0) {
    plVar3 = (longlong *)FUN_1803a7d00(*param_1 + 0x20);
    if (plVar3 != (longlong *)0x0) {
      uVar4 = (**(code **)(*plVar3 + 0x168))(plVar3,*(undefined8 *)(*plVar3 + 0x170));
      if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
        FUN_180359940();
      }
      FUN_180c99a10(uVar2,0x80,uVar4,0);
                    /* WARNING: Could not recover jumptable at 0x000181c419f9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*param_1 + 600))(param_1,param_2,param_3,param_4);
      return;
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

