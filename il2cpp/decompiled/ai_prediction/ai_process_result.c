// ai_process_result VA 0x1816a3540 RVA 0x16a3540

longlong FUN_1816a3540(longlong param_1,longlong param_2,undefined8 param_3,longlong param_4,
                      undefined2 param_5)

{
  int iVar1;
  code *pcVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 uVar8;
  uint uStack_50;
  
  if (cRam0000000185a3c84d == '\0') {
    FUN_18035da50(0x1857a48e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bf0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806bb0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc30);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a48e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ba8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4900);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580fd18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a48f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a4908);
    LOCK();
    UNLOCK();
    cRam0000000185a3c84d = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam00000001857a4908);
  if (lVar4 != 0) {
    *(longlong *)(lVar4 + 0x10) = param_2;
    if (param_2 == 0) {
      return 0;
    }
    if (*(char *)(param_2 + 0x24) != '\0') {
      lVar4 = FUN_1816a3bd0(param_1,param_4,param_5,0);
      return lVar4;
    }
    if (*(longlong *)(param_2 + 0x10) != 0) {
      lVar4 = FUN_1816a30c0(param_1,*(undefined8 *)(param_2 + 0x10),param_4,0);
      return lVar4;
    }
    if (param_4 != 0) {
      lVar6 = *(longlong *)(param_4 + 0x38);
      uVar5 = FUN_1803a51b0(uRam000000018580fd18);
      FUN_180e639d0(uVar5,lVar4,uRam00000001857a48f0,0);
      if (lVar6 != 0) {
        lVar6 = FUN_1836f55d0(lVar6,uVar5,uRam00000001857a48e0);
        if (*(longlong *)(param_4 + 0x40) == 0) {
code_r0x0001816a3734:
          if (lVar6 != 0) {
code_r0x0001816a373d:
            iVar1 = *(int *)(lVar6 + 0x1c);
            uStack_50 = 0;
            while (lVar6 != 0) {
              if ((iVar1 != *(int *)(lVar6 + 0x1c)) || (*(uint *)(lVar6 + 0x18) <= uStack_50)) {
                if ((*(byte *)(*(longlong *)(lRam0000000185806bc8 + 0x20) + 0x135) & 1) == 0) {
                  FUN_18034f1d0(*(longlong *)(lRam0000000185806bc8 + 0x20));
                }
                if (lVar6 == 0) goto code_r0x0001816a39c9;
                if (iVar1 == *(int *)(lVar6 + 0x1c)) {
                  return 0;
                }
                goto code_r0x0001816a39c2;
              }
              lVar7 = *(longlong *)(lVar6 + 0x10);
              if (lVar7 == 0) goto code_r0x0001816a39bd;
              if (*(uint *)(lVar7 + 0x18) <= uStack_50) goto code_r0x0001816a3995;
              uVar5 = *(undefined8 *)(lVar7 + 0x20 + (longlong)(int)uStack_50 * 8);
              uStack_50 = uStack_50 + 1;
              if (*(longlong *)(param_1 + 0x20) == 0) goto code_r0x0001816a39b8;
              lVar7 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308);
              if (lVar7 == 0) goto code_r0x0001816a39b3;
              lVar7 = FUN_18366e0e0(lVar7,uRam00000001857a96f0);
              if (*(longlong *)(lVar4 + 0x10) == 0) goto code_r0x0001816a39ae;
              FUN_1816a4540(param_1,uVar5,param_4,
                            *(undefined4 *)(*(longlong *)(lVar4 + 0x10) + 0x20),param_3,lVar7,
                            param_5._1_1_,0);
              if (lVar7 == 0) goto code_r0x0001816a39a9;
              if (*(int *)(lVar7 + 0x18) != 0) {
                if (*(longlong *)(param_1 + 0x20) == 0) goto code_r0x0001816a39a4;
                lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x20) + 0x308);
                if (lVar4 == 0) goto code_r0x0001816a399f;
                uVar8 = FUN_18366e0e0(lVar4,uRam00000001857a96f0);
                FUN_1816a3d20(param_1,uVar5,param_4,uVar8,(undefined1)param_5,0);
                lVar4 = FUN_1803a51b0(uRam00000001857a48e8);
                if (lVar4 != 0) {
                  *(undefined8 *)(lVar4 + 0x18) = uVar5;
                  *(longlong *)(lVar4 + 0x10) = lVar7;
                  *(undefined8 *)(lVar4 + 0x20) = uVar8;
                  return lVar4;
                }
                goto code_r0x0001816a399a;
              }
              if (*(longlong *)(lVar7 + 0x28) != 0) {
                FUN_18366e200(*(longlong *)(lVar7 + 0x28),lVar7,
                              *(undefined8 *)
                               (*(longlong *)(*(longlong *)(lRam00000001857a4900 + 0x20) + 0xc0) +
                               0x20));
              }
            }
            goto code_r0x0001816a39ce;
          }
        }
        else if ((*(longlong *)(lVar4 + 0x10) != 0) &&
                (lVar7 = *(longlong *)(*(longlong *)(param_4 + 0x40) + 0x38), lVar7 != 0)) {
          cVar3 = FUN_180febd70(lVar7,*(undefined8 *)(*(longlong *)(lVar4 + 0x10) + 0x18),0);
          if (cVar3 == '\0') goto code_r0x0001816a3734;
          if (lVar6 == 0) goto code_r0x0001816a398f;
          FUN_180029e60(lVar6,*(undefined8 *)(param_4 + 0x40));
          goto code_r0x0001816a373d;
        }
      }
    }
  }
code_r0x0001816a398f:
  FUN_180352450();
code_r0x0001816a3995:
  FUN_180352460();
code_r0x0001816a399a:
  FUN_180352450();
code_r0x0001816a399f:
  FUN_180352450();
code_r0x0001816a39a4:
  FUN_180352450();
code_r0x0001816a39a9:
  FUN_180352450();
code_r0x0001816a39ae:
  FUN_180352450();
code_r0x0001816a39b3:
  FUN_180352450();
code_r0x0001816a39b8:
  FUN_180352450();
code_r0x0001816a39bd:
  FUN_180352450();
code_r0x0001816a39c2:
  FUN_183aed800(0);
code_r0x0001816a39c9:
  FUN_180352450();
code_r0x0001816a39ce:
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  lVar4 = (*pcVar2)();
  return lVar4;
}

