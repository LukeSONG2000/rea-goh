// subobject_event VA 0x1811b83c0 RVA 0x11b83c0

void FUN_1811b83c0(longlong param_1,undefined8 param_2)

{
  undefined8 *puVar1;
  undefined4 *puVar2;
  code *pcVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  uint uVar7;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  if (cRam0000000185a3aac1 == '\0') {
    FUN_18035da50(0x185774500);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774508);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774510);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744d0);
    LOCK();
    UNLOCK();
    cRam0000000185a3aac1 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x28) != 0) {
    iVar4 = FUN_18332e340(*(longlong *)(param_1 + 0x28),param_2,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam0000000185774500 + 0x20) + 0xc0) + 0x108
                           ));
    if (iVar4 < 0) {
      return;
    }
    lVar5 = *(longlong *)(param_1 + 0x28);
    uVar7 = 0;
    if (lVar5 != 0) {
      while (lVar5 = FUN_18332db60(lVar5,param_2,uRam0000000185774508), lVar5 != 0) {
        if (*(int *)(lVar5 + 0x18) <= (int)uVar7) {
          return;
        }
        if ((*(longlong *)(param_1 + 0x28) == 0) ||
           (lVar5 = FUN_18332db60(*(longlong *)(param_1 + 0x28),param_2,uRam0000000185774508),
           lVar5 == 0)) break;
        if (*(uint *)(lVar5 + 0x18) <= uVar7) {
          FUN_183aeda10(0);
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        lVar5 = *(longlong *)(lVar5 + 0x10);
        if (lVar5 == 0) break;
        if (*(uint *)(lVar5 + 0x18) <= uVar7) {
          FUN_180352460();
          pcVar3 = (code *)swi(3);
          (*pcVar3)();
          return;
        }
        lVar6 = (longlong)(int)uVar7 * 0x40;
        puVar1 = (undefined8 *)(lVar5 + 0x20 + lVar6);
        uStack_48 = *puVar1;
        uStack_40 = puVar1[1];
        puVar1 = (undefined8 *)(lVar5 + 0x30 + lVar6);
        uStack_38 = *puVar1;
        uStack_30 = puVar1[1];
        puVar2 = (undefined4 *)(lVar5 + 0x40 + lVar6);
        uStack_28 = *puVar2;
        uStack_24 = puVar2[1];
        uStack_20 = puVar2[2];
        uStack_1c = puVar2[3];
        puVar2 = (undefined4 *)(lVar5 + 0x50 + lVar6);
        uStack_18 = *puVar2;
        uStack_14 = puVar2[1];
        uStack_10 = puVar2[2];
        uStack_c = puVar2[3];
        FUN_1811b8560(param_1,&uStack_48,0);
        lVar5 = *(longlong *)(param_1 + 0x28);
        uVar7 = uVar7 + 1;
        if (lVar5 == 0) break;
      }
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

