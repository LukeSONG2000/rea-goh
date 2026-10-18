// ai_module_median VA 0x1816a4cb0 RVA 0x16a4cb0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_1816a4cb0(undefined8 param_1,longlong param_2,longlong param_3,undefined8 param_4,
                  longlong *param_5)

{
  longlong lVar1;
  undefined8 uVar2;
  code *pcVar3;
  undefined1 uVar4;
  uint uVar5;
  longlong lVar6;
  bool bVar7;
  float fVar8;
  float extraout_XMM0_Da;
  float extraout_XMM0_Da_00;
  float fVar9;
  
  if (cRam0000000185a3c854 == '\0') {
    FUN_18035da50(0x18580cfb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cf28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9430);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a47f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4800);
    LOCK();
    UNLOCK();
    cRam0000000185a3c854 = '\x01';
  }
  *param_5 = param_3;
  if (param_3 != 0) {
    if (*(int *)(param_3 + 0x18) < 1) {
      return false;
    }
    if (param_2 != 0) {
      fVar8 = (float)FUN_180fec9b0(param_2,0);
      if (*(int *)(lRam00000001857a4800 + 0xe4) == 0) {
        FUN_180359940();
      }
      lVar6 = *(longlong *)(*(longlong *)(lRam00000001857a4800 + 0xb8) + 8);
      if (lVar6 == 0) {
        if (*(int *)(lRam00000001857a4800 + 0xe4) == 0) {
          FUN_180359940();
        }
        uVar2 = **(undefined8 **)(lRam00000001857a4800 + 0xb8);
        lVar6 = FUN_1803a51b0(uRam000000018580cfb8);
        FUN_1832562a0(lVar6,uVar2,uRam00000001857a47f8,0);
        *(longlong *)(*(longlong *)(lRam00000001857a4800 + 0xb8) + 8) = lVar6;
      }
      FUN_1836de080(param_3,lVar6,uRam000000018580cf28);
      uVar5 = FUN_180d38560((float)*(int *)(param_3 + 0x18) * _UNK_18529ee14);
      if (*(uint *)(param_3 + 0x18) <= uVar5 - 1) {
code_r0x0001816a4f39:
        FUN_183aeda10(0);
        pcVar3 = (code *)swi(3);
        uVar4 = (*pcVar3)();
        return (bool)uVar4;
      }
      lVar6 = *(longlong *)(param_3 + 0x10);
      if (lVar6 != 0) {
        lVar1 = (longlong)(int)uVar5 + -1;
        if (*(uint *)(lVar6 + 0x18) <= (uint)lVar1) {
code_r0x0001816a4f33:
          FUN_180352460();
          pcVar3 = (code *)swi(3);
          uVar4 = (*pcVar3)();
          return (bool)uVar4;
        }
        lVar6 = *(longlong *)(lVar6 + 0x20 + lVar1 * 8);
        if (lVar6 != 0) {
          FUN_181bb9a10(lVar6,0);
          fVar9 = extraout_XMM0_Da;
          if ((*(byte *)(param_3 + 0x18) & 1) != 0) {
code_r0x0001816a4ed1:
            if (*(int *)(param_2 + 0x28) == 0) {
              bVar7 = fVar9 < fVar8;
            }
            else {
              if (*(int *)(param_2 + 0x28) == 1) {
                if (*(int *)(lRam0000000185830790 + 0xe4) == 0) {
                  FUN_180359940();
                }
                uVar4 = FUN_180cadfd0();
                return (bool)uVar4;
              }
              bVar7 = fVar8 < fVar9;
            }
            return !bVar7 && fVar8 != fVar9;
          }
          if (*(uint *)(param_3 + 0x18) <= uVar5) goto code_r0x0001816a4f39;
          lVar6 = *(longlong *)(param_3 + 0x10);
          if (lVar6 != 0) {
            if (*(uint *)(lVar6 + 0x18) <= uVar5) goto code_r0x0001816a4f33;
            lVar6 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar5 * 8);
            if (lVar6 != 0) {
              FUN_181bb9a10(lVar6,0);
              fVar9 = (extraout_XMM0_Da_00 + extraout_XMM0_Da) * _UNK_18529ee14;
              goto code_r0x0001816a4ed1;
            }
          }
        }
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  uVar4 = (*pcVar3)();
  return (bool)uVar4;
}

