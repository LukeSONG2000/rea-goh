// select_units_ability VA 0x181bc4890 RVA 0x1bc4890

void FUN_181bc4890(longlong param_1,longlong param_2)

{
  int iVar1;
  uint uVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  char cVar6;
  longlong lVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 *puVar10;
  longlong lVar11;
  undefined8 uVar12;
  ushort uVar13;
  ulonglong uVar14;
  
  if (cRam0000000185a3e925 == '\0') {
    FUN_18035da50(0x18580d1c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d1b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858316b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d208);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806c60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806c90);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a93c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4808);
    LOCK();
    UNLOCK();
    cRam0000000185a3e925 = '\x01';
  }
  lVar7 = *(longlong *)(param_1 + 0x268);
  if (lVar7 != 0) {
    *(int *)(lVar7 + 0x1c) = *(int *)(lVar7 + 0x1c) + 1;
    iVar1 = *(int *)(lVar7 + 0x18);
    *(undefined4 *)(lVar7 + 0x18) = 0;
    if (0 < iVar1) {
      FUN_183af4330(*(undefined8 *)(lVar7 + 0x10),0,iVar1,0);
    }
    if (param_2 == 0) goto code_r0x000181bc4edd;
    FUN_18184d000(param_2,0);
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x10) == 0)) goto code_r0x000181bc4edd;
    cVar6 = FUN_1800084f0(0x16,uRam00000001857a3928);
    if (cVar6 == '\x01') {
      plVar9 = (longlong *)FUN_1803a3dc0(uRam000000018580d208,2);
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar7 = FUN_1800084f0(8,uRam00000001858316b0);
      if (plVar9 == (longlong *)0x0) goto code_r0x000181bc4edd;
      if (lVar7 != 0) {
        lVar8 = FUN_1803a50f0(lVar7,*(undefined8 *)(*plVar9 + 0x40));
        if (lVar8 == 0) {
          uVar12 = func_0x000180352550();
          FUN_180352410(uVar12,0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
      }
      if ((int)plVar9[3] == 0) goto code_r0x000181bc4f49;
      plVar9[4] = lVar7;
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar7 = FUN_1800084f0(0xd,uRam00000001858316b0);
      if (lVar7 != 0) {
        lVar8 = FUN_1803a50f0(lVar7,*(undefined8 *)(*plVar9 + 0x40));
        if (lVar8 == 0) {
          uVar12 = func_0x000180352550();
          FUN_180352410(uVar12,0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
      }
      if (*(uint *)(plVar9 + 3) < 2) goto code_r0x000181bc4f49;
      plVar9[5] = lVar7;
      lVar7 = FUN_1803a51b0(uRam000000018580d1b0);
      *(longlong **)(lVar7 + 0x10) = plVar9;
    }
    else {
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar7 = FUN_1800084f0(8,uRam00000001858316b0);
    }
    lVar7 = FUN_18184c080(param_2,lVar7,*(undefined8 *)(param_1 + 600),
                          *(undefined8 *)(param_1 + 0x260),0);
    if ((lVar7 == 0) || (lVar8 = *(longlong *)(param_1 + 0x268), lVar8 == 0))
    goto code_r0x000181bc4edd;
    FUN_1836f5a70(lVar8,*(undefined4 *)(lVar8 + 0x18),*(undefined8 *)(lVar7 + 0x10),
                  *(undefined8 *)
                   (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90));
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (*(longlong *)(*(longlong *)(param_1 + 0x88) + 0x10) == 0)) goto code_r0x000181bc4edd;
    cVar6 = FUN_1800084f0(0x16,uRam00000001857a3928);
    if (cVar6 == '\x01') {
      plVar9 = (longlong *)FUN_1803a3dc0(uRam000000018580d208,2);
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar8 = FUN_1800084f0(9,uRam00000001858316b0);
      if (plVar9 == (longlong *)0x0) goto code_r0x000181bc4edd;
      if (lVar8 != 0) {
        lVar11 = FUN_1803a50f0(lVar8,*(undefined8 *)(*plVar9 + 0x40));
        if (lVar11 == 0) {
          uVar12 = func_0x000180352550();
          FUN_180352410(uVar12,0);
          pcVar4 = (code *)swi(3);
          (*pcVar4)();
          return;
        }
      }
      if ((int)plVar9[3] == 0) {
code_r0x000181bc4f49:
        FUN_180352460();
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
      plVar9[4] = lVar8;
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar8 = FUN_1800084f0(0xe,uRam00000001858316b0);
      if (lVar8 != 0) {
        lVar11 = FUN_1803a50f0(lVar8,*(undefined8 *)(*plVar9 + 0x40));
        if (lVar11 == 0) {
          uVar12 = func_0x000180352550();
          FUN_180352410(uVar12,0);
code_r0x000181bc4f23:
          FUN_180352460();
code_r0x000181bc4f29:
          FUN_180352450();
code_r0x000181bc4f2e:
          FUN_180352450();
code_r0x000181bc4f33:
          FUN_180352450();
code_r0x000181bc4f38:
          FUN_180352450();
          goto code_r0x000181bc4f3d;
        }
      }
      if (*(uint *)(plVar9 + 3) < 2) goto code_r0x000181bc4f49;
      plVar9[5] = lVar8;
      lVar8 = FUN_1803a51b0(uRam000000018580d1b0);
      *(longlong **)(lVar8 + 0x10) = plVar9;
    }
    else {
      if (*(longlong *)(param_1 + 0xe8) == 0) goto code_r0x000181bc4edd;
      lVar8 = FUN_1800084f0(9,uRam00000001858316b0);
    }
    lVar8 = FUN_18184c080(param_2,lVar8,*(undefined8 *)(param_1 + 600),
                          *(undefined8 *)(param_1 + 0x260),0);
    if ((lVar8 != 0) && (lVar11 = *(longlong *)(param_1 + 0x268), lVar11 != 0)) {
      FUN_1836f5a70(lVar11,*(undefined4 *)(lVar11 + 0x18),*(undefined8 *)(lVar8 + 0x10),
                    *(undefined8 *)
                     (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90));
      if (*(longlong *)(lVar8 + 0x10) != 0) {
        plVar9 = (longlong *)FUN_1800084f0(0,uRam0000000185806c60);
        while (plVar9 != (longlong *)0x0) {
          cVar6 = FUN_1800084f0(0,uRam0000000185820d00);
          if (cVar6 == '\0') {
            if (plVar9 != (longlong *)0x0) {
              FUN_180005650(0,uRam000000018580d620,plVar9);
            }
            uVar12 = FUN_180f94fb0(lVar8,lVar7,0);
            FUN_18184c590(param_2,uVar12,0);
            return;
          }
          if (plVar9 == (longlong *)0x0) goto code_r0x000181bc4f38;
          lVar11 = *plVar9;
          if (*(ushort *)(lVar11 + 0x12e) != 0) {
            uVar14 = 0;
            do {
              if (*(longlong *)(*(longlong *)(lVar11 + 0xb0) + uVar14 * 0x10) ==
                  lRam0000000185806c90) {
                puVar10 = (undefined8 *)
                          ((longlong)*(int *)(*(longlong *)(lVar11 + 0xb0) + 8 + uVar14 * 0x10) *
                           0x10 + 0x138 + lVar11);
                goto code_r0x000181bc4d96;
              }
              uVar13 = (short)uVar14 + 1;
              uVar14 = (ulonglong)uVar13;
            } while (uVar13 < *(ushort *)(lVar11 + 0x12e));
          }
          puVar10 = (undefined8 *)FUN_18034f630(plVar9,lRam0000000185806c90,0);
code_r0x000181bc4d96:
          lVar11 = (*(code *)*puVar10)(plVar9,puVar10[1]);
          if (lVar11 == 0) goto code_r0x000181bc4f33;
          if (cRam0000000185a3e919 == '\0') {
            FUN_18035da50(0x1857a9420);
            LOCK();
            UNLOCK();
            cRam0000000185a3e919 = '\x01';
          }
          lVar5 = lRam00000001857a9420;
          lVar11 = *(longlong *)(lVar11 + 0x270);
          if (lVar11 == 0) goto code_r0x000181bc4f2e;
          *(int *)(lVar11 + 0x1c) = *(int *)(lVar11 + 0x1c) + 1;
          lVar3 = *(longlong *)(lVar11 + 0x10);
          if (lVar3 == 0) goto code_r0x000181bc4f29;
          uVar2 = *(uint *)(lVar11 + 0x18);
          if (uVar2 < *(uint *)(lVar3 + 0x18)) {
            *(uint *)(lVar11 + 0x18) = uVar2 + 1;
            if (*(uint *)(lVar3 + 0x18) <= uVar2) goto code_r0x000181bc4f23;
            *(longlong *)(lVar3 + 0x20 + (longlong)(int)uVar2 * 8) = param_1;
          }
          else {
            FUN_1836f50c0(lVar11,param_1,
                          *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x70))
            ;
          }
        }
code_r0x000181bc4f3d:
        FUN_180352450();
        FUN_180352440();
        pcVar4 = (code *)swi(3);
        (*pcVar4)();
        return;
      }
    }
  }
code_r0x000181bc4edd:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

