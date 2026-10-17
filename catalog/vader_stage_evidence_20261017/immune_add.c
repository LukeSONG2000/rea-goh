// immune_add VA 0x181926320 RVA 0x1926320

void FUN_181926320(longlong param_1,longlong param_2)

{
  int *piVar1;
  longlong lVar2;
  code *pcVar3;
  longlong *plVar4;
  char cVar5;
  ushort uVar6;
  undefined8 *puVar7;
  longlong lVar8;
  undefined8 uVar9;
  uint auStackX_10 [2];
  longlong *plStackX_20;
  longlong lStack_30;
  longlong **pplStack_28;
  undefined8 uStack_20;
  
  if (cRam0000000185a3d884 == '\0') {
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1228);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185785660);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1218);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857856c8);
    LOCK();
    UNLOCK();
    cRam0000000185a3d884 = '\x01';
  }
  plStackX_20 = (longlong *)0x0;
  if ((((param_2 == 0) || (*(longlong *)(param_2 + 0x38) == 0)) ||
      (lVar8 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar8 == 0)) ||
     (lStack_30 = *(longlong *)(lVar8 + 0xc0), lStack_30 == 0)) {
    FUN_180352450();
    pcVar3 = (code *)swi(3);
    (*pcVar3)();
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
  do {
    if (plStackX_20 == (longlong *)0x0) {
code_r0x000181926639:
      FUN_180352450();
      FUN_180352440();
      pcVar3 = (code *)swi(3);
      (*pcVar3)();
      return;
    }
    cVar5 = FUN_1800084f0(0,uRam0000000185820d00);
    plVar4 = plStackX_20;
    if (cVar5 == '\0') {
      if (plStackX_20 != (longlong *)0x0) {
        FUN_180005650(0,uRam000000018580d620,plStackX_20);
      }
      return;
    }
    if (plStackX_20 == (longlong *)0x0) {
code_r0x000181926634:
      FUN_180352450();
      goto code_r0x000181926639;
    }
    lVar8 = *plStackX_20;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      uVar6 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar6 * 0x10) ==
            lRam00000001857b1228) {
          puVar7 = (undefined8 *)
                   ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar6 * 0x10) *
                    0x10 + 0x138 + lVar8);
          goto code_r0x0001819264d6;
        }
        uVar6 = uVar6 + 1;
      } while (uVar6 < *(ushort *)(lVar8 + 0x12e));
    }
    puVar7 = (undefined8 *)FUN_18034f630(plStackX_20,lRam00000001857b1228,0);
code_r0x0001819264d6:
    lVar8 = (*(code *)*puVar7)(plVar4,puVar7[1]);
    if (lVar8 == 0) {
code_r0x00018192662f:
      FUN_180352450();
      goto code_r0x000181926634;
    }
    lVar2 = *(longlong *)(param_1 + 0x18);
    if (lVar2 == 0) {
code_r0x00018192662a:
      FUN_180352450();
      goto code_r0x00018192662f;
    }
    auStackX_10[0] = 0;
    cVar5 = FUN_182d56dd0(lVar2,*(undefined8 *)(lVar8 + 0x10),auStackX_10,
                          *(undefined8 *)
                           (*(longlong *)
                             (*(longlong *)
                               (*(longlong *)
                                 (*(longlong *)(*(longlong *)(lRam00000001857856c8 + 0x20) + 0xc0) +
                                 0x128) + 0x20) + 0xc0) + 0xa0));
    if (cVar5 == '\0') {
      lVar2 = *(longlong *)(param_1 + 0x18);
      uVar9 = FUN_181926700(param_1,*(undefined8 *)(lVar8 + 0x10),0);
      if (lVar2 == 0) {
        FUN_180352450();
code_r0x000181926614:
        FUN_180352460();
code_r0x000181926619:
        FUN_180352450();
code_r0x00018192661e:
        FUN_180352450();
code_r0x000181926623:
        FUN_183aeda10(0);
        goto code_r0x00018192662a;
      }
      FUN_182d55f20(lVar2,uVar9,uRam0000000185785660);
    }
    else {
      if (*(uint *)(lVar2 + 0x18) <= auStackX_10[0]) goto code_r0x000181926623;
      lVar8 = *(longlong *)(lVar2 + 0x10);
      if (lVar8 == 0) goto code_r0x00018192661e;
      if (*(uint *)(lVar8 + 0x18) <= auStackX_10[0]) goto code_r0x000181926614;
      lVar8 = *(longlong *)(lVar8 + 0x20 + (longlong)(int)auStackX_10[0] * 8);
      if (lVar8 == 0) goto code_r0x000181926619;
      piVar1 = (int *)(lVar8 + 0x18);
      *piVar1 = *piVar1 + 1;
    }
  } while( true );
}

