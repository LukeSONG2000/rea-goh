// unit_vfx_start VA 0x180fe0d80 RVA 0xfe0d80

void FUN_180fe0d80(longlong param_1)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  uint uVar9;
  longlong alStack_58 [2];
  undefined8 uStack_48;
  longlong *plStack_40;
  
  if (cRam0000000185a39efd == '\0') {
    FUN_18035da50(0x1857a9490);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8e18);
    LOCK();
    UNLOCK();
    cRam0000000185a39efd = '\x01';
  }
  lVar6 = lRam00000001857a9490;
  lVar1 = *(longlong *)(param_1 + 0x28);
  if (*(longlong *)(lRam00000001857a9490 + 0x38) == 0) {
    FUN_18034f250(lRam00000001857a9490);
  }
  lVar5 = FUN_1844a1720(param_1,0);
  if ((lVar5 == 0) || (lVar6 = FUN_182b24240(lVar5,1,**(undefined8 **)(lVar6 + 0x38)), lVar1 == 0))
  {
    FUN_180352450();
    pcVar2 = (code *)swi(3);
    (*pcVar2)();
    return;
  }
  uVar9 = *(uint *)(lVar1 + 0x18);
  lVar5 = *(longlong *)(*(longlong *)(*(longlong *)(lRam00000001857a8e18 + 0x20) + 0xc0) + 0x90);
  if (cRam0000000185a47991 == '\0') {
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    cRam0000000185a47991 = '\x01';
  }
  alStack_58[0] = 0;
  if (lVar6 == 0) {
    FUN_183aecdf0(6,0);
code_r0x0001836f5d87:
    FUN_180352450();
code_r0x0001836f5d8d:
    FUN_180352450();
    FUN_180352440();
    pcVar2 = (code *)swi(3);
    (*pcVar2)();
    return;
  }
  if (*(uint *)(lVar1 + 0x18) < uVar9) {
    FUN_183aeda10(0);
    pcVar2 = (code *)swi(3);
    (*pcVar2)();
    return;
  }
  lVar7 = *(longlong *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x28);
  if ((*(byte *)(lVar7 + 0x135) & 1) == 0) {
    lVar7 = FUN_18034f1d0(lVar7);
  }
  lVar7 = FUN_1803a50f0(lVar6,lVar7);
  if (lVar7 == 0) {
    if ((int)uVar9 < *(int *)(lVar1 + 0x18)) {
      lVar7 = *(longlong *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x20);
      if ((*(byte *)(lVar7 + 0x135) & 1) == 0) {
        lVar7 = FUN_18034f1d0(lVar7);
      }
      alStack_58[0] = FUN_1800084f0(0,lVar7,lVar6);
      uStack_48 = 0;
      plStack_40 = alStack_58;
      while (alStack_58[0] != 0) {
        cVar3 = FUN_1800084f0(0,uRam0000000185820d00);
        lVar6 = alStack_58[0];
        if (cVar3 == '\0') {
          if (alStack_58[0] != 0) {
            FUN_180005650(0,uRam000000018580d620,alStack_58[0]);
          }
          goto code_r0x0001836f5d67;
        }
        if (alStack_58[0] == 0) goto code_r0x0001836f5d87;
        lVar7 = *(longlong *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x148);
        if ((*(byte *)(lVar7 + 0x135) & 1) == 0) {
          lVar7 = FUN_18034f1d0(lVar7);
        }
        uVar8 = FUN_1800084f0(0,lVar7,lVar6);
        FUN_1836f58b0(lVar1,uVar9,uVar8,
                      *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x160));
        uVar9 = uVar9 + 1;
      }
      goto code_r0x0001836f5d8d;
    }
    FUN_1836f64b0(lVar1,lVar6,
                  *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x40));
  }
  else {
    lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x28);
    if ((*(byte *)(lVar6 + 0x135) & 1) == 0) {
      lVar6 = FUN_18034f1d0(lVar6);
    }
    iVar4 = FUN_1800084f0(0,lVar6,lVar7);
    if (0 < iVar4) {
      FUN_1801fbbf0(lVar1,*(int *)(lVar1 + 0x18) + iVar4,
                    *(undefined8 *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x78));
      if ((int)uVar9 < *(int *)(lVar1 + 0x18)) {
        FUN_183af4660(*(undefined8 *)(lVar1 + 0x10),uVar9,*(undefined8 *)(lVar1 + 0x10),
                      iVar4 + uVar9,*(int *)(lVar1 + 0x18) - uVar9,0);
      }
      if (lVar1 == lVar7) {
        FUN_183af4660(*(undefined8 *)(lVar1 + 0x10),0,*(undefined8 *)(lVar1 + 0x10),uVar9,uVar9,0);
        FUN_183af4660(*(undefined8 *)(lVar1 + 0x10),iVar4 + uVar9,*(undefined8 *)(lVar1 + 0x10),
                      uVar9 * 2,*(int *)(lVar1 + 0x18) - uVar9,0);
      }
      else {
        lVar6 = *(longlong *)(*(longlong *)(*(longlong *)(lVar5 + 0x20) + 0xc0) + 0x28);
        if ((*(byte *)(lVar6 + 0x135) & 1) == 0) {
          FUN_18034f1d0(lVar6);
        }
        FUN_1800e4420();
      }
      *(int *)(lVar1 + 0x18) = *(int *)(lVar1 + 0x18) + iVar4;
    }
  }
code_r0x0001836f5d67:
  *(int *)(lVar1 + 0x1c) = *(int *)(lVar1 + 0x1c) + 1;
  return;
}

