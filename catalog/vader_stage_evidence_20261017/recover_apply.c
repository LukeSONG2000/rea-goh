// recover_apply VA 0x181371880 RVA 0x1371880

void FUN_181371880(longlong *param_1,undefined8 param_2,longlong param_3,longlong param_4,
                  undefined8 param_5,longlong param_6,undefined1 param_7,undefined1 param_8,
                  longlong param_9)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  code *pcVar4;
  char cVar5;
  longlong lVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  uint uStack_60;
  
  if (cRam0000000185a3b379 == '\0') {
    FUN_18035da50(0x18580d270);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d2c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ed0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806eb8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185806e58);
    LOCK();
    UNLOCK();
    cRam0000000185a3b379 = '\x01';
  }
  uVar7 = (**(code **)(*param_1 + 0x1f8))(param_1,param_4,*(undefined8 *)(*param_1 + 0x200));
  if (param_4 != 0) {
    if (cRam0000000185a3b34c == '\0') {
      FUN_18035da50(0x18580d398);
      LOCK();
      UNLOCK();
      cRam0000000185a3b34c = '\x01';
    }
    if ((*(longlong *)(param_4 + 0x10) != 0) &&
       (lVar6 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar6 != 0)) {
      lVar6 = FUN_1832a2dd0(lVar6,uRam000000018580d398);
      if (param_3 != 0) {
        iVar1 = *(int *)(param_3 + 0x1c);
        uStack_60 = 0;
        while (param_3 != 0) {
          if (iVar1 != *(int *)(param_3 + 0x1c)) {
code_r0x000181371b2e:
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (param_3 == 0) goto code_r0x000181371bfe;
            if (iVar1 == *(int *)(param_3 + 0x1c)) {
              FUN_181360310(param_4,lVar6,0);
              if (param_9 != 0) {
                (**(code **)(param_9 + 0x18))
                          (*(undefined8 *)(param_9 + 0x40),*(undefined8 *)(param_9 + 0x28));
              }
              return;
            }
            goto code_r0x000181371bf7;
          }
          if (*(uint *)(param_3 + 0x18) <= uStack_60) goto code_r0x000181371b2e;
          lVar2 = *(longlong *)(param_3 + 0x10);
          if (lVar2 == 0) goto code_r0x000181371bf2;
          if (*(uint *)(lVar2 + 0x18) <= uStack_60) goto code_r0x000181371bdd;
          uVar3 = *(undefined8 *)(lVar2 + 0x20 + (longlong)(int)uStack_60 * 8);
          uStack_60 = uStack_60 + 1;
          if (param_6 == 0) goto code_r0x000181371bed;
          cVar5 = FUN_1833355c0(param_6,uVar3,uRam000000018580d270);
          if (cVar5 == '\0') {
            uVar8 = FUN_181371c10(param_1,param_2,uVar3,uVar7,param_5,param_7,param_8,0);
            if (lVar6 == 0) goto code_r0x000181371be3;
            FUN_18335f400(lVar6,uVar3,uVar8,1,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110
                           ));
          }
          else {
            if (lVar6 == 0) goto code_r0x000181371be8;
            FUN_18335f400(lVar6,uVar3,0,1,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110
                           ));
          }
        }
        goto code_r0x000181371c03;
      }
    }
  }
  FUN_180352450();
code_r0x000181371bdd:
  FUN_180352460();
code_r0x000181371be3:
  FUN_180352450();
code_r0x000181371be8:
  FUN_180352450();
code_r0x000181371bed:
  FUN_180352450();
code_r0x000181371bf2:
  FUN_180352450();
code_r0x000181371bf7:
  FUN_183aed800(0);
code_r0x000181371bfe:
  FUN_180352450();
code_r0x000181371c03:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

