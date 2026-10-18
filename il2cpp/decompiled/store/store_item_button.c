// store_item_button VA 0x181c98b00 RVA 0x1c98b00

void FUN_181c98b00(longlong param_1,uint param_2)

{
  char cVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined *puVar10;
  
  if (cRam0000000185a3ee5b == '\0') {
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e1748);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d95b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857946b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e1728);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e1708);
    LOCK();
    UNLOCK();
    cRam0000000185a3ee5b = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam00000001857e1708);
  if (lVar4 != 0) {
    *(longlong *)(lVar4 + 0x10) = param_1;
    *(uint *)(lVar4 + 0x18) = param_2;
    switch(*(undefined4 *)(param_1 + 0x360)) {
    case 0:
    case 3:
    case 6:
    case 8:
    case 9:
    case 10:
      if (*(longlong *)(param_1 + 0x2f0) != 0) {
        lVar4 = *(longlong *)(param_1 + 0x368);
        if (lVar4 != 0) {
          if (*(uint *)(lVar4 + 0x18) <= param_2) {
            FUN_183aeda10(0);
            pcVar3 = (code *)swi(3);
            (*pcVar3)();
            return;
          }
          lVar4 = *(longlong *)(lVar4 + 0x10);
          if (lVar4 != 0) {
            if (param_2 < *(uint *)(lVar4 + 0x18)) {
              lVar5 = *(longlong *)(param_1 + 0x2f0);
              (**(code **)(lVar5 + 0x18))
                        (*(undefined8 *)(lVar5 + 0x40),
                         *(undefined8 *)(lVar4 + 0x20 + (longlong)(int)param_2 * 8),
                         *(undefined8 *)(lVar5 + 0x28));
              return;
            }
code_r0x000181c98e64:
            FUN_180352460();
            pcVar3 = (code *)swi(3);
            (*pcVar3)();
            return;
          }
        }
        goto code_r0x000181c98e5e;
      }
      break;
    case 2:
      if (-1 < (int)param_2) {
        lVar5 = *(longlong *)(param_1 + 0x378);
        if (lVar5 != 0) {
          if (*(int *)(lVar5 + 0x18) <= (int)param_2) {
            return;
          }
          lVar5 = FUN_180007510(lVar5,param_2);
          if (lVar5 != 0) {
            if (*(char *)(lVar5 + 0x108) == '\0') {
              FUN_181c98f90(param_1,*(undefined4 *)(lVar4 + 0x18),0);
              return;
            }
            lVar5 = *(longlong *)(param_1 + 0x2d8);
            lVar2 = *(longlong *)(param_1 + 0x60);
            if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
              FUN_180359940(lRam00000001857ff558);
            }
            if ((lVar2 != 0) &&
               (lVar7 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x1800),
               lVar7 != 0)) {
              uVar6 = FUN_180ca5830(lVar7,0);
              uVar6 = FUN_181164190(lVar2,uVar6,0,0,0);
              if (*(longlong *)(param_1 + 0x378) != 0) {
                lVar2 = *(longlong *)(param_1 + 0x60);
                lVar7 = FUN_180007510(*(longlong *)(param_1 + 0x378),*(undefined4 *)(lVar4 + 0x18));
                if ((lVar7 != 0) && (lVar2 != 0)) {
                  uVar8 = FUN_181164190(lVar2,*(undefined8 *)(lVar7 + 0x100),0,0,0);
                  lVar7 = FUN_1803a51b0(uRam000000018580d858);
                  lVar2 = lRam00000001857e1728;
                  puVar10 = *(undefined **)(lRam00000001857e1728 + 8);
                  *(undefined **)(lVar7 + 0x10) = puVar10;
                  *(longlong *)(lVar7 + 0x28) = lVar2;
                  *(longlong *)(lVar7 + 0x20) = lVar4;
                  cVar1 = *(char *)(lVar2 + 0x52);
                  *(longlong *)(lVar7 + 0x40) = lVar7;
                  if (((*(byte *)(lVar2 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
                    *(longlong *)(lVar7 + 0x40) = lVar4;
                  }
                  else {
                    puVar10 = &UNK_180006440;
                  }
                  *(undefined **)(lVar7 + 0x18) = puVar10;
                  *(code **)(lVar7 + 0x38) = FUN_1800063e0;
                  if (*(longlong *)(param_1 + 0x378) != 0) {
                    lVar2 = *(longlong *)(param_1 + 0x2c0);
                    lVar4 = FUN_180007510(*(longlong *)(param_1 + 0x378),
                                          *(undefined4 *)(lVar4 + 0x18));
                    if ((lVar4 != 0) && (lVar4 = *(longlong *)(lVar4 + 0xd0), lVar4 != 0)) {
                      if (*(int *)(lVar4 + 0x18) == 0) goto code_r0x000181c98e64;
                      if ((lVar2 != 0) &&
                         (uVar9 = FUN_181c92ea0(lVar2,*(undefined8 *)(lVar4 + 0x20),0), lVar5 != 0))
                      {
                        FUN_18126b8f0(lVar5,uVar6,uVar8,lVar7,0,uVar9,0,0,0,0,1,0,0);
                        return;
                      }
                    }
                  }
                }
              }
            }
          }
        }
        goto code_r0x000181c98e5e;
      }
    }
    return;
  }
code_r0x000181c98e5e:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  (*pcVar3)();
  return;
}

