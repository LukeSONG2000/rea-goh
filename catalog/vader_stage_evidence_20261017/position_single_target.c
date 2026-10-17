// position_single_target VA 0x180fd54e0 RVA 0xfd54e0

longlong * FUN_180fd54e0(longlong param_1,longlong *param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  longlong lVar3;
  longlong *plVar4;
  ushort uVar5;
  
  if (cRam0000000185a39eb8 == '\0') {
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50();
    LOCK();
    UNLOCK();
    cRam0000000185a39eb8 = '\x01';
  }
  if (*(int *)(param_1 + 0x28) == 1) {
    if ((param_2 != (longlong *)0x0) &&
       (lVar3 = FUN_1800084f0(0xe,lRam00000001857a97e0,param_2), lVar3 != 0)) {
      if (*(int *)(lVar3 + 0x18) < 1) {
        return param_2;
      }
      if (*(int *)(lVar3 + 0x18) == 0) {
        FUN_183aeda10(0);
        pcVar1 = (code *)swi(3);
        plVar4 = (longlong *)(*pcVar1)();
        return plVar4;
      }
      lVar3 = *(longlong *)(lVar3 + 0x10);
      if (lVar3 != 0) {
        if (*(int *)(lVar3 + 0x18) == 0) {
          FUN_180352460();
          pcVar1 = (code *)swi(3);
          plVar4 = (longlong *)(*pcVar1)();
          return plVar4;
        }
        return *(longlong **)(lVar3 + 0x20);
      }
    }
  }
  else {
    if (*(int *)(param_1 + 0x28) == 3) {
      return param_2;
    }
    if (param_2 != (longlong *)0x0) {
      lVar3 = *param_2;
      if (*(ushort *)(lVar3 + 0x12e) != 0) {
        uVar5 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + (ulonglong)uVar5 * 0x10) ==
              lRam00000001857a97e0) {
            puVar2 = (undefined8 *)
                     ((longlong)*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + (ulonglong)uVar5 * 0x10)
                      * 0x10 + 0x138 + lVar3);
            goto code_r0x000180008543;
          }
          uVar5 = uVar5 + 1;
        } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
      }
      puVar2 = (undefined8 *)FUN_18034f630(param_2,lRam00000001857a97e0,0);
code_r0x000180008543:
                    /* WARNING: Could not recover jumptable at 0x000180008554. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      plVar4 = (longlong *)(*(code *)*puVar2)(param_2,puVar2[1]);
      return plVar4;
    }
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  plVar4 = (longlong *)(*pcVar1)();
  return plVar4;
}

