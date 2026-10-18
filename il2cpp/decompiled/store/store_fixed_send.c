// store_fixed_send VA 0x181c4f990 RVA 0x1c4f990

void FUN_181c4f990(longlong param_1,longlong param_2,int param_3,char param_4,undefined8 param_5,
                  undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  char cVar1;
  undefined4 uVar2;
  longlong lVar3;
  code *pcVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong *plVar11;
  longlong lVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  undefined *puVar16;
  int iVar17;
  longlong lVar18;
  undefined8 in_stack_ffffffffffffffa8;
  undefined4 uVar19;
  ulonglong in_stack_ffffffffffffffc0;
  
  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffffa8 >> 0x20);
  if (cRam0000000185a3ec93 == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d6e40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d6a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d97a0);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec93 = '\x01';
  }
  lVar8 = FUN_1803a51b0(uRam00000001857d97a0);
  if (lVar8 == 0) goto code_r0x000181c5040e;
  *(longlong *)(lVar8 + 0x10) = param_1;
  iVar17 = 0;
  if (param_4 == '\0') {
    if ((param_2 == 0) || (lVar9 = *(longlong *)(param_2 + 0x108), lVar9 == 0))
    goto code_r0x000181c5040e;
    iVar17 = param_3 * *(int *)(lVar9 + 0x14);
  }
  else if ((param_2 == 0) || (lVar9 = *(longlong *)(param_2 + 0x108), lVar9 == 0))
  goto code_r0x000181c5040e;
  if (*(int *)(lVar9 + 0x10) == 4) {
code_r0x000181c5038a:
    if (*(longlong *)(param_1 + 0x178) != 0) {
      uVar10 = FUN_1815d07b0(*(longlong *)(param_1 + 0x178),0);
      *(undefined8 *)(param_1 + 0x218) = uVar10;
      if (*(longlong *)(param_1 + 0x180) != 0) {
        uVar10 = FUN_180f62f80(*(longlong *)(param_1 + 0x180),0);
        *(undefined8 *)(param_1 + 0x220) = uVar10;
        FUN_181c40c50(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,0);
        return;
      }
    }
    goto code_r0x000181c5040e;
  }
  if ((*(longlong *)(param_1 + 0x98) == 0) ||
     (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0xd8), lVar9 == 0))
  goto code_r0x000181c5040e;
  iVar7 = FUN_180fffb20(lVar9,*(undefined4 *)(*(longlong *)(param_2 + 0x108) + 0x10),0);
  if (iVar17 <= iVar7) goto code_r0x000181c5038a;
  if ((*(longlong *)(param_2 + 0x108) == 0) || (*(longlong *)(param_1 + 0x90) == 0))
  goto code_r0x000181c5040e;
  lVar9 = FUN_181c71060(*(longlong *)(param_1 + 0x90),
                        *(undefined4 *)(*(longlong *)(param_2 + 0x108) + 0x10),0);
  *(longlong *)(lVar8 + 0x18) = lVar9;
  if ((lVar9 != 0) &&
     ((*(longlong *)(lVar9 + 0x38) == 0 || (*(int *)(*(longlong *)(lVar9 + 0x38) + 0x1c) != 2)))) {
    if (*(longlong *)(param_2 + 0x108) != 0) {
      lVar9 = *(longlong *)(param_1 + 0xa8);
      uVar2 = *(undefined4 *)(*(longlong *)(param_2 + 0x108) + 0x10);
      if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
        FUN_180359940();
      }
      uVar10 = FUN_18125add0(uVar2,0);
      if (lVar9 != 0) {
        lVar9 = FUN_181164190(lVar9,uVar10,0,0,0);
        if (*(longlong *)(lVar8 + 0x18) != 0) {
          lVar3 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x18);
          if ((lVar3 == 0) || (*(int *)(lVar3 + 0x10) == 0)) {
            bVar6 = false;
          }
          else {
            bVar6 = true;
          }
          if (*(char *)(*(longlong *)(lVar8 + 0x18) + 0x30) == '\0') {
            lVar3 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x38);
            bVar5 = false;
            if (lVar3 != 0) {
              bVar5 = bVar6;
            }
            if (!bVar5) {
              if (bVar6) {
                lVar3 = *(longlong *)(param_1 + 0xa0);
                lVar18 = *(longlong *)(param_1 + 0xa8);
                if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
                  FUN_180359940(lRam00000001857ff558);
                }
                if ((lVar18 != 0) &&
                   (lVar12 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x60f8),
                   lVar12 != 0)) {
                  uVar10 = FUN_180ca5830(lVar12,0);
                  uVar10 = FUN_181164190(lVar18,uVar10,0,0,0);
                  if (*(longlong *)(lVar8 + 0x18) != 0) {
                    uVar13 = *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x18);
                    lVar18 = *(longlong *)(param_1 + 0xa8);
                    plVar11 = (longlong *)FUN_1803a3dc0(uRam000000018580d6a0,1);
                    if (plVar11 != (longlong *)0x0) {
                      if ((lVar9 != 0) &&
                         (lVar12 = FUN_1803a50f0(lVar9,*(undefined8 *)(*plVar11 + 0x40)),
                         lVar12 == 0)) {
                        uVar10 = func_0x000180352550();
                        FUN_180352410(uVar10,0);
                        pcVar4 = (code *)swi(3);
                        (*pcVar4)();
                        return;
                      }
                      if ((int)plVar11[3] == 0) goto code_r0x000181c50444;
                      plVar11[4] = lVar9;
                      if (lVar18 != 0) {
                        uVar13 = FUN_181164550(lVar18,uVar13,plVar11,0);
                        if (*(longlong *)(lVar8 + 0x18) != 0) {
                          lVar9 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x20);
                          if ((lVar9 == 0) || (*(int *)(lVar9 + 0x10) == 0)) {
                            uVar14 = 0;
                          }
                          else {
                            if (*(longlong *)(param_1 + 0xa8) == 0) goto code_r0x000181c5040e;
                            uVar14 = FUN_181164190(*(longlong *)(param_1 + 0xa8),
                                                   *(undefined8 *)
                                                    (*(longlong *)(lVar8 + 0x18) + 0x20),0,0,0);
                          }
                          if (lVar3 != 0) {
                            FUN_18126b6f0(lVar3,uVar10,uVar13,0,uVar14,0,0,1,CONCAT44(uVar19,2),0,0,
                                          0);
                            return;
                          }
                        }
                      }
                    }
                  }
                }
              }
              else {
                if (lVar3 == 0) {
                  return;
                }
                if (*(longlong *)(param_1 + 0x1b8) != 0) {
                  FUN_1815c6310(*(longlong *)(param_1 + 0x1b8),
                                *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x38),0,0,0);
                  return;
                }
              }
              goto code_r0x000181c5040e;
            }
            lVar3 = *(longlong *)(param_1 + 0xa0);
            lVar18 = *(longlong *)(param_1 + 0xa8);
            if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
              FUN_180359940(lRam00000001857ff558);
            }
            if ((lVar18 == 0) ||
               (lVar12 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x60f8),
               lVar12 == 0)) goto code_r0x000181c5040e;
            uVar10 = FUN_180ca5830(lVar12,0);
            uVar10 = FUN_181164190(lVar18,uVar10,0,0,0);
            if (*(longlong *)(lVar8 + 0x18) == 0) goto code_r0x000181c5040e;
            uVar13 = *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x18);
            lVar18 = *(longlong *)(param_1 + 0xa8);
            plVar11 = (longlong *)FUN_1803a3dc0(uRam000000018580d6a0,1);
            if (plVar11 == (longlong *)0x0) goto code_r0x000181c5040e;
            if ((lVar9 != 0) &&
               (lVar12 = FUN_1803a50f0(lVar9,*(undefined8 *)(*plVar11 + 0x40)), lVar12 == 0)) {
              uVar10 = func_0x000180352550();
              FUN_180352410(uVar10,0);
              pcVar4 = (code *)swi(3);
              (*pcVar4)();
              return;
            }
            if ((int)plVar11[3] == 0) goto code_r0x000181c50444;
            plVar11[4] = lVar9;
            if (lVar18 == 0) goto code_r0x000181c5040e;
            uVar13 = FUN_181164550(lVar18,uVar13,plVar11,0);
            lVar9 = FUN_1803a51b0(uRam000000018580d858);
            lVar18 = lRam00000001857d97b0;
            puVar16 = *(undefined **)(lRam00000001857d97b0 + 8);
            *(undefined **)(lVar9 + 0x10) = puVar16;
            *(longlong *)(lVar9 + 0x28) = lVar18;
            *(longlong *)(lVar9 + 0x20) = lVar8;
            cVar1 = *(char *)(lVar18 + 0x52);
            *(longlong *)(lVar9 + 0x40) = lVar9;
            if (((*(byte *)(lVar18 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
              *(longlong *)(lVar9 + 0x40) = lVar8;
            }
            else {
              puVar16 = &UNK_180006440;
            }
            *(undefined **)(lVar9 + 0x18) = puVar16;
            *(code **)(lVar9 + 0x38) = FUN_1800063e0;
            if (*(longlong *)(lVar8 + 0x18) == 0) goto code_r0x000181c5040e;
            lVar18 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x20);
            if ((lVar18 == 0) || (*(int *)(lVar18 + 0x10) == 0)) {
              uVar14 = 0;
            }
            else {
              if (*(longlong *)(param_1 + 0xa8) == 0) goto code_r0x000181c5040e;
              uVar14 = FUN_181164190(*(longlong *)(param_1 + 0xa8),
                                     *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x20),0,0,0);
            }
            uVar15 = 0;
            lVar8 = *(longlong *)(lVar8 + 0x18);
            if (lVar8 == 0) goto code_r0x000181c5040e;
            if ((*(longlong *)(lVar8 + 0x28) != 0) &&
               (*(int *)(*(longlong *)(lVar8 + 0x28) + 0x10) != 0)) {
              if (*(longlong *)(param_1 + 0xa8) == 0) goto code_r0x000181c5040e;
              uVar15 = FUN_181164190(*(longlong *)(param_1 + 0xa8),*(undefined8 *)(lVar8 + 0x28),0,0
                                     ,0);
            }
          }
          else {
            lVar3 = *(longlong *)(param_1 + 0xa0);
            lVar18 = *(longlong *)(param_1 + 0xa8);
            if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
              FUN_180359940(lRam00000001857ff558);
            }
            if ((lVar18 == 0) ||
               (lVar12 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x60f8),
               lVar12 == 0)) goto code_r0x000181c5040e;
            uVar10 = FUN_180ca5830(lVar12,0);
            uVar10 = FUN_181164190(lVar18,uVar10,0,0,0);
            if (*(longlong *)(lVar8 + 0x18) == 0) goto code_r0x000181c5040e;
            uVar13 = *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x18);
            lVar18 = *(longlong *)(param_1 + 0xa8);
            plVar11 = (longlong *)FUN_1803a3dc0(uRam000000018580d6a0,1);
            if (plVar11 == (longlong *)0x0) goto code_r0x000181c5040e;
            if ((lVar9 != 0) &&
               (lVar12 = FUN_1803a50f0(lVar9,*(undefined8 *)(*plVar11 + 0x40)), lVar12 == 0)) {
              uVar10 = func_0x000180352550();
              FUN_180352410(uVar10,0);
              pcVar4 = (code *)swi(3);
              (*pcVar4)();
              return;
            }
            if ((int)plVar11[3] == 0) {
code_r0x000181c50444:
              FUN_180352460();
              pcVar4 = (code *)swi(3);
              (*pcVar4)();
              return;
            }
            plVar11[4] = lVar9;
            if (lVar18 == 0) goto code_r0x000181c5040e;
            uVar13 = FUN_181164550(lVar18,uVar13,plVar11,0);
            lVar9 = FUN_1803a51b0(uRam000000018580d858);
            lVar18 = lRam00000001857d97a8;
            puVar16 = *(undefined **)(lRam00000001857d97a8 + 8);
            *(undefined **)(lVar9 + 0x10) = puVar16;
            *(longlong *)(lVar9 + 0x28) = lVar18;
            *(longlong *)(lVar9 + 0x20) = lVar8;
            cVar1 = *(char *)(lVar18 + 0x52);
            *(longlong *)(lVar9 + 0x40) = lVar9;
            if (((*(byte *)(lVar18 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
              *(longlong *)(lVar9 + 0x40) = lVar8;
            }
            else {
              puVar16 = &UNK_180006440;
            }
            *(undefined **)(lVar9 + 0x18) = puVar16;
            *(code **)(lVar9 + 0x38) = FUN_1800063e0;
            if (*(longlong *)(lVar8 + 0x18) == 0) goto code_r0x000181c5040e;
            lVar18 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x20);
            if ((lVar18 == 0) || (*(int *)(lVar18 + 0x10) == 0)) {
              lVar18 = *(longlong *)(param_1 + 0xa8);
              if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
                FUN_180359940(lRam00000001857ff558);
              }
              if ((lVar18 == 0) ||
                 (lVar12 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x1808),
                 lVar12 == 0)) goto code_r0x000181c5040e;
              uVar14 = FUN_180ca5830(lVar12,0);
            }
            else {
              if ((*(longlong *)(lVar8 + 0x18) == 0) ||
                 (lVar18 = *(longlong *)(param_1 + 0xa8), lVar18 == 0)) goto code_r0x000181c5040e;
              uVar14 = *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x20);
            }
            uVar15 = 0;
            uVar14 = FUN_181164190(lVar18,uVar14,0,0,0);
            if (*(longlong *)(lVar8 + 0x18) == 0) goto code_r0x000181c5040e;
            lVar18 = *(longlong *)(*(longlong *)(lVar8 + 0x18) + 0x28);
            if ((lVar18 != 0) && (*(int *)(lVar18 + 0x10) != 0)) {
              if (*(longlong *)(param_1 + 0xa8) == 0) goto code_r0x000181c5040e;
              uVar15 = FUN_181164190(*(longlong *)(param_1 + 0xa8),
                                     *(undefined8 *)(*(longlong *)(lVar8 + 0x18) + 0x28),0,0,0);
            }
          }
          if (lVar3 != 0) {
            FUN_18126b8f0(lVar3,uVar10,uVar13,lVar9,0,uVar14,0,uVar15,0,0,1,
                          in_stack_ffffffffffffffc0 & 0xffffffffffffff00,0);
            return;
          }
        }
      }
    }
    goto code_r0x000181c5040e;
  }
  if (*(longlong *)(param_1 + 0x228) == 0) goto code_r0x000181c5040e;
  lVar9 = FUN_182b30790(*(longlong *)(param_1 + 0x228),uRam00000001857d6e40);
  lVar8 = *(longlong *)(param_1 + 0x1f0);
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056f == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056f = '\x01';
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5054a == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5054a = '\x01';
  }
  if (lVar8 == 0) {
code_r0x000181c5035a:
    uVar10 = **(undefined8 **)(lRam0000000185a33e80 + 0xb8);
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar8 + 0x10) == 0) goto code_r0x000181c5035a;
    if ((*(longlong **)(param_1 + 0x1f0) == (longlong *)0x0) ||
       (plVar11 = (longlong *)FUN_1803a7d00(**(longlong **)(param_1 + 0x1f0) + 0x20),
       plVar11 == (longlong *)0x0)) goto code_r0x000181c5040e;
    uVar10 = (**(code **)(*plVar11 + 0x1b8))(plVar11,*(undefined8 *)(*plVar11 + 0x1c0));
  }
  if (lVar9 != 0) {
    FUN_181c58ea0(lVar9,uVar10,0,0);
    return;
  }
code_r0x000181c5040e:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

