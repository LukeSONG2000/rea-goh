// immune_remove VA 0x181925fb0 RVA 0x1925fb0

void FUN_181925fb0(longlong param_1,longlong param_2)

{
  longlong lVar1;
  code *pcVar2;
  longlong *plVar3;
  char cVar4;
  ushort uVar5;
  int iVar6;
  undefined8 *puVar7;
  longlong lVar8;
  undefined8 uVar9;
  uint auStackX_10 [2];
  longlong *plStackX_20;
  longlong lStack_30;
  longlong **pplStack_28;
  undefined8 uStack_20;
  
  if (cRam0000000185a3d883 == '\0') {
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1228);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1218);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857856b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857856c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857856d8);
    LOCK();
    UNLOCK();
    cRam0000000185a3d883 = '\x01';
  }
  plStackX_20 = (longlong *)0x0;
  if ((((param_2 == 0) || (*(longlong *)(param_2 + 0x38) == 0)) ||
      (lVar8 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar8 == 0)) ||
     (lStack_30 = *(longlong *)(lVar8 + 0xc0), lStack_30 == 0)) {
    FUN_180352450();
    pcVar2 = (code *)swi(3);
    (*pcVar2)();
    return;
  }
  pplStack_28 = (longlong **)((ulonglong)*(uint *)(lStack_30 + 0x1c) << 0x20);
  uStack_20 = 0;
  plStackX_20 = (longlong *)
                FUN_1803a4e80(*(undefined8 *)
                               (*(longlong *)(*(longlong *)(lRam00000001857b1218 + 0x20) + 0xc0) +
                               0x30),&lStack_30);
  lStack_30 = 0;
  pplStack_28 = &plStackX_20;
code_r0x0001819260f0:
  if (plStackX_20 != (longlong *)0x0) {
    cVar4 = FUN_1800084f0(0,uRam0000000185820d00);
    plVar3 = plStackX_20;
    if (cVar4 == '\0') {
      if (plStackX_20 != (longlong *)0x0) {
        FUN_180005650(0,uRam000000018580d620,plStackX_20);
      }
      return;
    }
    if (plStackX_20 != (longlong *)0x0) {
      lVar8 = *plStackX_20;
      if (*(ushort *)(lVar8 + 0x12e) != 0) {
        uVar5 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar5 * 0x10) ==
              lRam00000001857b1228) {
            puVar7 = (undefined8 *)
                     ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar5 * 0x10)
                      * 0x10 + 0x138 + lVar8);
            goto code_r0x000181926176;
          }
          uVar5 = uVar5 + 1;
        } while (uVar5 < *(ushort *)(lVar8 + 0x12e));
      }
      puVar7 = (undefined8 *)FUN_18034f630(plStackX_20,lRam00000001857b1228,0);
code_r0x000181926176:
      lVar8 = (*(code *)*puVar7)(plVar3,puVar7[1]);
      if (lVar8 != 0) {
        lVar1 = *(longlong *)(param_1 + 0x18);
        if (lVar1 != 0) goto code_r0x000181926198;
        goto code_r0x0001819262ff;
      }
      goto code_r0x000181926304;
    }
    goto code_r0x000181926309;
  }
code_r0x00018192630e:
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
code_r0x000181926198:
  auStackX_10[0] = 0;
  cVar4 = FUN_182d56dd0(lVar1,*(undefined8 *)(lVar8 + 0x10),auStackX_10,
                        *(undefined8 *)
                         (*(longlong *)
                           (*(longlong *)
                             (*(longlong *)
                               (*(longlong *)(*(longlong *)(lRam00000001857856c8 + 0x20) + 0xc0) +
                               0x128) + 0x20) + 0xc0) + 0xa0));
  if (cVar4 == '\0') goto code_r0x0001819260f0;
  if (*(uint *)(lVar1 + 0x18) <= auStackX_10[0]) goto code_r0x0001819262f8;
  lVar1 = *(longlong *)(lVar1 + 0x10);
  if (lVar1 != 0) {
    if (auStackX_10[0] < *(uint *)(lVar1 + 0x18)) {
      lVar1 = *(longlong *)(lVar1 + 0x20 + (longlong)(int)auStackX_10[0] * 8);
      if (lVar1 != 0) {
        iVar6 = *(int *)(lVar1 + 0x18) + -1;
        *(int *)(lVar1 + 0x18) = iVar6;
        if (0 < iVar6) goto code_r0x0001819260f0;
        if (*(longlong *)(param_1 + 0x18) != 0) {
          uVar9 = FUN_182d56280(*(longlong *)(param_1 + 0x18),*(undefined8 *)(lVar8 + 0x10),
                                uRam00000001857856d8);
          FUN_181926650(param_1,uVar9,0);
          if (*(longlong *)(param_1 + 0x18) != 0) {
            FUN_182d55ff0(*(longlong *)(param_1 + 0x18),*(undefined8 *)(lVar8 + 0x10),
                          uRam00000001857856b8);
            goto code_r0x0001819260f0;
          }
          goto code_r0x0001819262e4;
        }
        goto code_r0x0001819262e9;
      }
    }
    else {
      FUN_180352460();
code_r0x0001819262e4:
      FUN_180352450();
code_r0x0001819262e9:
      FUN_180352450();
    }
    FUN_180352450();
  }
  FUN_180352450();
code_r0x0001819262f8:
  FUN_183aeda10(0);
code_r0x0001819262ff:
  FUN_180352450();
code_r0x000181926304:
  FUN_180352450();
code_r0x000181926309:
  FUN_180352450();
  goto code_r0x00018192630e;
}

