// attack_finished VA 0x181bc4f50 RVA 0x1bc4f50

void FUN_181bc4f50(longlong param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  code *pcVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  if (cRam0000000185a3e926 == '\0') {
    FUN_18035da50(0x185806c20);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e926 = '\x01';
  }
  if (param_2 == 0) {
    lVar3 = *(longlong *)(param_1 + 0xf8);
  }
  else {
    lVar3 = FUN_181bc3bb0(param_1,param_3,0);
  }
  if ((((lVar3 == 0) || (*(longlong *)(lVar3 + 0x38) == 0)) ||
      (lVar3 = *(longlong *)(*(longlong *)(lVar3 + 0x38) + 0x70), lVar3 == 0)) ||
     (lVar3 = *(longlong *)(lVar3 + 0xa8), lVar3 == 0)) goto code_r0x000181bc50a8;
  if (param_2 == 0) {
    uVar4 = 9;
    if (*(int *)(lVar3 + 0x18) < 1) {
      FUN_181bccd50(param_1,9,0);
    }
    else {
code_r0x000181bc501e:
      FUN_181bcd640(param_1,uVar4,param_1,lVar3,0);
    }
  }
  else if (param_2 == 1) {
    uVar4 = 10;
    if (0 < *(int *)(lVar3 + 0x18)) goto code_r0x000181bc501e;
    FUN_181bccd50(param_1,10,0);
  }
  if (*(int *)(lVar3 + 0x18) < 1) {
    FUN_181bccd50(param_1,8,0);
  }
  else {
    FUN_181bcd640(param_1,8,param_1,lVar3,0);
  }
  lVar3 = *(longlong *)(param_1 + 0xe0);
  if (lVar3 != 0) {
    iVar1 = *(int *)(lVar3 + 0x18);
    *(int *)(lVar3 + 0x1c) = *(int *)(lVar3 + 0x1c) + 1;
    *(undefined4 *)(lVar3 + 0x18) = 0;
    if (0 < iVar1) {
      FUN_183af4330(*(undefined8 *)(lVar3 + 0x10),0,iVar1,0);
    }
    if (*(longlong *)(param_1 + 0x48) != 0) {
      lVar3 = *(longlong *)(param_1 + 0x48);
      (**(code **)(lVar3 + 0x18))
                (*(undefined8 *)(lVar3 + 0x40),param_1,*(undefined8 *)(lVar3 + 0x28));
    }
    return;
  }
code_r0x000181bc50a8:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

