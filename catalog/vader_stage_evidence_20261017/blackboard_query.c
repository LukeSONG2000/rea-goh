// blackboard_query VA 0x181362a80 RVA 0x1362a80

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_181362a80(longlong *param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  float fVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  bool bVar5;
  code *pcVar6;
  float fVar7;
  char cVar8;
  uint uVar9;
  longlong lVar10;
  ulonglong uVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  longlong in_stack_00000048;
  uint uStack_78;
  
  if (cRam0000000185a3b356 == '\0') {
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
    cRam0000000185a3b356 = '\x01';
  }
  if (param_4 != 0) {
    if (cRam0000000185a3b34c == '\0') {
      FUN_18035da50(0x18580d398);
      LOCK();
      UNLOCK();
      cRam0000000185a3b34c = '\x01';
    }
    if ((*(longlong *)(param_4 + 0x10) != 0) &&
       (lVar10 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar10 != 0)) {
      lVar10 = FUN_1832a2dd0(lVar10,uRam000000018580d398);
      fVar12 = (float)(**(code **)(*param_1 + 0x1f8))
                                (param_1,param_4,*(undefined8 *)(*param_1 + 0x200));
      fVar7 = _UNK_18529ef3c;
      if (param_3 != 0) {
        iVar2 = *(int *)(param_3 + 0x1c);
        uStack_78 = 0;
        while (param_3 != 0) {
          if ((iVar2 != *(int *)(param_3 + 0x1c)) || (*(uint *)(param_3 + 0x18) <= uStack_78)) {
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (param_3 == 0) goto code_r0x000181362fc1;
            if (iVar2 == *(int *)(param_3 + 0x1c)) {
              FUN_181360310(param_4,lVar10,0);
              if (in_stack_00000048 != 0) {
                (**(code **)(in_stack_00000048 + 0x18))
                          (*(undefined8 *)(in_stack_00000048 + 0x40),
                           *(undefined8 *)(in_stack_00000048 + 0x28));
              }
              return;
            }
            goto code_r0x000181362fba;
          }
          lVar3 = *(longlong *)(param_3 + 0x10);
          if (lVar3 == 0) goto code_r0x000181362fb5;
          if (*(uint *)(lVar3 + 0x18) <= uStack_78) goto code_r0x000181362f64;
          lVar3 = *(longlong *)(lVar3 + 0x20 + (longlong)(int)uStack_78 * 8);
          uStack_78 = uStack_78 + 1;
          if (param_1[6] == 0) goto code_r0x000181362fb0;
          lVar4 = *(longlong *)(param_1[6] + 0x38);
          if (lVar4 == 0) goto code_r0x000181362fab;
          fVar1 = *(float *)(lVar4 + 0x2c);
          if (lVar3 == 0) goto code_r0x000181362fa6;
          if (*(longlong *)(lVar3 + 0x80) == 0) goto code_r0x000181362fa1;
          fVar14 = *(float *)(lVar4 + 0x28);
          fVar13 = (float)FUN_1816a8bb0(*(longlong *)(lVar3 + 0x80),*(undefined8 *)(lVar4 + 0x68),0)
          ;
          if (param_1[6] == 0) goto code_r0x000181362f9c;
          lVar4 = *(longlong *)(param_1[6] + 0x38);
          if (lVar4 == 0) goto code_r0x000181362f97;
          fVar14 = fVar12 * fVar1 + fVar14;
          if ((*(uint *)(lVar4 + 0xf0) & 0x8000000) == 0) {
            if ((*(uint *)(*(longlong *)(param_1[6] + 0x38) + 0xf0) & 0x10000000) == 0) {
              if ((*(uint *)(*(longlong *)(param_1[6] + 0x38) + 0xf0) & 0x20000000) == 0) {
                if ((*(uint *)(*(longlong *)(param_1[6] + 0x38) + 0xf0) & 0x40000000) == 0) {
                  if ((*(uint *)(*(longlong *)(param_1[6] + 0x38) + 0xf0) & 0x80000000) == 0) {
                    lVar4 = *(longlong *)(param_1[6] + 0x38);
                    if ((*(ulonglong *)(lVar4 + 0xf0) & 0x100000000) == 0) {
                      if ((*(ulonglong *)(lVar4 + 0xf0) & 0x8000000000000000) == 0) {
                        if ((*(byte *)(lVar4 + 0xf0) & 1) == 0) {
                          if (lVar10 == 0) goto code_r0x000181362f6a;
                        }
                        else {
                          uVar11 = FUN_180d6b000((int)fVar13);
                          fVar13 = fVar7;
                          if ((uVar11 & 1) != 0) {
                            fVar13 = 0.0;
                          }
                          if (lVar10 == 0) goto code_r0x000181362f6f;
                        }
                      }
                      else {
                        uVar9 = FUN_180d6b000((int)fVar13);
                        uVar9 = uVar9 & 0x80000001;
                        if ((int)uVar9 < 0) {
                          uVar9 = (uVar9 - 1 | 0xfffffffe) + 1;
                        }
                        fVar13 = fVar7;
                        if (uVar9 != 1) {
                          fVar13 = 0.0;
                        }
                        if (lVar10 == 0) goto code_r0x000181362f74;
                      }
                    }
                    else {
                      cVar8 = FUN_180c75680(lVar4,fVar14);
                      fVar13 = fVar7;
                      if (cVar8 != '\0') {
                        fVar13 = 0.0;
                      }
                      if (lVar10 == 0) goto code_r0x000181362f79;
                    }
                  }
                  else {
                    bVar5 = fVar14 < fVar13;
                    fVar13 = fVar7;
                    if (bVar5) {
                      fVar13 = 0.0;
                    }
                    if (lVar10 == 0) goto code_r0x000181362f7e;
                  }
                }
                else {
                  bVar5 = fVar13 < fVar14;
                  fVar13 = fVar7;
                  if (bVar5) {
                    fVar13 = 0.0;
                  }
                  if (lVar10 == 0) goto code_r0x000181362f83;
                }
              }
              else {
                bVar5 = fVar14 <= fVar13;
                fVar13 = fVar7;
                if (bVar5) {
                  fVar13 = 0.0;
                }
                if (lVar10 == 0) goto code_r0x000181362f88;
              }
            }
            else {
              bVar5 = fVar13 <= fVar14;
              fVar13 = fVar7;
              if (bVar5) {
                fVar13 = 0.0;
              }
              if (lVar10 == 0) goto code_r0x000181362f8d;
            }
          }
          else {
            cVar8 = FUN_180c75680(fVar13,fVar14);
            fVar13 = fVar7;
            if (cVar8 == '\0') {
              fVar13 = 0.0;
            }
            if (lVar10 == 0) goto code_r0x000181362f92;
          }
          FUN_18335f400(lVar10,lVar3,fVar13,1,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110))
          ;
        }
        goto code_r0x000181362fc6;
      }
    }
  }
  FUN_180352450();
code_r0x000181362f64:
  FUN_180352460();
code_r0x000181362f6a:
  FUN_180352450();
code_r0x000181362f6f:
  FUN_180352450();
code_r0x000181362f74:
  FUN_180352450();
code_r0x000181362f79:
  FUN_180352450();
code_r0x000181362f7e:
  FUN_180352450();
code_r0x000181362f83:
  FUN_180352450();
code_r0x000181362f88:
  FUN_180352450();
code_r0x000181362f8d:
  FUN_180352450();
code_r0x000181362f92:
  FUN_180352450();
code_r0x000181362f97:
  FUN_180352450();
code_r0x000181362f9c:
  FUN_180352450();
code_r0x000181362fa1:
  FUN_180352450();
code_r0x000181362fa6:
  FUN_180352450();
code_r0x000181362fab:
  FUN_180352450();
code_r0x000181362fb0:
  FUN_180352450();
code_r0x000181362fb5:
  FUN_180352450();
code_r0x000181362fba:
  FUN_183aed800(0);
code_r0x000181362fc1:
  FUN_180352450();
code_r0x000181362fc6:
  FUN_180352450();
  FUN_180352440();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}

