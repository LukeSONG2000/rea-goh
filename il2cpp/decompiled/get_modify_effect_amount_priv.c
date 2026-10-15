// ==== get_modify_effect_amount_priv @ 0x181a73980 ====


ulonglong FUN_181a73980(longlong param_1,longlong param_2,longlong param_3,undefined1 param_4,
                       longlong param_5,char param_6,char param_7)

{
  float *pfVar1;
  longlong lVar2;
  code *pcVar3;
  char cVar4;
  uint uVar5;
  longlong lVar6;
  longlong lVar7;
  int iVar8;
  bool bVar9;
  ulonglong uVar10;
  float fVar11;
  float afStackX_8 [2];
  ulonglong in_stack_fffffffffffffe88;
  longlong lStack_160;
  longlong lStack_158;
  uint uStack_150;
  int iStack_14c;
  longlong lStack_148;
  int iStack_140;
  float fStack_13c;
  longlong lStack_138;
  longlong alStack_130 [2];
  longlong *plStack_120;
  undefined8 uStack_118;
  longlong lStack_110;
  undefined8 uStack_108;
  undefined4 uStack_100;
  int iStack_fc;
  undefined8 uStack_f8;
  longlong lStack_f0;
  uint uStack_e8;
  int iStack_e4;
  longlong lStack_e0;
  longlong lStack_d8;
  undefined8 uStack_d0;
  longlong *plStack_c8;
  undefined8 uStack_c0;
  longlong *plStack_b8;
  undefined8 uStack_b0;
  longlong *plStack_a8;
  undefined8 uStack_a0;
  longlong *plStack_98;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  int iStack_4c;
  undefined8 uStack_48;
  
  if (cRam0000000185a3e0b0 == '\0') {
    FUN_18035da50(0x185810140);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810138);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810128);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810120);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d110);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d130);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d128);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858103e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1228);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857855b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d20);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d120);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d0d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b1218);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810380);
    LOCK();
    UNLOCK();
    cRam0000000185a3e0b0 = '\x01';
  }
  lStack_138 = 0;
  alStack_130[0] = 0;
  lStack_160 = 0;
  lStack_f0 = 0;
  uStack_e8 = 0;
  iStack_e4 = 0;
  lStack_e0 = 0;
  lStack_110 = 0;
  lStack_158 = 0;
  uStack_150 = 0;
  iStack_14c = 0;
  lStack_148 = 0;
  if ((*(longlong *)(param_1 + 0x68) == 0) ||
     (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x340) == 0)) {
    FUN_180352450();
code_r0x000181a74820:
    FUN_180352450();
code_r0x000181a74826:
    FUN_180352450();
code_r0x000181a7482b:
    FUN_180352450();
code_r0x000181a74830:
    FUN_180352450();
code_r0x000181a74835:
    FUN_180352450();
code_r0x000181a7483a:
    FUN_180352450();
code_r0x000181a7483f:
    FUN_180352450();
code_r0x000181a74844:
    FUN_180352450();
code_r0x000181a7484a:
    FUN_180352460();
code_r0x000181a74850:
    FUN_180352450();
code_r0x000181a74855:
    FUN_180352450();
code_r0x000181a7485a:
    FUN_180352450();
code_r0x000181a7485f:
    FUN_180352450();
code_r0x000181a74865:
    FUN_180352460();
code_r0x000181a7486b:
    FUN_180352450();
code_r0x000181a74870:
    FUN_180352450();
code_r0x000181a74875:
    FUN_180352450();
code_r0x000181a7487a:
    FUN_180352450();
code_r0x000181a7487f:
    FUN_180352450();
code_r0x000181a74884:
    FUN_180352450();
code_r0x000181a74889:
    FUN_180352450();
    FUN_180352440();
code_r0x000181a74894:
    FUN_180352450();
code_r0x000181a74899:
    FUN_180352450();
code_r0x000181a7489e:
    FUN_180352450();
code_r0x000181a748a3:
    FUN_180352450();
code_r0x000181a748a8:
    FUN_180352450();
code_r0x000181a748ad:
    FUN_180352450();
code_r0x000181a748b2:
    FUN_180352450();
code_r0x000181a748b7:
    FUN_180352460();
code_r0x000181a748bc:
    FUN_180352450();
code_r0x000181a748c1:
    FUN_180352450();
code_r0x000181a748c6:
    FUN_180352450();
code_r0x000181a748cb:
    FUN_180352450();
code_r0x000181a748d0:
    FUN_180352450();
code_r0x000181a748d5:
    FUN_180352450();
code_r0x000181a748da:
    FUN_180352450();
code_r0x000181a748df:
    FUN_180352450();
code_r0x000181a748e4:
    FUN_180352450();
code_r0x000181a748e9:
    FUN_183aed800(0);
code_r0x000181a748f0:
    FUN_180352450();
code_r0x000181a748f5:
    FUN_180352450();
    FUN_180352440();
    FUN_180352440();
code_r0x000181a74907:
    FUN_180352450();
code_r0x000181a7490c:
    FUN_180352450();
code_r0x000181a74911:
    FUN_180352450();
code_r0x000181a74917:
    FUN_180352460();
code_r0x000181a7491c:
    FUN_180352450();
    uVar5 = uStack_150;
code_r0x000181a74921:
    uStack_150 = uVar5;
    FUN_180352450();
code_r0x000181a74926:
    FUN_180352450();
code_r0x000181a7492b:
    FUN_180352450();
code_r0x000181a74930:
    FUN_183aed800(0);
code_r0x000181a74937:
    FUN_180352450();
    FUN_180352440();
code_r0x000181a74943:
    FUN_180352460();
code_r0x000181a74948:
    FUN_180352450();
    uVar5 = uStack_150;
code_r0x000181a7494d:
    uStack_150 = uVar5;
    FUN_180352450();
code_r0x000181a74952:
    FUN_180352450();
code_r0x000181a74957:
    FUN_180352450();
code_r0x000181a7495c:
    FUN_183aed800(0);
code_r0x000181a74963:
    FUN_180352450();
    FUN_180352440();
code_r0x000181a7496e:
    FUN_180352450();
code_r0x000181a74973:
    FUN_180352450();
code_r0x000181a74978:
    FUN_180352450();
code_r0x000181a7497d:
    FUN_180352450();
code_r0x000181a74982:
    FUN_180352450();
code_r0x000181a74987:
    FUN_180352450();
code_r0x000181a7498c:
    FUN_180352450();
code_r0x000181a74991:
    FUN_180352450();
    FUN_180352440();
    FUN_180352440();
  }
  else {
    fStack_13c = 0.0;
    lStack_138 = FUN_180035f00();
    uStack_b0 = 0;
    plStack_a8 = &lStack_138;
    if (*(longlong *)(param_1 + 0x68) == 0) goto code_r0x000181a749a8;
    if (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x340) != 0) {
      alStack_130[0] = FUN_180035f00();
      uStack_c0 = 0;
      plStack_b8 = alStack_130;
      if (*(longlong *)(param_1 + 0x68) != 0) {
        lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x310);
        if (lVar6 != 0) {
          lVar6 = FUN_18366e0e0(lVar6,uRam00000001857855b0);
          uStack_d0 = 0;
          plStack_c8 = &lStack_160;
          lStack_160 = lVar6;
          if (param_6 != '\x1c') {
            if (param_6 == ' ') {
              if (param_7 == '\0') {
                if (*(int *)(lRam0000000185810140 + 0xe4) == 0) {
                  FUN_180359940(lRam0000000185810140);
                }
                if (param_2 == 0) goto code_r0x000181a74844;
                if (*(longlong *)(param_2 + 0x60) == 0) goto code_r0x000181a7483f;
                afStackX_8[0] =
                     (float)CONCAT13(afStackX_8[0]._3_1_,
                                     *(undefined3 *)(*(longlong *)(lRam0000000185810140 + 0xb8) + 3)
                                    );
                FUN_181a74c30(*(longlong *)(param_2 + 0x60),lVar6,afStackX_8,param_4,
                              in_stack_fffffffffffffe88 & 0xffffffffffffff00,0);
              }
              else if (param_7 == '\x01') {
                if (*(int *)(lRam0000000185810140 + 0xe4) == 0) {
                  FUN_180359940(lRam0000000185810140);
                }
                if (param_2 == 0) goto code_r0x000181a7483a;
                if (*(longlong *)(param_2 + 0x60) == 0) goto code_r0x000181a74835;
                afStackX_8[0] =
                     (float)CONCAT13(afStackX_8[0]._3_1_,
                                     *(undefined3 *)(*(longlong *)(lRam0000000185810140 + 0xb8) + 9)
                                    );
                FUN_181a74c30(*(longlong *)(param_2 + 0x60),lVar6,afStackX_8,param_4,1,0);
              }
              else if (param_7 == '\x02') {
                if (*(int *)(lRam0000000185810140 + 0xe4) == 0) {
                  FUN_180359940(lRam0000000185810140);
                }
                if (param_2 == 0) goto code_r0x000181a74830;
                if (*(longlong *)(param_2 + 0x60) == 0) goto code_r0x000181a7482b;
                afStackX_8[0] =
                     (float)CONCAT13(afStackX_8[0]._3_1_,
                                     *(undefined3 *)
                                      (*(longlong *)(lRam0000000185810140 + 0xb8) + 0xc));
                FUN_181a74c30(*(longlong *)(param_2 + 0x60),lVar6,afStackX_8,param_4,2,0);
              }
              else if (param_7 == '\x03') {
                if (*(int *)(lRam0000000185810140 + 0xe4) == 0) {
                  FUN_180359940(lRam0000000185810140);
                }
                if (param_2 == 0) goto code_r0x000181a74826;
                if (*(longlong *)(param_2 + 0x60) == 0) goto code_r0x000181a74820;
                afStackX_8[0] =
                     (float)CONCAT13(afStackX_8[0]._3_1_,
                                     *(undefined3 *)(*(longlong *)(lRam0000000185810140 + 0xb8) + 6)
                                    );
                FUN_181a74c30(*(longlong *)(param_2 + 0x60),lVar6,afStackX_8,param_4,3,0);
              }
            }
code_r0x000181a73eec:
            if (lStack_160 != 0) {
              uStack_118 = 0;
              iStack_e4 = *(int *)(lStack_160 + 0x1c);
              lStack_f0 = lStack_160;
              uStack_e8 = 0;
              lStack_e0 = 0;
              alStack_130[1] = 0;
              plStack_120 = &lStack_f0;
code_r0x000181a73f50:
              if (lStack_f0 != 0) {
                if ((iStack_e4 == *(int *)(lStack_f0 + 0x1c)) &&
                   (uStack_e8 < *(uint *)(lStack_f0 + 0x18))) {
                  lVar6 = *(longlong *)(lStack_f0 + 0x10);
                  if (lVar6 != 0) {
                    if (uStack_e8 < *(uint *)(lVar6 + 0x18)) {
                      lVar6 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uStack_e8 * 8);
                      uStack_e8 = uStack_e8 + 1;
                      lStack_e0 = lVar6;
                      if (lVar6 != 0) {
                        if (*(longlong *)(lVar6 + 0x38) != 0) {
                          lVar7 = *(longlong *)(*(longlong *)(lVar6 + 0x38) + 0x38);
                          if (lVar7 != 0) {
                            lVar7 = *(longlong *)(lVar7 + 0xc0);
                            if (lVar7 != 0) {
                              bVar9 = *(int *)(lVar7 + 0x18) == 0;
                              afStackX_8[0] = (float)((uint)afStackX_8[0] & 0xffffff00);
                              iVar8 = 0;
                              iStack_140 = 0;
                              uStack_100 = 0;
                              iStack_fc = *(int *)(lVar7 + 0x1c);
                              uStack_108._0_4_ = (undefined4)lVar7;
                              uStack_108._4_4_ = (undefined4)((ulonglong)lVar7 >> 0x20);
                              uStack_58 = (undefined4)uStack_108;
                              uStack_54 = uStack_108._4_4_;
                              uStack_50 = 0;
                              uStack_48 = 0;
                              uStack_108 = lVar7;
                              lStack_d8 = lVar6;
                              iStack_4c = iStack_fc;
                              lStack_110 = FUN_1803a4e80(*(undefined8 *)
                                                          (*(longlong *)
                                                            (*(longlong *)
                                                              (lRam00000001857b1218 + 0x20) + 0xc0)
                                                          + 0x30),&uStack_58);
                              uStack_a0 = 0;
                              plStack_98 = &lStack_110;
code_r0x000181a74081:
                              do {
                                while( true ) {
                                  if (lStack_110 == 0) goto code_r0x000181a74889;
                                  cVar4 = FUN_1800084f0(0,uRam0000000185820d00);
                                  if (cVar4 == '\0') {
                                    if (lStack_110 != 0) {
                                      FUN_180005650(0,uRam000000018580d620,lStack_110);
                                    }
                                    if (*(longlong *)(lVar6 + 0x38) == 0) goto code_r0x000181a7489e;
                                    lVar7 = *(longlong *)(*(longlong *)(lVar6 + 0x38) + 0x38);
                                    if (lVar7 == 0) goto code_r0x000181a74899;
                                    lVar7 = *(longlong *)(lVar7 + 0xc0);
                                    if (lVar7 == 0) goto code_r0x000181a74894;
                                    cVar4 = afStackX_8[0]._0_1_;
                                    if (iVar8 == *(int *)(lVar7 + 0x18)) goto code_r0x000181a741ea;
                                    goto code_r0x000181a741d8;
                                  }
                                  if (lStack_110 == 0) goto code_r0x000181a74884;
                                  lVar7 = FUN_1800175a0();
                                  if (lVar7 == 0) goto code_r0x000181a7487f;
                                  if (*(char *)(lVar7 + 0x18) == '\0') break;
                                  iVar8 = iVar8 + 1;
                                  iStack_140 = iVar8;
                                  if (param_3 == 0) goto code_r0x000181a7487a;
                                  cVar4 = FUN_181bbb910(param_3,*(undefined8 *)(lVar7 + 0x10),0);
                                  if ((cVar4 != '\0') ||
                                     (cVar4 = FUN_181bca780(param_3,*(undefined8 *)(lVar7 + 0x10),0)
                                     , cVar4 != '\0')) goto code_r0x000181a741b6;
                                  if (param_5 == 0) goto code_r0x000181a74875;
                                  if (*(longlong *)(param_5 + 0x38) == 0) goto code_r0x000181a74870;
                                  lVar2 = *(longlong *)(*(longlong *)(param_5 + 0x38) + 0xb8);
                                  if (lVar2 == 0) goto code_r0x000181a7486b;
                                  cVar4 = FUN_1800075c0(lVar2,*(undefined8 *)(lVar7 + 0x10));
                                  if (cVar4 != '\0') goto code_r0x000181a741b6;
                                }
                                if (!bVar9) {
                                  if (param_3 == 0) goto code_r0x000181a7485f;
                                  cVar4 = FUN_181bbb910(param_3,*(undefined8 *)(lVar7 + 0x10),0);
                                  if ((cVar4 == '\0') &&
                                     (cVar4 = FUN_181bca780(param_3,*(undefined8 *)(lVar7 + 0x10),0)
                                     , cVar4 == '\0')) {
                                    if (param_5 == 0) goto code_r0x000181a7485a;
                                    if (*(longlong *)(param_5 + 0x38) == 0)
                                    goto code_r0x000181a74855;
                                    lVar2 = *(longlong *)(*(longlong *)(param_5 + 0x38) + 0xb8);
                                    if (lVar2 == 0) goto code_r0x000181a74850;
                                    cVar4 = FUN_1800075c0(lVar2,*(undefined8 *)(lVar7 + 0x10));
                                    if (cVar4 == '\0') goto code_r0x000181a74081;
                                  }
                                }
                                bVar9 = true;
                              } while( true );
                            }
                            goto code_r0x000181a748d0;
                          }
                          goto code_r0x000181a748d5;
                        }
                        goto code_r0x000181a748da;
                      }
                      goto code_r0x000181a748df;
                    }
                    goto code_r0x000181a7484a;
                  }
                  goto code_r0x000181a748e4;
                }
                if ((*(byte *)(*(longlong *)(lRam000000018580d130 + 0x20) + 0x135) & 1) == 0) {
                  FUN_18034f1d0(*(longlong *)(lRam000000018580d130 + 0x20));
                }
                if (lStack_f0 == 0) goto code_r0x000181a748f0;
                if (iStack_e4 != *(int *)(lStack_f0 + 0x1c)) goto code_r0x000181a748e9;
                uStack_e8 = *(int *)(lStack_f0 + 0x18) + 1;
                lStack_e0 = 0;
                if (*plStack_c8 != 0) {
                  FUN_180005650(0,uRam000000018580d620,*plStack_c8);
                }
                if (lStack_138 == 0) goto code_r0x000181a74987;
                lVar6 = FUN_180035ea0();
                if (lVar6 == 0) goto code_r0x000181a74982;
                uStack_108 = *(longlong *)(lVar6 + 0x10);
                uStack_f8 = 0;
                if (uStack_108 == 0) goto code_r0x000181a7497d;
                afStackX_8[0] = 0.0;
                iStack_fc = *(int *)(uStack_108 + 0x2c);
                uStack_100 = 0;
                uStack_150 = 0;
                lStack_148 = 0;
                alStack_130[1] = 0;
                plStack_120 = &lStack_158;
                goto code_r0x000181a74576;
              }
              goto code_r0x000181a748f5;
            }
            goto code_r0x000181a74907;
          }
          if (*(int *)(lRam0000000185810140 + 0xe4) == 0) {
            FUN_180359940(lRam0000000185810140);
          }
          if (param_2 != 0) {
            if (*(longlong *)(param_2 + 0x60) != 0) {
              afStackX_8[0] =
                   (float)CONCAT13(afStackX_8[0]._3_1_,
                                   **(undefined3 **)(lRam0000000185810140 + 0xb8));
              FUN_181a74c30(*(longlong *)(param_2 + 0x60),lVar6,afStackX_8,param_4,0xff,0);
              goto code_r0x000181a73eec;
            }
            goto code_r0x000181a7490c;
          }
          goto code_r0x000181a74911;
        }
        goto code_r0x000181a7498c;
      }
      goto code_r0x000181a74991;
    }
  }
  FUN_180352450();
code_r0x000181a749a8:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  uVar10 = (*pcVar3)();
  return uVar10;
code_r0x000181a74576:
  fVar11 = afStackX_8[0];
  lStack_158 = uStack_108;
  iStack_14c = iStack_fc;
  if (uStack_108 == 0) goto code_r0x000181a74937;
  if (iStack_fc != *(int *)(uStack_108 + 0x2c)) goto code_r0x000181a74930;
  do {
    if (uStack_108 == 0) goto code_r0x000181a7492b;
    if (*(uint *)(uStack_108 + 0x20) <= uStack_150) {
      if (uStack_108 == 0) goto code_r0x000181a74926;
      uStack_150 = *(int *)(uStack_108 + 0x20) + 1;
      lStack_148 = 0;
      if (alStack_130[0] == 0) goto code_r0x000181a74978;
      lVar6 = FUN_180035ea0();
      if (lVar6 == 0) goto code_r0x000181a74973;
      uStack_108 = *(longlong *)(lVar6 + 0x10);
      uStack_f8 = 0;
      if (uStack_108 == 0) goto code_r0x000181a7496e;
      iStack_fc = *(int *)(uStack_108 + 0x2c);
      uStack_100 = 0;
      uStack_150 = 0;
      lStack_148 = 0;
      alStack_130[1] = 0;
      plStack_120 = &lStack_158;
      goto code_r0x000181a746a4;
    }
    lVar6 = *(longlong *)(uStack_108 + 0x18);
    lVar7 = (longlong)(int)uStack_150;
    uVar5 = uStack_150 + 1;
    if (lVar6 == 0) goto code_r0x000181a74921;
    bVar9 = *(uint *)(lVar6 + 0x18) <= uStack_150;
    uStack_150 = uVar5;
    if (bVar9) goto code_r0x000181a74917;
  } while (*(int *)(lVar6 + 0x20 + lVar7 * 0x18) < 0);
  lStack_148 = *(longlong *)(lVar6 + 0x30 + lVar7 * 0x18);
  if (lStack_148 == 0) goto code_r0x000181a7491c;
  afStackX_8[0] = afStackX_8[0] + *(float *)(lStack_148 + 0x14);
  goto code_r0x000181a74576;
code_r0x000181a746a4:
  lStack_158 = uStack_108;
  iStack_14c = iStack_fc;
  if (uStack_108 == 0) goto code_r0x000181a74963;
  if (iStack_fc != *(int *)(uStack_108 + 0x2c)) goto code_r0x000181a7495c;
  do {
    if (uStack_108 == 0) goto code_r0x000181a74957;
    if (*(uint *)(uStack_108 + 0x20) <= uStack_150) {
      if (uStack_108 != 0) {
        _uStack_150 = CONCAT44(iStack_fc,*(int *)(uStack_108 + 0x20) + 1);
        lStack_148 = 0;
        fStack_13c = fVar11;
        if (*plStack_b8 != 0) {
          FUN_180005650(0,uRam000000018580d620,*plStack_b8);
        }
        if (*plStack_a8 != 0) {
          FUN_180005650(0,uRam000000018580d620,*plStack_a8);
        }
        return (ulonglong)(uint)fVar11;
      }
      goto code_r0x000181a74952;
    }
    lVar6 = *(longlong *)(uStack_108 + 0x18);
    lVar7 = (longlong)(int)uStack_150;
    uVar5 = uStack_150 + 1;
    if (lVar6 == 0) goto code_r0x000181a7494d;
    bVar9 = *(uint *)(lVar6 + 0x18) <= uStack_150;
    uStack_150 = uVar5;
    if (bVar9) goto code_r0x000181a74943;
  } while (*(int *)(lVar6 + 0x20 + lVar7 * 0x18) < 0);
  lStack_148 = *(longlong *)(lVar6 + 0x30 + lVar7 * 0x18);
  if (lStack_148 == 0) goto code_r0x000181a74948;
  fVar11 = fVar11 + *(float *)(lStack_148 + 0x14);
  afStackX_8[0] = fVar11;
  goto code_r0x000181a746a4;
code_r0x000181a741b6:
  afStackX_8[0] = (float)CONCAT31(afStackX_8[0]._1_3_,1);
  cVar4 = '\x01';
  if (lStack_110 != 0) {
    FUN_180005650(0,uRam000000018580d620,lStack_110);
  }
code_r0x000181a741d8:
  if ((bVar9) && (cVar4 == '\0')) {
code_r0x000181a741ea:
    lVar2 = alStack_130[0];
    lVar7 = lStack_138;
    if (0.0 < *(float *)(lVar6 + 0x14)) {
      if (lStack_138 == 0) goto code_r0x000181a748cb;
      uVar5 = FUN_18332e340(lStack_138,*(undefined8 *)(lVar6 + 0x20),
                            *(undefined8 *)
                             (*(longlong *)(*(longlong *)(lRam0000000185810128 + 0x20) + 0xc0) +
                             0x108));
      if (-1 < (int)uVar5) {
        lVar7 = *(longlong *)(lVar7 + 0x18);
        if (lVar7 == 0) goto code_r0x000181a748c1;
        if (*(uint *)(lVar7 + 0x18) <= uVar5) goto code_r0x000181a748b7;
        lVar7 = *(longlong *)(lVar7 + ((longlong)(int)uVar5 + 2) * 0x18);
        if (lVar7 == 0) goto code_r0x000181a748bc;
        pfVar1 = (float *)(lVar7 + 0x14);
        if (*(float *)(lVar6 + 0x14) < *pfVar1 || *(float *)(lVar6 + 0x14) == *pfVar1)
        goto code_r0x000181a73f50;
      }
      if (lStack_138 == 0) goto code_r0x000181a748c6;
      FUN_18334c740(lStack_138,*(undefined8 *)(lVar6 + 0x20),lVar6,1,
                    *(undefined8 *)
                     (*(longlong *)(*(longlong *)(lRam0000000185810120 + 0x20) + 0xc0) + 0x110));
      goto code_r0x000181a73f50;
    }
    if (*(float *)(lVar6 + 0x14) < 0.0) {
      if (alStack_130[0] == 0) goto code_r0x000181a748b2;
      uVar5 = FUN_18332e340(alStack_130[0],*(undefined8 *)(lVar6 + 0x20),
                            *(undefined8 *)
                             (*(longlong *)(*(longlong *)(lRam0000000185810128 + 0x20) + 0xc0) +
                             0x108));
      if (-1 < (int)uVar5) {
        lVar7 = *(longlong *)(lVar2 + 0x18);
        if (lVar7 == 0) goto code_r0x000181a748a8;
        if (*(uint *)(lVar7 + 0x18) <= uVar5) goto code_r0x000181a74865;
        lVar7 = *(longlong *)(lVar7 + ((longlong)(int)uVar5 + 2) * 0x18);
        if (lVar7 == 0) goto code_r0x000181a748a3;
        fVar11 = *(float *)(lVar7 + 0x14);
        if (fVar11 < *(float *)(lVar6 + 0x14) || fVar11 == *(float *)(lVar6 + 0x14))
        goto code_r0x000181a73f50;
      }
      if (alStack_130[0] == 0) goto code_r0x000181a748ad;
      FUN_18334c740(alStack_130[0],*(undefined8 *)(lVar6 + 0x20),lVar6,1,
                    *(undefined8 *)
                     (*(longlong *)(*(longlong *)(lRam0000000185810120 + 0x20) + 0xc0) + 0x110));
    }
  }
  goto code_r0x000181a73f50;
}

