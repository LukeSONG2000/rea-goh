// perform_move_callback1 VA 0x181cf79a0 RVA 0x1cf79a0

void FUN_181cf79a0(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  code *pcVar6;
  undefined8 *puVar7;
  ushort uVar8;
  
  if (cRam0000000185a3f064 == '\0') {
    FUN_18035da50();
    LOCK();
    UNLOCK();
    cRam0000000185a3f064 = '\x01';
  }
  if (*(int *)(param_1 + 0x18) == 1) {
    lVar1 = *(longlong *)(param_1 + 0x10);
    if ((lVar1 == 0) || (plVar2 = *(longlong **)(lVar1 + 0xe8), plVar2 == (longlong *)0x0)) {
      FUN_180352450();
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    lVar3 = *plVar2;
    uVar4 = *(undefined8 *)(param_1 + 0x20);
    uVar5 = *(undefined8 *)(lVar1 + 0x268);
    if (*(ushort *)(lVar3 + 0x12e) != 0) {
      uVar8 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + (ulonglong)uVar8 * 0x10) ==
            lRam00000001858316b0) {
          puVar7 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + (ulonglong)uVar8 * 0x10) + 3) * 0x10
                    + 0x138 + lVar3);
          goto code_r0x000181cf7a55;
        }
        uVar8 = uVar8 + 1;
      } while (uVar8 < *(ushort *)(lVar3 + 0x12e));
    }
    puVar7 = (undefined8 *)FUN_18034f630(plVar2,lRam00000001858316b0,3);
code_r0x000181cf7a55:
    (*(code *)*puVar7)(plVar2,uVar5,uVar4,puVar7[1]);
  }
  return;
}

