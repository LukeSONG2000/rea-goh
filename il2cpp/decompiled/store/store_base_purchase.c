// store_base_purchase VA 0x181c41d60 RVA 0x1c41d60

void FUN_181c41d60(longlong *param_1,longlong param_2,undefined4 param_3,undefined1 param_4,
                  undefined8 param_5)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  code *pcVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  ulonglong in_stack_ffffffffffffffb8;
  
  if (cRam0000000185a3ec49 == '\0') {
    FUN_18035da50(0x1857d7708);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec49 = '\x01';
  }
  if (param_2 == 0) goto code_r0x000181c41f88;
  if ((*(longlong *)(param_2 + 0x108) != 0) &&
     (*(int *)(*(longlong *)(param_2 + 0x108) + 0x10) == 4)) {
    lVar1 = param_1[0x1d];
    if (lVar1 == 0) goto code_r0x000181c41f88;
    if ((*(char *)(lVar1 + 0x1c0) == '\0') || (*(char *)(lVar1 + 0x1c1) == '\0')) {
      lVar1 = param_1[0x1c];
      if (lVar1 != 0) {
        if (cRam0000000185a3add5 == '\0') {
          FUN_18035da50(0x1857ff558);
          LOCK();
          UNLOCK();
          cRam0000000185a3add5 = '\x01';
        }
        lVar2 = *(longlong *)(lVar1 + 0x10);
        lVar3 = *(longlong *)(lVar1 + 0x48);
        if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857ff558);
        }
        if ((lVar3 != 0) &&
           (lVar4 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x20b0), lVar4 != 0))
        {
          uVar6 = FUN_180ca5830(lVar4,0);
          uVar6 = FUN_181164190(lVar3,uVar6,0,0,0);
          lVar1 = *(longlong *)(lVar1 + 0x48);
          if ((lVar1 != 0) &&
             (lVar3 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x41b0), lVar3 != 0
             )) {
            uVar7 = FUN_180ca5830(lVar3,0);
            uVar7 = FUN_181164190(lVar1,uVar7,0,0,0);
            if (lVar2 != 0) {
              FUN_18126cfb0(lVar2,uVar6,uVar7,0,0,0,in_stack_ffffffffffffffb8 & 0xffffffffffffff00,0
                            ,0);
              return;
            }
          }
        }
      }
      goto code_r0x000181c41f88;
    }
  }
  if (*(char *)(param_2 + 0x111) == '\0') {
    (**(code **)(*param_1 + 0x288))
              (param_1,param_2,param_3,param_4,0,param_5,*(undefined8 *)(*param_1 + 0x290));
  }
  else {
    param_1[0x24] = param_2;
    if ((param_1[0x1b] == 0) || (lVar1 = *(longlong *)(param_1[0x1b] + 0x120), lVar1 == 0)) {
code_r0x000181c41f88:
      FUN_180352450();
      pcVar5 = (code *)swi(3);
      (*pcVar5)();
      return;
    }
    FUN_181c41f90(param_1,*(undefined8 *)(lVar1 + 0xb0),0);
  }
  return;
}

