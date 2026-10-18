// sweep_update_count VA 0x181652ee0 RVA 0x1652ee0

void FUN_181652ee0(longlong param_1,int param_2)

{
  longlong *plVar1;
  longlong lVar2;
  code *pcVar3;
  int iVar4;
  undefined8 unaff_RBX;
  
  if (cRam0000000185a3c653 == '\0') {
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    cRam0000000185a3c653 = '\x01';
  }
  if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (param_2 < 1) {
    param_2 = 1;
  }
  iVar4 = *(int *)(param_1 + 0x1e0);
  if (param_2 <= *(int *)(param_1 + 0x1e0)) {
    iVar4 = param_2;
  }
  *(int *)(param_1 + 0x1f0) = iVar4;
  FUN_181652f60(param_1,0);
  plVar1 = *(longlong **)(param_1 + 0x138);
  if (plVar1 != (longlong *)0x0) {
    lVar2 = *plVar1;
    (**(code **)(lVar2 + 0x188))
              (plVar1,*(int *)(param_1 + 0x1f0) < *(int *)(param_1 + 0x1e0),
               *(undefined8 *)(lVar2 + 400),lVar2,unaff_RBX);
    plVar1 = *(longlong **)(param_1 + 0x140);
    if (plVar1 != (longlong *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00018165318a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*plVar1 + 0x188))
                (plVar1,1 < *(int *)(param_1 + 0x1f0),*(undefined8 *)(*plVar1 + 400));
      return;
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

