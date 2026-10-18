// store_base_send VA 0x181c40c50 RVA 0x1c40c50

void FUN_181c40c50(longlong *param_1,longlong param_2,undefined4 param_3,undefined1 param_4,
                  longlong param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  int iVar1;
  int iVar2;
  int iVar3;
  code *pcVar4;
  ulonglong uVar5;
  bool bVar6;
  char cVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined8 uVar12;
  longlong lVar13;
  undefined *puVar14;
  longlong lVar15;
  longlong *plVar16;
  undefined8 uVar17;
  ulonglong uVar18;
  byte bVar19;
  undefined *puVar20;
  byte bVar21;
  bool bVar22;
  undefined8 in_stack_ffffffffffffff98;
  undefined4 uVar23;
  ulonglong in_stack_ffffffffffffffa8;
  undefined4 auStack_38 [2];
  undefined8 uStack_30;
  
  uVar23 = (undefined4)((ulonglong)in_stack_ffffffffffffff98 >> 0x20);
  if (cRam0000000185a3ec44 == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d6a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7778);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7780);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7768);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7770);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7758);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d7760);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec44 = '\x01';
  }
  lVar8 = FUN_1803a51b0(uRam00000001857d7758);
  if (lVar8 == 0) goto code_r0x000181c41613;
  *(undefined8 *)(lVar8 + 0x30) = param_6;
  *(undefined8 *)(lVar8 + 0x38) = param_8;
  *(undefined8 *)(lVar8 + 0x40) = param_7;
  *(longlong **)(lVar8 + 0x10) = param_1;
  *(longlong *)(lVar8 + 0x18) = param_2;
  *(undefined4 *)(lVar8 + 0x20) = param_3;
  *(undefined1 *)(lVar8 + 0x24) = param_4;
  *(longlong *)(lVar8 + 0x28) = param_5;
  uVar10 = uRam00000001857d7760;
  if (param_2 == 0) goto code_r0x000181c41613;
  if (*(longlong *)(param_2 + 0x108) == 0) {
    if (cRam0000000185a38afc == '\0') {
      FUN_18035da50(0x18582cc68);
      LOCK();
      UNLOCK();
      cRam0000000185a38afc = '\x01';
    }
    if ((param_1 != (longlong *)0x0) &&
       (plVar16 = (longlong *)FUN_1803a7d00(*param_1 + 0x20), plVar16 != (longlong *)0x0)) {
      uVar12 = (**(code **)(*plVar16 + 0x168))(plVar16,*(undefined8 *)(*plVar16 + 0x170));
      if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
        FUN_180359940();
      }
      FUN_180c996b0(uVar10,0xffffffff,uVar12,0);
      return;
    }
    goto code_r0x000181c41613;
  }
  if (param_1[7] == 0) goto code_r0x000181c41613;
  iVar1 = *(int *)(param_1[7] + 0x1db8);
  iVar2 = *(int *)(*(longlong *)(param_2 + 0x108) + 0x14);
  uVar18 = 0;
  bVar22 = false;
  if ((param_5 == 0) || (*(int *)(param_5 + 0x10) == 0)) {
code_r0x000181c40e45:
    bVar22 = true;
  }
  else {
    if ((param_1[0x1b] == 0) ||
       ((lVar9 = *(longlong *)(param_1[0x1b] + 0x120), lVar9 == 0 ||
        (lVar9 = *(longlong *)(lVar9 + 0xb0), lVar9 == 0)))) goto code_r0x000181c41613;
    if (lVar9 == param_5) goto code_r0x000181c40e45;
    if (*(int *)(lVar9 + 0x10) == *(int *)(param_5 + 0x10)) {
      uVar5 = (longlong)*(int *)(lVar9 + 0x10) * 2;
      if (uVar5 < 8) {
        if (uVar5 != 0) {
          do {
            if (*(char *)(lVar9 + 0x14 + uVar18) != *(char *)(param_5 + 0x14 + uVar18))
            goto code_r0x000181c40e4b;
            uVar18 = uVar18 + 1;
          } while (uVar18 < uVar5);
        }
        goto code_r0x000181c40e45;
      }
      if (uVar5 != 8) {
        do {
          if (*(longlong *)(lVar9 + 0x14 + uVar18) != *(longlong *)(param_5 + 0x14 + uVar18))
          goto code_r0x000181c40e4b;
          uVar18 = uVar18 + 8;
        } while (uVar18 < uVar5 - 8);
      }
      bVar22 = *(longlong *)(lVar9 + 0xc + uVar5) == *(longlong *)(param_5 + 0xc + uVar5);
    }
  }
code_r0x000181c40e4b:
  iVar3 = *(int *)(*(longlong *)(param_2 + 0x108) + 0x10);
  bVar6 = false;
  if (0 < *(int *)(param_1[7] + 0x1744)) {
    bVar6 = bVar22;
  }
  bVar21 = 0;
  bVar19 = bVar21;
  if (!(bool)(bVar6 & iVar3 == 4)) goto code_r0x000181c40fc8;
  lVar9 = param_1[0x12];
  if (lVar9 == 0) goto code_r0x000181c41613;
  lVar11 = *(longlong *)(param_2 + 0x170);
  lVar13 = param_2;
  if ((lVar11 != 0) && (*(int *)(lVar11 + 0x10) != 0)) {
    if (cRam0000000185a3ed51 == '\0') {
      FUN_18035da50(0x1857def68);
      LOCK();
      UNLOCK();
      cRam0000000185a3ed51 = '\x01';
    }
    if (*(longlong *)(lVar9 + 0x118) == 0) goto code_r0x000181c41613;
    lVar13 = FUN_182d56280(*(longlong *)(lVar9 + 0x118),lVar11,uRam00000001857def68);
  }
  if (((*(longlong *)(lVar9 + 0x30) == 0) ||
      (lVar11 = *(longlong *)(*(longlong *)(lVar9 + 0x30) + 0x120), lVar11 == 0)) || (lVar13 == 0))
  goto code_r0x000181c41613;
  uVar10 = FUN_181c79360(lVar13,*(undefined4 *)(lVar11 + 0xf0),0);
  cVar7 = FUN_181c70aa0(lVar9,uVar10,3,0);
  if (cVar7 == '\0') {
    cVar7 = FUN_181c70840(lVar9,param_2,0x15,0);
    if (cVar7 != '\0') {
      lVar11 = FUN_181c70520(lVar9,param_2,0);
      if (lVar11 == 0) goto code_r0x000181c41613;
      cVar7 = FUN_181c70aa0(lVar9,*(undefined8 *)(lVar11 + 0x20),3,0);
      if (cVar7 != '\0') goto code_r0x000181c40fc3;
    }
  }
  else {
code_r0x000181c40fc3:
    bVar19 = 1;
  }
code_r0x000181c40fc8:
  if (((*(longlong *)(lVar8 + 0x18) != 0) &&
      (lVar9 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x108), lVar9 != 0)) &&
     (param_1[0x19] != 0)) {
    lVar9 = FUN_18152f870(param_1[0x19],*(undefined8 *)(lVar9 + 0x30),0);
    if (((*(longlong *)(lVar8 + 0x18) != 0) &&
        (lVar11 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x108), lVar11 != 0)) &&
       (lVar13 = param_1[0x19], lVar13 != 0)) {
      uVar10 = FUN_1815314c0(lVar13,*(undefined8 *)(lVar8 + 0x18),*(undefined4 *)(lVar11 + 0x10),0);
      lVar11 = FUN_18152f920(lVar13,uVar10,0);
      if (((lVar9 == 0) || (lVar11 == 0)) || (*(int *)(lVar11 + 0x18) < *(int *)(lVar9 + 0x18))) {
        if (*(longlong *)(lVar8 + 0x18) != 0) {
          if (iVar3 == 3) {
            bVar21 = *(byte *)(*(longlong *)(lVar8 + 0x18) + 0x160) & iVar1 <= iVar2;
          }
          if ((bVar21 == 0) || (*(char *)(lVar8 + 0x24) != '\0')) {
            if (bVar19 == 0) {
              FUN_181c41630(param_1,*(undefined8 *)(lVar8 + 0x18),*(undefined4 *)(lVar8 + 0x20),
                            *(undefined1 *)(lVar8 + 0x24),*(undefined8 *)(lVar8 + 0x28),
                            *(undefined8 *)(lVar8 + 0x30),*(undefined8 *)(lVar8 + 0x38),0);
              return;
            }
            lVar9 = param_1[0x14];
            lVar11 = param_1[0x15];
            if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
              FUN_180359940(lRam00000001857ff558);
            }
            if ((lVar11 != 0) &&
               (lVar13 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x4708),
               lVar13 != 0)) {
              uVar10 = FUN_180ca5830(lVar13,0);
              uStack_30 = FUN_181164190(lVar11,uVar10,0,0,0);
              lVar11 = param_1[0x15];
              uVar10 = *(undefined8 *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x4700);
              plVar16 = (longlong *)FUN_1803a3dc0(uRam000000018580d6a0,1);
              if (param_1[7] != 0) {
                auStack_38[0] = *(undefined4 *)(param_1[7] + 0x1744);
                lVar13 = FUN_1803a4e80(uRam0000000185a33e38,auStack_38);
                if (plVar16 != (longlong *)0x0) {
                  if ((lVar13 != 0) &&
                     (lVar15 = FUN_1803a50f0(lVar13,*(undefined8 *)(*plVar16 + 0x40)), lVar15 == 0))
                  {
                    uVar10 = func_0x000180352550();
                    FUN_180352410(uVar10,0);
                    pcVar4 = (code *)swi(3);
                    (*pcVar4)();
                    return;
                  }
                  if ((int)plVar16[3] == 0) {
                    FUN_180352460();
                    pcVar4 = (code *)swi(3);
                    (*pcVar4)();
                    return;
                  }
                  plVar16[4] = lVar13;
                  if (lVar11 != 0) {
                    uVar10 = FUN_181164590(lVar11,uVar10,plVar16,0);
                    lVar13 = FUN_1803a51b0(uRam000000018580d858);
                    lVar11 = lRam00000001857d7768;
                    puVar14 = *(undefined **)(lRam00000001857d7768 + 8);
                    *(undefined **)(lVar13 + 0x10) = puVar14;
                    *(longlong *)(lVar13 + 0x28) = lVar11;
                    *(longlong *)(lVar13 + 0x20) = lVar8;
                    cVar7 = *(char *)(lVar11 + 0x52);
                    *(longlong *)(lVar13 + 0x40) = lVar13;
                    if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar7 != '\0')) {
                      *(longlong *)(lVar13 + 0x40) = lVar8;
                    }
                    else {
                      puVar14 = &UNK_180006440;
                    }
                    *(undefined **)(lVar13 + 0x18) = puVar14;
                    *(code **)(lVar13 + 0x38) = FUN_1800063e0;
                    lVar15 = FUN_1803a51b0(uRam000000018580d858);
                    lVar11 = lRam00000001857d7770;
                    puVar14 = *(undefined **)(lRam00000001857d7770 + 8);
                    *(undefined **)(lVar15 + 0x10) = puVar14;
                    *(longlong *)(lVar15 + 0x28) = lVar11;
                    *(longlong *)(lVar15 + 0x20) = lVar8;
                    cVar7 = *(char *)(lVar11 + 0x52);
                    *(longlong *)(lVar15 + 0x40) = lVar15;
                    if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) ||
                       (puVar20 = &UNK_180006440, cVar7 != '\0')) {
                      *(longlong *)(lVar15 + 0x40) = lVar8;
                      puVar20 = puVar14;
                    }
                    *(undefined **)(lVar15 + 0x18) = puVar20;
                    *(code **)(lVar15 + 0x38) = FUN_1800063e0;
                    lVar8 = param_1[0x15];
                    if ((lVar8 != 0) &&
                       (lVar11 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x46f8),
                       lVar11 != 0)) {
                      uVar12 = FUN_180ca5830(lVar11,0);
                      uVar12 = FUN_181164190(lVar8,uVar12,0,0,0);
                      lVar8 = param_1[0x15];
                      if ((lVar8 != 0) &&
                         (lVar11 = *(longlong *)
                                    (*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x46f0),
                         lVar11 != 0)) {
                        uVar17 = FUN_180ca5830(lVar11,0);
                        uVar17 = FUN_181164190(lVar8,uVar17,0,0,0);
                        if (lVar9 != 0) {
                          FUN_18126c020(lVar9,uStack_30,uVar10,lVar13,lVar15,0,uVar12,0,uVar17,0,0,0
                                        ,0);
                          return;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else {
            lVar9 = param_1[0x12];
            uVar10 = *(undefined8 *)(lVar8 + 0x18);
            lVar13 = FUN_1803a51b0(uRam000000018580d858);
            lVar11 = lRam00000001857d7778;
            puVar14 = *(undefined **)(lRam00000001857d7778 + 8);
            *(undefined **)(lVar13 + 0x10) = puVar14;
            *(longlong *)(lVar13 + 0x28) = lVar11;
            *(longlong *)(lVar13 + 0x20) = lVar8;
            cVar7 = *(char *)(lVar11 + 0x52);
            *(longlong *)(lVar13 + 0x40) = lVar13;
            if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar7 != '\0')) {
              *(longlong *)(lVar13 + 0x40) = lVar8;
            }
            else {
              puVar14 = &UNK_180006440;
            }
            *(undefined **)(lVar13 + 0x18) = puVar14;
            *(code **)(lVar13 + 0x38) = FUN_1800063e0;
            lVar15 = FUN_1803a51b0(uRam000000018580d858);
            lVar11 = lRam00000001857d7780;
            puVar14 = *(undefined **)(lRam00000001857d7780 + 8);
            *(undefined **)(lVar15 + 0x10) = puVar14;
            *(longlong *)(lVar15 + 0x28) = lVar11;
            *(longlong *)(lVar15 + 0x20) = lVar8;
            cVar7 = *(char *)(lVar11 + 0x52);
            *(longlong *)(lVar15 + 0x40) = lVar15;
            if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) ||
               (puVar20 = &UNK_180006440, cVar7 != '\0')) {
              *(longlong *)(lVar15 + 0x40) = lVar8;
              puVar20 = puVar14;
            }
            *(undefined **)(lVar15 + 0x18) = puVar20;
            *(code **)(lVar15 + 0x38) = FUN_1800063e0;
            if (lVar9 != 0) {
              FUN_181c76e80(lVar9,uVar10,lVar13,lVar15,0);
              return;
            }
          }
        }
      }
      else {
        lVar9 = param_1[0x14];
        lVar11 = param_1[0x15];
        if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857ff558);
        }
        if ((lVar11 != 0) &&
           (lVar13 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x3ab0), lVar13 != 0
           )) {
          uVar10 = FUN_180ca5830(lVar13,0);
          uVar10 = FUN_181164190(lVar11,uVar10,0,0,0);
          lVar11 = param_1[0x15];
          if ((lVar11 != 0) &&
             (lVar13 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x3aa8),
             lVar13 != 0)) {
            uVar12 = FUN_180ca5830(lVar13,0);
            uVar12 = FUN_181164190(lVar11,uVar12,0,0,0);
            if (lVar9 != 0) {
              FUN_18126b6f0(lVar9,uVar10,uVar12,0,0,0,0,1,CONCAT44(uVar23,2),0,
                            in_stack_ffffffffffffffa8 & 0xffffffffffffff00,0);
              (**(code **)(*param_1 + 0x248))
                        (param_1,*(undefined8 *)(lVar8 + 0x40),*(undefined8 *)(*param_1 + 0x250));
              return;
            }
          }
        }
      }
    }
  }
code_r0x000181c41613:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

