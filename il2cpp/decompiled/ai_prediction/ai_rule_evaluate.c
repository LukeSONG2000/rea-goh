// ai_rule_evaluate VA 0x181bb0200 RVA 0x1bb0200

void FUN_181bb0200(undefined8 param_1,longlong param_2,longlong param_3,longlong param_4)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  char cVar5;
  undefined4 uVar6;
  longlong lVar7;
  longlong lVar8;
  uint uStack_38;
  
  if (cRam0000000185a3e8b8 == '\0') {
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9420);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a96e0);
    LOCK();
    UNLOCK();
    cRam0000000185a3e8b8 = '\x01';
  }
  if ((param_3 != 0) && (*(longlong *)(param_3 + 0x10) != 0)) {
    if (*(int *)(*(longlong *)(param_3 + 0x10) + 0x18) != 0) {
      lVar7 = FUN_181bb0ec0(param_1,param_2,param_3,0);
      lVar2 = *(longlong *)(param_3 + 0x10);
      if (lVar2 == 0) {
code_r0x000181bb0648:
        FUN_180352450();
        goto code_r0x000181bb064e;
      }
      iVar1 = *(int *)(lVar2 + 0x1c);
      uStack_38 = 0;
code_r0x000181bb0350:
      do {
        do {
          if (lVar2 == 0) goto code_r0x000181bb0636;
          if ((iVar1 != *(int *)(lVar2 + 0x1c)) || (*(uint *)(lVar2 + 0x18) <= uStack_38)) {
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (lVar2 != 0) {
              if (iVar1 == *(int *)(lVar2 + 0x1c)) {
                if (lVar7 == 0) {
                  return;
                }
                FUN_180005650(0,uRam000000018580d620,lVar7);
                return;
              }
              goto code_r0x000181bb062a;
            }
            goto code_r0x000181bb0631;
          }
          lVar3 = *(longlong *)(lVar2 + 0x10);
          if (lVar3 == 0) goto code_r0x000181bb0625;
          if (*(uint *)(lVar3 + 0x18) <= uStack_38) {
            FUN_180352460();
code_r0x000181bb060c:
            FUN_180352450();
code_r0x000181bb0611:
            FUN_180352450();
            goto code_r0x000181bb0616;
          }
          lVar3 = *(longlong *)(lVar3 + 0x20 + (longlong)(int)uStack_38 * 8);
          uStack_38 = uStack_38 + 1;
          if (cRam0000000185a3e8ba == '\0') {
            FUN_18035da50(0x185831830);
            LOCK();
            UNLOCK();
            FUN_18035da50(0x1857a8fc0);
            LOCK();
            UNLOCK();
            FUN_18035da50(0x1857a96e0);
            LOCK();
            UNLOCK();
            FUN_18035da50(0x1857a9430);
            LOCK();
            UNLOCK();
            cRam0000000185a3e8ba = '\x01';
          }
          if (lVar7 == 0) goto code_r0x000181bb0620;
          cVar5 = FUN_180014a60(lVar7,lVar3);
        } while (cVar5 == '\0');
        if (param_2 == 0) goto code_r0x000181bb061b;
        switch(*(undefined1 *)(param_2 + 0x11)) {
        case 0:
          goto code_r0x000181bb0512;
        case 1:
          cVar5 = *(char *)(param_2 + 0x10);
          if (cVar5 != '\x01') {
            if (cVar5 == '\x02') {
              cVar5 = *(longlong *)(param_3 + 0x28) == lVar3;
              break;
            }
            if (cVar5 != '\x03') goto code_r0x000181bb0350;
          }
          cVar5 = *(longlong *)(param_3 + 0x20) == lVar3;
          break;
        case 2:
        case 6:
        case 7:
          if (*(int *)(lVar7 + 0x18) < 1) goto code_r0x000181bb0350;
          uVar6 = FUN_180d6b000((longlong)*(int *)(param_3 + 0x38) %
                                (longlong)*(int *)(lVar7 + 0x18) & 0xffffffff);
          lVar8 = FUN_180007510(lVar7,uVar6);
          cVar5 = lVar3 == lVar8;
          break;
        case 3:
          cVar5 = *(longlong *)(param_3 + 0x18) == lVar3;
          break;
        case 4:
          if (*(longlong *)(param_3 + 0x30) == 0) goto code_r0x000181bb0350;
          cVar5 = FUN_182b100c0(*(undefined8 *)(param_3 + 0x30),lVar3,uRam0000000185831830);
          break;
        case 5:
          lVar8 = FUN_181bb2ff0(param_1,*(undefined1 *)(param_2 + 0x10),param_3);
          goto code_r0x000181bb0504;
        case 8:
          if (lVar3 == 0) goto code_r0x000181bb060c;
          cVar5 = *(char *)(lVar3 + 0x148) == '\0';
          break;
        case 9:
          if (lVar3 == 0) goto code_r0x000181bb0611;
          cVar5 = *(char *)(lVar3 + 0x148) == '\x01';
          break;
        case 10:
          lVar8 = FUN_181bb3130(param_1,*(undefined1 *)(param_2 + 0x10),param_3,0);
code_r0x000181bb0504:
          cVar5 = lVar8 == lVar3;
          break;
        default:
          goto code_r0x000181bb0350;
        }
        if (cVar5 == '\0') goto code_r0x000181bb0350;
code_r0x000181bb0512:
        if (param_4 == 0) {
code_r0x000181bb0616:
          FUN_180352450();
code_r0x000181bb061b:
          FUN_180352450();
code_r0x000181bb0620:
          FUN_180352450();
code_r0x000181bb0625:
          FUN_180352450();
code_r0x000181bb062a:
          FUN_183aed800(0);
code_r0x000181bb0631:
          FUN_180352450();
code_r0x000181bb0636:
          FUN_180352450();
          FUN_180352440();
          FUN_180352440();
          goto code_r0x000181bb0648;
        }
        FUN_18000d200(param_4,lVar3,uRam00000001857a9420);
        cVar5 = func_0x000180ff8930(param_2,0);
      } while (cVar5 == '\0');
      if (lVar7 != 0) {
        FUN_180005650(0,uRam000000018580d620,lVar7);
      }
    }
    return;
  }
code_r0x000181bb064e:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

