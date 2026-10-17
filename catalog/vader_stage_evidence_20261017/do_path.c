// do_path VA 0x180d37590 RVA 0xd37590

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_180d37590(longlong param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong lVar7;
  undefined8 uVar8;
  undefined8 *puVar9;
  code *pcVar10;
  char cVar11;
  undefined1 uVar12;
  undefined8 *puVar13;
  uint uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  undefined4 uVar19;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined4 uStack_48;
  
  if (cRam0000000185a38ebc == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858274c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857cde88);
    LOCK();
    UNLOCK();
    cRam0000000185a38ebc = '\x01';
  }
  if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x000180d383df;
  lVar3 = *(longlong *)(param_1 + 0xa8);
  uVar14 = 0;
  if (lVar3 == 0) {
    lVar3 = 0;
  }
  else if (*(int *)(lVar3 + 0x10) == 0) {
    lVar3 = FUN_183cbddf0(*(longlong *)(param_1 + 0x30),0);
  }
  else {
    if (*(longlong *)(lVar3 + 0x18) == 0) goto code_r0x000180d383df;
    lVar3 = FUN_183c9dd40(*(longlong *)(lVar3 + 0x18),0);
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056e == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056e = '\x01';
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
  if (lVar3 != 0) {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar3 + 0x10) != 0) {
      if (*(longlong *)(param_1 + 0xb8) != 0) {
        lVar4 = FUN_1803a3dc0(uRam00000001858274c0,
                              *(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x18));
        lVar7 = *(longlong *)(param_1 + 0xb8);
        if (lVar7 != 0) {
          while ((int)uVar14 < *(int *)(lVar7 + 0x18)) {
            lVar7 = *(longlong *)(param_1 + 0xb8);
            if (lVar7 == 0) goto code_r0x000180d383df;
            if (*(uint *)(lVar7 + 0x18) <= uVar14) {
code_r0x000180d383f1:
              FUN_180352460();
              pcVar10 = (code *)swi(3);
              lVar3 = (*pcVar10)();
              return lVar3;
            }
            lVar7 = *(longlong *)(lVar7 + 0x20 + (longlong)(int)uVar14 * 8);
            if (lVar7 == 0) goto code_r0x000180d383df;
            if (cRam0000000185a4a2bd == '\0') {
              FUN_18035da50(0x18582fe18);
              LOCK();
              UNLOCK();
              cRam0000000185a4a2bd = '\x01';
            }
            lVar5 = FUN_183ca5340(lVar7);
            if (lVar5 == 0) {
              uVar15 = *(undefined4 *)(lVar7 + 0x38);
              uVar16 = *(undefined4 *)(lVar7 + 0x3c);
              uVar19 = *(undefined4 *)(lVar7 + 0x40);
            }
            else {
              plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
              if ((plVar6 == (longlong *)0x0) ||
                 (plVar6 = (longlong *)
                           (**(code **)(*plVar6 + 0x2f8))(plVar6,*(undefined8 *)(*plVar6 + 0x300)),
                 plVar6 == (longlong *)0x0)) goto code_r0x000180d383df;
              if (*(longlong *)(*plVar6 + 0x40) != *(longlong *)(lRam000000018582fe18 + 0x40)) {
                FUN_180352e70(plVar6,lRam000000018582fe18);
                pcVar10 = (code *)swi(3);
                lVar3 = (*pcVar10)();
                return lVar3;
              }
              uVar15 = (undefined4)plVar6[2];
              uVar19 = 0;
              uVar16 = *(undefined4 *)((longlong)plVar6 + 0x14);
            }
            if (lVar4 == 0) goto code_r0x000180d383df;
            if (*(uint *)(lVar4 + 0x18) <= uVar14) goto code_r0x000180d383f1;
            lVar7 = (longlong)(int)uVar14;
            uVar14 = uVar14 + 1;
            *(ulonglong *)(lVar4 + 0x20 + lVar7 * 0xc) = CONCAT44(uVar16,uVar15);
            *(undefined4 *)(lVar4 + 0x28 + lVar7 * 0xc) = uVar19;
            lVar7 = *(longlong *)(param_1 + 0xb8);
            if (lVar7 == 0) goto code_r0x000180d383df;
          }
          lVar7 = *(longlong *)(param_1 + 0xb0);
          if (lVar7 != 0) {
            lVar5 = FUN_183ca5340(lVar7,0);
            if (lVar5 == 0) {
              cVar11 = *(char *)(lVar7 + 0x38);
            }
            else {
              plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
              if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
              iVar2 = (**(code **)(*plVar6 + 0x358))(plVar6,*(undefined8 *)(*plVar6 + 0x360));
              cVar11 = 0 < iVar2;
            }
            iVar2 = *(int *)(param_1 + 0xd8);
            if (cVar11 == '\0') {
              if (iVar2 == 1) {
                if (cRam0000000185a5034b == '\0') {
                  FUN_18035da50(0x1857b8000);
                  LOCK();
                  UNLOCK();
                  FUN_18035da50(0x1857b7cf0);
                  LOCK();
                  UNLOCK();
                  cRam0000000185a5034b = '\x01';
                }
                lVar7 = *(longlong *)(lVar3 + 0x10);
                if (lVar7 == 0) {
code_r0x000180d38425:
                  FUN_1844c5660(lVar3,0);
                  pcVar10 = (code *)swi(3);
                  lVar3 = (*pcVar10)();
                  return lVar3;
                }
                pcVar10 = pcRam0000000185a503b0;
                if ((pcRam0000000185a503b0 == (code *)0x0) &&
                   (pcVar10 = (code *)FUN_1803679a0(&UNK_184b5add0), pcVar10 == (code *)0x0)) {
                  uVar8 = func_0x000180367810(&UNK_184b5add0);
                  FUN_1803660c0(uVar8,0);
                  pcVar10 = (code *)swi(3);
                  lVar3 = (*pcVar10)();
                  return lVar3;
                }
                pcRam0000000185a503b0 = pcVar10;
                uVar8 = (*pcRam0000000185a503b0)(lVar7);
                uVar8 = FUN_182c7e680(uVar8,uRam00000001857b7cf0);
                lVar3 = *(longlong *)(param_1 + 0x58);
                if (lVar3 != 0) {
                  lVar7 = FUN_183ca5340(lVar3,0);
                  if (lVar7 == 0) {
                    uVar15 = *(undefined4 *)(lVar3 + 0x38);
                  }
                  else {
                    plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                    if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                    uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350))
                    ;
                  }
                  uStack_48 = 0;
                  uStack_58 = 0;
                  uStack_50 = 0;
                  lVar3 = FUN_18248b930(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                        *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
code_r0x000180d38203:
                  lVar7 = *(longlong *)(param_1 + 0xd0);
                  if (lVar7 != 0) {
                    lVar4 = FUN_183ca5340(lVar7,0);
                    if (lVar4 == 0) {
                      uVar12 = *(undefined1 *)(lVar7 + 0x38);
                    }
                    else {
                      plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                      if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                      iVar2 = (**(code **)(*plVar6 + 0x358))
                                        (plVar6,*(undefined8 *)(*plVar6 + 0x360));
                      uVar12 = 0 < iVar2;
                    }
                    if ((lVar3 != 0) && (*(char *)(lVar3 + 0xe8) != '\0')) {
                      uVar15 = *(undefined4 *)(param_1 + 200);
                      *(undefined1 *)(lVar3 + 0x150) = uVar12;
                      *(undefined4 *)(lVar3 + 0x148) = 0;
                      *(undefined4 *)(lVar3 + 0x14c) = uVar15;
                    }
                    lVar7 = *(longlong *)(param_1 + 0xe0);
                    if (lVar7 != 0) {
                      lVar4 = FUN_183ca5340(lVar7,0);
                      if (lVar4 == 0) {
                        fVar17 = *(float *)(lVar7 + 0x38);
                      }
                      else {
                        plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                        if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                        fVar17 = (float)(**(code **)(*plVar6 + 0x348))
                                                  (plVar6,*(undefined8 *)(*plVar6 + 0x350));
                      }
                      if (cRam0000000185a33cad == '\0') {
                        FUN_18035da50(0x1858208a0);
                        LOCK();
                        UNLOCK();
                        cRam0000000185a33cad = '\x01';
                      }
                      fVar18 = _UNK_18529ec48;
                      if (lVar3 == 0) {
                        return 0;
                      }
                      if (*(char *)(lVar3 + 0xe8) == '\0') {
                        return lVar3;
                      }
                      uVar15 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
                      uVar8 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
                      *(undefined4 *)(lVar3 + 0x144) = 1;
                      if (fVar18 <= fVar17) {
                        fVar18 = fVar17;
                      }
                      *(float *)(lVar3 + 0x168) = fVar18;
                      *(undefined8 *)(lVar3 + 0x154) = uVar8;
code_r0x000180d38349:
                      puVar9 = &uStack_58;
                      puVar13 = &uStack_68;
code_r0x000180d38351:
                      uStack_50 = 0;
                      uStack_58 = 0;
                      uStack_60 = 0;
                      uStack_68 = 0;
                      *(undefined4 *)(lVar3 + 0x15c) = uVar15;
                      *(undefined1 *)(lVar3 + 0x192) = 0;
                      FUN_182495220(lVar3,puVar13,puVar9,0);
                      return lVar3;
                    }
                  }
                }
              }
              else if (iVar2 == 3) {
                uVar8 = FUN_1844a54d0(lVar3,0);
                lVar3 = *(longlong *)(param_1 + 0x58);
                if (lVar3 != 0) {
                  lVar7 = FUN_183ca5340(lVar3,0);
                  if (lVar7 == 0) {
                    uVar15 = *(undefined4 *)(lVar3 + 0x38);
                  }
                  else {
                    plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                    if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                    uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350))
                    ;
                  }
                  uStack_48 = 0;
                  uStack_58 = 0;
                  uStack_50 = 0;
                  lVar3 = FUN_18248b930(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                        *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
code_r0x000180d37ba1:
                  lVar7 = *(longlong *)(param_1 + 0xd0);
                  if (lVar7 != 0) {
                    lVar4 = FUN_183ca5340(lVar7,0);
                    if (lVar4 == 0) {
                      uVar12 = *(undefined1 *)(lVar7 + 0x38);
                    }
                    else {
                      plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                      if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                      iVar2 = (**(code **)(*plVar6 + 0x358))
                                        (plVar6,*(undefined8 *)(*plVar6 + 0x360));
                      uVar12 = 0 < iVar2;
                    }
                    if ((lVar3 != 0) && (*(char *)(lVar3 + 0xe8) != '\0')) {
                      uVar15 = *(undefined4 *)(param_1 + 200);
                      *(undefined1 *)(lVar3 + 0x150) = uVar12;
                      *(undefined4 *)(lVar3 + 0x148) = 0;
                      *(undefined4 *)(lVar3 + 0x14c) = uVar15;
                    }
                    if (*(longlong *)(param_1 + 0xe8) != 0) {
                      puVar9 = (undefined8 *)
                               FUN_183ca4f20(&uStack_68,*(longlong *)(param_1 + 0xe8),0);
                      if (lVar3 == 0) {
                        return 0;
                      }
                      if (*(char *)(lVar3 + 0xe8) == '\0') {
                        return lVar3;
                      }
                      uVar16 = (undefined4)*puVar9;
                      uVar19 = (undefined4)((ulonglong)*puVar9 >> 0x20);
                      uVar15 = *(undefined4 *)(puVar9 + 1);
                      *(undefined4 *)(lVar3 + 0x144) = 3;
                      goto code_r0x000180d37c62;
                    }
                  }
                }
              }
              else if (iVar2 == 2) {
                uVar8 = FUN_1844a54d0(lVar3);
                lVar3 = *(longlong *)(param_1 + 0x58);
                if (lVar3 != 0) {
                  lVar7 = FUN_183ca5340(lVar3,0);
                  if (lVar7 == 0) {
                    uVar15 = *(undefined4 *)(lVar3 + 0x38);
                  }
                  else {
                    plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                    if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                    uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350))
                    ;
                  }
                  uStack_48 = 0;
                  uStack_58 = 0;
                  uStack_50 = 0;
                  lVar3 = FUN_18248b930(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                        *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
                  lVar7 = *(longlong *)(param_1 + 0xd0);
                  if (lVar7 != 0) {
                    lVar4 = FUN_183ca5340(lVar7,0);
                    if (lVar4 == 0) {
                      uVar12 = *(undefined1 *)(lVar7 + 0x38);
                    }
                    else {
                      plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                      if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                      iVar2 = (**(code **)(*plVar6 + 0x358))
                                        (plVar6,*(undefined8 *)(*plVar6 + 0x360));
                      uVar12 = 0 < iVar2;
                    }
                    if ((lVar3 != 0) && (*(char *)(lVar3 + 0xe8) != '\0')) {
                      uVar15 = *(undefined4 *)(param_1 + 200);
                      *(undefined1 *)(lVar3 + 0x150) = uVar12;
                      *(undefined4 *)(lVar3 + 0x148) = 0;
                      *(undefined4 *)(lVar3 + 0x14c) = uVar15;
                    }
                    if ((*(longlong *)(param_1 + 0xf0) != 0) &&
                       (lVar7 = FUN_183c9dd40(*(longlong *)(param_1 + 0xf0),0), lVar7 != 0)) {
                      uVar8 = FUN_1844a54d0(lVar7,0);
                      if (cRam0000000185a33cad == '\0') {
                        FUN_18035da50(0x1858208a0);
                        LOCK();
                        UNLOCK();
                        cRam0000000185a33cad = '\x01';
                      }
                      if (lVar3 == 0) {
                        return 0;
                      }
                      if (*(char *)(lVar3 + 0xe8) == '\0') {
                        return lVar3;
                      }
                      uVar1 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
                      uVar15 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
                      *(undefined4 *)(lVar3 + 0x144) = 2;
                      *(undefined8 *)(lVar3 + 0x160) = uVar8;
                      *(undefined8 *)(lVar3 + 0x154) = uVar1;
                      puVar9 = &uStack_68;
                      puVar13 = &uStack_58;
                      goto code_r0x000180d38351;
                    }
                  }
                }
              }
              else {
                uVar8 = FUN_1844a54d0(lVar3);
                lVar3 = *(longlong *)(param_1 + 0x58);
                if (lVar3 != 0) {
                  lVar7 = FUN_183ca5340(lVar3,0);
                  if (lVar7 == 0) {
                    uVar15 = *(undefined4 *)(lVar3 + 0x38);
                  }
                  else {
                    plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                    if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                    uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350))
                    ;
                  }
                  uStack_48 = 0;
                  uStack_58 = 0;
                  uStack_50 = 0;
                  lVar3 = FUN_18248b930(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                        *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
code_r0x000180d37e3a:
                  lVar7 = *(longlong *)(param_1 + 0xd0);
                  if (lVar7 != 0) {
                    lVar4 = FUN_183ca5340(lVar7,0);
                    if (lVar4 == 0) {
                      uVar12 = *(undefined1 *)(lVar7 + 0x38);
                    }
                    else {
                      plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                      if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                      iVar2 = (**(code **)(*plVar6 + 0x358))
                                        (plVar6,*(undefined8 *)(*plVar6 + 0x360));
                      uVar12 = 0 < iVar2;
                    }
                    if (lVar3 == 0) {
                      return 0;
                    }
                    if (*(char *)(lVar3 + 0xe8) == '\0') {
                      return lVar3;
                    }
                    uVar15 = *(undefined4 *)(param_1 + 200);
                    *(undefined1 *)(lVar3 + 0x150) = uVar12;
                    *(undefined4 *)(lVar3 + 0x148) = 0;
                    *(undefined4 *)(lVar3 + 0x14c) = uVar15;
                    return lVar3;
                  }
                }
              }
            }
            else if (iVar2 == 1) {
              if (cRam0000000185a5034b == '\0') {
                FUN_18035da50(0x1857b8000);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x1857b7cf0);
                LOCK();
                UNLOCK();
                cRam0000000185a5034b = '\x01';
              }
              lVar7 = *(longlong *)(lVar3 + 0x10);
              if (lVar7 == 0) goto code_r0x000180d38425;
              pcVar10 = pcRam0000000185a503b0;
              if ((pcRam0000000185a503b0 == (code *)0x0) &&
                 (pcVar10 = (code *)FUN_1803679a0(&UNK_184b5add0), pcVar10 == (code *)0x0)) {
                uVar8 = func_0x000180367810(&UNK_184b5add0);
                FUN_1803660c0(uVar8,0);
                pcVar10 = (code *)swi(3);
                lVar3 = (*pcVar10)();
                return lVar3;
              }
              pcRam0000000185a503b0 = pcVar10;
              uVar8 = (*pcRam0000000185a503b0)(lVar7);
              uVar8 = FUN_182c7e680(uVar8,uRam00000001857b7cf0);
              lVar3 = *(longlong *)(param_1 + 0x58);
              if (lVar3 != 0) {
                lVar7 = FUN_183ca5340(lVar3,0);
                if (lVar7 == 0) {
                  uVar15 = *(undefined4 *)(lVar3 + 0x38);
                }
                else {
                  plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                  if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                  uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350));
                }
                uStack_48 = 0;
                uStack_58 = 0;
                uStack_50 = 0;
                lVar3 = FUN_18248bbf0(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                      *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
                goto code_r0x000180d38203;
              }
            }
            else if (iVar2 == 3) {
              uVar8 = FUN_1844a54d0(lVar3,0);
              lVar3 = *(longlong *)(param_1 + 0x58);
              if (lVar3 != 0) {
                lVar7 = FUN_183ca5340(lVar3,0);
                if (lVar7 == 0) {
                  uVar15 = *(undefined4 *)(lVar3 + 0x38);
                }
                else {
                  plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                  if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                  uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350));
                }
                uStack_48 = 0;
                uStack_58 = 0;
                uStack_50 = 0;
                lVar3 = FUN_18248bbf0(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                      *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
                goto code_r0x000180d37ba1;
              }
            }
            else if (iVar2 == 2) {
              uVar8 = FUN_1844a54d0(lVar3);
              lVar3 = *(longlong *)(param_1 + 0x58);
              if (lVar3 != 0) {
                lVar7 = FUN_183ca5340(lVar3,0);
                if (lVar7 == 0) {
                  uVar15 = *(undefined4 *)(lVar3 + 0x38);
                }
                else {
                  plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                  if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                  uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350));
                }
                uStack_48 = 0;
                uStack_58 = 0;
                uStack_50 = 0;
                lVar3 = FUN_18248bbf0(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                      *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
                lVar7 = *(longlong *)(param_1 + 0xd0);
                if (lVar7 != 0) {
                  lVar4 = FUN_183ca5340(lVar7,0);
                  if (lVar4 == 0) {
                    uVar12 = *(undefined1 *)(lVar7 + 0x38);
                  }
                  else {
                    plVar6 = (longlong *)FUN_183ca5340(lVar7,0);
                    if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                    iVar2 = (**(code **)(*plVar6 + 0x358))(plVar6,*(undefined8 *)(*plVar6 + 0x360));
                    uVar12 = 0 < iVar2;
                  }
                  if ((lVar3 != 0) && (*(char *)(lVar3 + 0xe8) != '\0')) {
                    uVar15 = *(undefined4 *)(param_1 + 200);
                    *(undefined1 *)(lVar3 + 0x150) = uVar12;
                    *(undefined4 *)(lVar3 + 0x148) = 0;
                    *(undefined4 *)(lVar3 + 0x14c) = uVar15;
                  }
                  if ((*(longlong *)(param_1 + 0xf0) != 0) &&
                     (lVar7 = FUN_183c9dd40(*(longlong *)(param_1 + 0xf0),0), lVar7 != 0)) {
                    uVar8 = FUN_1844a54d0(lVar7,0);
                    if (cRam0000000185a33cad == '\0') {
                      FUN_18035da50(0x1858208a0);
                      LOCK();
                      UNLOCK();
                      cRam0000000185a33cad = '\x01';
                    }
                    if (lVar3 == 0) {
                      return 0;
                    }
                    if (*(char *)(lVar3 + 0xe8) == '\0') {
                      return lVar3;
                    }
                    uVar1 = **(undefined8 **)(lRam00000001858208a0 + 0xb8);
                    uVar16 = (undefined4)uVar1;
                    uVar19 = (undefined4)((ulonglong)uVar1 >> 0x20);
                    uVar15 = *(undefined4 *)(*(undefined8 **)(lRam00000001858208a0 + 0xb8) + 1);
                    *(undefined4 *)(lVar3 + 0x144) = 2;
                    *(undefined8 *)(lVar3 + 0x160) = uVar8;
code_r0x000180d37c62:
                    *(ulonglong *)(lVar3 + 0x154) = CONCAT44(uVar19,uVar16);
                    goto code_r0x000180d38349;
                  }
                }
              }
            }
            else {
              uVar8 = FUN_1844a54d0(lVar3);
              lVar3 = *(longlong *)(param_1 + 0x58);
              if (lVar3 != 0) {
                lVar7 = FUN_183ca5340(lVar3,0);
                if (lVar7 == 0) {
                  uVar15 = *(undefined4 *)(lVar3 + 0x38);
                }
                else {
                  plVar6 = (longlong *)FUN_183ca5340(lVar3,0);
                  if (plVar6 == (longlong *)0x0) goto code_r0x000180d383df;
                  uVar15 = (**(code **)(*plVar6 + 0x348))(plVar6,*(undefined8 *)(*plVar6 + 0x350));
                }
                uStack_48 = 0;
                uStack_58 = 0;
                uStack_50 = 0;
                lVar3 = FUN_18248bbf0(uVar8,lVar4,uVar15,*(undefined4 *)(param_1 + 0xc0),
                                      *(undefined4 *)(param_1 + 0xc4),10,&uStack_58,0);
                goto code_r0x000180d37e3a;
              }
            }
          }
        }
      }
      goto code_r0x000180d383df;
    }
  }
  if (*(longlong *)(param_1 + 0x20) != 0) {
    uVar8 = FUN_1844aedc0(*(longlong *)(param_1 + 0x20),0);
    uVar8 = FUN_1838da1a0(uRam00000001857cde88,uVar8,0);
    FUN_183cd89f0(param_1,uVar8,0);
    return 0;
  }
code_r0x000180d383df:
  FUN_180352450();
  pcVar10 = (code *)swi(3);
  lVar3 = (*pcVar10)();
  return lVar3;
}

