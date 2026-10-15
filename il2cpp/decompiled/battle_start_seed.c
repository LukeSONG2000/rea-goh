// ==== BattleFsmController._SendBattleStartRequest_b__0 @ 0x181a5efb0 ====

void __c__DisplayClass87_0__SendBattleStartRequest_b__0(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 *puVar6;
  ushort uVar7;
  
  if (DAT_185a3e033 == '\0') {
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18578c2a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185810e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185810e50);
    LOCK();
    UNLOCK();
    DAT_185a3e033 = '\x01';
  }
  if (((*(longlong *)(param_1 + 0x10) != 0) &&
      (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0)) &&
     (*(longlong *)(lVar4 + 0x1a8) != 0)) {
    lVar4 = *(longlong *)(lVar4 + 0xa8);
    lVar3 = FUN_180014430();
    if ((lVar3 != 0) && (lVar4 != 0)) {
      FUN_18000a790(7,DAT_1857a3910,lVar4,*(undefined8 *)(lVar3 + 0x10));
      if ((*(longlong *)(param_1 + 0x10) != 0) &&
         ((lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0 &&
          (*(longlong *)(lVar4 + 0xb0) != 0)))) {
        FUN_180005650(0x17,DAT_18578c2a8);
        if ((*(longlong *)(param_1 + 0x10) != 0) &&
           ((lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0 &&
            (*(longlong *)(lVar4 + 0x10) != 0)))) {
          FUN_180005650(0x2f,DAT_1857a3928);
          if ((*(longlong *)(param_1 + 0x10) != 0) &&
             ((lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0 &&
              (*(longlong *)(lVar4 + 0x1a8) != 0)))) {
            lVar4 = FUN_180014430();
            if ((*(longlong *)(param_1 + 0x10) != 0) &&
               ((lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar3 != 0 &&
                (*(longlong *)(lVar3 + 0x10) != 0)))) {
              uVar5 = FUN_1800084f0(0xe,DAT_1857a3928);
              if (lVar4 != 0) {
                *(undefined8 *)(lVar4 + 0x48) = uVar5;
                if (((*(longlong *)(param_1 + 0x10) != 0) &&
                    (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0)) &&
                   (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                  lVar4 = FUN_180014430();
                  if (((*(longlong *)(param_1 + 0x10) != 0) &&
                      (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar3 != 0)) &&
                     (*(longlong *)(lVar3 + 0x10) != 0)) {
                    uVar5 = FUN_1800084f0(0xd,DAT_1857a3928);
                    if (lVar4 != 0) {
                      *(undefined8 *)(lVar4 + 0x50) = uVar5;
                      if (((*(longlong *)(param_1 + 0x10) != 0) &&
                          (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78), lVar4 != 0))
                         && (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                        plVar2 = *(longlong **)(lVar4 + 0x10);
                        lVar4 = FUN_180014430();
                        if (lVar4 != 0) {
                          uVar5 = BattleDataDO_GetSquad(lVar4,*(undefined8 *)(lVar4 + 0x10),0);
                          if (plVar2 != (longlong *)0x0) {
                            lVar4 = *plVar2;
                            if (*(ushort *)(lVar4 + 0x12e) != 0) {
                              uVar7 = 0;
                              do {
                                if (*(longlong *)
                                     (*(longlong *)(lVar4 + 0xb0) + (ulonglong)uVar7 * 0x10) ==
                                    DAT_1857a3928) {
                                  puVar6 = (undefined8 *)
                                           ((longlong)
                                            (*(int *)(*(longlong *)(lVar4 + 0xb0) + 8 +
                                                     (ulonglong)uVar7 * 0x10) + 0x11) * 0x10 + 0x138
                                           + lVar4);
                                  goto LAB_181a5f2a9;
                                }
                                uVar7 = uVar7 + 1;
                              } while (uVar7 < *(ushort *)(lVar4 + 0x12e));
                            }
                            puVar6 = (undefined8 *)FUN_18034f630(plVar2,DAT_1857a3928,0x11);
LAB_181a5f2a9:
                            (*(code *)*puVar6)(plVar2,uVar5,puVar6[1]);
                            if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78),
                                lVar4 != 0)) && (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                              lVar3 = FUN_180014430();
                              if (lVar3 != 0) {
                                iVar1 = *(int *)(lVar3 + 0x18);
                                lVar3 = FUN_1803a51b0(DAT_185810e58);
                                *(ulonglong *)(lVar3 + 0x10) =
                                     ((longlong)iVar1 ^ 0x5deece66dU) & 0xffffffffffff;
                                *(longlong *)(lVar4 + 0x158) = lVar3;
                                if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                    (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78),
                                    lVar4 != 0)) && (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                                  lVar3 = FUN_180014430();
                                  if (lVar3 != 0) {
                                    iVar1 = *(int *)(lVar3 + 0x18);
                                    lVar3 = FUN_1803a51b0(DAT_185810e58);
                                    *(ulonglong *)(lVar3 + 0x10) =
                                         ((longlong)iVar1 ^ 0x5deece66dU) & 0xffffffffffff;
                                    *(longlong *)(lVar4 + 0x160) = lVar3;
                                    if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                        (lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x78)
                                        , lVar4 != 0)) && (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                                      lVar3 = FUN_180014430();
                                      if (lVar3 != 0) {
                                        iVar1 = *(int *)(lVar3 + 0x18);
                                        lVar3 = FUN_1803a51b0(DAT_185810e58);
                                        *(ulonglong *)(lVar3 + 0x10) =
                                             ((longlong)iVar1 ^ 0x5deece66dU) & 0xffffffffffff;
                                        *(longlong *)(lVar4 + 0x168) = lVar3;
                                        if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                            (lVar4 = *(longlong *)
                                                      (*(longlong *)(param_1 + 0x10) + 0x78),
                                            lVar4 != 0)) && (*(longlong *)(lVar4 + 0x1a8) != 0)) {
                                          lVar3 = FUN_180014430();
                                          if (lVar3 != 0) {
                                            iVar1 = *(int *)(lVar3 + 0x18);
                                            lVar3 = FUN_1803a51b0(DAT_185810e58);
                                            *(ulonglong *)(lVar3 + 0x10) =
                                                 ((longlong)iVar1 ^ 0x5deece66dU) & 0xffffffffffff;
                                            *(longlong *)(lVar4 + 0x170) = lVar3;
                                            if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                                (lVar4 = *(longlong *)
                                                          (*(longlong *)(param_1 + 0x10) + 0x78),
                                                lVar4 != 0)) && (*(longlong *)(lVar4 + 0x1a8) != 0))
                                            {
                                              lVar3 = FUN_180014430();
                                              if (lVar3 != 0) {
                                                iVar1 = *(int *)(lVar3 + 0x18);
                                                lVar3 = FUN_1803a51b0(DAT_185810e58);
                                                *(ulonglong *)(lVar3 + 0x10) =
                                                     ((longlong)iVar1 ^ 0x5deece66dU) &
                                                     0xffffffffffff;
                                                *(longlong *)(lVar4 + 0x178) = lVar3;
                                                if (((*(longlong *)(param_1 + 0x10) != 0) &&
                                                    (lVar4 = *(longlong *)
                                                              (*(longlong *)(param_1 + 0x10) + 0x78)
                                                    , lVar4 != 0)) &&
                                                   (lVar4 = *(longlong *)(lVar4 + 0x1c0), lVar4 != 0
                                                   )) {
                                                  if (*(longlong *)(lVar4 + 0x120) != 0) {
                                                    if (*(longlong *)(lVar4 + 0x50) == 0)
                                                    goto LAB_181a5f566;
                                                    if (*(int *)(*(longlong *)(lVar4 + 0x120) + 0xf0
                                                                ) <
                                                        *(int *)(*(longlong *)(lVar4 + 0x50) + 0xd1c
                                                                )) {
                                                      lVar4 = *(longlong *)
                                                               (*(longlong *)(param_1 + 0x10) + 0x10
                                                               );
                                                      if (lVar4 == 0) goto LAB_181a5f566;
                                                                                                            
                                                  OnboardingTelemetryController_SendOnboardingTelemetry
                                                            (lVar4,DAT_185810e50,0);
                                                  }
                                                  }
                                                  lVar4 = *(longlong *)(param_1 + 0x18);
                                                  if (lVar4 != 0) {
                                                    (**(code **)(lVar4 + 0x18))
                                                              (*(undefined8 *)(lVar4 + 0x40),
                                                               *(undefined8 *)(lVar4 + 0x28));
                                                  }
                                                  return;
                                                }
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_181a5f566:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

