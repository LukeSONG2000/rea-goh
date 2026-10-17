// position_action_enter VA 0x18131c000 RVA 0x131c000

void FUN_18131c000(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  undefined8 unaff_RBX;
  
  FUN_18131c040(param_1,0);
  if (*(char *)(param_1 + 0x68) != '\0') {
    return;
  }
  if (*(char *)(param_1 + 0x1b) != '\0') {
    return;
  }
  lVar2 = *(longlong *)(param_1 + 0x28);
  *(undefined2 *)(param_1 + 0x1a) = 0x100;
  if (lVar2 != 0) {
    if (cRam0000000185a4a45a == '\0') {
      FUN_18035da50(0x185767f68);
      LOCK();
      UNLOCK();
      cRam0000000185a4a45a = '\x01';
    }
    lVar5 = FUN_183cd5030(lVar2,0);
    lVar2 = lRam0000000185767f68;
    if (lVar5 != 0) {
      *(int *)(lVar5 + 0x1c) = *(int *)(lVar5 + 0x1c) + 1;
      lVar3 = *(longlong *)(lVar5 + 0x10);
      if (lVar3 != 0) {
        uVar1 = *(uint *)(lVar5 + 0x18);
        if (*(uint *)(lVar3 + 0x18) <= uVar1) {
          FUN_1836f50c0(lVar5,param_1,
                        *(undefined8 *)(*(longlong *)(*(longlong *)(lVar2 + 0x20) + 0xc0) + 0x70),
                        lVar5,unaff_RBX);
          return;
        }
        *(uint *)(lVar5 + 0x18) = uVar1 + 1;
        if (*(uint *)(lVar3 + 0x18) <= uVar1) {
          FUN_180352460();
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
        *(longlong *)(lVar3 + 0x20 + (longlong)(int)uVar1 * 8) = param_1;
        return;
      }
    }
  }
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

