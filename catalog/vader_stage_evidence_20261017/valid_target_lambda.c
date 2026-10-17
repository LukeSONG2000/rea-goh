// valid_target_lambda VA 0x181bafe00 RVA 0x1bafe00

void FUN_181bafe00(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  code *pcVar2;
  ushort uVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  
  if (cRam0000000185a3e8b4 == '\0') {
    FUN_18035da50(0x185831918);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858318d8);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8b4 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x20) != 0) {
    plVar5 = (longlong *)FUN_1800084f0(1,uRam0000000185831918);
    if (plVar5 != (longlong *)0x0) {
      uVar3 = 0;
      lVar1 = *plVar5;
      if (*(ushort *)(lVar1 + 0x12e) != 0) {
        do {
          if (*(longlong *)(*(longlong *)(lVar1 + 0xb0) + (ulonglong)uVar3 * 0x10) ==
              lRam00000001858318d8) {
            puVar4 = (undefined8 *)
                     ((longlong)
                      (*(int *)(*(longlong *)(lVar1 + 0xb0) + 8 + (ulonglong)uVar3 * 0x10) + 1) *
                      0x10 + 0x138 + lVar1);
            goto code_r0x00018000a7e0;
          }
          uVar3 = uVar3 + 1;
        } while (uVar3 < *(ushort *)(lVar1 + 0x12e));
      }
      puVar4 = (undefined8 *)FUN_18034f630(plVar5);
code_r0x00018000a7e0:
                    /* WARNING: Could not recover jumptable at 0x00018000a7f9. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*(code *)*puVar4)(plVar5,param_2,puVar4[1]);
      return;
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

