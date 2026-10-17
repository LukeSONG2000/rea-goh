// query_count VA 0x18136f1b0 RVA 0x136f1b0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_18136f1b0(longlong param_1,undefined8 param_2,longlong param_3,longlong param_4,
                  undefined8 param_5,longlong param_6,undefined8 param_7,undefined8 param_8,
                  longlong param_9)

{
  longlong lVar1;
  code *pcVar2;
  float fVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  int iVar7;
  float fVar8;
  longlong lStack_100;
  longlong lStack_f8;
  uint uStack_f0;
  int iStack_ec;
  longlong lStack_e8;
  longlong lStack_e0;
  longlong lStack_d8;
  undefined8 uStack_d0;
  longlong *plStack_c8;
  undefined8 uStack_c0;
  longlong *plStack_b8;
  undefined8 uStack_b0;
  longlong lStack_a8;
  longlong lStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  longlong *plStack_88;
  longlong lStack_80;
  undefined4 uStack_78;
  uint uStack_74;
  
  if (cRam0000000185a3b374 == '\0') {
    FUN_18035da50(0x18580cf40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d270);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d2c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c590);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c578);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c570);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858062b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c560);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d0d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858062c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858062b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc38);
    LOCK();
    UNLOCK();
    cRam0000000185a3b374 = '\x01';
  }
  lStack_f8 = 0;
  uStack_f0 = 0;
  iStack_ec = 0;
  lStack_e8 = 0;
  lStack_100 = 0;
  lStack_a8 = 0;
  lStack_a0 = 0;
  uStack_98 = 0;
  if (param_4 != 0) {
    if (cRam0000000185a3b34c == '\0') {
      FUN_18035da50(0x18580d398);
      LOCK();
      UNLOCK();
      cRam0000000185a3b34c = '\x01';
    }
    if (((*(longlong *)(param_4 + 0x10) != 0) &&
        (lVar5 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar5 != 0)) &&
       (lVar5 = FUN_1832a2dd0(lVar5,uRam000000018580d398), fVar3 = _UNK_18529ef3c, param_3 != 0)) {
      uStack_b0 = 0;
      iStack_ec = *(int *)(param_3 + 0x1c);
      uStack_f0 = 0;
      lStack_e8 = 0;
      uStack_c0 = 0;
      plStack_b8 = &lStack_f8;
      lStack_f8 = param_3;
      lStack_e0 = lVar5;
      while (lStack_f8 != 0) {
        if ((iStack_ec != *(int *)(lStack_f8 + 0x1c)) || (*(uint *)(lStack_f8 + 0x18) <= uStack_f0))
        {
          if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
          }
          if (lStack_f8 == 0) goto code_r0x00018136f8e1;
          if (iStack_ec == *(int *)(lStack_f8 + 0x1c)) {
            _uStack_f0 = CONCAT44(iStack_ec,*(int *)(lStack_f8 + 0x18) + 1);
            lStack_e8 = 0;
            FUN_181360310(param_4,lVar5,0);
            if (param_9 != 0) {
              (**(code **)(param_9 + 0x18))
                        (*(undefined8 *)(param_9 + 0x40),*(undefined8 *)(param_9 + 0x28));
            }
            return;
          }
          goto code_r0x00018136f8da;
        }
        lVar1 = *(longlong *)(lStack_f8 + 0x10);
        if (lVar1 == 0) goto code_r0x00018136f8d5;
        if (*(uint *)(lVar1 + 0x18) <= uStack_f0) goto code_r0x00018136f86e;
        lVar1 = *(longlong *)(lVar1 + 0x20 + (longlong)(int)uStack_f0 * 8);
        uStack_f0 = uStack_f0 + 1;
        lStack_e8 = lVar1;
        if (param_6 == 0) goto code_r0x00018136f8d0;
        lStack_d8 = lVar1;
        cVar4 = FUN_1833355c0(param_6,lVar1,uRam000000018580d270);
        if (cVar4 == '\0') {
          if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x00018136f8c6;
          lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38);
          if (lVar6 == 0) goto code_r0x00018136f8c1;
          if ((*(uint *)(lVar6 + 0xf0) & 0x400000) == 0) {
            if (lVar5 == 0) goto code_r0x00018136f8bc;
            FUN_18335f400(lVar5,lVar1,fVar3,1,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110
                           ));
          }
          else {
            if (lVar5 == 0) goto code_r0x00018136f8b7;
            FUN_18335f400(lVar5,lVar1,0,1,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110
                           ));
            if (*(longlong *)(param_1 + 0x38) == 0) goto code_r0x00018136f8b2;
            lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x38) + 0x318);
            if (lVar6 == 0) goto code_r0x00018136f8ad;
            lStack_100 = FUN_18366e0e0(lVar6,uRam00000001858062b8);
            uStack_d0 = 0;
            plStack_c8 = &lStack_100;
            if (lVar1 == 0) goto code_r0x00018136f8a2;
            if (*(longlong *)(lVar1 + 0x60) == 0) goto code_r0x00018136f89d;
            FUN_181a6eb30(*(longlong *)(lVar1 + 0x60),lStack_100,0);
            iVar7 = 0;
            while( true ) {
              if (lStack_100 == 0) goto code_r0x00018136f898;
              if (*(int *)(lStack_100 + 0x18) <= iVar7) break;
              lVar6 = FUN_180007510(lStack_100,iVar7);
              if (lVar6 == 0) goto code_r0x00018136f893;
              if (*(longlong *)(lVar6 + 0x38) == 0) goto code_r0x00018136f88e;
              lVar6 = *(longlong *)(*(longlong *)(lVar6 + 0x38) + 0xb8);
              if (lVar6 == 0) goto code_r0x00018136f889;
              uStack_78 = 0;
              uStack_74 = *(uint *)(lVar6 + 0x1c);
              lStack_a0 = (ulonglong)uStack_74 << 0x20;
              uStack_98 = 0;
              uStack_90 = 0;
              plStack_88 = &lStack_a8;
              lStack_a8 = lVar6;
              lStack_80 = lVar6;
              do {
                cVar4 = FUN_1833f1b60(&lStack_a8,uRam000000018582c578);
                if (cVar4 == '\0') goto code_r0x00018136f692;
                if (*(longlong *)(param_1 + 0x30) == 0) goto code_r0x00018136f87e;
                lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38);
                if (lVar6 == 0) goto code_r0x00018136f879;
                lVar6 = *(longlong *)(lVar6 + 0xc0);
                if (lVar6 == 0) goto code_r0x00018136f874;
              } while ((*(int *)(lVar6 + 0x18) != 0) &&
                      (cVar4 = FUN_180029df0(*(undefined8 *)
                                              (*(longlong *)(*(longlong *)(param_1 + 0x30) + 0x38) +
                                              0xc0),uStack_98), cVar4 == '\0'));
              fVar8 = (float)FUN_18335ecf0(lVar5,lVar1,uRam000000018580cf40);
              FUN_18335f400(lVar5,lVar1,fVar8 + fVar3,1,
                            *(undefined8 *)
                             (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) +
                             0x110));
code_r0x00018136f692:
              iVar7 = iVar7 + 1;
            }
            if (lStack_100 != 0) {
              FUN_180005650(0,uRam000000018580d620,lStack_100);
            }
          }
        }
        else {
          if (lVar5 == 0) goto code_r0x00018136f8cb;
          FUN_18335f400(lVar5,lVar1,0,1,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110))
          ;
        }
      }
      goto code_r0x00018136f8e6;
    }
  }
  FUN_180352450();
code_r0x00018136f86e:
  FUN_180352460();
code_r0x00018136f874:
  FUN_180352450();
code_r0x00018136f879:
  FUN_180352450();
code_r0x00018136f87e:
  FUN_180352450();
  FUN_180352440();
code_r0x00018136f889:
  FUN_180352450();
code_r0x00018136f88e:
  FUN_180352450();
code_r0x00018136f893:
  FUN_180352450();
code_r0x00018136f898:
  FUN_180352450();
code_r0x00018136f89d:
  FUN_180352450();
code_r0x00018136f8a2:
  FUN_180352450();
  FUN_180352440();
code_r0x00018136f8ad:
  FUN_180352450();
code_r0x00018136f8b2:
  FUN_180352450();
code_r0x00018136f8b7:
  FUN_180352450();
code_r0x00018136f8bc:
  FUN_180352450();
code_r0x00018136f8c1:
  FUN_180352450();
code_r0x00018136f8c6:
  FUN_180352450();
code_r0x00018136f8cb:
  FUN_180352450();
code_r0x00018136f8d0:
  FUN_180352450();
code_r0x00018136f8d5:
  FUN_180352450();
code_r0x00018136f8da:
  FUN_183aed800(0);
code_r0x00018136f8e1:
  FUN_180352450();
code_r0x00018136f8e6:
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

