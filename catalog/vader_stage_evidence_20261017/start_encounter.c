// start_encounter VA 0x181930eb0 RVA 0x1930eb0

void FUN_181930eb0(longlong param_1,undefined8 param_2)

{
  char cVar1;
  code *pcVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  longlong lVar10;
  undefined8 uVar11;
  longlong lVar12;
  ulonglong uVar13;
  ulonglong uVar14;
  undefined *puVar15;
  uint uVar16;
  ulonglong uVar17;
  bool bVar18;
  undefined4 auStackX_20 [2];
  
  if (cRam0000000185a3d8cc == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fdd48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c2a80);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3910);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18578c2a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3928);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3920);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810488);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185784f40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185784f48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185784f50);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185784f58);
    LOCK();
    UNLOCK();
    cRam0000000185a3d8cc = '\x01';
  }
  lVar7 = FUN_1803a51b0(uRam0000000185784f48);
  if (lVar7 == 0) goto code_r0x0001819317b4;
  *(longlong *)(lVar7 + 0x10) = param_1;
  *(undefined8 *)(lVar7 + 0x18) = param_2;
  if ((*(longlong *)(param_1 + 0x78) == 0) ||
     (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x10), lVar9 == 0))
  goto code_r0x0001819317b4;
  FUN_18000a350(9,uRam00000001857a3928,lVar9,0);
  if ((*(longlong *)(param_1 + 0x78) == 0) ||
     (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x20), lVar9 == 0))
  goto code_r0x0001819317b4;
  if (cRam0000000185a3f0b3 == '\0') {
    FUN_18035da50(0x1857a4808);
    LOCK();
    UNLOCK();
    cRam0000000185a3f0b3 = '\x01';
  }
  lVar9 = *(longlong *)(lVar9 + 0xa0);
  if (lVar9 == 0) goto code_r0x0001819317b4;
  iVar3 = *(int *)(lVar9 + 0x18);
  uVar13 = 0;
  *(int *)(lVar9 + 0x1c) = *(int *)(lVar9 + 0x1c) + 1;
  *(undefined4 *)(lVar9 + 0x18) = 0;
  if (0 < iVar3) {
    FUN_183af4330(*(undefined8 *)(lVar9 + 0x10),0,iVar3,0);
  }
  *(undefined4 *)(param_1 + 0x88) = 1;
  if ((*(longlong *)(param_1 + 0x78) == 0) ||
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x1a8) == 0)) goto code_r0x0001819317b4;
  uVar8 = FUN_1800144c0();
  if ((*(longlong *)(param_1 + 0x78) == 0) ||
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) == 0)) goto code_r0x0001819317b4;
  lVar9 = FUN_1800084f0(6,uRam00000001857a3910);
  if (((lVar9 == 0) || (*(int *)(lVar9 + 0x10) == 0)) ||
     (iVar3 = FUN_183addc20(lVar9 + 0x14,0x3a,*(undefined4 *)(lVar9 + 0x10),0), iVar3 < 1)) {
    uVar14 = 0xffffffff;
  }
  else {
    uVar14 = uVar13;
    if (iVar3 <= *(int *)(lVar9 + 0x10)) {
      uVar17 = uVar13;
      do {
        if ((longlong)*(int *)(lVar9 + 0x10) <= (longlong)uVar17) {
          FUN_183aed040(0,uVar14);
          pcVar2 = (code *)swi(3);
          (*pcVar2)();
          return;
        }
        uVar16 = (int)uVar17 + 1;
        uVar14 = (ulonglong)
                 ((uint)*(ushort *)(lVar9 + 0x14 + uVar17 * 2) + ((int)uVar14 * 5 + -0x18) * 2);
        uVar17 = (ulonglong)uVar16;
      } while ((int)uVar16 < iVar3);
    }
  }
  lVar9 = FUN_180f97990(uVar8,uVar14,0);
  if ((*(longlong *)(param_1 + 0x78) == 0) ||
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) == 0)) goto code_r0x0001819317b4;
  lVar10 = FUN_1800084f0(6,uRam00000001857a3910);
  if (lVar9 == lVar10) goto code_r0x00018193121e;
  if (((lVar9 != 0) && (lVar10 != 0)) && (*(int *)(lVar9 + 0x10) == *(int *)(lVar10 + 0x10))) {
    uVar14 = (longlong)*(int *)(lVar9 + 0x10) * 2;
    if (uVar14 < 8) {
      if (uVar14 != 0) {
        do {
          if (*(char *)(lVar9 + 0x14 + uVar13) != *(char *)(lVar10 + 0x14 + uVar13))
          goto code_r0x00018193170c;
          uVar13 = uVar13 + 1;
        } while (uVar13 < uVar14);
      }
      bVar18 = true;
    }
    else {
      if (uVar14 != 8) {
        do {
          if (*(longlong *)(lVar9 + 0x14 + uVar13) != *(longlong *)(lVar10 + 0x14 + uVar13))
          goto code_r0x00018193170c;
          uVar13 = uVar13 + 8;
        } while (uVar13 < uVar14 - 8);
      }
      bVar18 = *(longlong *)(uVar14 + 0xc + lVar9) == *(longlong *)(uVar14 + 0xc + lVar10);
    }
    goto code_r0x000181931216;
  }
code_r0x000181931312:
  if ((*(longlong *)(param_1 + 0x78) != 0) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) != 0)) {
    FUN_1800084f0(0xb,uRam00000001857a3910);
    if ((*(longlong *)(param_1 + 0x78) != 0) &&
       (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x1f0) != 0)) {
      uVar8 = 0;
      FUN_1800342a0();
      if (((*(longlong *)(param_1 + 0x78) != 0) &&
          (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x20), lVar9 != 0)) &&
         (lVar9 = *(longlong *)(lVar9 + 0x70), lVar9 != 0)) {
        FUN_181baf120(lVar9,0,0,0,uVar8);
        uVar6 = (undefined4)((ulonglong)uVar8 >> 0x20);
        if ((*(longlong *)(param_1 + 0x78) != 0) &&
           (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) != 0)) {
          uVar4 = FUN_1800084f0(8,uRam00000001857a3910);
          lVar9 = *(longlong *)(param_1 + 0x78);
          if ((lVar9 != 0) && (*(longlong *)(lVar9 + 0xa8) != 0)) {
            lVar9 = *(longlong *)(lVar9 + 0x180);
            uVar8 = FUN_1800084f0(6,uRam00000001857a3910);
            uVar5 = FUN_180f974b0(uVar8,0);
            if (lVar9 != 0) {
              FUN_18001d200(0x5a,uRam00000001857a96a0,lVar9,uVar5,CONCAT44(uVar6,uVar4));
              if ((*(longlong *)(param_1 + 0x78) != 0) &&
                 (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x180) != 0)) {
                FUN_180005650(0x1d,uRam00000001857a96a0);
                if ((*(longlong *)(param_1 + 0x78) != 0) &&
                   (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) != 0)) {
                  FUN_180005650(0x40,uRam00000001857a3910);
                  if ((*(longlong *)(param_1 + 0x78) != 0) &&
                     (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x1c0), lVar9 != 0)) {
                    if (*(longlong *)(lVar9 + 0x120) != 0) {
                      if (*(longlong *)(lVar9 + 0x50) == 0) goto code_r0x0001819317b4;
                      if (*(int *)(*(longlong *)(lVar9 + 0x120) + 0xf0) <
                          *(int *)(*(longlong *)(lVar9 + 0x50) + 0xd1c)) {
                        if (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x10) == 0)
                        goto code_r0x0001819317b4;
                        lVar9 = FUN_180014310();
                        if (*(int *)(lRam00000001857fdd48 + 0xe4) == 0) {
                          FUN_180359940();
                        }
                        if (cRam0000000185a3b4d5 == '\0') {
                          FUN_18035da50(0x1857fdd48);
                          LOCK();
                          UNLOCK();
                          cRam0000000185a3b4d5 = '\x01';
                        }
                        if (*(int *)(lRam00000001857fdd48 + 0xe4) == 0) {
                          FUN_180359940();
                        }
                        if (lVar9 != 0) {
                          if ((*(longlong *)(param_1 + 0x78) == 0) ||
                             (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) == 0))
                          goto code_r0x0001819317b4;
                          lVar9 = *(longlong *)(param_1 + 0x10);
                          uVar8 = FUN_1800084f0(6,uRam00000001857a3910);
                          auStackX_20[0] = FUN_180f974b0(uVar8,0);
                          uVar8 = FUN_1803a4e80(uRam0000000185a33e38,auStackX_20);
                          if ((*(longlong *)(param_1 + 0x78) == 0) ||
                             ((*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x10) == 0 ||
                              (lVar10 = FUN_180014310(), lVar10 == 0)))) goto code_r0x0001819317b4;
                          uVar11 = FUN_1813aead0(lVar10,0);
                          uVar8 = FUN_1838daaa0(uRam0000000185784f58,uVar8,uVar11,0);
                          if (lVar9 == 0) goto code_r0x0001819317b4;
                          FUN_1818b9ab0(lVar9,uVar8,0);
                        }
                      }
                    }
                    if ((*(longlong *)(param_1 + 0x78) != 0) &&
                       (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x200), lVar9 != 0)) {
                      FUN_18000a130(1,uRam0000000185810488,lVar9,0xc);
                      if ((*(longlong *)(param_1 + 0x78) != 0) &&
                         (lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x108), lVar9 != 0))
                      {
                        FUN_181bb3a20(lVar9,0);
                        if (*(char *)(param_1 + 0xa2) == '\0') {
                          *(undefined1 *)(param_1 + 0xa2) = 1;
                          if ((*(longlong *)(param_1 + 0x78) == 0) ||
                             (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) == 0))
                          goto code_r0x0001819317b4;
                          FUN_180005650(0x2e,uRam00000001857a3910);
                          lVar9 = *(longlong *)(param_1 + 0xd8);
                          if (lVar9 != 0) {
                            (**(code **)(lVar9 + 0x18))
                                      (*(undefined8 *)(lVar9 + 0x40),*(undefined8 *)(lVar9 + 0x28));
                          }
                        }
                        if (*(longlong *)(param_1 + 0x78) != 0) {
                          lVar9 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0x18);
                          lVar12 = FUN_1803a51b0(uRam000000018580d858);
                          lVar10 = lRam0000000185784f40;
                          puVar15 = *(undefined **)(lRam0000000185784f40 + 8);
                          *(undefined **)(lVar12 + 0x10) = puVar15;
                          *(longlong *)(lVar12 + 0x28) = lVar10;
                          *(longlong *)(lVar12 + 0x20) = lVar7;
                          cVar1 = *(char *)(lVar10 + 0x52);
                          *(longlong *)(lVar12 + 0x40) = lVar12;
                          if (((*(byte *)(lVar10 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
                            *(longlong *)(lVar12 + 0x40) = lVar7;
                          }
                          else {
                            puVar15 = &UNK_180006440;
                          }
                          *(undefined **)(lVar12 + 0x18) = puVar15;
                          *(code **)(lVar12 + 0x38) = FUN_1800063e0;
                          if (lVar9 != 0) {
                            *(longlong *)(lVar9 + 0x30) = lVar12;
                            if ((*(longlong *)(param_1 + 0x78) != 0) &&
                               (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) != 0)) {
                              FUN_180005650(0x30,uRam00000001857a3910);
                              if ((*(longlong *)(param_1 + 0x78) != 0) &&
                                 (lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8),
                                 lVar7 != 0)) {
                                uVar6 = FUN_18000a130(0x33,uRam00000001857a3910,lVar7,1);
                                *(undefined4 *)(param_1 + 0x84) = uVar6;
                                lVar7 = *(longlong *)(param_1 + 0xf0);
                                if (lVar7 != 0) {
                                  (**(code **)(lVar7 + 0x18))
                                            (*(undefined8 *)(lVar7 + 0x40),
                                             *(undefined8 *)(lVar7 + 0x28));
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
code_r0x0001819317b4:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
code_r0x00018193170c:
  bVar18 = false;
code_r0x000181931216:
  if (bVar18) {
code_r0x00018193121e:
    lVar9 = *(longlong *)(param_1 + 0x78);
    if (((lVar9 == 0) || (*(longlong *)(lVar9 + 0xa8) == 0)) ||
       (lVar9 = *(longlong *)(lVar9 + 0x1f0), FUN_1800084f0(9,uRam00000001857a3910), lVar9 == 0))
    goto code_r0x0001819317b4;
    FUN_1800342a0();
    if ((*(longlong *)(param_1 + 0x78) == 0) ||
       (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0xa8) == 0)) goto code_r0x0001819317b4;
    FUN_1800084f0(10,uRam00000001857a3910);
    if ((*(longlong *)(param_1 + 0x78) == 0) ||
       (*(longlong *)(*(longlong *)(param_1 + 0x78) + 0x1f0) == 0)) goto code_r0x0001819317b4;
    FUN_1800342a0();
    lVar9 = *(longlong *)(param_1 + 0x78);
    if (((lVar9 == 0) || (*(longlong *)(lVar9 + 0xb0) == 0)) ||
       (lVar9 = *(longlong *)(lVar9 + 0x1f0), FUN_1800084f0(4,uRam000000018578c2a8), lVar9 == 0))
    goto code_r0x0001819317b4;
    FUN_1800342a0();
  }
  goto code_r0x000181931312;
}

