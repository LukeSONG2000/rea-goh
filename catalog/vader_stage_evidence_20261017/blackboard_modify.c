// blackboard_modify VA 0x181362370 RVA 0x1362370

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_181362370(longlong *param_1,undefined8 param_2,longlong param_3,longlong param_4)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  undefined4 uVar5;
  char cVar6;
  longlong lVar7;
  undefined8 uVar8;
  longlong lVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  longlong in_stack_00000048;
  uint uStack_b8;
  
  if (cRam0000000185a3b355 == '\0') {
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
    cRam0000000185a3b355 = '\x01';
  }
  if (param_4 != 0) {
    if (cRam0000000185a3b34c == '\0') {
      FUN_18035da50(0x18580d398);
      LOCK();
      UNLOCK();
      cRam0000000185a3b34c = '\x01';
    }
    if ((*(longlong *)(param_4 + 0x10) != 0) &&
       (lVar7 = *(longlong *)(*(longlong *)(param_4 + 0x10) + 800), lVar7 != 0)) {
      lVar7 = FUN_1832a2dd0(lVar7,uRam000000018580d398);
      fVar10 = (float)(**(code **)(*param_1 + 0x1f8))
                                (param_1,param_4,*(undefined8 *)(*param_1 + 0x200));
      uVar5 = _UNK_18529ef3c;
      if (param_3 != 0) {
        iVar1 = *(int *)(param_3 + 0x1c);
        uStack_b8 = 0;
        while (param_3 != 0) {
          if ((iVar1 != *(int *)(param_3 + 0x1c)) || (*(uint *)(param_3 + 0x18) <= uStack_b8)) {
            if ((*(byte *)(*(longlong *)(lRam0000000185806eb8 + 0x20) + 0x135) & 1) == 0) {
              FUN_18034f1d0(*(longlong *)(lRam0000000185806eb8 + 0x20));
            }
            if (param_3 == 0) goto code_r0x000181362a67;
            if (iVar1 == *(int *)(param_3 + 0x1c)) {
              FUN_181360310(param_4,lVar7,0);
              if (in_stack_00000048 != 0) {
                (**(code **)(in_stack_00000048 + 0x18))
                          (*(undefined8 *)(in_stack_00000048 + 0x40),
                           *(undefined8 *)(in_stack_00000048 + 0x28));
              }
              return;
            }
            goto code_r0x000181362a60;
          }
          lVar2 = *(longlong *)(param_3 + 0x10);
          if (lVar2 == 0) goto code_r0x000181362a5b;
          if (*(uint *)(lVar2 + 0x18) <= uStack_b8) goto code_r0x0001813629ab;
          lVar2 = *(longlong *)(lVar2 + 0x20 + (longlong)(int)uStack_b8 * 8);
          uStack_b8 = uStack_b8 + 1;
          if (param_1[6] == 0) goto code_r0x000181362a56;
          lVar9 = *(longlong *)(param_1[6] + 0x38);
          if (lVar9 == 0) goto code_r0x000181362a51;
          if (lVar2 == 0) goto code_r0x000181362a4c;
          if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x000181362a47;
          fVar11 = (float)FUN_1816a8bb0(*(longlong *)(lVar2 + 0x80),*(undefined8 *)(lVar9 + 0x68),0)
          ;
          lVar9 = param_1[6];
          if (lVar9 == 0) goto code_r0x000181362a42;
          lVar3 = *(longlong *)(lVar9 + 0x38);
          if (lVar3 == 0) goto code_r0x000181362a3d;
          fVar15 = fVar10 * *(float *)(lVar3 + 0x2c);
          if ((*(byte *)(lVar3 + 0xf0) & 2) != 0) {
            fVar15 = fVar11 * fVar15;
          }
          fVar13 = *(float *)(*(longlong *)(lVar9 + 0x38) + 0x24);
          fVar14 = *(float *)(lVar3 + 0x28);
          if ((*(uint *)(*(longlong *)(lVar9 + 0x38) + 0xf0) & 0x4000000) == 0) {
            fVar12 = 0.0;
          }
          else {
            if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x0001813629b1;
            fVar12 = (float)FUN_1816a8bb0(*(longlong *)(lVar2 + 0x80),
                                          *(undefined8 *)(*(longlong *)(lVar9 + 0x38) + 0x68),0);
          }
          lVar9 = param_1[6];
          if (lVar9 == 0) goto code_r0x000181362a38;
          lVar3 = *(longlong *)(lVar9 + 0x38);
          if (lVar3 == 0) goto code_r0x000181362a33;
          fVar15 = fVar14 + fVar11 * fVar13 + fVar15;
          if ((*(uint *)(lVar3 + 0xf0) & 0x4000000) == 0) {
            FUN_181bcb090();
            if (param_1[6] == 0) goto code_r0x0001813629d4;
            lVar9 = *(longlong *)(param_1[6] + 0x38);
            if (lVar9 == 0) goto code_r0x0001813629cf;
            if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x0001813629ca;
            fVar13 = (float)FUN_1816a8bb0(*(longlong *)(lVar2 + 0x80),*(undefined8 *)(lVar9 + 0x68),
                                          0);
            cVar6 = FUN_180c75680(fVar11,fVar13);
            if (cVar6 == '\0') {
              if (fVar12 < fVar15) {
                if (param_1[6] == 0) goto code_r0x0001813629c5;
                lVar9 = *(longlong *)(param_1[6] + 0x38);
                if (lVar9 == 0) goto code_r0x0001813629c0;
code_r0x00018136278e:
                uVar8 = 0x61;
              }
              else {
                if (fVar12 <= fVar15) goto code_r0x0001813627a4;
                if (param_1[6] == 0) goto code_r0x0001813629bb;
                lVar9 = *(longlong *)(param_1[6] + 0x38);
                if (lVar9 == 0) goto code_r0x0001813629b6;
code_r0x000181362770:
                uVar8 = 0x62;
              }
              FUN_181bcd460(lVar2,uVar8,param_2,*(undefined8 *)(lVar9 + 0x68),0);
            }
          }
          else {
            FUN_181bcafe0(lVar2,*(undefined8 *)(lVar3 + 0x68),lVar9,fVar15,0);
            if (param_1[6] == 0) goto code_r0x000181362a2e;
            lVar9 = *(longlong *)(param_1[6] + 0x38);
            if (lVar9 == 0) goto code_r0x000181362a29;
            if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x000181362a24;
            fVar13 = (float)FUN_1816a8bb0(*(longlong *)(lVar2 + 0x80),*(undefined8 *)(lVar9 + 0x68),
                                          0);
            cVar6 = FUN_180c75680(fVar11,fVar13);
            if (cVar6 == '\0') {
              if (fVar12 < fVar15) {
                if (param_1[6] != 0) {
                  lVar9 = *(longlong *)(param_1[6] + 0x38);
                  if (lVar9 != 0) goto code_r0x00018136278e;
                  goto code_r0x0001813629e3;
                }
                goto code_r0x0001813629e8;
              }
              if (fVar15 < fVar12) {
                if (param_1[6] != 0) {
                  lVar9 = *(longlong *)(param_1[6] + 0x38);
                  if (lVar9 != 0) goto code_r0x000181362770;
                  goto code_r0x0001813629d9;
                }
                goto code_r0x0001813629de;
              }
            }
          }
code_r0x0001813627a4:
          if (param_1[6] == 0) goto code_r0x000181362a1f;
          lVar9 = *(longlong *)(param_1[6] + 0x38);
          if (lVar9 == 0) goto code_r0x000181362a1a;
          if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x000181362a15;
          fVar15 = (float)FUN_1816a9330(*(longlong *)(lVar2 + 0x80),*(undefined8 *)(lVar9 + 0x68));
          if (param_1[6] == 0) goto code_r0x000181362a10;
          lVar9 = *(longlong *)(param_1[6] + 0x38);
          if (lVar9 == 0) goto code_r0x000181362a0b;
          if (*(longlong *)(lVar2 + 0x80) == 0) goto code_r0x000181362a06;
          fVar14 = (float)FUN_1816a9270(*(longlong *)(lVar2 + 0x80),*(undefined8 *)(lVar9 + 0x68));
          cVar6 = FUN_180c75680(fVar11,fVar13);
          if ((cVar6 == '\0') && (fVar15 <= fVar13)) {
            if (param_1[6] == 0) goto code_r0x0001813629f2;
            lVar9 = *(longlong *)(param_1[6] + 0x38);
            if (lVar9 == 0) goto code_r0x0001813629ed;
            FUN_181bcd460(lVar2,0x60,param_2,*(undefined8 *)(lVar9 + 0x68),0);
          }
          cVar6 = FUN_180c75680(fVar11,fVar13);
          if ((cVar6 == '\0') && (fVar13 <= fVar14)) {
            if (param_1[6] == 0) goto code_r0x0001813629fc;
            lVar9 = *(longlong *)(param_1[6] + 0x38);
            if (lVar9 == 0) goto code_r0x0001813629f7;
            FUN_181bcd460(lVar2,0x5f,param_2,*(undefined8 *)(lVar9 + 0x68),0);
          }
          if (lVar7 == 0) goto code_r0x000181362a01;
          FUN_18335f400(lVar7,lVar2,uVar5,1,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam000000018580d2c8 + 0x20) + 0xc0) + 0x110))
          ;
        }
        goto code_r0x000181362a6c;
      }
    }
  }
  FUN_180352450();
code_r0x0001813629ab:
  FUN_180352460();
code_r0x0001813629b1:
  FUN_180352450();
code_r0x0001813629b6:
  FUN_180352450();
code_r0x0001813629bb:
  FUN_180352450();
code_r0x0001813629c0:
  FUN_180352450();
code_r0x0001813629c5:
  FUN_180352450();
code_r0x0001813629ca:
  FUN_180352450();
code_r0x0001813629cf:
  FUN_180352450();
code_r0x0001813629d4:
  FUN_180352450();
code_r0x0001813629d9:
  FUN_180352450();
code_r0x0001813629de:
  FUN_180352450();
code_r0x0001813629e3:
  FUN_180352450();
code_r0x0001813629e8:
  FUN_180352450();
code_r0x0001813629ed:
  FUN_180352450();
code_r0x0001813629f2:
  FUN_180352450();
code_r0x0001813629f7:
  FUN_180352450();
code_r0x0001813629fc:
  FUN_180352450();
code_r0x000181362a01:
  FUN_180352450();
code_r0x000181362a06:
  FUN_180352450();
code_r0x000181362a0b:
  FUN_180352450();
code_r0x000181362a10:
  FUN_180352450();
code_r0x000181362a15:
  FUN_180352450();
code_r0x000181362a1a:
  FUN_180352450();
code_r0x000181362a1f:
  FUN_180352450();
code_r0x000181362a24:
  FUN_180352450();
code_r0x000181362a29:
  FUN_180352450();
code_r0x000181362a2e:
  FUN_180352450();
code_r0x000181362a33:
  FUN_180352450();
code_r0x000181362a38:
  FUN_180352450();
code_r0x000181362a3d:
  FUN_180352450();
code_r0x000181362a42:
  FUN_180352450();
code_r0x000181362a47:
  FUN_180352450();
code_r0x000181362a4c:
  FUN_180352450();
code_r0x000181362a51:
  FUN_180352450();
code_r0x000181362a56:
  FUN_180352450();
code_r0x000181362a5b:
  FUN_180352450();
code_r0x000181362a60:
  FUN_183aed800(0);
code_r0x000181362a67:
  FUN_180352450();
code_r0x000181362a6c:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

