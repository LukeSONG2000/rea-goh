// ai_enemy_targets VA 0x1816a4540 RVA 0x16a4540

void FUN_1816a4540(longlong param_1,longlong param_2,longlong param_3,int param_4,longlong param_5,
                  longlong param_6,char param_7)

{
  int iVar1;
  longlong lVar2;
  code *pcVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  int iVar8;
  float fVar9;
  undefined8 uVar10;
  longlong *plVar11;
  undefined8 uVar12;
  uint uStack_78;
  longlong lStack_68;
  uint uStack_60;
  int iStack_5c;
  undefined8 uStack_58;
  longlong lStack_50;
  
  if (cRam0000000185a3c852 == '\0') {
    FUN_18035da50(0x18582c590);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c578);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c570);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a93c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c560);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c852 = '\x01';
  }
  lStack_68 = 0;
  uStack_60 = 0;
  iStack_5c = 0;
  uStack_58 = 0;
  if (param_5 == 0) {
    return;
  }
  if (*(int *)(param_5 + 0x18) == 0) {
    return;
  }
  if ((*(longlong *)(param_1 + 0x20) != 0) &&
     (lVar5 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308), lVar5 != 0)) {
    lVar5 = FUN_18366e0e0(lVar5,uRam00000001857a96f0);
    lStack_50 = lVar5;
    if ((param_4 != 0) && (param_4 == 1)) {
      iVar1 = *(int *)(param_5 + 0x1c);
      uStack_78 = 0;
      lVar6 = 0;
code_r0x0001816a4710:
      if (param_5 != 0) {
        if ((iVar1 == *(int *)(param_5 + 0x1c)) && (uStack_78 < *(uint *)(param_5 + 0x18))) {
          lVar2 = *(longlong *)(param_5 + 0x10);
          if (lVar2 == 0) goto code_r0x0001816a4b23;
          if (*(uint *)(lVar2 + 0x18) <= uStack_78) goto code_r0x0001816a4b18;
          lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)uStack_78 * 8);
          uStack_78 = uStack_78 + 1;
          if (lVar6 == 0) goto code_r0x0001816a47c6;
          if (lVar2 != 0) goto code_r0x0001816a476b;
          goto code_r0x0001816a4b1e;
        }
        if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0();
        }
        if (param_5 == 0) goto code_r0x0001816a4b2f;
        if (iVar1 != *(int *)(param_5 + 0x1c)) goto code_r0x0001816a4b28;
        if (lVar5 == 0) goto code_r0x0001816a4b12;
        FUN_18000d200(lVar5,lVar6,uRam00000001857a9420);
        goto code_r0x0001816a48a4;
      }
      goto code_r0x0001816a4b34;
    }
    if (lVar5 == 0) goto code_r0x0001816a4b12;
    FUN_1836f5a70(lVar5,*(undefined4 *)(lVar5 + 0x18),param_5,
                  *(undefined8 *)
                   (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90));
code_r0x0001816a48a4:
    if (((param_2 == 0) || (*(longlong *)(param_2 + 0x38) == 0)) ||
       (lVar6 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70), lVar6 == 0))
    goto code_r0x0001816a4b12;
    cVar4 = '\0';
    if (*(longlong *)(lVar6 + 0x80) != 0) {
      cVar4 = param_7;
    }
    if (cVar4 != '\0') {
      if ((*(longlong *)(param_1 + 0x20) == 0) ||
         (lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308), lVar6 == 0))
      goto code_r0x0001816a4b12;
      lVar6 = FUN_18366e0e0(lVar6,uRam00000001857a96f0);
      if (((*(longlong *)(param_2 + 0x38) == 0) ||
          ((lVar2 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70), lVar2 == 0 ||
           (lVar2 = *(longlong *)(lVar2 + 0x80), lVar2 == 0)))) ||
         (lVar2 = *(longlong *)(lVar2 + 0x20), lVar2 == 0)) goto code_r0x0001816a4b12;
      uVar12 = 0;
      iStack_5c = *(int *)(lVar2 + 0x1c);
      uStack_60 = 0;
      uStack_58 = 0;
      uVar10 = 0;
      plVar11 = &lStack_68;
      lStack_68 = lVar2;
      do {
        if (lStack_68 == 0) goto code_r0x0001816a4b5b;
        if ((iStack_5c != *(int *)(lStack_68 + 0x1c)) || (*(uint *)(lStack_68 + 0x18) <= uStack_60))
        {
          if ((*(byte *)(*(longlong *)(lRam000000018582c578 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam000000018582c578 + 0x20));
          }
          if (lStack_68 == 0) goto code_r0x0001816a4b56;
          if (iStack_5c != *(int *)(lStack_68 + 0x1c)) goto code_r0x0001816a4b4f;
          uStack_60 = *(int *)(lStack_68 + 0x18) + 1;
          uStack_58 = 0;
          break;
        }
        lVar2 = *(longlong *)(lStack_68 + 0x10);
        if (lVar2 == 0) goto code_r0x0001816a4b4a;
        if (*(uint *)(lVar2 + 0x18) <= uStack_60) goto code_r0x0001816a4b40;
        uStack_58 = *(undefined8 *)(lVar2 + 0x20 + (longlong)(int)uStack_60 * 8);
        uStack_60 = uStack_60 + 1;
        if (param_3 == 0) goto code_r0x0001816a4b45;
        cVar4 = FUN_1816a4380(param_1,uStack_58,*(undefined8 *)(param_3 + 0x20),param_3,lVar6,0,
                              uVar10,plVar11,uVar12);
      } while (cVar4 == '\0');
      if (lVar6 == 0) goto code_r0x0001816a4b12;
      if (*(int *)(lVar6 + 0x18) != 0) {
        if (param_6 == 0) goto code_r0x0001816a4b12;
        uVar10 = *(undefined8 *)
                  (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90);
        lVar5 = lVar6;
        goto code_r0x0001816a4aa0;
      }
      if (((*(longlong *)(param_2 + 0x38) == 0) ||
          (lVar6 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70), lVar6 == 0)) ||
         (lVar6 = *(longlong *)(lVar6 + 0x80), lVar6 == 0)) goto code_r0x0001816a4b12;
      if (*(char *)(lVar6 + 0x11) != '\0') {
        return;
      }
    }
    if (param_6 != 0) {
      uVar10 = *(undefined8 *)
                (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90);
code_r0x0001816a4aa0:
      FUN_1836f5a70(param_6,*(undefined4 *)(param_6 + 0x18),lVar5,uVar10);
      return;
    }
  }
code_r0x0001816a4b12:
  FUN_180352450();
code_r0x0001816a4b18:
  FUN_180352460();
code_r0x0001816a4b1e:
  FUN_180352450();
code_r0x0001816a4b23:
  FUN_180352450();
code_r0x0001816a4b28:
  FUN_183aed800(0);
code_r0x0001816a4b2f:
  FUN_180352450();
code_r0x0001816a4b34:
  FUN_180352450();
  FUN_180352440();
code_r0x0001816a4b40:
  FUN_180352460();
code_r0x0001816a4b45:
  FUN_180352450();
code_r0x0001816a4b4a:
  FUN_180352450();
code_r0x0001816a4b4f:
  FUN_183aed800(0);
code_r0x0001816a4b56:
  FUN_180352450();
code_r0x0001816a4b5b:
  FUN_180352450();
  FUN_180352440();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
code_r0x0001816a476b:
  iVar7 = 0;
  if (-1 < *(int *)(lVar2 + 0xc0)) {
    iVar7 = *(int *)(lVar2 + 0xc0);
  }
  fVar9 = (float)FUN_181bbae00(lVar2,1,0);
  if ((int)fVar9 <= iVar7) {
    iVar7 = (int)fVar9;
  }
  iVar8 = 0;
  if (-1 < *(int *)(lVar6 + 0xc0)) {
    iVar8 = *(int *)(lVar6 + 0xc0);
  }
  fVar9 = (float)FUN_181bbae00(lVar6,1,0);
  if ((int)fVar9 <= iVar8) {
    iVar8 = (int)fVar9;
  }
  if (iVar7 < iVar8) {
code_r0x0001816a47c6:
    lVar6 = lVar2;
  }
  goto code_r0x0001816a4710;
}

