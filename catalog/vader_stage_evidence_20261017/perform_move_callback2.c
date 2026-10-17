// perform_move_callback2 VA 0x181cf7aa0 RVA 0x1cf7aa0

void FUN_181cf7aa0(longlong param_1)

{
  undefined4 uVar1;
  int iVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  undefined8 uVar6;
  
  lVar3 = *(longlong *)(param_1 + 0x10);
  if (lVar3 == 0) {
    FUN_180352450();
    pcVar4 = (code *)swi(3);
    (*pcVar4)();
    return;
  }
  uVar1 = *(undefined4 *)(param_1 + 0x28);
  iVar2 = *(int *)(param_1 + 0x18);
  if (cRam0000000185a3e926 == '\0') {
    FUN_18035da50(0x185806c20,iVar2,uVar1,0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e926 = '\x01';
  }
  if (iVar2 == 0) {
    lVar5 = *(longlong *)(lVar3 + 0xf8);
  }
  else {
    lVar5 = FUN_181bc3bb0(lVar3,uVar1,0);
  }
  if ((((lVar5 == 0) || (*(longlong *)(lVar5 + 0x38) == 0)) ||
      (lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x38) + 0x70), lVar5 == 0)) ||
     (lVar5 = *(longlong *)(lVar5 + 0xa8), lVar5 == 0)) goto code_r0x000181bc50a8;
  if (iVar2 == 0) {
    uVar6 = 9;
    if (*(int *)(lVar5 + 0x18) < 1) {
      FUN_181bccd50(lVar3,9,0);
    }
    else {
code_r0x000181bc501e:
      FUN_181bcd640(lVar3,uVar6,lVar3,lVar5,0);
    }
  }
  else if (iVar2 == 1) {
    uVar6 = 10;
    if (0 < *(int *)(lVar5 + 0x18)) goto code_r0x000181bc501e;
    FUN_181bccd50(lVar3,10,0);
  }
  if (*(int *)(lVar5 + 0x18) < 1) {
    FUN_181bccd50(lVar3,8,0);
  }
  else {
    FUN_181bcd640(lVar3,8,lVar3,lVar5,0);
  }
  lVar5 = *(longlong *)(lVar3 + 0xe0);
  if (lVar5 != 0) {
    iVar2 = *(int *)(lVar5 + 0x18);
    *(int *)(lVar5 + 0x1c) = *(int *)(lVar5 + 0x1c) + 1;
    *(undefined4 *)(lVar5 + 0x18) = 0;
    if (0 < iVar2) {
      FUN_183af4330(*(undefined8 *)(lVar5 + 0x10),0,iVar2,0);
    }
    if (*(longlong *)(lVar3 + 0x48) != 0) {
      lVar5 = *(longlong *)(lVar3 + 0x48);
      (**(code **)(lVar5 + 0x18))(*(undefined8 *)(lVar5 + 0x40),lVar3,*(undefined8 *)(lVar5 + 0x28))
      ;
    }
    return;
  }
code_r0x000181bc50a8:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

