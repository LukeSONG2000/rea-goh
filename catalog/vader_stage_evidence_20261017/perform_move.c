// perform_move VA 0x181bc3f20 RVA 0x1bc3f20

undefined8
FUN_181bc3f20(longlong param_1,undefined4 param_2,longlong param_3,undefined8 param_4,
             undefined4 param_5,byte param_6)

{
  int *piVar1;
  short sVar2;
  longlong lVar3;
  longlong *plVar4;
  code *pcVar5;
  char cVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 *puVar10;
  longlong lVar11;
  undefined *puVar12;
  undefined8 uVar13;
  longlong lVar14;
  ushort uVar15;
  ulonglong uVar17;
  undefined *puVar18;
  bool bVar19;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  uint uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  uint uStack_1c;
  ulonglong uVar16;
  
  if (cRam0000000185a3e923 == '\0') {
    FUN_18035da50(0x185831448);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185831440);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185831438);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185831470);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185831468);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185831460);
    LOCK();
    UNLOCK();
    cRam0000000185a3e923 = '\x01';
  }
  lVar8 = FUN_1803a51b0(uRam0000000185831460);
  if (lVar8 == 0) goto code_r0x000181bc45a4;
  uVar17 = 0;
  *(longlong *)(lVar8 + 0x10) = param_1;
  *(undefined4 *)(lVar8 + 0x18) = param_2;
  *(undefined4 *)(lVar8 + 0x28) = param_5;
  cVar6 = FUN_181bc2bd0(param_1,param_2,param_5,0,0);
  if (cVar6 == '\0') {
    return 0;
  }
  if (*(int *)(param_1 + 0x250) == 1) {
    uStack_38 = *(undefined4 *)(lVar8 + 0x18);
    if (*(longlong *)(param_1 + 0x140) != 0) {
      uStack_50 = (undefined4)param_3;
      uStack_4c = (undefined4)((ulonglong)param_3 >> 0x20);
      uStack_48 = (undefined4)param_4;
      uStack_44 = (undefined4)((ulonglong)param_4 >> 0x20);
      uStack_3c = (uint)param_6;
      uStack_34 = 0;
      uStack_30 = uStack_50;
      uStack_2c = uStack_4c;
      uStack_28 = uStack_48;
      uStack_24 = uStack_44;
      uStack_20 = 0;
      uStack_1c = uStack_3c;
      FUN_182e2a3f0(*(longlong *)(param_1 + 0x140),&uStack_38,uRam0000000185831440);
      return 0;
    }
    goto code_r0x000181bc45a4;
  }
  *(int *)(param_1 + 0x138) = *(int *)(param_1 + 0x138) + 1;
  *(longlong *)(param_1 + 600) = param_3;
  *(undefined8 *)(param_1 + 0x260) = param_4;
  if (((param_3 == 0) || (*(char *)(param_3 + 0x1e8) != '\0')) || (*(int *)(param_3 + 0xc0) < 1)) {
    FUN_181bc45b0(param_1,0);
    return 0;
  }
  if ((*(longlong *)(param_1 + 0x88) == 0) ||
     (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x70), lVar9 == 0))
  goto code_r0x000181bc45a4;
  *(undefined4 *)(lVar9 + 0x60) = 0;
  *(undefined4 *)(param_1 + 0x250) = 1;
  if (*(int *)(lVar8 + 0x18) == 0) {
    lVar9 = *(longlong *)(param_1 + 0xf8);
  }
  else {
    lVar9 = FUN_181bc3bb0(param_1,*(undefined4 *)(lVar8 + 0x28),0);
  }
  *(longlong *)(lVar8 + 0x20) = lVar9;
  if (((lVar9 == 0) || (*(longlong *)(lVar9 + 0x38) == 0)) ||
     ((lVar3 = *(longlong *)(*(longlong *)(lVar9 + 0x38) + 0x70), lVar3 == 0 ||
      ((*(longlong *)(param_1 + 0x88) == 0 ||
       (plVar4 = *(longlong **)(*(longlong *)(param_1 + 0x88) + 0x10), plVar4 == (longlong *)0x0))))
     )) goto code_r0x000181bc45a4;
  lVar11 = *plVar4;
  lVar3 = *(longlong *)(lVar3 + 0xa8);
  if (*(ushort *)(lVar11 + 0x12e) != 0) {
    uVar16 = uVar17;
    do {
      if (*(longlong *)(*(longlong *)(lVar11 + 0xb0) + uVar16 * 0x10) == lRam00000001857a3928) {
        puVar10 = (undefined8 *)
                  ((longlong)(*(int *)(*(longlong *)(lVar11 + 0xb0) + 8 + uVar16 * 0x10) + 0x28) *
                   0x10 + 0x138 + lVar11);
        goto code_r0x000181bc4169;
      }
      uVar15 = (short)uVar16 + 1;
      uVar16 = (ulonglong)uVar15;
    } while (uVar15 < *(ushort *)(lVar11 + 0x12e));
  }
  puVar10 = (undefined8 *)FUN_18034f630(plVar4,lRam00000001857a3928,0x28);
code_r0x000181bc4169:
  (*(code *)*puVar10)(plVar4,lVar9,puVar10[1]);
  if ((*(longlong *)(lVar8 + 0x20) == 0) ||
     (lVar9 = *(longlong *)(*(longlong *)(lVar8 + 0x20) + 0x38), lVar9 == 0))
  goto code_r0x000181bc45a4;
  cVar6 = FUN_180fec480(lVar9,0);
  if (cVar6 != '\0') {
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x60), lVar9 == 0))
    goto code_r0x000181bc45a4;
    FUN_181babf20(lVar9,*(undefined8 *)(lVar8 + 0x20),param_1,0);
  }
  iVar7 = FUN_181bb9960(param_1,0);
  if (iVar7 == 0) {
    if (((((*(longlong *)(lVar8 + 0x20) == 0) ||
          (lVar9 = *(longlong *)(*(longlong *)(lVar8 + 0x20) + 0x38), lVar9 == 0)) ||
         (lVar9 = *(longlong *)(lVar9 + 0x70), lVar9 == 0)) ||
        ((*(longlong *)(param_1 + 0x98) == 0 ||
         (lVar11 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x40), lVar11 == 0)))) ||
       ((lVar11 = *(longlong *)(lVar11 + 0xe0), lVar11 == 0 ||
        (lVar11 = *(longlong *)(lVar11 + 0x70), lVar11 == 0)))) goto code_r0x000181bc45a4;
    lVar9 = *(longlong *)(lVar9 + 0x70);
    lVar11 = *(longlong *)(lVar11 + 0x70);
    if (lVar9 != lVar11) {
      if (((lVar9 != 0) && (lVar11 != 0)) && (*(int *)(lVar9 + 0x10) == *(int *)(lVar11 + 0x10))) {
        uVar16 = (longlong)*(int *)(lVar9 + 0x10) * 2;
        if (uVar16 < 8) {
          if (uVar16 != 0) {
            do {
              if (*(char *)(lVar9 + 0x14 + uVar17) != *(char *)(lVar11 + 0x14 + uVar17))
              goto code_r0x000181bc433a;
              uVar17 = uVar17 + 1;
            } while (uVar17 < uVar16);
          }
          bVar19 = true;
        }
        else {
          if (uVar16 != 8) {
            do {
              if (*(longlong *)(lVar9 + 0x14 + uVar17) != *(longlong *)(lVar11 + 0x14 + uVar17))
              goto code_r0x000181bc433a;
              uVar17 = uVar17 + 8;
            } while (uVar17 < uVar16 - 8);
          }
          bVar19 = *(longlong *)(uVar16 + 0xc + lVar9) == *(longlong *)(uVar16 + 0xc + lVar11);
        }
        goto code_r0x000181bc42f2;
      }
      goto code_r0x000181bc42f6;
    }
  }
code_r0x000181bc4316:
  if (*(int *)(lVar8 + 0x18) == 0) {
    if (lVar3 == 0) goto code_r0x000181bc45a4;
    if (*(int *)(lVar3 + 0x18) < 1) {
      FUN_181bccd50(param_1,0x25,0);
    }
    else {
      FUN_181bcd640(param_1,0x25,param_1,lVar3,0);
    }
  }
  else if (lVar3 == 0) goto code_r0x000181bc45a4;
  if (*(int *)(lVar8 + 0x18) == 1) {
    if (*(int *)(lVar3 + 0x18) < 1) {
      FUN_181bccd50(param_1,0x26,0);
    }
    else {
      FUN_181bcd640(param_1,0x26,param_1,lVar3,0);
    }
  }
  if (*(int *)(lVar3 + 0x18) < 1) {
    FUN_181bccd50(param_1,0x24,0);
  }
  else {
    FUN_181bcd640(param_1,0x24,param_1,lVar3,0);
  }
  if ((*(int *)(lVar8 + 0x18) == 1) && (param_6 == 0)) {
    lVar9 = *(longlong *)(lVar8 + 0x20);
    if ((lVar9 == 0) || (*(longlong *)(lVar9 + 0x38) == 0)) goto code_r0x000181bc45a4;
    sVar2 = *(short *)(*(longlong *)(lVar9 + 0x38) + 0x18);
    iVar7 = (int)sVar2;
    if (sVar2 < 0) {
      iVar7 = 0;
    }
    *(int *)(lVar9 + 0x14) = iVar7;
  }
  if (*(longlong *)(lVar8 + 0x20) != 0) {
    FUN_181bc4890(param_1,*(undefined8 *)(*(longlong *)(lVar8 + 0x20) + 0x30),0);
    if (*(longlong *)(param_1 + 0x40) != 0) {
      lVar9 = *(longlong *)(param_1 + 0x40);
      (**(code **)(lVar9 + 0x18))
                (*(undefined8 *)(lVar9 + 0x40),param_1,*(undefined8 *)(lVar9 + 0x28));
    }
    if (*(longlong *)(lVar8 + 0x20) != 0) {
      lVar9 = *(longlong *)(*(longlong *)(lVar8 + 0x20) + 0x30);
      lVar11 = FUN_1803a51b0(uRam000000018580d858);
      lVar3 = lRam0000000185831438;
      puVar12 = *(undefined **)(lRam0000000185831438 + 8);
      *(undefined **)(lVar11 + 0x10) = puVar12;
      *(longlong *)(lVar11 + 0x28) = lVar3;
      *(longlong *)(lVar11 + 0x20) = lVar8;
      cVar6 = *(char *)(lVar3 + 0x52);
      *(longlong *)(lVar11 + 0x40) = lVar11;
      if (((*(byte *)(lVar3 + 0x4c) & 0x10) == 0) || (cVar6 != '\0')) {
        *(longlong *)(lVar11 + 0x40) = lVar8;
      }
      else {
        puVar12 = &UNK_180006440;
      }
      *(undefined **)(lVar11 + 0x18) = puVar12;
      *(code **)(lVar11 + 0x38) = FUN_1800063e0;
      uVar13 = FUN_1803a51b0(uRam0000000185831448);
      FUN_180c84ee0(uVar13,lVar8,uRam0000000185831470,0);
      lVar14 = FUN_1803a51b0(uRam000000018580d858);
      lVar3 = lRam0000000185831468;
      puVar12 = *(undefined **)(lRam0000000185831468 + 8);
      *(undefined **)(lVar14 + 0x10) = puVar12;
      *(longlong *)(lVar14 + 0x28) = lVar3;
      *(longlong *)(lVar14 + 0x20) = lVar8;
      cVar6 = *(char *)(lVar3 + 0x52);
      *(longlong *)(lVar14 + 0x40) = lVar14;
      if (((*(byte *)(lVar3 + 0x4c) & 0x10) == 0) || (puVar18 = &UNK_180006440, cVar6 != '\0')) {
        *(longlong *)(lVar14 + 0x40) = lVar8;
        puVar18 = puVar12;
      }
      *(undefined **)(lVar14 + 0x18) = puVar18;
      *(code **)(lVar14 + 0x38) = FUN_1800063e0;
      if (lVar9 != 0) {
        FUN_18184adf0(lVar9,lVar11,uVar13,lVar14,0);
        return 1;
      }
    }
  }
code_r0x000181bc45a4:
  FUN_180352450();
  pcVar5 = (code *)swi(3);
  uVar13 = (*pcVar5)();
  return uVar13;
code_r0x000181bc433a:
  bVar19 = false;
code_r0x000181bc42f2:
  if (!bVar19) {
code_r0x000181bc42f6:
    if ((*(longlong *)(param_1 + 0x88) == 0) ||
       (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x88) + 0x60), lVar9 == 0))
    goto code_r0x000181bc45a4;
    piVar1 = (int *)(lVar9 + 0x60);
    *piVar1 = *piVar1 + 1;
  }
  goto code_r0x000181bc4316;
}

