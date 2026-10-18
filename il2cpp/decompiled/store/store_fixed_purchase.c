// store_fixed_purchase VA 0x181c4f050 RVA 0x1c4f050

void FUN_181c4f050(longlong *param_1,longlong param_2,undefined4 param_3,undefined1 param_4,
                  longlong param_5)

{
  char cVar1;
  longlong lVar2;
  undefined8 *puVar3;
  code *pcVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  undefined *puVar10;
  ulonglong in_stack_ffffffffffffffb8;
  
  if (cRam0000000185a3ec8f == '\0') {
    FUN_18035da50(0x1857d3b90);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9808);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97d8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec8f = '\x01';
  }
  lVar7 = FUN_1803a51b0(uRam00000001857d97d8);
  if (lVar7 == 0) {
code_r0x000181c4f239:
    FUN_180352450();
    pcVar4 = (code *)swi(3);
    (*pcVar4)();
    return;
  }
  *(longlong **)(lVar7 + 0x10) = param_1;
  *(longlong *)(lVar7 + 0x18) = param_2;
  *(undefined4 *)(lVar7 + 0x20) = param_3;
  *(undefined1 *)(lVar7 + 0x24) = param_4;
  if (param_1[7] == 0) goto code_r0x000181c4f239;
  if (*(char *)(param_1[7] + 0x1062) != '\0') {
    if (param_1[0x40] == 0) goto code_r0x000181c4f239;
    if (*(int *)(param_1[0x40] + 0x20) == 6) {
      FUN_181c4ee90(param_1,param_2,0);
      return;
    }
  }
  if ((param_5 == 0) || (*(int *)(param_5 + 0x10) == 0)) {
    if (param_1[0xb] != 0) {
      lVar8 = FUN_180fcd800(param_1[0xb],1,param_1,0);
      param_1[0x42] = lVar8;
      lVar9 = FUN_1803a51b0(uRam00000001857d3b90);
      lVar8 = lRam00000001857d9808;
      puVar10 = *(undefined **)(lRam00000001857d9808 + 8);
      *(undefined **)(lVar9 + 0x10) = puVar10;
      *(longlong *)(lVar9 + 0x28) = lVar8;
      *(longlong *)(lVar9 + 0x20) = lVar7;
      cVar1 = *(char *)(lVar8 + 0x52);
      *(longlong *)(lVar9 + 0x40) = lVar9;
      if (((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) || (cVar1 != '\x02')) {
        *(longlong *)(lVar9 + 0x40) = lVar7;
      }
      else {
        puVar10 = &UNK_1800083d0;
      }
      *(undefined **)(lVar9 + 0x18) = puVar10;
      *(code **)(lVar9 + 0x38) = FUN_1800293a0;
      if (param_1[0x12] != 0) {
        puVar3 = *(undefined8 **)(lRam0000000185a33e80 + 0xb8);
        *(undefined8 *)(param_1[0x12] + 0x170) = *puVar3;
        if (lVar9 == 0) {
          return;
        }
        if (*(code **)(lVar9 + 0x18) != (code *)&UNK_1800083d0) {
                    /* WARNING: Could not recover jumptable at 0x000181c4f236. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (**(code **)(lVar9 + 0x18))(*(longlong *)(lVar9 + 0x40),1,0);
          return;
        }
                    /* WARNING: Could not recover jumptable at 0x000181c4f220. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(*(longlong *)(lVar9 + 0x40) + 0x10))
                  (CONCAT71((int7)((ulonglong)puVar3 >> 8),1),0,*(undefined8 *)(lVar9 + 0x28));
        return;
      }
    }
    goto code_r0x000181c4f239;
  }
  if (cRam0000000185a3ec49 == '\0') {
    FUN_18035da50(0x1857d7708,param_2,param_3,param_4);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec49 = '\x01';
  }
  if (param_2 == 0) goto code_r0x000181c41f88;
  if ((*(longlong *)(param_2 + 0x108) != 0) &&
     (*(int *)(*(longlong *)(param_2 + 0x108) + 0x10) == 4)) {
    lVar7 = param_1[0x1d];
    if (lVar7 == 0) goto code_r0x000181c41f88;
    if ((*(char *)(lVar7 + 0x1c0) == '\0') || (*(char *)(lVar7 + 0x1c1) == '\0')) {
      lVar7 = param_1[0x1c];
      if (lVar7 != 0) {
        if (cRam0000000185a3add5 == '\0') {
          FUN_18035da50(0x1857ff558);
          LOCK();
          UNLOCK();
          cRam0000000185a3add5 = '\x01';
        }
        lVar8 = *(longlong *)(lVar7 + 0x10);
        lVar9 = *(longlong *)(lVar7 + 0x48);
        if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857ff558);
        }
        if ((lVar9 != 0) &&
           (lVar2 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x20b0), lVar2 != 0))
        {
          uVar5 = FUN_180ca5830(lVar2,0);
          uVar5 = FUN_181164190(lVar9,uVar5,0,0,0);
          lVar7 = *(longlong *)(lVar7 + 0x48);
          if ((lVar7 != 0) &&
             (lVar9 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x41b0), lVar9 != 0
             )) {
            uVar6 = FUN_180ca5830(lVar9,0);
            uVar6 = FUN_181164190(lVar7,uVar6,0,0,0);
            if (lVar8 != 0) {
              FUN_18126cfb0(lVar8,uVar5,uVar6,0,0,0,in_stack_ffffffffffffffb8 & 0xffffffffffffff00,0
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
    if ((param_1[0x1b] == 0) || (lVar7 = *(longlong *)(param_1[0x1b] + 0x120), lVar7 == 0)) {
code_r0x000181c41f88:
      FUN_180352450();
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
    FUN_181c41f90(param_1,*(undefined8 *)(lVar7 + 0xb0),0);
  }
  return;
}

