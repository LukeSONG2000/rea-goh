// ==== static_helper_180015180 @ 0x180015180 ====


void FUN_180015180(undefined8 param_1,undefined8 param_2,longlong *param_3,undefined1 param_4,
                  undefined1 param_5,undefined4 param_6)

{
  longlong lVar1;
  undefined8 *puVar2;
  ushort uVar3;
  
  lVar1 = *param_3;
  if (*(ushort *)(lVar1 + 0x12e) != 0) {
    uVar3 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar1 + 0xb0) + (ulonglong)uVar3 * 0x10) ==
          lRam00000001857a8f48) {
        puVar2 = (undefined8 *)
                 ((longlong)
                  (*(int *)(*(longlong *)(lVar1 + 0xb0) + 8 + (ulonglong)uVar3 * 0x10) + 5) * 0x10 +
                  0x138 + lVar1);
        goto code_r0x0001800151d8;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < *(ushort *)(lVar1 + 0x12e));
  }
  puVar2 = (undefined8 *)FUN_18034f630(param_3,lRam00000001857a8f48,5);
code_r0x0001800151d8:
                    /* WARNING: Could not recover jumptable at 0x0001800151fd. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*puVar2)(param_3,param_4,param_5,param_6);
  return;
}

