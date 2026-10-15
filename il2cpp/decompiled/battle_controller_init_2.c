// ==== BattleController_Initialize_2 @ 0x1817dfd90 ====

void BattleController_Initialize_2(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined1 uVar5;
  undefined4 uVar6;
  int iVar7;
  longlong lVar8;
  undefined8 uVar9;
  undefined1 *puVar10;
  code *pcVar11;
  bool bVar12;
  undefined1 *puVar13;
  
  if (DAT_185a3d04f == '\0') {
    FUN_18035da50(&DAT_1857c31d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3160);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18578c2a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a3920);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185810878);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3150);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a46e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3178);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a89e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185773ad0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3170);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857a44f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_18582c748);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3190);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185790020);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3180);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c31a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c31a0);
    LOCK();
    UNLOCK();
    DAT_185a3d04f = '\x01';
  }
  if (*(longlong *)(param_1 + 0x98) == 0) {
    FUN_182b6f030(param_1,DAT_1857c31a0,DAT_1857c3180,DAT_1857c31a8,0xffffffff,DAT_18582c748);
  }
  if ((*(longlong *)(param_1 + 0x98) != 0) &&
     (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x1c0), lVar2 != 0)) {
    uVar5 = PlayerDomainController_IsPlayerInOnboarding(lVar2,0);
    *(undefined1 *)(lVar2 + 0x161) = uVar5;
    if (*(longlong *)(param_1 + 0x98) != 0) {
      lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x1a8);
      lVar8 = FUN_1803a51b0(DAT_1857c31d0);
      lVar4 = DAT_1857c3160;
      puVar10 = *(undefined1 **)(DAT_1857c3160 + 8);
      *(undefined1 **)(lVar8 + 0x10) = puVar10;
      *(longlong *)(lVar8 + 0x28) = lVar4;
      *(longlong *)(lVar8 + 0x20) = param_1;
      cVar1 = *(char *)(lVar4 + 0x52);
      *(longlong *)(lVar8 + 0x40) = lVar8;
      if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) || (cVar1 != '\x02')) {
        *(longlong *)(lVar8 + 0x40) = param_1;
      }
      else {
        puVar10 = &LAB_180007b30;
      }
      *(undefined1 **)(lVar8 + 0x18) = puVar10;
      *(code **)(lVar8 + 0x38) = FUN_180007ae0;
      if (lVar2 != 0) {
        FUN_1800326d0(9,puVar10,lVar2,lVar8);
        if (*(longlong *)(param_1 + 0x98) != 0) {
          lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x1a8);
          lVar8 = FUN_1803a51b0(DAT_1857c31d0);
          lVar4 = DAT_1857c3160;
          puVar10 = *(undefined1 **)(DAT_1857c3160 + 8);
          *(undefined1 **)(lVar8 + 0x10) = puVar10;
          *(longlong *)(lVar8 + 0x28) = lVar4;
          *(longlong *)(lVar8 + 0x20) = param_1;
          cVar1 = *(char *)(lVar4 + 0x52);
          *(longlong *)(lVar8 + 0x40) = lVar8;
          if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) || (puVar13 = &LAB_180007b30, cVar1 != '\x02')
             ) {
            *(longlong *)(lVar8 + 0x40) = param_1;
            puVar13 = puVar10;
          }
          *(undefined1 **)(lVar8 + 0x18) = puVar13;
          *(code **)(lVar8 + 0x38) = FUN_180007ae0;
          if (lVar2 != 0) {
            FUN_1800326d0(8,puVar10,lVar2,lVar8);
            *(undefined2 *)(param_1 + 0x78) = 0x100;
            if ((*(longlong *)(param_1 + 0x10) != 0) &&
               (plVar3 = *(longlong **)(*(longlong *)(param_1 + 0x10) + 0x98),
               plVar3 != (longlong *)0x0)) {
              lVar2 = *(longlong *)(param_1 + 0x18);
              uVar6 = (**(code **)(*plVar3 + 0x228))(plVar3,*(undefined8 *)(*plVar3 + 0x230));
              if (lVar2 != 0) {
                UISystem_SetBackdropType(lVar2,uVar6,0,0);
                if (*(longlong *)(param_1 + 0x38) != 0) {
                  uVar6 = *(undefined4 *)(*(longlong *)(param_1 + 0x38) + 0x1c8);
                  uVar9 = FUN_1803a51b0(DAT_185790020);
                  FUN_182e294b0(uVar9,uVar6,DAT_1857c3190);
                  *(undefined8 *)(param_1 + 200) = uVar9;
                  if ((*(longlong *)(param_1 + 0x98) != 0) &&
                     (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x270), lVar2 != 0)) {
                    FUN_181137b00(lVar2,*(byte *)(lVar2 + 0xa9) | 8,0);
                    if ((*(longlong *)(param_1 + 0x98) != 0) &&
                       ((lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 600), lVar2 != 0 &&
                        (lVar2 = *(longlong *)(lVar2 + 0x28), lVar2 != 0)))) {
                      FUN_18000a350(5,DAT_185773ad0,lVar2,1);
                      if ((*(longlong *)(param_1 + 0x98) != 0) &&
                         (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x300), lVar2 != 0))
                      {
                        *(undefined1 *)(lVar2 + 0x119) = 1;
                        if (*(longlong *)(param_1 + 0x98) != 0) {
                          *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x10) = param_1;
                          if ((*(longlong *)(param_1 + 0x98) != 0) &&
                             (*(longlong *)(*(longlong *)(param_1 + 0x98) + 0x2c8) != 0)) {
                            FUN_180005650(5,DAT_1857a89e0);
                            if ((*(longlong *)(param_1 + 0x98) != 0) &&
                               (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x58),
                               lVar2 != 0)) {
                              BattleRecordController_Initialize
                                        (lVar2,*(undefined8 *)(param_1 + 0x98),0);
                              if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                 (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x90),
                                 lVar2 != 0)) {
                                *(undefined8 *)(lVar2 + 0x38) = *(undefined8 *)(param_1 + 0x98);
                                lVar2 = *(longlong *)(param_1 + 0x98);
                                if ((lVar2 != 0) && (*(longlong *)(lVar2 + 0xb0) != 0)) {
                                  FUN_18000a790(0,DAT_18578c2a8,*(longlong *)(lVar2 + 0xb0),lVar2);
                                  if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                     (*(longlong *)(*(longlong *)(param_1 + 0x98) + 0xe8) != 0)) {
                                    bVar12 = true;
                                    FUN_180005650(1,DAT_1857a44f8);
                                    if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                       (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x98) + 0x120)
                                       , lVar2 != 0)) {
                                      FUN_18000a790(0,DAT_185810878,lVar2,
                                                    *(undefined8 *)(param_1 + 0x98));
                                      if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                         (*(longlong *)(*(longlong *)(param_1 + 0x98) + 0x128) != 0)
                                         ) {
                                        FUN_180005650(0,DAT_1857c3150);
                                        if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                           (lVar2 = *(longlong *)
                                                     (*(longlong *)(param_1 + 0x98) + 0x2b8),
                                           lVar2 != 0)) {
                                          FUN_18000a790(0,DAT_1857a46e8,lVar2,
                                                        *(undefined8 *)(param_1 + 0x98));
                                          if (DAT_185a3d050 == '\0') {
                                            FUN_18035da50(&DAT_1857ff438);
                                            LOCK();
                                            UNLOCK();
                                            FUN_18035da50(&DAT_1857c3130);
                                            LOCK();
                                            UNLOCK();
                                            FUN_18035da50(&DAT_185820f00);
                                            LOCK();
                                            UNLOCK();
                                            FUN_18035da50(&DAT_185821028);
                                            LOCK();
                                            UNLOCK();
                                            DAT_185a3d050 = '\x01';
                                          }
                                          uVar9 = FUN_1803a51b0(DAT_185821028);
                                          FUN_182d09b30(uVar9,DAT_185820f00);
                                          if (*(longlong *)(param_1 + 0x98) != 0) {
                                            lVar2 = *(longlong *)
                                                     (*(longlong *)(param_1 + 0x98) + 0x208);
                                            lVar8 = FUN_1803a51b0(DAT_1857ff438);
                                            lVar4 = DAT_1857c3130;
                                            pcVar11 = *(code **)(DAT_1857c3130 + 8);
                                            *(code **)(lVar8 + 0x10) = pcVar11;
                                            *(longlong *)(lVar8 + 0x28) = lVar4;
                                            *(longlong *)(lVar8 + 0x20) = param_1;
                                            cVar1 = *(char *)(lVar4 + 0x52);
                                            *(longlong *)(lVar8 + 0x40) = lVar8;
                                            if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) ||
                                               (cVar1 != '\0')) {
                                              *(longlong *)(lVar8 + 0x40) = param_1;
                                            }
                                            else {
                                              pcVar11 = FUN_180006440;
                                            }
                                            *(code **)(lVar8 + 0x18) = pcVar11;
                                            *(code **)(lVar8 + 0x38) = FUN_1800063e0;
                                            if (lVar2 != 0) {
                                              ProgressiveDownloadController_BlockingDownload_3
                                                        (lVar2,uVar9,lVar8,1,0);
                                              if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                                 (lVar2 = *(longlong *)
                                                           (*(longlong *)(param_1 + 0x98) + 0x2d8),
                                                 lVar2 != 0)) {
                                                FUN_18000a790(0,DAT_1857c3178,lVar2,
                                                              *(undefined8 *)(param_1 + 0x98));
                                                if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                                   (lVar2 = *(longlong *)
                                                             (*(longlong *)(param_1 + 0x98) + 0x2e0)
                                                   , lVar2 != 0)) {
                                                  FUN_18000a790(1,DAT_1857c3170,lVar2,
                                                                *(undefined8 *)(param_1 + 0x98));
                                                  if (*(char *)(param_1 + 0x121) == '\x03') {
                                                    if ((*(longlong *)(param_1 + 0x98) == 0) ||
                                                       (*(longlong *)
                                                         (*(longlong *)(param_1 + 0x98) + 0xa8) == 0
                                                       )) goto LAB_1817e04fb;
                                                    iVar7 = FUN_1800084f0(8,DAT_1857a3910);
                                                    bVar12 = iVar7 < 2;
                                                  }
                                                  if ((*(longlong *)(param_1 + 0x98) != 0) &&
                                                     (lVar2 = *(longlong *)
                                                               (*(longlong *)(param_1 + 0x98) +
                                                               0x2b0), lVar2 != 0)) {
                                                    CameraTransitionSystem_EnableSystem
                                                              (lVar2,bVar12,0);
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
  }
LAB_1817e04fb:
                    /* WARNING: Subroutine does not return */
  FUN_180352450();
}

