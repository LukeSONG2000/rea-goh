// store_fixed_process VA 0x181c4f240 RVA 0x1c4f240

void FUN_181c4f240(longlong *param_1,undefined8 param_2,undefined4 param_3,undefined1 param_4,
                  undefined8 param_5,undefined8 param_6)

{
  int iVar1;
  code *pcVar2;
  longlong lVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  undefined *puVar10;
  uint uStack_48;
  
  if (cRam0000000185a3ec90 == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fdc78);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fdc68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fdc48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d6218);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fdc58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97c8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec90 = '\x01';
  }
  lVar5 = FUN_1803a51b0(uRam00000001857d97c8);
  if (lVar5 != 0) {
    *(longlong **)(lVar5 + 0x10) = param_1;
    *(undefined8 *)(lVar5 + 0x18) = param_2;
    *(undefined4 *)(lVar5 + 0x20) = param_3;
    *(undefined1 *)(lVar5 + 0x24) = param_4;
    *(undefined8 *)(lVar5 + 0x28) = param_5;
    if (param_1[0x12] != 0) {
      cVar4 = FUN_181c70840(param_1[0x12],param_2,0x1b,0);
      if (cVar4 == '\0') {
code_r0x000181c4f575:
        (**(code **)(*param_1 + 0x238))
                  (param_1,*(undefined8 *)(lVar5 + 0x18),*(undefined4 *)(lVar5 + 0x20),
                   *(undefined1 *)(lVar5 + 0x24),*(undefined8 *)(lVar5 + 0x28),0,0,param_6,
                   *(undefined8 *)(*param_1 + 0x240));
      }
      else {
        if ((*(longlong *)(lVar5 + 0x18) == 0) ||
           (lVar6 = FUN_181c799c0(*(longlong *)(lVar5 + 0x18),0), lVar6 == 0))
        goto code_r0x000181c4f5cc;
        iVar1 = *(int *)(lVar6 + 0x1c);
        uStack_48 = 0;
        do {
          do {
            if (lVar6 == 0) goto code_r0x000181c4f5fd;
            if ((iVar1 != *(int *)(lVar6 + 0x1c)) || (*(uint *)(lVar6 + 0x18) <= uStack_48)) {
              if ((*(byte *)(*(longlong *)(lRam00000001857fdc68 + 0x20) + 0x135) & 1) == 0) {
                FUN_18034f1d0(*(longlong *)(lRam00000001857fdc68 + 0x20));
              }
              if (lVar6 == 0) goto code_r0x000181c4f5f8;
              if (iVar1 == *(int *)(lVar6 + 0x1c)) goto code_r0x000181c4f575;
              goto code_r0x000181c4f5f1;
            }
            lVar9 = *(longlong *)(lVar6 + 0x10);
            if (lVar9 == 0) goto code_r0x000181c4f5ec;
            if (*(uint *)(lVar9 + 0x18) <= uStack_48) goto code_r0x000181c4f5d2;
            lVar9 = *(longlong *)(lVar9 + 0x20 + (longlong)(int)uStack_48 * 8);
            uStack_48 = uStack_48 + 1;
            if (lVar9 == 0) goto code_r0x000181c4f5e7;
          } while (*(char *)(lVar9 + 0x12) != '\x1b');
          if (param_1[0x45] == 0) goto code_r0x000181c4f5e2;
          lVar7 = FUN_182b30790(param_1[0x45],uRam00000001857d6218);
          if (param_1[0x25] == 0) goto code_r0x000181c4f5dd;
          uVar8 = FUN_181be7d20(param_1[0x25],*(undefined8 *)(lVar9 + 0x28),0);
          lVar9 = *(longlong *)(lVar5 + 0x30);
          if (lVar9 == 0) {
            lVar9 = FUN_1803a51b0(uRam000000018580d858);
            lVar3 = lRam00000001857d97d0;
            *(undefined8 *)(lVar9 + 0x10) = *(undefined8 *)(lRam00000001857d97d0 + 8);
            *(longlong *)(lVar9 + 0x28) = lVar3;
            *(longlong *)(lVar9 + 0x20) = lVar5;
            cVar4 = *(char *)(lVar3 + 0x52);
            *(longlong *)(lVar9 + 0x40) = lVar9;
            if (((*(byte *)(lVar3 + 0x4c) & 0x10) == 0) || (cVar4 != '\0')) {
              puVar10 = *(undefined **)(lVar9 + 0x10);
              *(longlong *)(lVar9 + 0x40) = lVar5;
            }
            else {
              puVar10 = &UNK_180006440;
            }
            *(undefined **)(lVar9 + 0x18) = puVar10;
            *(code **)(lVar9 + 0x38) = FUN_1800063e0;
            *(longlong *)(lVar5 + 0x30) = lVar9;
          }
          if (lVar7 == 0) goto code_r0x000181c4f5d8;
          cVar4 = FUN_181397530(lVar7,uVar8,lVar9,0,0);
        } while (cVar4 == '\0');
      }
      return;
    }
  }
code_r0x000181c4f5cc:
  FUN_180352450();
code_r0x000181c4f5d2:
  FUN_180352460();
code_r0x000181c4f5d8:
  FUN_180352450();
code_r0x000181c4f5dd:
  FUN_180352450();
code_r0x000181c4f5e2:
  FUN_180352450();
code_r0x000181c4f5e7:
  FUN_180352450();
code_r0x000181c4f5ec:
  FUN_180352450();
code_r0x000181c4f5f1:
  FUN_183aed800(0);
code_r0x000181c4f5f8:
  FUN_180352450();
code_r0x000181c4f5fd:
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

