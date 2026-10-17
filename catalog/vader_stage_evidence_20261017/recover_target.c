// recover_target VA 0x181371c10 RVA 0x1371c10

void FUN_181371c10(longlong param_1,longlong param_2,longlong param_3,float param_4,
                  undefined8 param_5,undefined1 param_6,undefined1 param_7)

{
  float fVar1;
  uint uVar2;
  longlong lVar3;
  code *pcVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined8 uVar11;
  longlong lVar12;
  longlong lVar13;
  longlong lVar14;
  float fVar15;
  float fVar16;
  double dVar17;
  undefined8 extraout_XMM0_Qb;
  undefined1 auVar18 [16];
  undefined1 auVar19 [16];
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  ulonglong in_stack_fffffffffffffea8;
  ulonglong uVar25;
  undefined8 uVar26;
  undefined4 uVar28;
  ulonglong uVar27;
  undefined4 in_stack_fffffffffffffeb8;
  undefined3 uVar29;
  undefined4 in_stack_fffffffffffffec0;
  undefined3 uVar30;
  undefined8 in_stack_fffffffffffffec8;
  undefined7 uVar31;
  undefined8 in_stack_fffffffffffffed0;
  undefined7 uVar32;
  longlong lStack_118;
  undefined8 uStack_110;
  longlong lStack_108;
  float fStack_100;
  undefined4 uStack_fc;
  undefined8 uStack_f8;
  longlong *plStack_f0;
  undefined8 uStack_e8;
  float fStack_e0;
  float fStack_dc;
  float fStack_d8;
  longlong lStack_d0;
  undefined8 uStack_c8;
  longlong lStack_c0;
  
  if (cRam0000000185a3b37a == '\0') {
    FUN_18035da50(0x185806bf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc30);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ba8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc20);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185830790);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc10);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e7270);
    LOCK();
    UNLOCK();
    cRam0000000185a3b37a = '\x01';
  }
  lStack_118 = 0;
  uStack_110 = 0;
  lStack_108 = 0;
  if ((*(longlong *)(param_1 + 0x30) == 0) ||
     (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 == 0)) {
code_r0x000181372d04:
    FUN_180352450();
code_r0x000181372d0a:
    FUN_180352460();
code_r0x000181372d0f:
    FUN_180352450();
code_r0x000181372d14:
    FUN_180352450();
code_r0x000181372d19:
    FUN_180352450();
code_r0x000181372d1e:
    FUN_180352450();
code_r0x000181372d23:
    FUN_180352450();
code_r0x000181372d28:
    FUN_180352450();
code_r0x000181372d2d:
    FUN_183aed800(0);
code_r0x000181372d34:
    FUN_180352450();
  }
  else {
    lVar14 = param_2;
    if (((*(ulonglong *)(lVar13 + 0xf0) & 0x800000000) != 0) &&
       (*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x30) != 0)) {
      lVar14 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x30);
    }
    fVar23 = 0.0;
    fVar21 = 0.0;
    fStack_100 = 0.0;
    lVar13 = *(longlong *)(param_1 + 0x30);
    lVar12 = *(longlong *)(lVar13 + 0x38);
    param_4 = param_4 * *(float *)(lVar12 + 0x2c);
    fVar20 = *(float *)(lVar12 + 0x28);
    fVar22 = *(float *)(lVar12 + 0x24);
    uStack_fc = 0;
    cVar5 = *(char *)(lVar12 + 0x22);
    fStack_e0 = param_4;
    fStack_dc = fVar20;
    fStack_d8 = fVar22;
    lStack_d0 = lVar14;
    if (cVar5 == '\0') {
      if (param_3 != 0) {
        fVar15 = (float)FUN_181bbae00(param_3,1,0);
        if ((*(longlong *)(param_1 + 0x30) != 0) &&
           (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)) {
          if ((*(byte *)(lVar13 + 0xf0) & 2) != 0) {
            fVar21 = (float)FUN_181bbae00(param_3,1,0);
            param_4 = param_4 * (float)(int)fVar21;
          }
          if (param_2 != 0) {
            uVar27 = 0;
            uVar25 = in_stack_fffffffffffffea8 & 0xffffffffffffff00;
            fVar21 = (float)FUN_181bcec50(param_2,param_3,*(undefined8 *)(param_1 + 0x30),0x20,
                                          uVar25,0);
            uVar28 = (undefined4)(uVar25 >> 0x20);
            iVar6 = 0;
            if (-1 < *(int *)(param_3 + 0xc0)) {
              iVar6 = *(int *)(param_3 + 0xc0);
            }
            fVar16 = (float)FUN_181bbae00(param_3,1,0);
            iVar7 = (int)fVar16;
            if (iVar6 < (int)fVar16) {
              iVar7 = iVar6;
            }
            fVar21 = fVar21 * (fVar20 + param_4 + (float)(int)fVar15 * fVar22);
            fVar20 = (float)FUN_181bbae00(param_3,1,0);
            if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
              FUN_180359940();
            }
            fVar20 = ((float)iVar7 + fVar21) - (float)(int)fVar20;
            if (0.0 <= fVar20) {
              fVar23 = fVar20;
            }
            uVar11 = *(undefined8 *)(param_1 + 0x30);
            uVar9 = *(undefined4 *)(param_1 + 0x40);
            dVar17 = (double)FUN_18000cb30((double)fVar21);
            uVar32 = (undefined7)((ulonglong)in_stack_fffffffffffffed0 >> 8);
            uVar31 = (undefined7)((ulonglong)in_stack_fffffffffffffec8 >> 8);
            uVar30 = (undefined3)((uint)in_stack_fffffffffffffec0 >> 8);
            uVar29 = (undefined3)((uint)in_stack_fffffffffffffeb8 >> 8);
            uVar27 = uVar27 & 0xffffffff00000000;
            uVar25 = CONCAT44(uVar28,(int)dVar17);
code_r0x000181372beb:
            FUN_181bc6060(param_3,lVar14,uVar11,uVar9,uVar25,uVar27,CONCAT31(uVar29,param_6),
                          CONCAT31(uVar30,param_7),CONCAT71(uVar31,1),CONCAT71(uVar32,1),0);
            uVar28 = (undefined4)(uVar25 >> 0x20);
            goto code_r0x000181372bfc;
          }
        }
      }
      goto code_r0x000181372d04;
    }
    if (cVar5 == '\x01') {
      if (lVar13 != 0) {
        if (*(char *)(lVar13 + 0x10) == '\0') {
          if (lVar12 != 0) {
            if ((*(byte *)(lVar12 + 0xf0) & 0x40) != 0) goto code_r0x000181372667;
            if (param_3 == 0) goto code_r0x000181372d04;
            cVar5 = FUN_181bcac50(param_3,lVar14,0);
            uVar28 = (undefined4)(in_stack_fffffffffffffea8 >> 0x20);
            if (cVar5 == '\0') goto code_r0x000181372667;
            FUN_181bc7f70(param_3,0);
            if (((*(longlong *)(param_1 + 0x30) != 0) &&
                (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)) &&
               (lVar13 = *(longlong *)(lVar13 + 0xb8), lVar13 != 0)) {
              if (*(int *)(lVar13 + 0x18) < 1) {
                FUN_181bcd360(param_3,0x22,lVar14,0);
              }
              else {
                if ((*(longlong *)(param_1 + 0x30) == 0) ||
                   (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 == 0))
                goto code_r0x000181372d04;
                uVar28 = 0;
                FUN_181bcd640(param_3,0x22,lVar14,*(undefined8 *)(lVar13 + 0xb8),0);
              }
              if (((*(longlong *)(param_1 + 0x30) != 0) &&
                  (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)) &&
                 (lVar13 = *(longlong *)(lVar13 + 0xb8), lVar13 != 0)) {
                if (*(int *)(lVar13 + 0x18) < 1) {
                  if (lVar14 != 0) {
                    FUN_181bcd360(lVar14,0x23,param_3,0);
                    goto code_r0x000181372bfc;
                  }
                }
                else if (((*(longlong *)(param_1 + 0x30) != 0) &&
                         (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)
                         ) && (lVar14 != 0)) {
                  uVar28 = 0;
                  FUN_181bcd640(lVar14,0x23,param_3,*(undefined8 *)(lVar13 + 0xb8),0);
                  goto code_r0x000181372bfc;
                }
              }
            }
          }
        }
        else {
code_r0x000181372667:
          if ((((*(longlong *)(param_1 + 0x38) != 0) &&
               (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x1e8), lVar13 != 0)) &&
              (lVar12 = *(longlong *)(param_1 + 0x30), lVar12 != 0)) &&
             (*(longlong *)(lVar12 + 0x38) != 0)) {
            fVar15 = *(float *)(lVar13 + 0x24b4);
            if ((*(byte *)(*(longlong *)(lVar12 + 0x38) + 0xf0) & 2) != 0) {
              param_4 = param_4 * fVar15;
            }
            if (param_2 != 0) {
              uVar26 = CONCAT71((int7)(in_stack_fffffffffffffea8 >> 8),1);
              uVar11 = FUN_181bcec50(param_2,param_3,lVar12,0x20,uVar26,0);
              uVar28 = (undefined4)((ulonglong)uVar26 >> 0x20);
              auVar18._8_4_ = (undefined4)extraout_XMM0_Qb;
              auVar18._12_4_ = (undefined4)((ulonglong)extraout_XMM0_Qb >> 0x20);
              if (param_3 != 0) {
                fVar22 = (float)uVar11 * (fVar20 + param_4 + fVar15 * fVar22);
                fVar20 = *(float *)(param_3 + 0x20c);
                if (*(int *)(lRam0000000185830790 + 0xe4) == 0) {
                  FUN_180359940();
                }
                fVar16 = fVar22;
                if (fVar22 <= 0.0) {
                  fVar16 = 0.0;
                }
                auVar18._4_4_ = (int)((ulonglong)uVar11 >> 0x20);
                auVar18._0_4_ = fVar22;
                auVar19._4_12_ = auVar18._4_12_;
                auVar19._0_4_ = fVar22 + *(float *)(param_3 + 0x208);
                fVar21 = (float)FUN_180cae830(auVar19._0_8_);
                fVar1 = *(float *)(param_3 + 0x208);
                FUN_181bcdb50(param_3);
                fVar24 = fVar21;
                if (fVar15 <= fVar21) {
                  fVar24 = fVar16 + fVar20;
                }
                *(float *)(param_3 + 0x20c) = fVar24;
                if (0.0 < fVar22) {
                  if ((*(longlong *)(param_1 + 0x30) != 0) &&
                     (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)) {
                    uVar11 = 0x40;
code_r0x0001813727c3:
                    uVar28 = 0;
                    FUN_181bcd640(param_3,uVar11,lVar14,*(undefined8 *)(lVar13 + 0xb8),0);
                    goto code_r0x0001813727da;
                  }
                }
                else if (0.0 <= fVar22) {
code_r0x0001813727da:
                  fVar21 = fVar21 - fVar1;
                  if (0.0 < fVar21) {
                    if ((*(longlong *)(param_1 + 0x38) != 0) &&
                       (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x1e8), lVar13 != 0))
                    {
                      FUN_181bcafe0(param_3,*(undefined8 *)(lVar13 + 0x280),
                                    *(undefined8 *)(param_1 + 0x30),fVar21,0);
                      if ((*(longlong *)(param_1 + 0x30) != 0) &&
                         (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)
                         ) {
                        uVar11 = 0x43;
code_r0x0001813728f2:
                        uVar28 = 0;
                        FUN_181bcd640(param_3,uVar11,lVar14,*(undefined8 *)(lVar13 + 0xb8),0);
                        goto code_r0x000181372909;
                      }
                    }
                  }
                  else if (0.0 <= fVar21) {
code_r0x000181372909:
                    if ((*(longlong *)(param_1 + 0x30) != 0) &&
                       (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0))
                    {
                      if ((*(uint *)(lVar13 + 0xf0) & 0x200000) == 0) {
code_r0x000181372bfc:
                        if ((*(longlong *)(param_1 + 0x38) != 0) && (lVar14 != 0)) {
                          lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x58);
                          uVar11 = *(undefined8 *)(lVar14 + 0x98);
                          if (cRam0000000185a3e8f5 == '\0') {
                            FUN_18035da50(0x1858316b0);
                            LOCK();
                            UNLOCK();
                            cRam0000000185a3e8f5 = '\x01';
                          }
                          if (*(longlong *)(lVar14 + 0xe8) != 0) {
                            uVar9 = FUN_1800084f0(0xc,uRam00000001858316b0);
                            uVar26 = *(undefined8 *)(param_3 + 0x98);
                            if (cRam0000000185a3e8f5 == '\0') {
                              FUN_18035da50(0x1858316b0);
                              LOCK();
                              UNLOCK();
                              cRam0000000185a3e8f5 = '\x01';
                            }
                            if (*(longlong *)(param_3 + 0xe8) != 0) {
                              uVar10 = FUN_1800084f0(0xc,uRam00000001858316b0);
                              if (((*(longlong *)(param_1 + 0x30) != 0) &&
                                  (lVar14 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38),
                                  lVar14 != 0)) && (lVar13 != 0)) {
                                FUN_181ba6f80(lVar13,uVar11,uVar9,uVar26,CONCAT44(uVar28,uVar10),
                                              *(undefined1 *)(lVar14 + 0x22),fVar21,fVar23,param_5,0
                                             );
                                return;
                              }
                            }
                          }
                        }
                      }
                      else if ((*(longlong *)(param_1 + 0x38) != 0) &&
                              (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x20),
                              lVar13 != 0)) {
                        if (cRam0000000185a3f0bd == '\0') {
                          FUN_18035da50(0x1857a9420);
                          LOCK();
                          UNLOCK();
                          FUN_18035da50(0x185810980);
                          LOCK();
                          UNLOCK();
                          FUN_18035da50(0x1857a9648);
                          LOCK();
                          UNLOCK();
                          FUN_18035da50(0x1857f5d20);
                          LOCK();
                          UNLOCK();
                          FUN_18035da50(0x1857f5d28);
                          LOCK();
                          UNLOCK();
                          cRam0000000185a3f0bd = '\x01';
                        }
                        lVar12 = FUN_1803a51b0(uRam00000001857f5d28);
                        if (lVar12 != 0) {
                          *(longlong *)(lVar12 + 0x10) = param_3;
                          lVar3 = *(longlong *)(lVar13 + 0xb8);
                          uVar11 = FUN_1803a51b0(uRam00000001857a9648);
                          FUN_180e639d0(uVar11,lVar12,uRam00000001857f5d20,0);
                          if (lVar3 != 0) {
                            FUN_1836f60b0(lVar3,uVar11,uRam0000000185810980);
                            lVar3 = lRam00000001857a9420;
                            lVar13 = *(longlong *)(lVar13 + 0xb8);
                            if (lVar13 != 0) {
                              uVar11 = *(undefined8 *)(lVar12 + 0x10);
                              *(int *)(lVar13 + 0x1c) = *(int *)(lVar13 + 0x1c) + 1;
                              lVar12 = *(longlong *)(lVar13 + 0x10);
                              if (lVar12 != 0) {
                                uVar2 = *(uint *)(lVar13 + 0x18);
                                if (uVar2 < *(uint *)(lVar12 + 0x18)) {
                                  *(uint *)(lVar13 + 0x18) = uVar2 + 1;
                                  if (*(uint *)(lVar12 + 0x18) <= uVar2) {
                                    FUN_180352460();
                                    pcVar4 = (code *)swi(3);
                                    (*pcVar4)();
                                    return;
                                  }
                                  *(undefined8 *)(lVar12 + 0x20 + (longlong)(int)uVar2 * 8) = uVar11
                                  ;
                                }
                                else {
                                  FUN_1836f50c0(lVar13,uVar11,
                                                *(undefined8 *)
                                                 (*(longlong *)(*(longlong *)(lVar3 + 0x20) + 0xc0)
                                                 + 0x70));
                                }
                                goto code_r0x000181372bfc;
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  else if (lVar14 != 0) {
                    iVar6 = FUN_181bb9960(lVar14,0);
                    if ((iVar6 == 0) && (iVar6 = FUN_181bb9960(param_3,0), iVar6 == 1)) {
                      if ((*(longlong *)(param_1 + 0x38) == 0) ||
                         (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x60), lVar13 == 0)
                         ) goto code_r0x000181372d04;
                      *(double *)(lVar13 + 0x58) = *(double *)(lVar13 + 0x58) - (double)fVar21;
                    }
                    if ((*(longlong *)(param_1 + 0x38) != 0) &&
                       (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x1e8), lVar13 != 0))
                    {
                      FUN_181bcafe0(param_3,*(undefined8 *)(lVar13 + 0x280),
                                    *(undefined8 *)(param_1 + 0x30),fVar21,0);
                      if ((*(longlong *)(param_1 + 0x30) != 0) &&
                         (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)
                         ) {
                        uVar11 = 0x44;
                        goto code_r0x0001813728f2;
                      }
                    }
                  }
                }
                else if ((*(longlong *)(param_1 + 0x30) != 0) &&
                        (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0))
                {
                  uVar11 = 0x41;
                  goto code_r0x0001813727c3;
                }
              }
            }
          }
        }
      }
      goto code_r0x000181372d04;
    }
    if (cVar5 != '\x02') {
      if (cVar5 != '\x03') {
        uVar11 = FUN_1838da370(uRam000000018580cc10,*(undefined8 *)(lVar12 + 0xa0),
                               uRam00000001857e7270,0);
        FUN_180c9a510(param_1,uVar11,0xffffffff,0);
        return;
      }
      if (((param_3 != 0) && (*(longlong *)(param_3 + 0x78) != 0)) &&
         (lVar13 = *(longlong *)(*(longlong *)(param_3 + 0x78) + 0x18), lVar13 != 0)) {
        fVar15 = (float)FUN_181bbae00(lVar13,0x19,0);
        if (*(longlong *)(param_3 + 0x78) != 0) {
          iVar6 = FUN_181cf95e0(*(longlong *)(param_3 + 0x78),0);
          if ((*(longlong *)(param_1 + 0x30) != 0) &&
             (lVar13 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38), lVar13 != 0)) {
            if ((*(byte *)(lVar13 + 0xf0) & 2) != 0) {
              iVar7 = FUN_181bb9da0(param_3,0);
              param_4 = param_4 * (float)iVar7;
            }
            if (param_2 != 0) {
              uVar28 = 0;
              uVar25 = CONCAT71((int7)(in_stack_fffffffffffffea8 >> 8),3);
              fVar21 = (float)FUN_181bcec50(param_2,param_3,*(undefined8 *)(param_1 + 0x30),0x20,
                                            uVar25,0);
              iVar7 = FUN_181bb9c20(param_3,0);
              if ((*(longlong *)(param_3 + 0x78) != 0) &&
                 (lVar13 = *(longlong *)(*(longlong *)(param_3 + 0x78) + 0x18), lVar13 != 0)) {
                fVar16 = (float)FUN_181bbae00(lVar13,0x19,0);
                if (*(longlong *)(param_3 + 0x78) != 0) {
                  fVar21 = fVar21 * (fVar20 + param_4 + (float)(iVar6 + (int)fVar15) * fVar22);
                  iVar6 = FUN_181cf95e0(*(longlong *)(param_3 + 0x78),0);
                  if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
                    FUN_180359940();
                  }
                  fVar20 = ((float)iVar7 + fVar21) - (float)((int)fVar16 + iVar6);
                  if (0.0 <= fVar20) {
                    fVar23 = fVar20;
                  }
                  uVar11 = *(undefined8 *)(param_1 + 0x30);
                  uVar9 = *(undefined4 *)(param_1 + 0x40);
                  dVar17 = (double)FUN_18000cb30((double)fVar21);
                  uVar32 = (undefined7)((ulonglong)in_stack_fffffffffffffed0 >> 8);
                  uVar31 = (undefined7)((ulonglong)in_stack_fffffffffffffec8 >> 8);
                  uVar30 = (undefined3)((uint)in_stack_fffffffffffffec0 >> 8);
                  uVar29 = (undefined3)((uint)in_stack_fffffffffffffeb8 >> 8);
                  uVar27 = CONCAT44(uVar28,(int)dVar17);
                  uVar25 = uVar25 & 0xffffffff00000000;
                  goto code_r0x000181372beb;
                }
              }
            }
          }
        }
      }
      goto code_r0x000181372d04;
    }
    if (param_3 == 0) goto code_r0x000181372d04;
    uVar11 = *(undefined8 *)(param_3 + 0x100);
    lVar12 = FUN_1803a51b0(uRam000000018580cc20);
    FUN_1836f4d00(lVar12,uVar11,uRam000000018580cc28);
    lVar13 = *(longlong *)(param_3 + 0x110);
    if (lVar13 == 0) goto code_r0x000181372d04;
    uStack_e8 = 0;
    plStack_f0 = (longlong *)((ulonglong)*(uint *)(lVar13 + 0x1c) << 0x20);
    uStack_110 = (longlong)plStack_f0;
    lStack_108 = 0;
    uStack_f8 = 0;
    plStack_f0 = &lStack_118;
    lStack_118 = lVar13;
    lStack_c0 = lVar12;
    while (lStack_118 != 0) {
      if (uStack_110._4_4_ != *(int *)(lStack_118 + 0x1c)) {
code_r0x0001813721b3:
        if ((*(byte *)(*(longlong *)(lRam0000000185806bc8 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(lRam0000000185806bc8 + 0x20));
        }
        if (lStack_118 == 0) goto code_r0x000181372d34;
        if (uStack_110._4_4_ != *(int *)(lStack_118 + 0x1c)) goto code_r0x000181372d2d;
        uStack_110 = CONCAT44(uStack_110._4_4_,*(int *)(lStack_118 + 0x18) + 1);
        lStack_108 = 0;
        if (lVar12 == 0) goto code_r0x000181372d04;
        uStack_e8 = 0;
        plStack_f0 = (longlong *)((ulonglong)*(uint *)(lVar12 + 0x1c) << 0x20);
        uStack_110 = (longlong)plStack_f0;
        lStack_108 = 0;
        uStack_f8 = 0;
        plStack_f0 = &lStack_118;
        lStack_118 = lVar12;
        goto code_r0x0001813722c0;
      }
      if (*(uint *)(lStack_118 + 0x18) <= (uint)uStack_110) goto code_r0x0001813721b3;
      lVar13 = *(longlong *)(lStack_118 + 0x10);
      if (lVar13 == 0) goto code_r0x000181372d28;
      if (*(uint *)(lVar13 + 0x18) <= (uint)uStack_110) goto code_r0x000181372d0a;
      lVar13 = *(longlong *)(lVar13 + 0x20 + (longlong)(int)(uint)uStack_110 * 8);
      uStack_110 = CONCAT44(uStack_110._4_4_,(uint)uStack_110 + 1);
      lStack_108 = lVar13;
      if (lVar13 == 0) goto code_r0x000181372d23;
      if (*(longlong *)(lVar13 + 0x38) == 0) goto code_r0x000181372d1e;
      lVar3 = *(longlong *)(*(longlong *)(lVar13 + 0x38) + 0x70);
      if (lVar3 == 0) goto code_r0x000181372d19;
      uStack_c8 = uRam000000018580cc18;
      if (cRam0000000185a39f9f == '\0') {
        FUN_18035da50(0x185820d20);
        LOCK();
        UNLOCK();
        cRam0000000185a39f9f = '\x01';
      }
      lVar3 = *(longlong *)(lVar3 + 0xa8);
      if (lVar3 == 0) goto code_r0x000181372d14;
      cVar5 = FUN_1800075c0(lVar3,uStack_c8);
      if (cVar5 != '\0') {
        if (lVar12 == 0) goto code_r0x000181372d0f;
        FUN_180029e60(lVar12,lVar13);
      }
    }
  }
  FUN_180352450();
  FUN_180352440();
code_r0x000181372d45:
  FUN_180352460();
code_r0x000181372d4a:
  FUN_180352450();
code_r0x000181372d4f:
  FUN_180352450();
code_r0x000181372d54:
  FUN_180352450();
code_r0x000181372d59:
  FUN_180352450();
code_r0x000181372d5e:
  FUN_180352450();
code_r0x000181372d63:
  FUN_180352450();
code_r0x000181372d68:
  FUN_180352450();
code_r0x000181372d6d:
  FUN_180352450();
code_r0x000181372d72:
  FUN_183aed800(0);
code_r0x000181372d79:
  FUN_180352450();
code_r0x000181372d7e:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
code_r0x0001813722c0:
  uVar28 = (undefined4)(in_stack_fffffffffffffea8 >> 0x20);
  if (lStack_118 != 0) {
    if (uStack_110._4_4_ != *(int *)(lStack_118 + 0x1c)) {
code_r0x000181372441:
      if ((*(byte *)(*(longlong *)(lRam0000000185806bc8 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam0000000185806bc8 + 0x20));
      }
      if (lStack_118 == 0) goto code_r0x000181372d79;
      if (uStack_110._4_4_ != *(int *)(lStack_118 + 0x1c)) goto code_r0x000181372d72;
      uStack_110 = CONCAT44(uStack_110._4_4_,*(int *)(lStack_118 + 0x18) + 1);
      lStack_108 = 0;
      if (cRam0000000185a3e92c == '\0') {
        FUN_18035da50(0x1858316b0);
        LOCK();
        UNLOCK();
        cRam0000000185a3e92c = '\x01';
      }
      if (*(longlong *)(param_3 + 0xe8) == 0) goto code_r0x000181372d04;
      FUN_180005650(6,uRam00000001858316b0);
      goto code_r0x000181372bfc;
    }
    if (*(uint *)(lStack_118 + 0x18) <= (uint)uStack_110) goto code_r0x000181372441;
    lVar13 = *(longlong *)(lStack_118 + 0x10);
    if (lVar13 == 0) goto code_r0x000181372d6d;
    if (*(uint *)(lVar13 + 0x18) <= (uint)uStack_110) goto code_r0x000181372d45;
    lVar13 = *(longlong *)(lVar13 + 0x20 + (longlong)(int)(uint)uStack_110 * 8);
    uStack_110 = CONCAT44(uStack_110._4_4_,(uint)uStack_110 + 1);
    lStack_108 = lVar13;
    if (lVar13 == 0) goto code_r0x000181372d68;
    cVar5 = FUN_181372d90(param_1,*(undefined8 *)(lVar13 + 0x38),0);
    if (cVar5 != '\0') {
      if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x000181372d63;
      lVar12 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38);
      if (lVar12 == 0) goto code_r0x000181372d5e;
      if ((*(byte *)(lVar12 + 0xf0) & 2) == 0) {
        if (*(longlong *)(lVar13 + 0x38) == 0) goto code_r0x000181372d59;
      }
      else if (*(longlong *)(lVar13 + 0x38) == 0) goto code_r0x000181372d4a;
      if (param_2 == 0) goto code_r0x000181372d54;
      in_stack_fffffffffffffea8 = CONCAT71((int7)(in_stack_fffffffffffffea8 >> 8),2);
      FUN_181bcec50(param_2,param_3,*(undefined8 *)(param_1 + 0x30),0x20,in_stack_fffffffffffffea8,0
                   );
      if (*(int *)(lRam0000000185830790 + 0xe4) == 0) {
        FUN_180359940();
      }
      iVar6 = FUN_180caec30();
      if (*(longlong *)(lVar13 + 0x38) == 0) goto code_r0x000181372d4f;
      iVar7 = *(int *)(lVar13 + 0x14);
      iVar6 = iVar6 + iVar7;
      if (((iVar6 < 0) ||
          (iVar8 = (int)*(short *)(*(longlong *)(lVar13 + 0x38) + 0x18), iVar6 <= iVar8)) &&
         (iVar8 = iVar6, iVar6 < 0)) {
        iVar8 = 0;
      }
      *(int *)(lVar13 + 0x14) = iVar8;
      fVar21 = fVar21 + ((float)iVar8 - (float)iVar7);
      fStack_100 = fVar21;
    }
    goto code_r0x0001813722c0;
  }
  goto code_r0x000181372d7e;
}

