// inventory_material_list VA 0x1818080e0 RVA 0x18080e0

longlong FUN_1818080e0(longlong param_1)

{
  code *pcVar1;
  longlong *plVar2;
  char cVar3;
  ushort uVar4;
  longlong lVar5;
  undefined8 *puVar6;
  longlong lVar7;
  longlong *plStackX_8;
  longlong lStack_30;
  longlong **pplStack_28;
  undefined8 uStack_20;
  
  if (cRam0000000185a3d133 == '\0') {
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857cbac8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ac8e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ac8c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ac880);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857cbac0);
    LOCK();
    UNLOCK();
    cRam0000000185a3d133 = '\x01';
  }
  plStackX_8 = (longlong *)0x0;
  lVar5 = FUN_1803a51b0(uRam00000001857ac880);
  FUN_182d09b30(lVar5,uRam00000001857ac8c0);
  if ((*(longlong *)(param_1 + 0xd8) == 0) ||
     (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xd8) + 0x18), lVar7 == 0)) {
    FUN_180352450();
code_r0x000181808340:
    FUN_180352450();
code_r0x000181808346:
    FUN_180352450();
code_r0x00018180834b:
    FUN_180352450();
  }
  else {
    pplStack_28 = (longlong **)((ulonglong)*(uint *)(lVar7 + 0x1c) << 0x20);
    uStack_20 = 0;
    lStack_30 = lVar7;
    plStackX_8 = (longlong *)
                 FUN_1803a4e80(*(undefined8 *)
                                (*(longlong *)(*(longlong *)(lRam00000001857cbac0 + 0x20) + 0xc0) +
                                0x30),&lStack_30);
    lStack_30 = 0;
    pplStack_28 = &plStackX_8;
    while (plStackX_8 != (longlong *)0x0) {
      cVar3 = FUN_1800084f0(0,uRam0000000185820d00);
      plVar2 = plStackX_8;
      if (cVar3 == '\0') {
        if (plStackX_8 != (longlong *)0x0) {
          FUN_180005650(0,uRam000000018580d620,plStackX_8);
        }
        return lVar5;
      }
      if (plStackX_8 == (longlong *)0x0) goto code_r0x00018180834b;
      lVar7 = *plStackX_8;
      if (*(ushort *)(lVar7 + 0x12e) != 0) {
        uVar4 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar7 + 0xb0) + (ulonglong)uVar4 * 0x10) ==
              lRam00000001857cbac8) {
            puVar6 = (undefined8 *)
                     ((longlong)*(int *)(*(longlong *)(lVar7 + 0xb0) + 8 + (ulonglong)uVar4 * 0x10)
                      * 0x10 + 0x138 + lVar7);
            goto code_r0x000181808296;
          }
          uVar4 = uVar4 + 1;
        } while (uVar4 < *(ushort *)(lVar7 + 0x12e));
      }
      puVar6 = (undefined8 *)FUN_18034f630(plStackX_8,lRam00000001857cbac8,0);
code_r0x000181808296:
      lVar7 = (*(code *)*puVar6)(plVar2,puVar6[1]);
      if (lVar7 == 0) goto code_r0x000181808346;
      if (0 < *(int *)(lVar7 + 0x18)) {
        if (lVar5 == 0) goto code_r0x000181808340;
        FUN_1800164d0(lVar5,lVar7);
      }
    }
  }
  FUN_180352450();
  FUN_180352440();
  pcVar1 = (code *)swi(3);
  lVar5 = (*pcVar1)();
  return lVar5;
}

