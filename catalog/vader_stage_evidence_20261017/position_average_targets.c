// position_average_targets VA 0x180fd55d0 RVA 0xfd55d0

longlong FUN_180fd55d0(longlong param_1,longlong *param_2)

{
  code *pcVar1;
  bool bVar2;
  undefined8 *puVar3;
  longlong lVar4;
  int iVar5;
  longlong *plVar6;
  ushort uVar7;
  
  if (cRam0000000185a39eb9 == '\0') {
    FUN_18035da50(0x1857a9618);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9410);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9428);
    LOCK();
    UNLOCK();
    cRam0000000185a39eb9 = '\x01';
  }
  iVar5 = *(int *)(param_1 + 0x28);
  if (iVar5 == 1) {
    if (param_2 != (longlong *)0x0) {
      iVar5 = 0xe;
code_r0x000180fd566d:
      lVar4 = *param_2;
      if (*(ushort *)(lVar4 + 0x12e) != 0) {
        uVar7 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar4 + 0xb0) + (ulonglong)uVar7 * 0x10) ==
              lRam00000001857a97e0) {
            puVar3 = (undefined8 *)
                     ((longlong)
                      (*(int *)(*(longlong *)(lVar4 + 0xb0) + 8 + (ulonglong)uVar7 * 0x10) + iVar5)
                      * 0x10 + 0x138 + lVar4);
            goto code_r0x000180008543;
          }
          uVar7 = uVar7 + 1;
        } while (uVar7 < *(ushort *)(lVar4 + 0x12e));
      }
      puVar3 = (undefined8 *)FUN_18034f630(param_2,lRam00000001857a97e0,iVar5);
code_r0x000180008543:
                    /* WARNING: Could not recover jumptable at 0x000180008554. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      lVar4 = (*(code *)*puVar3)(param_2,puVar3[1]);
      return lVar4;
    }
  }
  else if (iVar5 == 2) {
    if (param_2 != (longlong *)0x0) {
      iVar5 = 0xd;
      goto code_r0x000180fd566d;
    }
  }
  else if (iVar5 == 3) {
    lVar4 = FUN_1803a51b0(uRam00000001857a9428);
    FUN_182d09b30(lVar4);
    if (lVar4 != 0) {
      plVar6 = (longlong *)0x0;
      if (param_2 != (longlong *)0x0) {
        if ((*(byte *)(*param_2 + 0x130) < *(byte *)(lRam00000001857a9618 + 0x130)) ||
           (bVar2 = true,
           *(longlong *)
            (*(longlong *)(*param_2 + 200) + -8 +
            (ulonglong)*(byte *)(lRam00000001857a9618 + 0x130) * 8) != lRam00000001857a9618)) {
          bVar2 = false;
        }
        if (bVar2) {
          plVar6 = param_2;
        }
      }
      FUN_18000d200(lVar4,plVar6,uRam00000001857a9420);
      return lVar4;
    }
  }
  else if (param_2 != (longlong *)0x0) {
    iVar5 = 2;
    goto code_r0x000180fd566d;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  lVar4 = (*pcVar1)();
  return lVar4;
}

