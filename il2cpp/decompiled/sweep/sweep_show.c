// sweep_show VA 0x18164f2d0 RVA 0x164f2d0

ulonglong FUN_18164f2d0(longlong param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  longlong *plVar2;
  code *pcVar3;
  char cVar4;
  char cVar5;
  undefined1 uVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  undefined8 uVar11;
  longlong lVar12;
  undefined *puVar13;
  undefined *puVar14;
  undefined8 unaff_RBX;
  undefined8 unaff_RBP;
  undefined8 unaff_RSI;
  uint uVar15;
  undefined8 unaff_RDI;
  undefined8 unaff_R14;
  float fVar16;
  undefined4 extraout_XMM0_Da;
  undefined4 extraout_XMM0_Db;
  ulonglong uVar17;
  float fVar18;
  undefined8 uVar19;
  
  if (cRam0000000185a3c639 == '\0') {
    FUN_18035da50(0x1857fd790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd798);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd788);
    LOCK();
    UNLOCK();
    cRam0000000185a3c639 = '\x01';
  }
  if (*(char *)(param_1 + 0x110) == '\x01') {
    lVar12 = *(longlong *)(param_1 + 0xf0);
    uVar11 = FUN_1803a51b0(uRam00000001857fd788);
    FUN_180e639d0(uVar11,param_1,uRam00000001857fd798,0);
    if (lVar12 == 0) goto code_r0x00018164f464;
    uVar6 = FUN_183673160(lVar12,uVar11,uRam00000001857fd790);
  }
  else {
    if (*(longlong *)(param_1 + 0xb0) == 0) goto code_r0x00018164f464;
    lVar12 = FUN_1815d7180(*(longlong *)(param_1 + 0xb0),0);
    uVar6 = lVar12 != 0;
  }
  if ((*(longlong *)(param_1 + 0xf0) != 0) && (*(longlong *)(param_1 + 0x98) != 0)) {
    uVar19 = 0;
    uVar11 = FUN_1816532c0(*(longlong *)(param_1 + 0x98),param_2,*(undefined4 *)(param_1 + 0x120),
                           param_3,*(undefined4 *)(*(longlong *)(param_1 + 0xf0) + 0x18),
                           *(undefined8 *)(param_1 + 0x108),uVar6,*(undefined1 *)(param_1 + 0x125),0
                          );
    if (*(longlong *)(param_1 + 0xe0) != 0) {
      FUN_181652750(*(longlong *)(param_1 + 0xe0),uVar11,0);
      lVar12 = *(longlong *)(param_1 + 0xe0);
      if (lVar12 != 0) {
        if (cRam0000000185a3ae5a == '\0') {
          FUN_18035da50(0x18580d858,1,param_4,0,uVar19,unaff_RDI,unaff_RSI,unaff_RBP,unaff_RBX);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857c5ec8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857dd908);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857dd910);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857ff4f8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185794090);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857940a0);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857940a8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185794070);
          LOCK();
          UNLOCK();
          cRam0000000185a3ae5a = '\x01';
        }
        lVar7 = FUN_1803a51b0(uRam0000000185794070);
        if (lVar7 != 0) {
          *(longlong *)(lVar7 + 0x10) = lVar12;
          *(undefined1 *)(lVar7 + 0x18) = 1;
          *(undefined8 *)(lVar7 + 0x20) = param_4;
          if (*(int *)(lRam00000001857c5ec8 + 0xe4) == 0) {
            FUN_180359940();
          }
          uVar11 = FUN_181137920(0);
          *(undefined8 *)(lVar12 + 0xb0) = uVar11;
          cVar5 = *(char *)(lVar7 + 0x18);
          cVar4 = FUN_1812b9970(lVar12,0);
          uVar17 = CONCAT44(extraout_XMM0_Db,extraout_XMM0_Da);
          if (cVar5 == cVar4) {
            lVar9 = *(longlong *)(lVar7 + 0x20);
            if (lVar9 != 0) {
              uVar17 = (**(code **)(lVar9 + 0x18))
                                 (*(undefined8 *)(lVar9 + 0x40),*(undefined8 *)(lVar9 + 0x28));
            }
            if (*(int *)(lRam00000001857ff4f8 + 0xe4) == 0) {
              uVar17 = FUN_180359940(lRam00000001857ff4f8);
            }
            lVar9 = **(longlong **)(lRam00000001857ff4f8 + 0xb8);
            if (lVar9 == 0) {
              return uVar17;
            }
                    /* WARNING: Could not recover jumptable at 0x0001812ba66c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            uVar17 = (**(code **)(lVar9 + 0x18))
                               (*(undefined8 *)(lVar9 + 0x40),lVar12,*(undefined1 *)(lVar7 + 0x18),
                                *(undefined8 *)(lVar9 + 0x28));
            return uVar17;
          }
          lVar9 = *(longlong *)(lVar12 + 0x50);
          if ((lVar9 != 0) && (*(longlong *)(lVar9 + 0x60) != 0)) {
            *(undefined1 *)(*(longlong *)(lVar9 + 0x60) + 0x32) = 0;
            if (*(longlong *)(lVar9 + 0x60) != 0) {
              *(undefined1 *)(*(longlong *)(lVar9 + 0x60) + 0x31) = 0;
              if (*(char *)(lVar7 + 0x18) == '\0') {
                if (*(char *)(lVar12 + 0xc9) != '\0') {
                  if (*(longlong *)(lVar7 + 0x20) != 0) {
                    if (*(longlong *)(lVar12 + 0xd0) == 0) goto code_r0x0001812ba670;
                    uVar17 = FUN_18000d460(*(longlong *)(lVar12 + 0xd0),
                                           *(undefined8 *)(lVar7 + 0x20));
                  }
                  if (*(int *)(lRam00000001857ff4f8 + 0xe4) == 0) {
                    uVar17 = FUN_180359940(lRam00000001857ff4f8);
                  }
                  if (**(longlong **)(lRam00000001857ff4f8 + 0xb8) != 0) {
                    lVar12 = *(longlong *)(lVar12 + 0xd0);
                    lVar8 = FUN_1803a51b0(uRam000000018580d858);
                    lVar9 = lRam0000000185794090;
                    puVar13 = *(undefined **)(lRam0000000185794090 + 8);
                    *(undefined **)(lVar8 + 0x10) = puVar13;
                    *(longlong *)(lVar8 + 0x28) = lVar9;
                    *(longlong *)(lVar8 + 0x20) = lVar7;
                    cVar5 = *(char *)(lVar9 + 0x52);
                    *(longlong *)(lVar8 + 0x40) = lVar8;
                    if (((*(byte *)(lVar9 + 0x4c) & 0x10) == 0) || (cVar5 != '\0')) {
                      *(longlong *)(lVar8 + 0x40) = lVar7;
                    }
                    else {
                      puVar13 = &UNK_180006440;
                    }
                    *(undefined **)(lVar8 + 0x18) = puVar13;
                    *(code **)(lVar8 + 0x38) = FUN_1800063e0;
                    if (lVar12 == 0) goto code_r0x0001812ba670;
                    uVar17 = FUN_18000d460(lVar12,lVar8);
                  }
                  return uVar17;
                }
              }
              else {
                lVar9 = FUN_1844a1720(lVar12,0);
                if (lVar9 == 0) goto code_r0x0001812ba670;
                FUN_1844a56e0(lVar9,1,0);
                lVar9 = *(longlong *)(lVar12 + 0xd0);
                if (lVar9 == 0) goto code_r0x0001812ba670;
                iVar1 = *(int *)(lVar9 + 0x18);
                *(int *)(lVar9 + 0x1c) = *(int *)(lVar9 + 0x1c) + 1;
                *(undefined4 *)(lVar9 + 0x18) = 0;
                if (0 < iVar1) {
                  FUN_183af4330(*(undefined8 *)(lVar9 + 0x10),0,iVar1,0);
                }
              }
              *(bool *)(lVar12 + 0xc9) = *(char *)(lVar7 + 0x18) == '\0';
              lVar8 = FUN_1803a51b0(uRam000000018580d858);
              lVar9 = lRam00000001857940a0;
              puVar13 = *(undefined **)(lRam00000001857940a0 + 8);
              *(undefined **)(lVar8 + 0x10) = puVar13;
              *(longlong *)(lVar8 + 0x28) = lVar9;
              *(longlong *)(lVar8 + 0x20) = lVar7;
              cVar5 = *(char *)(lVar9 + 0x52);
              *(longlong *)(lVar8 + 0x40) = lVar8;
              if (((*(byte *)(lVar9 + 0x4c) & 0x10) == 0) || (cVar5 != '\0')) {
                *(longlong *)(lVar8 + 0x40) = lVar7;
              }
              else {
                puVar13 = &UNK_180006440;
              }
              *(undefined **)(lVar8 + 0x18) = puVar13;
              *(code **)(lVar8 + 0x38) = FUN_1800063e0;
              *(longlong *)(lVar7 + 0x28) = lVar8;
              if (*(int *)(lRam00000001857ff4f8 + 0xe4) == 0) {
                FUN_180359940();
              }
              cVar5 = FUN_1812b9770(0);
              if (cVar5 == '\0') {
                if (*(int *)(lRam00000001857ff4f8 + 0xe4) == 0) {
                  FUN_180359940();
                }
                cVar5 = FUN_1812b9870(0);
                if (cVar5 == '\0') {
                  lVar9 = 0x30;
                  if (*(char *)(lVar7 + 0x18) == '\0') {
                    lVar9 = 0x38;
                  }
                  lVar9 = *(longlong *)(lVar9 + lVar12);
                  *(undefined1 *)(lVar12 + 0xd8) = *(undefined1 *)(lVar7 + 0x18);
                  lVar10 = FUN_1803a51b0(uRam000000018580d858);
                  lVar8 = lRam00000001857940a8;
                  puVar13 = *(undefined **)(lRam00000001857940a8 + 8);
                  *(undefined **)(lVar10 + 0x10) = puVar13;
                  *(longlong *)(lVar10 + 0x28) = lVar8;
                  *(longlong *)(lVar10 + 0x20) = lVar7;
                  cVar5 = *(char *)(lVar8 + 0x52);
                  *(longlong *)(lVar10 + 0x40) = lVar10;
                  if (((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) ||
                     (puVar14 = &UNK_180006440, cVar5 != '\0')) {
                    *(longlong *)(lVar10 + 0x40) = lVar7;
                    puVar14 = puVar13;
                  }
                  *(undefined **)(lVar10 + 0x18) = puVar14;
                  *(code **)(lVar10 + 0x38) = FUN_1800063e0;
                  if (lVar9 != 0) {
                    if (cRam0000000185a3ae32 == '\0') {
                      FUN_18035da50(0x18578f120,lVar12,lVar10,0,unaff_RSI,unaff_RBP,unaff_R14);
                      LOCK();
                      UNLOCK();
                      FUN_18035da50(0x1857943a0);
                      LOCK();
                      UNLOCK();
                      FUN_18035da50(0x18578f128);
                      LOCK();
                      UNLOCK();
                      FUN_18035da50(0x1857943b0);
                      LOCK();
                      UNLOCK();
                      FUN_18035da50(0x185814ea8);
                      LOCK();
                      UNLOCK();
                      cRam0000000185a3ae32 = '\x01';
                    }
                    if (*(longlong *)(lVar9 + 0x10) != 0) {
                      uVar15 = 0;
                      fVar18 = 0.0;
                      if (0 < *(int *)(*(longlong *)(lVar9 + 0x10) + 0x18)) {
                        do {
                          lVar7 = *(longlong *)(lVar9 + 0x10);
                          if (lVar7 == 0) goto DAT_1812b67e5;
                          if (*(uint *)(lVar7 + 0x18) <= uVar15) goto code_r0x0001812b67f1;
                          lVar7 = *(longlong *)(lVar7 + 0x10);
                          if (lVar7 == 0) goto DAT_1812b67e5;
                          if (*(uint *)(lVar7 + 0x18) <= uVar15) goto code_r0x0001812b67eb;
                          plVar2 = *(longlong **)(lVar7 + 0x20 + (longlong)(int)uVar15 * 8);
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
                          if (plVar2 != (longlong *)0x0) {
                            if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                              FUN_180359940();
                            }
                            if (plVar2[2] != 0) {
                              fVar16 = *(float *)(plVar2 + 7) + *(float *)((longlong)plVar2 + 0x34);
                              if (fVar18 <= fVar16) {
                                fVar18 = fVar16;
                              }
                              func_0x000180f0caf0(plVar2,0);
                              (**(code **)(*plVar2 + 0x188))
                                        (plVar2,1,*(undefined8 *)(*plVar2 + 400));
                            }
                          }
                          if (*(longlong *)(lVar9 + 0x10) == 0) goto DAT_1812b67e5;
                          uVar15 = uVar15 + 1;
                        } while ((int)uVar15 < *(int *)(*(longlong *)(lVar9 + 0x10) + 0x18));
                      }
                      if (*(longlong *)(lVar9 + 0x18) != 0) {
                        uVar15 = 0;
                        if (0 < *(int *)(*(longlong *)(lVar9 + 0x18) + 0x18)) {
                          do {
                            lVar7 = *(longlong *)(lVar9 + 0x18);
                            if (lVar7 == 0) goto DAT_1812b67e5;
                            if (*(uint *)(lVar7 + 0x18) <= uVar15) {
code_r0x0001812b67f1:
                              FUN_183aeda10(0);
                              pcVar3 = (code *)swi(3);
                              uVar17 = (*pcVar3)();
                              return uVar17;
                            }
                            lVar7 = *(longlong *)(lVar7 + 0x10);
                            if (lVar7 == 0) goto DAT_1812b67e5;
                            if (*(uint *)(lVar7 + 0x18) <= uVar15) {
code_r0x0001812b67eb:
                              FUN_180352460();
                              pcVar3 = (code *)swi(3);
                              uVar17 = (*pcVar3)();
                              return uVar17;
                            }
                            lVar7 = *(longlong *)(lVar7 + 0x20 + (longlong)(int)uVar15 * 8);
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
                              FUN_18035da50();
                              LOCK();
                              UNLOCK();
                              cRam0000000185a5054a = '\x01';
                            }
                            if (lVar7 != 0) {
                              if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                                FUN_180359940();
                              }
                              if (*(longlong *)(lVar7 + 0x10) != 0) {
                                if (fVar18 <= *(float *)(lVar7 + 0x20)) {
                                  fVar18 = *(float *)(lVar7 + 0x20);
                                }
                                FUN_181349510(lVar7,0);
                              }
                            }
                            if (*(longlong *)(lVar9 + 0x18) == 0) goto DAT_1812b67e5;
                            uVar15 = uVar15 + 1;
                          } while ((int)uVar15 < *(int *)(*(longlong *)(lVar9 + 0x18) + 0x18));
                        }
                        if (lVar10 != 0) {
                          cVar5 = FUN_180c75680(fVar18,0);
                          if (cVar5 == '\0') {
                            if (cRam0000000185a38c2c == '\0') {
                              FUN_18035da50(0x18582ff68);
                              LOCK();
                              UNLOCK();
                              cRam0000000185a38c2c = '\x01';
                            }
                            lVar7 = FUN_1803a51b0(uRam000000018582ff68);
                            *(float *)(lVar7 + 0x20) = fVar18;
                            *(undefined4 *)(lVar7 + 0x10) = 0;
                            *(longlong *)(lVar7 + 0x28) = lVar10;
                            if (lVar12 == 0) goto DAT_1812b67e5;
                            FUN_1844a8030(lVar12,lVar7,0);
                          }
                          else {
                            (**(code **)(lVar10 + 0x18))
                                      (*(undefined8 *)(lVar10 + 0x40),*(undefined8 *)(lVar10 + 0x28)
                                      );
                          }
                        }
                        return (ulonglong)(uint)fVar18;
                      }
                    }
DAT_1812b67e5:
                    FUN_180352450();
                    pcVar3 = (code *)swi(3);
                    uVar17 = (*pcVar3)();
                    return uVar17;
                  }
                  goto code_r0x0001812ba670;
                }
              }
              *(undefined1 *)(lVar12 + 0xd8) = 0;
              lVar12 = *(longlong *)(lVar7 + 0x28);
              if (lVar12 != 0) {
                    /* WARNING: Could not recover jumptable at 0x0001812ba605. Too many branches */
                    /* WARNING: Treating indirect jump as call */
                uVar17 = (**(code **)(lVar12 + 0x18))
                                   (*(undefined8 *)(lVar12 + 0x40),*(undefined8 *)(lVar12 + 0x28));
                return uVar17;
              }
            }
          }
        }
code_r0x0001812ba670:
        FUN_180352450();
        pcVar3 = (code *)swi(3);
        uVar17 = (*pcVar3)();
        return uVar17;
      }
    }
  }
code_r0x00018164f464:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  uVar17 = (*pcVar3)();
  return uVar17;
}

