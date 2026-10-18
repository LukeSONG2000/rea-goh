// ai_ally_targets VA 0x1816a3d20 RVA 0x16a3d20

void FUN_1816a3d20(longlong param_1,longlong param_2,longlong param_3,longlong param_4,byte param_5)

{
  longlong lVar1;
  code *pcVar2;
  bool bVar3;
  char cVar4;
  longlong lStackX_8;
  longlong lStack_98;
  longlong lStack_90;
  uint uStack_88;
  int iStack_84;
  undefined8 uStack_80;
  undefined8 uStack_78;
  longlong *plStack_70;
  undefined8 uStack_68;
  longlong *plStack_60;
  undefined8 uStack_58;
  longlong *plStack_50;
  undefined8 uStack_48;
  
  if (cRam0000000185a3c850 == '\0') {
    FUN_18035da50(0x18582c590);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c578);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c570);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a93c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c560);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820fd8);
    LOCK();
    UNLOCK();
    cRam0000000185a3c850 = '\x01';
  }
  lStackX_8 = 0;
  lStack_98 = 0;
  lStack_90 = 0;
  uStack_88 = 0;
  iStack_84 = 0;
  uStack_80 = 0;
  if ((*(longlong *)(param_1 + 0x20) == 0) ||
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308), lVar1 == 0)) {
    FUN_180352450();
    pcVar2 = (code *)swi(3);
    (*pcVar2)();
    return;
  }
  lStackX_8 = FUN_18366e0e0(lVar1,uRam00000001857a96f0);
  uStack_78 = 0;
  plStack_70 = &lStackX_8;
  if (param_2 == 0) goto code_r0x0001816a4365;
  if (*(longlong *)(param_2 + 0x38) != 0) {
    lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70);
    if (lVar1 != 0) {
      if (param_3 != 0) {
        FUN_1816a4380(param_1,*(undefined8 *)(lVar1 + 0x68),*(undefined8 *)(param_3 + 0x30),param_3,
                      lStackX_8,0);
        if (lStackX_8 != 0) {
          bVar3 = false;
          if (*(int *)(lStackX_8 + 0x18) != 0) goto code_r0x0001816a3f91;
          if (*(longlong *)(param_2 + 0x38) == 0) {
code_r0x0001816a42dc:
            FUN_180352450();
code_r0x0001816a42e1:
            FUN_180352450();
code_r0x0001816a42e6:
            FUN_180352450();
code_r0x0001816a42ec:
            FUN_180352460();
code_r0x0001816a42f1:
            FUN_180352450();
code_r0x0001816a42f6:
            FUN_183aed800(0);
code_r0x0001816a42fd:
            FUN_180352450();
code_r0x0001816a4302:
            FUN_180352450();
            FUN_180352440();
code_r0x0001816a430e:
            FUN_180352450();
code_r0x0001816a4314:
            FUN_180352450();
code_r0x0001816a4319:
            FUN_180352450();
code_r0x0001816a431e:
            FUN_180352450();
code_r0x0001816a4323:
            FUN_180352450();
code_r0x0001816a4328:
            FUN_180352450();
code_r0x0001816a432d:
            FUN_180352450();
            FUN_180352440();
code_r0x0001816a4338:
            FUN_180352450();
code_r0x0001816a433d:
            FUN_180352450();
code_r0x0001816a4342:
            FUN_180352450();
code_r0x0001816a4347:
            FUN_180352450();
          }
          else {
            lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70);
            if (lVar1 == 0) {
              FUN_180352450();
              goto code_r0x0001816a42dc;
            }
            if ((*(longlong *)(lVar1 + 0x80) != 0) &&
               (*(char *)(*(longlong *)(lVar1 + 0x80) + 0x10) != '\0')) {
              if (lStackX_8 == 0) {
                return;
              }
              FUN_180005650(0,uRam000000018580d620,lStackX_8);
              return;
            }
            if (*(int *)(lStackX_8 + 0x18) == 0) {
              if (*(longlong *)(param_2 + 0x38) != 0) {
                lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70);
                if (lVar1 != 0) {
                  lVar1 = *(longlong *)(lVar1 + 0x68);
                  if (((lVar1 == 0) || (*(int *)(lVar1 + 0x10) == 0)) &&
                     ((lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70),
                      *(longlong *)(lVar1 + 0x80) == 0 ||
                      (*(char *)(*(longlong *)(lVar1 + 0x80) + 0x10) == '\0')))) {
                    FUN_1836f5a70(lStackX_8,*(undefined4 *)(lStackX_8 + 0x18),
                                  *(undefined8 *)(param_3 + 0x30),
                                  *(undefined8 *)
                                   (*(longlong *)(*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0)
                                   + 0x90));
                  }
                  goto code_r0x0001816a3f91;
                }
                goto code_r0x0001816a42e1;
              }
              goto code_r0x0001816a42e6;
            }
code_r0x0001816a3f91:
            if (*(longlong *)(param_2 + 0x38) != 0) {
              lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70);
              if (lVar1 != 0) {
                if (*(longlong *)(lVar1 + 0x80) == 0) {
code_r0x0001816a3fd5:
                  if ((param_5 & bVar3) == 0) {
code_r0x0001816a419d:
                    if (param_4 != 0) {
                      FUN_1836f5a70(param_4,*(undefined4 *)(param_4 + 0x18),lStackX_8,
                                    *(undefined8 *)
                                     (*(longlong *)
                                       (*(longlong *)(lRam00000001857a93c0 + 0x20) + 0xc0) + 0x90));
                      if (lStackX_8 != 0) {
                        FUN_180005650(0,uRam000000018580d620,lStackX_8);
                      }
                      return;
                    }
                    goto code_r0x0001816a430e;
                  }
                  if (*(longlong *)(param_1 + 0x20) != 0) {
                    lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308);
                    if (lVar1 != 0) {
                      lStack_98 = FUN_18366e0e0(lVar1,uRam00000001857a96f0);
                      uStack_68 = 0;
                      plStack_60 = &lStack_98;
                      if (*(longlong *)(param_2 + 0x38) != 0) {
                        lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x70);
                        if (lVar1 != 0) {
                          lVar1 = *(longlong *)(lVar1 + 0x80);
                          if (lVar1 != 0) {
                            lVar1 = *(longlong *)(lVar1 + 0x18);
                            if (lVar1 != 0) {
                              uStack_48 = 0;
                              iStack_84 = *(int *)(lVar1 + 0x1c);
                              uStack_88 = 0;
                              uStack_80 = 0;
                              uStack_58 = 0;
                              plStack_50 = &lStack_90;
                              lStack_90 = lVar1;
                              do {
                                if (lStack_90 == 0) goto code_r0x0001816a4302;
                                if ((iStack_84 != *(int *)(lStack_90 + 0x1c)) ||
                                   (*(uint *)(lStack_90 + 0x18) <= uStack_88)) {
                                  if ((*(byte *)(*(longlong *)(lRam000000018582c578 + 0x20) + 0x135)
                                      & 1) == 0) {
                                    FUN_18034f1d0(*(longlong *)(lRam000000018582c578 + 0x20));
                                  }
                                  if (lStack_90 == 0) goto code_r0x0001816a42fd;
                                  if (iStack_84 != *(int *)(lStack_90 + 0x1c))
                                  goto code_r0x0001816a42f6;
                                  uStack_88 = *(int *)(lStack_90 + 0x18) + 1;
                                  uStack_80 = 0;
                                  break;
                                }
                                lVar1 = *(longlong *)(lStack_90 + 0x10);
                                if (lVar1 == 0) goto code_r0x0001816a42f1;
                                if (*(uint *)(lVar1 + 0x18) <= uStack_88) goto code_r0x0001816a42ec;
                                uStack_80 = *(undefined8 *)
                                             (lVar1 + 0x20 + (longlong)(int)uStack_88 * 8);
                                uStack_88 = uStack_88 + 1;
                                cVar4 = FUN_1816a4380(param_1,uStack_80,lStackX_8,param_3,lStack_98,
                                                      0);
                              } while (cVar4 == '\0');
                              if (lStack_98 != 0) {
                                if (*(int *)(lStack_98 + 0x18) == 0) {
                                  if (lStack_98 != 0) {
                                    FUN_180005650(0,uRam000000018580d620,lStack_98);
                                  }
                                  goto code_r0x0001816a419d;
                                }
                                if (param_4 != 0) {
                                  FUN_1836f5a70(param_4,*(undefined4 *)(param_4 + 0x18),lStack_98,
                                                *(undefined8 *)
                                                 (*(longlong *)
                                                   (*(longlong *)(lRam00000001857a93c0 + 0x20) +
                                                   0xc0) + 0x90));
                                  if (lStack_98 != 0) {
                                    FUN_180005650(0,uRam000000018580d620,lStack_98);
                                  }
                                  if (lStackX_8 == 0) {
                                    return;
                                  }
                                  FUN_180005650(0,uRam000000018580d620,lStackX_8);
                                  return;
                                }
                                goto code_r0x0001816a4314;
                              }
                              goto code_r0x0001816a4319;
                            }
                            goto code_r0x0001816a431e;
                          }
                          goto code_r0x0001816a4323;
                        }
                        goto code_r0x0001816a4328;
                      }
                      goto code_r0x0001816a432d;
                    }
                    goto code_r0x0001816a4338;
                  }
                  goto code_r0x0001816a433d;
                }
                lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x80) + 0x18);
                if (lVar1 != 0) {
                  bVar3 = 0 < *(int *)(lVar1 + 0x18);
                  goto code_r0x0001816a3fd5;
                }
                goto code_r0x0001816a4342;
              }
              goto code_r0x0001816a4347;
            }
          }
          FUN_180352450();
        }
        FUN_180352450();
      }
      FUN_180352450();
    }
    FUN_180352450();
  }
  FUN_180352450();
code_r0x0001816a4365:
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

