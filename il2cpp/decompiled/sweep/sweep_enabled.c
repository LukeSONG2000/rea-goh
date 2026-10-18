// sweep_enabled VA 0x1816502a0 RVA 0x16502a0

bool FUN_1816502a0(longlong param_1,longlong param_2,char param_3)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  code *pcVar5;
  undefined1 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  
  if (param_2 != 0) {
    if (*(char *)(param_2 + 0x15) != '\0') {
      return true;
    }
    if (cRam0000000185a3c641 == '\0') {
      FUN_18035da50(0x1857ff558);
      LOCK();
      UNLOCK();
      cRam0000000185a3c641 = '\x01';
    }
    if (*(longlong *)(param_2 + 0xe8) != 0) {
      iVar1 = *(int *)(*(longlong *)(param_2 + 0xe8) + 0x30);
      if ((param_3 == '\0') || (iVar1 == 5)) {
        return iVar1 == 5;
      }
      lVar2 = *(longlong *)(param_1 + 0x38);
      lVar3 = *(longlong *)(param_1 + 0x48);
      if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
        FUN_180359940(lRam00000001857ff558);
      }
      if ((lVar3 != 0) &&
         (lVar4 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x2ac8), lVar4 != 0)) {
        uVar7 = FUN_180ca5830(lVar4,0);
        uVar7 = FUN_181164190(lVar3,uVar7,0,0,0);
        lVar3 = *(longlong *)(param_1 + 0x48);
        if ((lVar3 != 0) &&
           (lVar4 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x2ac0), lVar4 != 0))
        {
          uVar8 = FUN_180ca5830(lVar4,0);
          uVar8 = FUN_181164190(lVar3,uVar8,0,0,0);
          if (lVar2 != 0) {
            FUN_18126b6f0(lVar2,uVar7,uVar8,0,0,0,0,1,2,0,0,0);
            return iVar1 == 5;
          }
        }
      }
    }
  }
  FUN_180352450();
  pcVar5 = (code *)swi(3);
  uVar6 = (*pcVar5)();
  return (bool)uVar6;
}

