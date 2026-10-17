// ability_fsm_action VA 0x181319a10 RVA 0x1319a10

void FUN_181319a10(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  undefined8 uVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 unaff_RBX;
  
  if (cRam0000000185a3b137 == '\0') {
    FUN_18035da50(0x1857a0f40);
    LOCK();
    UNLOCK();
    cRam0000000185a3b137 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x50) == 0) {
code_r0x000181319ac8:
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
    return;
  }
  uVar4 = FUN_183c9efd0(*(longlong *)(param_1 + 0x50),0);
  lVar5 = FUN_1803a50f0(uVar4,uRam00000001857a0f40);
  switch(*(undefined4 *)(param_1 + 0x58)) {
  case 2:
    if (lVar5 == 0) goto code_r0x000181319ac8;
    uVar4 = 0;
    break;
  case 3:
    if (lVar5 == 0) goto code_r0x000181319ac8;
    uVar4 = 1;
    break;
  default:
    goto code_r0x000181319ab9;
  case 6:
    if (lVar5 == 0) goto code_r0x000181319ac8;
    uVar4 = 2;
    break;
  case 7:
    if (lVar5 == 0) goto code_r0x000181319ac8;
    uVar4 = 3;
  }
  FUN_180005650(uVar4,uRam00000001857a0f40,lVar5);
code_r0x000181319ab9:
  if (*(char *)(param_1 + 0x1b) != '\0') {
    return;
  }
  lVar5 = *(longlong *)(param_1 + 0x28);
  *(undefined2 *)(param_1 + 0x1a) = 0x100;
  if (lVar5 != 0) {
    if (cRam0000000185a4a45a == '\0') {
      FUN_18035da50(0x185767f68);
      LOCK();
      UNLOCK();
      cRam0000000185a4a45a = '\x01';
    }
    lVar6 = FUN_183cd5030(lVar5,0);
    lVar5 = lRam0000000185767f68;
    if (lVar6 != 0) {
      *(int *)(lVar6 + 0x1c) = *(int *)(lVar6 + 0x1c) + 1;
      lVar2 = *(longlong *)(lVar6 + 0x10);
      if (lVar2 != 0) {
        uVar1 = *(uint *)(lVar6 + 0x18);
        if (*(uint *)(lVar2 + 0x18) <= uVar1) {
          FUN_1836f50c0(lVar6,param_1,
                        *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x70),
                        lVar6,unaff_RBX);
          return;
        }
        *(uint *)(lVar6 + 0x18) = uVar1 + 1;
        if (*(uint *)(lVar2 + 0x18) <= uVar1) {
          FUN_180352460();
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        *(longlong *)(lVar2 + 0x20 + (longlong)(int)uVar1 * 8) = param_1;
        return;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

