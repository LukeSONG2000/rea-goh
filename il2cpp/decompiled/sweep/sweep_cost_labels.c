// sweep_cost_labels VA 0x181652f60 RVA 0x1652f60

void FUN_181652f60(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong *plVar3;
  code *pcVar4;
  ulonglong uVar5;
  char cVar6;
  int iVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  bool bVar10;
  
  lVar2 = *(longlong *)(param_1 + 0x130);
  uVar8 = FUN_183ac4bd0(param_1 + 0x1f0,0);
  if (lVar2 == 0) goto code_r0x0001816530ba;
  FUN_180f3ab70(lVar2,uVar8,0);
  if (*(longlong *)(param_1 + 0x1e8) != 0) {
    if (*(longlong *)(param_1 + 0x1d8) == 0) goto code_r0x0001816530ba;
    lVar2 = *(longlong *)(param_1 + 0x1b8);
    uVar8 = FUN_1812ce720((longlong)
                          (*(int *)(*(longlong *)(param_1 + 0x1e8) + 0x20) *
                          *(int *)(param_1 + 0x1f0)),
                          *(undefined8 *)(*(longlong *)(param_1 + 0x1d8) + 0x10),0,0,0);
    if (lVar2 == 0) goto code_r0x0001816530ba;
    FUN_180f3ab70(lVar2,uVar8,0);
    if ((*(longlong *)(param_1 + 0x1b8) == 0) || (*(longlong *)(param_1 + 0x1c0) == 0))
    goto code_r0x0001816530ba;
    FUN_180f3ab70(*(longlong *)(param_1 + 0x1c0),
                  *(undefined8 *)(*(longlong *)(param_1 + 0x1b8) + 0x1b8),0);
  }
  iVar7 = *(int *)(param_1 + 0x1f0);
  if (*(char *)(param_1 + 0x1fc) != '\0') {
    iVar7 = iVar7 * *(int *)(param_1 + 0x1f8);
  }
  if (*(longlong *)(param_1 + 0x1d8) == 0) {
code_r0x0001816530ba:
    FUN_180352450();
    pcVar4 = (code *)swi(3);
    (*pcVar4)();
    return;
  }
  lVar2 = *(longlong *)(param_1 + 0x1c8);
  uVar8 = FUN_1812ce720((longlong)iVar7,*(undefined8 *)(*(longlong *)(param_1 + 0x1d8) + 0x10),0,0,0
                       );
  if (lVar2 == 0) goto code_r0x0001816530ba;
  FUN_180f3ab70(lVar2,uVar8,0);
  if ((*(longlong *)(param_1 + 0x1c8) == 0) ||
     (plVar3 = *(longlong **)(param_1 + 0x1d0), plVar3 == (longlong *)0x0))
  goto code_r0x0001816530ba;
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x1c8) + 0x1b8);
  if (cRam0000000185a39b10 == '\0') {
    FUN_18035da50(0x18580dd18,lVar2,0);
    LOCK();
    UNLOCK();
    cRam0000000185a39b10 = '\x01';
  }
  lVar1 = plVar3[0x37];
  if (lVar1 == lVar2) {
    return;
  }
  if (lVar1 == 0) goto code_r0x000180f3ac3a;
  if (lVar2 != 0) {
    if (*(int *)(lVar1 + 0x10) == *(int *)(lVar2 + 0x10)) {
      uVar9 = 0;
      uVar5 = (longlong)*(int *)(lVar1 + 0x10) * 2;
      if (uVar5 < 8) {
        if (uVar5 != 0) {
          do {
            if (*(char *)(lVar1 + 0x14 + uVar9) != *(char *)(lVar2 + 0x14 + uVar9))
            goto code_r0x000180f3ac8a;
            uVar9 = uVar9 + 1;
          } while (uVar9 < uVar5);
        }
        bVar10 = true;
      }
      else {
        if (uVar5 != 8) {
          do {
            if (*(longlong *)(lVar1 + 0x14 + uVar9) != *(longlong *)(lVar2 + 0x14 + uVar9))
            goto code_r0x000180f3ac8a;
            uVar9 = uVar9 + 8;
          } while (uVar9 < uVar5 - 8);
        }
        bVar10 = *(longlong *)(lVar1 + 0xc + uVar5) == *(longlong *)(lVar2 + 0xc + uVar5);
      }
      goto code_r0x000180f3ac32;
    }
    goto code_r0x000180f3ac3f;
  }
code_r0x000180f3ac8e:
  if ((lVar1 == 0) || (*(int *)(lVar1 + 0x10) == 0)) goto code_r0x000180f3acd8;
  plVar3[0x37] = lRam000000018580dd18;
  (**(code **)(*plVar3 + 0x368))(plVar3,*(undefined8 *)(*plVar3 + 0x370));
  FUN_180f3d050(plVar3,0);
  cVar6 = (char)plVar3[0x1c];
  goto joined_r0x000180f3ac86;
code_r0x000180f3ac8a:
  bVar10 = false;
code_r0x000180f3ac32:
  if (bVar10) {
    return;
  }
code_r0x000180f3ac3a:
  if (lVar2 == 0) goto code_r0x000180f3ac8e;
code_r0x000180f3ac3f:
  if (*(int *)(lVar2 + 0x10) == 0) goto code_r0x000180f3ac8e;
  cVar6 = FUN_1838d7290(plVar3[0x37],lVar2,0);
  if (cVar6 == '\0') goto code_r0x000180f3acd8;
  plVar3[0x37] = lVar2;
  (**(code **)(*plVar3 + 0x368))(plVar3,*(undefined8 *)(*plVar3 + 0x370));
  FUN_180f3d050(plVar3,0);
  cVar6 = (char)plVar3[0x1c];
joined_r0x000180f3ac86:
  if (cVar6 != '\0') {
    FUN_180efe010(plVar3,0);
  }
code_r0x000180f3acd8:
  (**(code **)(*plVar3 + 0x3f8))(plVar3,*(undefined8 *)(*plVar3 + 0x400));
  return;
}

