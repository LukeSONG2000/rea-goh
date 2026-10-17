// unit_play_hit VA 0x180fe9cb0 RVA 0xfe9cb0

void FUN_180fe9cb0(longlong param_1)

{
  ushort uVar1;
  longlong lVar2;
  longlong *plVar3;
  ushort uVar4;
  undefined8 *puVar5;
  undefined8 unaff_RBX;
  
  if (cRam0000000185a39f63 == '\0') {
    FUN_18035da50(0x1857ad6e8);
    LOCK();
    UNLOCK();
    cRam0000000185a39f63 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x18) != 0) {
    plVar3 = *(longlong **)(param_1 + 0x18);
    lVar2 = *plVar3;
    uVar4 = 0;
    uVar1 = *(ushort *)(lVar2 + 0x12e);
    if (uVar1 != 0) {
      do {
        if (*(longlong *)(*(longlong *)(lVar2 + 0xb0) + (ulonglong)uVar4 * 0x10) ==
            lRam00000001857ad6e8) {
          puVar5 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar2 + 0xb0) + 8 + (ulonglong)uVar4 * 0x10) + 5) * 0x10
                    + 0x138 + lVar2);
                    /* WARNING: Could not recover jumptable at 0x0001800056d9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*(code *)*puVar5)(plVar3,puVar5[1]);
          return;
        }
        uVar4 = uVar4 + 1;
      } while (uVar4 < uVar1);
    }
    puVar5 = (undefined8 *)FUN_18034f630(plVar3,lRam00000001857ad6e8,5,uVar1,unaff_RBX);
                    /* WARNING: Could not recover jumptable at 0x0001800056ad. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)*puVar5)(plVar3,puVar5[1]);
    return;
  }
  return;
}

