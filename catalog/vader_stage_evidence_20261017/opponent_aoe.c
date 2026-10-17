// opponent_aoe VA 0x181cf8020 RVA 0x1cf8020

void FUN_181cf8020(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ushort uVar5;
  
  if (cRam0000000185a3f06a == '\0') {
    FUN_18035da50(0x1857a98a0);
    LOCK();
    UNLOCK();
    cRam0000000185a3f06a = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x18) == 0) ||
     (plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x18) + 0x40), plVar2 == (longlong *)0x0)) {
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
    return;
  }
  lVar1 = *plVar2;
  if (*(ushort *)(lVar1 + 0x12e) != 0) {
    uVar5 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar1 + 0xb0) + (ulonglong)uVar5 * 0x10) ==
          lRam00000001857a98a0) {
        puVar4 = (undefined8 *)
                 ((longlong)
                  (*(int *)(*(longlong *)(lVar1 + 0xb0) + 8 + (ulonglong)uVar5 * 0x10) + 5) * 0x10 +
                  0x138 + lVar1);
        goto code_r0x000180008543;
      }
      uVar5 = uVar5 + 1;
    } while (uVar5 < *(ushort *)(lVar1 + 0x12e));
  }
  puVar4 = (undefined8 *)FUN_18034f630(plVar2,lRam00000001857a98a0,5);
code_r0x000180008543:
                    /* WARNING: Could not recover jumptable at 0x000180008554. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*puVar4)(plVar2,puVar4[1]);
  return;
}

