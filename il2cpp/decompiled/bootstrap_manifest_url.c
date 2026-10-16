// ==== WebBundleAssetManifestLoader.LoadManifest @ 0x18113dca0 ====


void FUN_18113dca0(longlong param_1,undefined4 param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  code *pcVar2;
  longlong lVar3;
  undefined8 uVar4;
  
  if (cRam0000000185a3a702 == '\0') {
    FUN_18035da50(0x18582cc58);
    LOCK();
    UNLOCK();
    cRam0000000185a3a702 = '\x01';
  }
  *(undefined8 *)(param_1 + 0x38) = param_3;
  if (*(longlong *)(param_1 + 0x10) != 0) {
    if (*(char *)(*(longlong *)(param_1 + 0x10) + 0x46c) != '\0') {
      lVar3 = FUN_18113e530(param_1,param_2,0);
      *(longlong *)(param_1 + 0x30) = lVar3;
      if (lVar3 != 0) {
        if (cRam0000000185a3a705 == '\0') {
          FUN_18035da50(0x185830b30);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x1857668a8);
          LOCK();
          UNLOCK();
          cRam0000000185a3a705 = '\x01';
        }
        if (cRam0000000185a38a08 == '\0') {
          FUN_18035da50(0x18582c860);
          LOCK();
          UNLOCK();
          cRam0000000185a38a08 = '\x01';
        }
        if (*(longlong *)(lVar3 + 0x40) != 0) {
          FUN_182b6ab00(param_1,uRam00000001857668a8,
                        *(undefined4 *)(*(longlong *)(lVar3 + 0x40) + 0x18),0xffffffff,
                        uRam0000000185830b30);
          *(longlong *)(param_1 + 0x30) = lVar3;
          lVar1 = *(longlong *)(param_1 + 0x38);
          if (lVar1 == 0) {
            return;
          }
          (**(code **)(lVar1 + 0x18))
                    (*(undefined8 *)(lVar1 + 0x40),lVar3,1,0,*(undefined8 *)(lVar1 + 0x28));
          return;
        }
        goto code_r0x00018113dec5;
      }
    }
    if (*(int *)(lRam000000018582cc58 + 0xe4) == 0) {
      FUN_180359940(lRam000000018582cc58);
    }
    if (cRam0000000185a38a02 == '\0') {
      FUN_18035da50(0x18582cc58);
      LOCK();
      UNLOCK();
      cRam0000000185a38a02 = '\x01';
    }
    if (*(int *)(lRam000000018582cc58 + 0xe4) == 0) {
      FUN_180359940(lRam000000018582cc58);
    }
    lVar3 = *(longlong *)(*(longlong *)(lRam000000018582cc58 + 0xb8) + 0x100);
    if ((lVar3 == 0) || (*(int *)(lVar3 + 0x10) == 0)) {
      uVar4 = **(undefined8 **)(lRam0000000185a33e80 + 0xb8);
    }
    else {
      if (*(int *)(lRam000000018582cc58 + 0xe4) == 0) {
        FUN_180359940(lRam000000018582cc58);
      }
      uVar4 = FUN_180ca8670(*(undefined8 *)(*(longlong *)(lRam000000018582cc58 + 0xb8) + 0x100),
                            *(undefined8 *)(*(longlong *)(lRam000000018582cc58 + 0xb8) + 0x90),0,0,0
                            ,0);
    }
    FUN_18113dee0(param_1,uVar4,param_2,0);
    if (param_4 == 0) {
      return;
    }
                    /* WARNING: Could not recover jumptable at 0x00018113dec2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_4 + 0x18))(*(undefined8 *)(param_4 + 0x40),*(undefined8 *)(param_4 + 0x28));
    return;
  }
code_r0x00018113dec5:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}




// ==== Client.SetContentNamespace @ 0x180c775a0 ====


void Client_SetContentNamespace(longlong param_1,undefined8 param_2)

{
  *(undefined8 *)(param_1 + 0x40) = param_2;
  return;
}




// ==== Client.GetContentNamespace @ 0x180c44d80 ====


undefined8 Client_GetContentNamespace(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x40);
}




// ==== Client.GetAssetS3Bucket @ 0x180c056c0 ====


undefined8 Client_GetAssetS3Bucket(longlong param_1)

{
  return *(undefined8 *)(param_1 + 0x18);
}




// ==== Client.ToString @ 0x180c84580 ====


longlong FUN_180c84580(longlong param_1)

{
  longlong lVar1;
  int iVar2;
  longlong *plVar3;
  longlong lVar4;
  code *pcVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 uVar9;
  uint uVar10;
  int iVar11;
  ulonglong uVar12;
  int iVar13;
  undefined8 uStack_28;
  undefined8 uStack_20;
  
  if (cRam0000000185a38a34 == '\0') {
    FUN_18035da50(0x18582c448);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c440);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c438);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582ca30);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c430);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d7a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582ca20);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c428);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c418);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c410);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c408);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c400);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c3a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c398);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c390);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c388);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c380);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c378);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c370);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c368);
    LOCK();
    UNLOCK();
    cRam0000000185a38a34 = '\x01';
  }
  lVar6 = FUN_1803a3dc0(uRam000000018580d7a0,0x32);
  if (lVar6 == 0) {
    FUN_180352450();
    pcVar5 = (code *)swi(3);
    lVar6 = (*pcVar5)();
    return lVar6;
  }
  if (*(int *)(lVar6 + 0x18) != 0) {
    *(undefined8 *)(lVar6 + 0x20) = uRam000000018582c418;
    uStack_28 = uRam000000018582ca30;
    uStack_20 = 0xffffffffffffffff;
    uVar7 = FUN_183b04fe0(&uStack_28,0);
    if (((((1 < *(uint *)(lVar6 + 0x18)) &&
          (*(undefined8 *)(lVar6 + 0x28) = uVar7, 2 < *(uint *)(lVar6 + 0x18))) &&
         (*(undefined8 *)(lVar6 + 0x30) = uRam000000018582c410, 3 < *(uint *)(lVar6 + 0x18))) &&
        ((*(undefined8 *)(lVar6 + 0x38) = *(undefined8 *)(param_1 + 0x48),
         4 < *(uint *)(lVar6 + 0x18) &&
         (*(undefined8 *)(lVar6 + 0x40) = uRam000000018582c390, 5 < *(uint *)(lVar6 + 0x18))))) &&
       ((*(undefined8 *)(lVar6 + 0x48) = *(undefined8 *)(param_1 + 0x50),
        6 < *(uint *)(lVar6 + 0x18) &&
        ((*(undefined8 *)(lVar6 + 0x50) = uRam000000018582c3f0, 7 < *(uint *)(lVar6 + 0x18) &&
         (*(undefined8 *)(lVar6 + 0x58) = *(undefined8 *)(param_1 + 0x28),
         8 < *(uint *)(lVar6 + 0x18))))))) {
      *(undefined8 *)(lVar6 + 0x60) = uRam000000018582c378;
      uStack_28 = uRam000000018582c430;
      uStack_20 = 0xffffffffffffffff;
      uVar7 = FUN_183b04fe0(&uStack_28,0);
      if ((9 < *(uint *)(lVar6 + 0x18)) &&
         (*(undefined8 *)(lVar6 + 0x68) = uVar7, 10 < *(uint *)(lVar6 + 0x18))) {
        *(undefined8 *)(lVar6 + 0x70) = uRam000000018582c370;
        uStack_28 = uRam000000018582c440;
        uStack_20 = 0xffffffffffffffff;
        uVar7 = FUN_183b04fe0(&uStack_28,0);
        if ((0xb < *(uint *)(lVar6 + 0x18)) &&
           (*(undefined8 *)(lVar6 + 0x78) = uVar7, 0xc < *(uint *)(lVar6 + 0x18))) {
          *(undefined8 *)(lVar6 + 0x80) = uRam000000018582c3f8;
          uStack_28 = uRam000000018582c448;
          uStack_20 = 0xffffffffffffffff;
          uVar7 = FUN_183b04fe0(&uStack_28,0);
          if ((0xd < *(uint *)(lVar6 + 0x18)) &&
             (*(undefined8 *)(lVar6 + 0x88) = uVar7, 0xe < *(uint *)(lVar6 + 0x18))) {
            *(undefined8 *)(lVar6 + 0x90) = uRam000000018582c3c8;
            uStack_28 = uRam000000018582ca20;
            uStack_20 = 0xffffffffffffffff;
            uVar7 = FUN_183b04fe0(&uStack_28,0);
            if ((0xf < *(uint *)(lVar6 + 0x18)) &&
               (*(undefined8 *)(lVar6 + 0x98) = uVar7, 0x10 < *(uint *)(lVar6 + 0x18))) {
              *(undefined8 *)(lVar6 + 0xa0) = uRam000000018582c3d8;
              uStack_28 = uRam000000018582c438;
              uStack_20 = 0xffffffffffffffff;
              uVar7 = FUN_183b04fe0(&uStack_28,0);
              if ((0x11 < *(uint *)(lVar6 + 0x18)) &&
                 ((((((*(undefined8 *)(lVar6 + 0xa8) = uVar7, 0x12 < *(uint *)(lVar6 + 0x18) &&
                      (*(undefined8 *)(lVar6 + 0xb0) = uRam000000018582c3b8,
                      0x13 < *(uint *)(lVar6 + 0x18))) &&
                     (*(undefined8 *)(lVar6 + 0xb8) = *(undefined8 *)(param_1 + 0x70),
                     0x14 < *(uint *)(lVar6 + 0x18))) &&
                    (((*(undefined8 *)(lVar6 + 0xc0) = uRam000000018582c3d0,
                      0x15 < *(uint *)(lVar6 + 0x18) &&
                      (*(undefined8 *)(lVar6 + 200) = *(undefined8 *)(param_1 + 0x78),
                      0x16 < *(uint *)(lVar6 + 0x18))) &&
                     ((*(undefined8 *)(lVar6 + 0xd0) = uRam000000018582c3e8,
                      0x17 < *(uint *)(lVar6 + 0x18) &&
                      ((*(undefined8 *)(lVar6 + 0xd8) = *(undefined8 *)(param_1 + 0x88),
                       0x18 < *(uint *)(lVar6 + 0x18) &&
                       (*(undefined8 *)(lVar6 + 0xe0) = uRam000000018582c408,
                       0x19 < *(uint *)(lVar6 + 0x18))))))))) &&
                   (*(undefined8 *)(lVar6 + 0xe8) = *(undefined8 *)(param_1 + 0x90),
                   0x1a < *(uint *)(lVar6 + 0x18))) &&
                  ((*(undefined8 *)(lVar6 + 0xf0) = uRam000000018582c400,
                   0x1b < *(uint *)(lVar6 + 0x18) &&
                   (*(undefined8 *)(lVar6 + 0xf8) = *(undefined8 *)(param_1 + 0x98),
                   0x1c < *(uint *)(lVar6 + 0x18))))))) {
                *(undefined8 *)(lVar6 + 0x100) = uRam000000018582c3a0;
                uVar7 = FUN_183ac4bd0(param_1 + 0x80,0);
                if (((0x1d < *(uint *)(lVar6 + 0x18)) &&
                    (((*(undefined8 *)(lVar6 + 0x108) = uVar7, 0x1e < *(uint *)(lVar6 + 0x18) &&
                      (*(undefined8 *)(lVar6 + 0x110) = uRam000000018582c428,
                      0x1f < *(uint *)(lVar6 + 0x18))) &&
                     (*(undefined8 *)(lVar6 + 0x118) = *(undefined8 *)(param_1 + 0xb0),
                     0x20 < *(uint *)(lVar6 + 0x18))))) &&
                   ((*(undefined8 *)(lVar6 + 0x120) = uRam000000018582c398,
                    0x21 < *(uint *)(lVar6 + 0x18) &&
                    (*(undefined8 *)(lVar6 + 0x128) = *(undefined8 *)(param_1 + 0xb8),
                    0x22 < *(uint *)(lVar6 + 0x18))))) {
                  *(undefined8 *)(lVar6 + 0x130) = uRam000000018582c368;
                  uVar7 = FUN_183ac4bd0(param_1 + 0xc0,0);
                  if (((0x23 < *(uint *)(lVar6 + 0x18)) &&
                      (((*(undefined8 *)(lVar6 + 0x138) = uVar7, 0x24 < *(uint *)(lVar6 + 0x18) &&
                        (*(undefined8 *)(lVar6 + 0x140) = uRam000000018582c420,
                        0x25 < *(uint *)(lVar6 + 0x18))) &&
                       ((*(undefined8 *)(lVar6 + 0x148) = *(undefined8 *)(param_1 + 200),
                        0x26 < *(uint *)(lVar6 + 0x18) &&
                        ((((*(undefined8 *)(lVar6 + 0x150) = uRam000000018582c380,
                           0x27 < *(uint *)(lVar6 + 0x18) &&
                           (*(undefined8 *)(lVar6 + 0x158) = *(undefined8 *)(param_1 + 0xd0),
                           0x28 < *(uint *)(lVar6 + 0x18))) &&
                          (*(undefined8 *)(lVar6 + 0x160) = uRam000000018582c3b0,
                          0x29 < *(uint *)(lVar6 + 0x18))) &&
                         ((*(undefined8 *)(lVar6 + 0x168) = *(undefined8 *)(param_1 + 0xd8),
                          0x2a < *(uint *)(lVar6 + 0x18) &&
                          (*(undefined8 *)(lVar6 + 0x170) = uRam000000018582c3a8,
                          0x2b < *(uint *)(lVar6 + 0x18))))))))))) &&
                     (*(undefined8 *)(lVar6 + 0x178) = *(undefined8 *)(param_1 + 0x40),
                     0x2c < *(uint *)(lVar6 + 0x18))) {
                    *(undefined8 *)(lVar6 + 0x180) = uRam000000018582c388;
                    plVar3 = *(longlong **)(param_1 + 0x138);
                    if (plVar3 == (longlong *)0x0) {
                      uVar7 = 0;
                    }
                    else {
                      uVar7 = (**(code **)(*plVar3 + 0x168))
                                        (plVar3,*(undefined8 *)(*plVar3 + 0x170));
                    }
                    if (((0x2d < *(uint *)(lVar6 + 0x18)) &&
                        (*(undefined8 *)(lVar6 + 0x188) = uVar7, 0x2e < *(uint *)(lVar6 + 0x18))) &&
                       ((*(undefined8 *)(lVar6 + 400) = uRam000000018582c3e0,
                        0x2f < *(uint *)(lVar6 + 0x18) &&
                        ((*(undefined8 *)(lVar6 + 0x198) = *(undefined8 *)(param_1 + 0x140),
                         0x30 < *(uint *)(lVar6 + 0x18) &&
                         (*(undefined8 *)(lVar6 + 0x1a0) = uRam000000018582c3c0,
                         0x31 < *(uint *)(lVar6 + 0x18))))))) {
                      *(undefined8 *)(lVar6 + 0x1a8) = *(undefined8 *)(param_1 + 0x148);
                      if (cRam0000000185a48699 == '\0') {
                        FUN_18035da50(0x18580d7a0,0);
                        LOCK();
                        UNLOCK();
                        cRam0000000185a48699 = '\x01';
                      }
                      if (lVar6 == 0) {
                        uVar7 = func_0x000180352160(0x185821320);
                        uVar7 = func_0x000180352400(uVar7);
                        uVar9 = func_0x000180352160(0x1857fa4a8);
                        FUN_183a0a050(uVar7,uVar9,0);
                        uVar9 = func_0x000180352160(0x1857d4ed8);
                        FUN_180352410(uVar7,uVar9);
                        pcVar5 = (code *)swi(3);
                        lVar6 = (*pcVar5)();
                        return lVar6;
                      }
                      if (*(int *)(lVar6 + 0x18) < 2) {
                        if (*(longlong *)(lVar6 + 0x18) == 0) {
                          return **(longlong **)(lRam0000000185a33e80 + 0xb8);
                        }
                        if (*(int *)(lVar6 + 0x18) == 0) {
code_r0x0001838da9fe:
                          FUN_180352460();
                          pcVar5 = (code *)swi(3);
                          lVar6 = (*pcVar5)();
                          return lVar6;
                        }
                        if (*(longlong *)(lVar6 + 0x20) != 0) {
                          return *(longlong *)(lVar6 + 0x20);
                        }
                      }
                      else {
                        uVar12 = 0;
                        for (uVar10 = 0; (int)uVar10 < (int)*(uint *)(lVar6 + 0x18);
                            uVar10 = uVar10 + 1) {
                          if (*(uint *)(lVar6 + 0x18) <= uVar10) goto code_r0x0001838da9fe;
                          lVar8 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar10 * 8);
                          if (lVar8 != 0) {
                            uVar12 = uVar12 + (longlong)*(int *)(lVar8 + 0x10);
                          }
                        }
                        if (0x7fffffff < (longlong)uVar12) {
                          uVar7 = func_0x000180352160(0x1857b3860);
                          uVar7 = func_0x000180352400(uVar7);
                          FUN_183aef8d0(uVar7,0);
                          uVar9 = func_0x000180352160(0x1857d4ed8);
                          FUN_180352410(uVar7,uVar9);
                          pcVar5 = (code *)swi(3);
                          lVar6 = (*pcVar5)();
                          return lVar6;
                        }
                        iVar11 = (int)uVar12;
                        if (iVar11 != 0) {
                          lVar8 = FUN_18034fbe0(uVar12 & 0xffffffff);
                          iVar13 = 0;
                          uVar10 = 0;
code_r0x0001838da7e0:
                          do {
                            if (*(int *)(lVar6 + 0x18) <= (int)uVar10) {
code_r0x0001838da881:
                              if (iVar13 == iVar11) {
                                return lVar8;
                              }
                              lVar6 = FUN_1803a4f40(lVar6);
                              uVar7 = uRam000000018580d7a0;
                              if (lVar6 == 0) {
                                lVar6 = FUN_1838da730(0,0);
                                return lVar6;
                              }
                              lVar8 = FUN_1803a50f0(lVar6,uRam000000018580d7a0);
                              if (lVar8 != 0) {
                                lVar6 = FUN_1838da730(lVar8,0);
                                return lVar6;
                              }
                              FUN_180352e70(lVar6,uVar7);
                              pcVar5 = (code *)swi(3);
                              lVar6 = (*pcVar5)();
                              return lVar6;
                            }
                            if (*(uint *)(lVar6 + 0x18) <= uVar10) goto code_r0x0001838da9fe;
                            lVar4 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar10 * 8);
                            if ((lVar4 != 0) && (*(int *)(lVar4 + 0x10) != 0)) {
                              iVar2 = *(int *)(lVar4 + 0x10);
                              if (iVar11 - iVar13 < iVar2) {
                                iVar13 = -1;
                                goto code_r0x0001838da881;
                              }
                              if (lVar8 == 0) {
                                FUN_180352450();
                                pcVar5 = (code *)swi(3);
                                lVar6 = (*pcVar5)();
                                return lVar6;
                              }
                              if (*(int *)(lVar8 + 0x10) - iVar13 < iVar2) {
                                uVar7 = func_0x000180352160(0x185820cd0);
                                uVar7 = func_0x000180352400(uVar7);
                                FUN_183ac3870(uVar7,0);
                                uVar9 = func_0x000180352160(0x1857d4f10);
                                FUN_180352410(uVar7,uVar9);
                                pcVar5 = (code *)swi(3);
                                lVar6 = (*pcVar5)();
                                return lVar6;
                              }
                              lVar1 = lVar8 + (longlong)iVar13 * 2;
                              if (((ulonglong)(uint)(iVar2 * 2) <= (ulonglong)(lVar1 - lVar4)) &&
                                 ((ulonglong)(uint)(iVar2 * 2) <= (ulonglong)(lVar4 - lVar1))) {
                                FUN_183afbf00();
                                iVar13 = iVar13 + iVar2;
                                uVar10 = uVar10 + 1;
                                goto code_r0x0001838da7e0;
                              }
                              FUN_180bafe60(lVar1 + 0x14);
                              iVar13 = iVar13 + iVar2;
                            }
                            uVar10 = uVar10 + 1;
                          } while( true );
                        }
                      }
                      return **(longlong **)(lRam0000000185a33e80 + 0xb8);
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
  FUN_180352460();
  pcVar5 = (code *)swi(3);
  lVar6 = (*pcVar5)();
  return lVar6;
}




// ==== ContentAssetManager.Initialize @ 0x1812f3860 ====


void FUN_1812f3860(longlong param_1,undefined8 param_2)

{
  char cVar1;
  undefined4 uVar2;
  longlong lVar3;
  code *pcVar4;
  longlong lVar5;
  longlong lVar6;
  undefined *puVar7;
  undefined8 in_stack_ffffffffffffffe8;
  undefined4 uVar8;
  
  uVar8 = (undefined4)((ulonglong)in_stack_ffffffffffffffe8 >> 0x20);
  if (cRam0000000185a3afe6 == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830910);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582cc58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fd0);
    LOCK();
    UNLOCK();
    cRam0000000185a3afe6 = '\x01';
  }
  lVar5 = FUN_1803a51b0(uRam0000000185796fc8);
  if (lVar5 != 0) {
    *(longlong *)(lVar5 + 0x10) = param_1;
    *(undefined8 *)(lVar5 + 0x18) = param_2;
    lVar3 = *(longlong *)(param_1 + 0x20);
    if (lVar3 != 0) {
      if (cRam0000000185a3b88a == '\0') {
        FUN_18035da50(0x185823980);
        LOCK();
        UNLOCK();
        cRam0000000185a3b88a = '\x01';
      }
      lVar6 = *(longlong *)(lVar3 + 0x18);
      if (lVar6 == 0) {
        lVar6 = FUN_1803a51b0(uRam0000000185823980);
        *(longlong *)(lVar3 + 0x18) = lVar6;
        if (lVar6 == 0) goto code_r0x0001812f3a2e;
      }
      uVar2 = *(undefined4 *)(lVar6 + 0x10);
      if (*(int *)(lRam000000018582cc58 + 0xe4) == 0) {
        FUN_180359940(lRam000000018582cc58);
      }
      FUN_182b6ba40(param_1,uRam0000000185796fd0,
                    *(undefined8 *)(*(longlong *)(lRam000000018582cc58 + 0xb8) + 0x100),uVar2,
                    CONCAT44(uVar8,0xffffffff),uRam0000000185830910);
      lVar6 = FUN_1803a51b0(uRam000000018580d858);
      lVar3 = lRam0000000185796fb8;
      puVar7 = *(undefined **)(lRam0000000185796fb8 + 8);
      *(undefined **)(lVar6 + 0x10) = puVar7;
      *(longlong *)(lVar6 + 0x28) = lVar3;
      *(longlong *)(lVar6 + 0x20) = lVar5;
      cVar1 = *(char *)(lVar3 + 0x52);
      *(longlong *)(lVar6 + 0x40) = lVar6;
      if (((*(byte *)(lVar3 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
        *(longlong *)(lVar6 + 0x40) = lVar5;
      }
      else {
        puVar7 = &UNK_180006440;
      }
      *(undefined **)(lVar6 + 0x18) = puVar7;
      *(code **)(lVar6 + 0x38) = FUN_1800063e0;
      FUN_1812f3a40(param_1,CONCAT71((int7)((ulonglong)puVar7 >> 8),7),0,lVar6,0);
      return;
    }
  }
code_r0x0001812f3a2e:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}




// ==== ContentAssetManager.BuildContentAssetCatalog @ 0x1812f3a40 ====


void FUN_1812f3a40(undefined8 param_1,undefined1 param_2,undefined1 param_3,undefined8 param_4)

{
  undefined1 uVar1;
  char cVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined8 uVar6;
  code *pcVar7;
  longlong lVar8;
  undefined8 *puVar9;
  longlong lVar10;
  longlong *plVar11;
  undefined8 uVar12;
  undefined *puVar13;
  ushort uVar14;
  bool bVar15;
  undefined8 in_stack_ffffffffffffff98;
  undefined4 uVar16;
  undefined8 in_stack_ffffffffffffffa0;
  undefined4 uVar17;
  undefined8 uStack_48;
  undefined *puStack_40;
  undefined1 uStack_38;
  undefined3 uStack_37;
  
  uVar16 = (undefined4)((ulonglong)in_stack_ffffffffffffff98 >> 0x20);
  uVar17 = (undefined4)((ulonglong)in_stack_ffffffffffffffa0 >> 0x20);
  if (cRam0000000185a3afe7 == '\0') {
    FUN_18035da50(0x1857c5ec8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fb0);
    LOCK();
    UNLOCK();
    cRam0000000185a3afe7 = '\x01';
  }
  lVar5 = FUN_1803a51b0(uRam0000000185796fb0);
  if (lVar5 == 0) {
code_r0x0001812f3b92:
    FUN_180352450();
    pcVar7 = (code *)swi(3);
    (*pcVar7)();
    return;
  }
  *(undefined8 *)(lVar5 + 0x10) = param_1;
  *(undefined1 *)(lVar5 + 0x18) = param_2;
  *(undefined1 *)(lVar5 + 0x19) = param_3;
  *(undefined8 *)(lVar5 + 0x28) = param_4;
  if (*(int *)(lRam00000001857c5ec8 + 0xe4) == 0) {
    FUN_180359940();
  }
  uVar6 = FUN_181137920(0);
  bVar15 = cRam0000000185a3aff6 == '\0';
  *(undefined8 *)(lVar5 + 0x20) = uVar6;
  if (bVar15) {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796ec0);
    LOCK();
    UNLOCK();
    cRam0000000185a3aff6 = '\x01';
  }
  if ((*(byte *)(lVar5 + 0x18) & 2) == 0) {
    if (cRam0000000185a3aff7 == '\0') {
      FUN_18035da50(0x18580d858);
      LOCK();
      UNLOCK();
      FUN_18035da50(0x185796eb0);
      LOCK();
      UNLOCK();
      cRam0000000185a3aff7 = '\x01';
    }
    if ((*(byte *)(lVar5 + 0x18) & 4) == 0) {
      if (cRam0000000185a3aff5 == '\0') {
        FUN_18035da50(0x1857c5ec8);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857cc848);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185796ed0);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185796eb8);
        LOCK();
        UNLOCK();
        cRam0000000185a3aff5 = '\x01';
      }
      plVar11 = *(longlong **)(lVar5 + 0x10);
      uStack_48 = uRam00000001857cc848;
      _uStack_38 = CONCAT31(uStack_37,*(undefined1 *)(lVar5 + 0x18));
      puStack_40 = (undefined *)0xffffffffffffffff;
      uVar6 = FUN_183b04fe0(&uStack_48,0);
      uVar1 = *(undefined1 *)(lVar5 + 0x19);
      if (*(int *)(lRam00000001857c5ec8 + 0xe4) == 0) {
        FUN_180359940();
      }
      lVar10 = FUN_181137920(0);
      lVar8 = lRam0000000185796ed0;
      uVar4 = uRam0000000185796eb8;
      lVar3 = *(longlong *)(lVar5 + 0x20);
      if (*(longlong *)(lRam0000000185796ed0 + 0x38) == 0) {
        FUN_18035da50(0x18582cc68);
        LOCK();
        UNLOCK();
        if (*(longlong *)(lVar8 + 0x38) == 0) {
          FUN_18034f250(lVar8);
        }
      }
      if ((plVar11 != (longlong *)0x0) &&
         (plVar11 = (longlong *)FUN_1803a7d00(*plVar11 + 0x20), plVar11 != (longlong *)0x0)) {
        uVar12 = (**(code **)(*plVar11 + 0x168))(plVar11,*(undefined8 *)(*plVar11 + 0x170));
        if (*(int *)(lRam000000018582cc68 + 0xe4) == 0) {
          FUN_180359940();
        }
        FUN_182b5f8d0(uVar4,uVar6,uVar1,lVar10 - lVar3,CONCAT44(uVar16,0x100000),uVar12,
                      *(undefined8 *)(*(longlong *)(lVar8 + 0x38) + 0x18));
        lVar5 = *(longlong *)(lVar5 + 0x28);
        if (lVar5 != 0) {
          (**(code **)(lVar5 + 0x18))(*(undefined8 *)(lVar5 + 0x40),*(undefined8 *)(lVar5 + 0x28));
        }
        return;
      }
    }
    else {
      lVar3 = *(longlong *)(lVar5 + 0x10);
      uVar1 = *(undefined1 *)(lVar5 + 0x19);
      lVar10 = FUN_1803a51b0(uRam000000018580d858);
      lVar8 = lRam0000000185796eb0;
      puVar13 = *(undefined **)(lRam0000000185796eb0 + 8);
      *(undefined **)(lVar10 + 0x10) = puVar13;
      *(longlong *)(lVar10 + 0x28) = lVar8;
      *(longlong *)(lVar10 + 0x20) = lVar5;
      cVar2 = *(char *)(lVar8 + 0x52);
      *(longlong *)(lVar10 + 0x40) = lVar10;
      if (((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
        *(longlong *)(lVar10 + 0x40) = lVar5;
      }
      else {
        puVar13 = &UNK_180006440;
      }
      *(undefined **)(lVar10 + 0x18) = puVar13;
      *(code **)(lVar10 + 0x38) = FUN_1800063e0;
      if (lVar3 != 0) {
        if (cRam0000000185a3afe9 == '\0') {
          FUN_18035da50(0x1857696a0);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185796f88);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185796f70);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185769690);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x18580dd18);
          LOCK();
          UNLOCK();
          cRam0000000185a3afe9 = '\x01';
        }
        lVar5 = FUN_1803a51b0(uRam0000000185796f70);
        if (lVar5 != 0) {
          *(longlong *)(lVar5 + 0x10) = lVar10;
          *(undefined1 *)(lVar5 + 0x18) = uVar1;
          *(longlong *)(lVar5 + 0x20) = lVar3;
          lVar3 = *(longlong *)(lVar3 + 0x28);
          uVar6 = FUN_1803a51b0(uRam00000001857696a0);
          FUN_180c84ee0(uVar6,lVar5,uRam0000000185796f88,0);
          if (lVar3 != 0) {
            FUN_181301040(lVar3,uRam0000000185769690,uRam000000018580dd18,0,uVar6,CONCAT44(uVar17,3)
                          ,0);
            return;
          }
        }
      }
    }
    FUN_180352450();
    pcVar7 = (code *)swi(3);
    (*pcVar7)();
    return;
  }
  lVar3 = *(longlong *)(lVar5 + 0x10);
  lVar10 = FUN_1803a51b0(uRam000000018580d858);
  lVar8 = lRam0000000185796ec0;
  puVar13 = *(undefined **)(lRam0000000185796ec0 + 8);
  *(undefined **)(lVar10 + 0x10) = puVar13;
  *(longlong *)(lVar10 + 0x28) = lVar8;
  *(longlong *)(lVar10 + 0x20) = lVar5;
  cVar2 = *(char *)(lVar8 + 0x52);
  *(longlong *)(lVar10 + 0x40) = lVar10;
  if (((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
    *(longlong *)(lVar10 + 0x40) = lVar5;
  }
  else {
    puVar13 = &UNK_180006440;
  }
  *(undefined **)(lVar10 + 0x18) = puVar13;
  *(code **)(lVar10 + 0x38) = FUN_1800063e0;
  if (lVar3 == 0) goto code_r0x0001812f3b92;
  if (cRam0000000185a3afe8 == '\0') {
    puStack_40 = &UNK_1812f3bca;
    FUN_18035da50(0x18580d858,lVar10,0);
    LOCK();
    UNLOCK();
    puStack_40 = &UNK_1812f3bdb;
    FUN_18035da50(0x185796f98);
    LOCK();
    UNLOCK();
    puStack_40 = &UNK_1812f3bec;
    FUN_18035da50(0x185796fa0);
    LOCK();
    UNLOCK();
    puStack_40 = &UNK_1812f3bfd;
    FUN_18035da50(0x18582ccc8);
    LOCK();
    UNLOCK();
    puStack_40 = &UNK_1812f3c0e;
    FUN_18035da50(0x185796fa8);
    LOCK();
    UNLOCK();
    puStack_40 = &UNK_1812f3c1f;
    FUN_18035da50(0x185796f80);
    LOCK();
    UNLOCK();
    cRam0000000185a3afe8 = '\x01';
  }
  puStack_40 = &UNK_1812f3c41;
  lVar5 = FUN_1803a51b0(uRam0000000185796f80);
  if (lVar5 == 0) goto code_r0x0001812f3e13;
  *(longlong *)(lVar5 + 0x10) = lVar3;
  *(longlong *)(lVar5 + 0x18) = lVar10;
  if ((*(longlong *)(lVar3 + 0x50) == 0) || (lVar8 = *(longlong *)(lVar3 + 0x20), lVar8 == 0))
  goto code_r0x0001812f3e13;
  plVar11 = *(longlong **)(lVar3 + 0x18);
  if (cRam0000000185a3b88a == '\0') {
    puStack_40 = &UNK_1812f3c86;
    FUN_18035da50(0x185823980);
    LOCK();
    UNLOCK();
    cRam0000000185a3b88a = '\x01';
  }
  lVar10 = *(longlong *)(lVar8 + 0x18);
  if (lVar10 == 0) {
    puStack_40 = &UNK_1812f3ca7;
    lVar10 = FUN_1803a51b0(uRam0000000185823980);
    *(longlong *)(lVar8 + 0x18) = lVar10;
    if (lVar10 == 0) goto code_r0x0001812f3e13;
  }
  uVar16 = *(undefined4 *)(lVar10 + 0x10);
  puStack_40 = &UNK_1812f3cc3;
  lVar10 = FUN_1803a51b0(uRam0000000185796f98);
  lVar8 = lRam0000000185796fa8;
  pcVar7 = *(code **)(lRam0000000185796fa8 + 8);
  *(code **)(lVar10 + 0x10) = pcVar7;
  *(longlong *)(lVar10 + 0x28) = lVar8;
  *(longlong *)(lVar10 + 0x20) = lVar5;
  cVar2 = *(char *)(lVar8 + 0x52);
  *(longlong *)(lVar10 + 0x40) = lVar10;
  if ((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) {
    if (cVar2 != '\x02') goto code_r0x0001812f3d07;
    pcVar7 = FUN_180009e90;
  }
  else if (cVar2 == '\x03') {
    pcVar7 = FUN_180009ed0;
  }
  else {
code_r0x0001812f3d07:
    *(longlong *)(lVar10 + 0x40) = lVar5;
  }
  *(code **)(lVar10 + 0x18) = pcVar7;
  *(code **)(lVar10 + 0x38) = FUN_180009e20;
  puStack_40 = &UNK_1812f3d26;
  lVar8 = FUN_1803a51b0(uRam000000018580d858);
  lVar5 = lRam0000000185796fa0;
  puVar13 = *(undefined **)(lRam0000000185796fa0 + 8);
  *(undefined **)(lVar8 + 0x10) = puVar13;
  *(longlong *)(lVar8 + 0x28) = lVar5;
  *(longlong *)(lVar8 + 0x20) = lVar3;
  cVar2 = *(char *)(lVar5 + 0x52);
  *(longlong *)(lVar8 + 0x40) = lVar8;
  if (((*(byte *)(lVar5 + 0x4c) & 0x10) == 0) || (cVar2 != '\0')) {
    *(longlong *)(lVar8 + 0x40) = lVar3;
  }
  else {
    puVar13 = &UNK_180006440;
  }
  *(undefined **)(lVar8 + 0x18) = puVar13;
  *(code **)(lVar8 + 0x38) = FUN_1800063e0;
  if (plVar11 != (longlong *)0x0) {
    lVar5 = *plVar11;
    if (*(ushort *)(lVar5 + 0x12e) != 0) {
      uVar14 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar5 + 0xb0) + (ulonglong)uVar14 * 0x10) ==
            lRam000000018582ccc8) {
          puVar9 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar5 + 0xb0) + 8 + (ulonglong)uVar14 * 0x10) + 1) *
                    0x10 + 0x138 + lVar5);
          goto code_r0x0001812f3dc5;
        }
        uVar14 = uVar14 + 1;
      } while (uVar14 < *(ushort *)(lVar5 + 0x12e));
    }
    puStack_40 = &UNK_1812f3dc5;
    puVar9 = (undefined8 *)FUN_18034f630(plVar11,lRam000000018582ccc8,1);
code_r0x0001812f3dc5:
    puStack_40 = &UNK_1812f3ddb;
    (*(code *)*puVar9)(plVar11,uVar16,lVar10,lVar8);
    return;
  }
code_r0x0001812f3e13:
  puStack_40 = &UNK_1812f3e18;
  FUN_180352450();
  pcVar7 = (code *)swi(3);
  (*pcVar7)();
  return;
}




// ==== ContentAssetManager.CatalogWebBundleContentAssets @ 0x1812f3ba0 ====


void FUN_1812f3ba0(longlong param_1,undefined8 param_2)

{
  char cVar1;
  undefined4 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong lVar5;
  code *pcVar6;
  longlong lVar7;
  undefined8 *puVar8;
  ushort uVar9;
  undefined *puVar10;
  
  if (cRam0000000185a3afe8 == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796f98);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fa0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582ccc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796fa8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185796f80);
    LOCK();
    UNLOCK();
    cRam0000000185a3afe8 = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam0000000185796f80);
  if (lVar4 == 0) goto code_r0x0001812f3e13;
  *(longlong *)(lVar4 + 0x10) = param_1;
  *(undefined8 *)(lVar4 + 0x18) = param_2;
  if ((*(longlong *)(param_1 + 0x50) == 0) || (lVar7 = *(longlong *)(param_1 + 0x20), lVar7 == 0))
  goto code_r0x0001812f3e13;
  plVar3 = *(longlong **)(param_1 + 0x18);
  if (cRam0000000185a3b88a == '\0') {
    FUN_18035da50(0x185823980);
    LOCK();
    UNLOCK();
    cRam0000000185a3b88a = '\x01';
  }
  lVar5 = *(longlong *)(lVar7 + 0x18);
  if (lVar5 == 0) {
    lVar5 = FUN_1803a51b0(uRam0000000185823980);
    *(longlong *)(lVar7 + 0x18) = lVar5;
    if (lVar5 == 0) goto code_r0x0001812f3e13;
  }
  uVar2 = *(undefined4 *)(lVar5 + 0x10);
  lVar5 = FUN_1803a51b0(uRam0000000185796f98);
  lVar7 = lRam0000000185796fa8;
  pcVar6 = *(code **)(lRam0000000185796fa8 + 8);
  *(code **)(lVar5 + 0x10) = pcVar6;
  *(longlong *)(lVar5 + 0x28) = lVar7;
  *(longlong *)(lVar5 + 0x20) = lVar4;
  cVar1 = *(char *)(lVar7 + 0x52);
  *(longlong *)(lVar5 + 0x40) = lVar5;
  if ((*(byte *)(lVar7 + 0x4c) & 0x10) == 0) {
    if (cVar1 != '\x02') goto code_r0x0001812f3d07;
    pcVar6 = FUN_180009e90;
  }
  else if (cVar1 == '\x03') {
    pcVar6 = FUN_180009ed0;
  }
  else {
code_r0x0001812f3d07:
    *(longlong *)(lVar5 + 0x40) = lVar4;
  }
  *(code **)(lVar5 + 0x18) = pcVar6;
  *(code **)(lVar5 + 0x38) = FUN_180009e20;
  lVar7 = FUN_1803a51b0(uRam000000018580d858);
  lVar4 = lRam0000000185796fa0;
  puVar10 = *(undefined **)(lRam0000000185796fa0 + 8);
  *(undefined **)(lVar7 + 0x10) = puVar10;
  *(longlong *)(lVar7 + 0x28) = lVar4;
  *(longlong *)(lVar7 + 0x20) = param_1;
  cVar1 = *(char *)(lVar4 + 0x52);
  *(longlong *)(lVar7 + 0x40) = lVar7;
  if (((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
    *(longlong *)(lVar7 + 0x40) = param_1;
  }
  else {
    puVar10 = &UNK_180006440;
  }
  *(undefined **)(lVar7 + 0x18) = puVar10;
  *(code **)(lVar7 + 0x38) = FUN_1800063e0;
  if (plVar3 != (longlong *)0x0) {
    lVar4 = *plVar3;
    if (*(ushort *)(lVar4 + 0x12e) != 0) {
      uVar9 = 0;
      do {
        if (*(longlong *)(*(longlong *)(lVar4 + 0xb0) + (ulonglong)uVar9 * 0x10) ==
            lRam000000018582ccc8) {
          puVar8 = (undefined8 *)
                   ((longlong)
                    (*(int *)(*(longlong *)(lVar4 + 0xb0) + 8 + (ulonglong)uVar9 * 0x10) + 1) * 0x10
                    + 0x138 + lVar4);
          goto code_r0x0001812f3dc5;
        }
        uVar9 = uVar9 + 1;
      } while (uVar9 < *(ushort *)(lVar4 + 0x12e));
    }
    puVar8 = (undefined8 *)FUN_18034f630(plVar3,lRam000000018582ccc8,1);
code_r0x0001812f3dc5:
    (*(code *)*puVar8)(plVar3,uVar2,lVar5,lVar7,puVar8[1]);
    return;
  }
code_r0x0001812f3e13:
  FUN_180352450();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}




// ==== ContentAsset.get_CachePath @ 0x1812eff60 ====


undefined8 FUN_1812eff60(longlong param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  if (cRam0000000185a3afca == '\0') {
    FUN_18035da50(0x185821540);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857972e0);
    LOCK();
    UNLOCK();
    cRam0000000185a3afca = '\x01';
  }
  if (*(longlong *)(param_1 + 0x10) == 0) {
    uVar1 = FUN_184452c10(0);
    uVar2 = FUN_184451c90(uVar1,0);
    uVar3 = *(undefined8 *)(param_1 + 0x28);
    if (*(int *)(lRam0000000185821540 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (cRam0000000185a49294 == '\0') {
      FUN_18035da50(0x185821540);
      LOCK();
      UNLOCK();
      cRam0000000185a49294 = '\x01';
    }
    if (*(int *)(lRam0000000185821540 + 0xe4) == 0) {
      FUN_180359940();
    }
    uVar3 = FUN_183a94740(uVar3,0);
    uVar3 = FUN_183a93520(uVar3,0,0);
    uStack_18 = *(undefined4 *)(param_1 + 0x58);
    uStack_14 = *(undefined4 *)(param_1 + 0x5c);
    uStack_10 = *(undefined4 *)(param_1 + 0x60);
    uStack_c = *(undefined4 *)(param_1 + 100);
    uVar4 = FUN_18448e440(&uStack_18,0);
    uVar3 = FUN_180ca8670(uVar2,uVar3,uVar4,uRam00000001857972e0,0,0);
    *(undefined8 *)(param_1 + 0x10) = uVar3;
  }
  return *(undefined8 *)(param_1 + 0x10);
}

