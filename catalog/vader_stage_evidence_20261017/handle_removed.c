// handle_removed VA 0x181a71c20 RVA 0x1a71c20

void FUN_181a71c20(longlong param_1,longlong param_2,undefined4 param_3,undefined4 param_4,
                  longlong param_5,char param_6,longlong param_7)

{
  int iVar1;
  undefined4 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  longlong lVar6;
  code *pcVar7;
  char cVar8;
  uint uVar9;
  undefined4 uVar10;
  longlong lVar11;
  ulonglong uVar12;
  longlong lVar13;
  int iVar14;
  ulonglong uVar15;
  uint uStack_48;
  
  if (cRam0000000185a3e0a7 == '\0') {
    FUN_18035da50(0x1857f5b78);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185835678);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c590);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c578);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c570);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c3150);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c560);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810270);
    LOCK();
    UNLOCK();
    cRam0000000185a3e0a7 = '\x01';
  }
  if (((param_2 == 0) || (*(longlong *)(param_2 + 0x38) == 0)) ||
     (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0)) {
code_r0x000181a724a9:
    FUN_180352450();
code_r0x000181a724af:
    FUN_180352460();
code_r0x000181a724b5:
    FUN_180352460();
code_r0x000181a724ba:
    FUN_180352450();
code_r0x000181a724bf:
    FUN_180352450();
code_r0x000181a724c4:
    FUN_180352450();
code_r0x000181a724c9:
    FUN_180352450();
code_r0x000181a724ce:
    FUN_183aed800(0);
code_r0x000181a724d5:
    FUN_180352450();
  }
  else {
    if (*(char *)(lVar3 + 0x20) == '%') {
      if (*(longlong *)(param_1 + 0x70) == 0) goto code_r0x000181a724a9;
      FUN_181bbb1d0(*(longlong *)(param_1 + 0x70),0);
    }
    if ((*(longlong *)(param_2 + 0x38) == 0) ||
       (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0))
    goto code_r0x000181a724a9;
    if (*(char *)(lVar3 + 0x20) == '&') {
      if (*(longlong *)(param_1 + 0x70) == 0) goto code_r0x000181a724a9;
      FUN_181bbb670(*(longlong *)(param_1 + 0x70),1,0);
    }
    if ((*(longlong *)(param_2 + 0x38) == 0) ||
       (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0))
    goto code_r0x000181a724a9;
    if (*(char *)(lVar3 + 0x20) == ')') {
      if ((*(longlong *)(param_1 + 0x70) == 0) ||
         (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x70) + 0x80), lVar3 == 0))
      goto code_r0x000181a724a9;
      FUN_1816a9090(lVar3,*(undefined8 *)
                           (*(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38) + 0x68),
                    *(longlong *)(param_2 + 0x38),0);
    }
    if (((*(longlong *)(param_2 + 0x38) == 0) ||
        (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0)) ||
       (lVar3 = *(longlong *)(lVar3 + 0xb8), lVar3 == 0)) goto code_r0x000181a724a9;
    uVar12 = 0;
    iVar1 = *(int *)(lVar3 + 0x1c);
    uStack_48 = 0;
    while (lVar3 != 0) {
      if (iVar1 != *(int *)(lVar3 + 0x1c)) {
code_r0x000181a71f12:
        if ((*(byte *)(*(longlong *)(lRam000000018582c578 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(lRam000000018582c578 + 0x20));
        }
        if (lVar3 == 0) goto code_r0x000181a724d5;
        if (iVar1 != *(int *)(lVar3 + 0x1c)) goto code_r0x000181a724ce;
        if (((*(longlong *)(param_2 + 0x38) == 0) ||
            (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0)) ||
           (lVar3 = *(longlong *)(lVar3 + 0xb8), lVar3 == 0)) goto code_r0x000181a724a9;
        iVar1 = *(int *)(lVar3 + 0x1c);
        uStack_48 = 0;
        goto code_r0x000181a71ff0;
      }
      if (*(uint *)(lVar3 + 0x18) <= uStack_48) goto code_r0x000181a71f12;
      lVar11 = *(longlong *)(lVar3 + 0x10);
      if (lVar11 == 0) goto code_r0x000181a724c9;
      if (*(uint *)(lVar11 + 0x18) <= uStack_48) goto code_r0x000181a724af;
      uVar4 = *(undefined8 *)(lVar11 + 0x20 + (longlong)(int)uStack_48 * 8);
      uStack_48 = uStack_48 + 1;
      lVar11 = *(longlong *)(param_1 + 0x60);
      if (lVar11 == 0) goto code_r0x000181a724c4;
      uVar9 = FUN_18332e340(lVar11,uVar4);
      if ((int)uVar9 < 0) {
        iVar14 = 0;
      }
      else {
        lVar11 = *(longlong *)(lVar11 + 0x18);
        if (lVar11 == 0) goto code_r0x000181a724bf;
        if (*(uint *)(lVar11 + 0x18) <= uVar9) goto code_r0x000181a724b5;
        iVar14 = *(int *)(lVar11 + ((longlong)(int)uVar9 + 2) * 0x18);
      }
      uVar15 = (ulonglong)(iVar14 - 1U);
      if ((int)(iVar14 - 1U) < 1) {
        uVar15 = uVar12;
      }
      if (*(longlong *)(param_1 + 0x60) == 0) goto code_r0x000181a724ba;
      uVar5 = *(undefined8 *)
               (*(longlong *)(*(longlong *)(lRam0000000185835678 + 0x20) + 0xc0) + 0x110);
      FUN_183344ac0(*(longlong *)(param_1 + 0x60),uVar4,uVar15,
                    CONCAT71((int7)((ulonglong)uVar5 >> 8),1),uVar5);
    }
  }
  FUN_180352450();
  FUN_180352440();
code_r0x000181a724e6:
  FUN_180352460();
code_r0x000181a724eb:
  FUN_180352460();
code_r0x000181a724f0:
  FUN_180352450();
code_r0x000181a724f5:
  FUN_180352450();
code_r0x000181a724fa:
  FUN_180352450();
code_r0x000181a724ff:
  FUN_183aed800(0);
code_r0x000181a72506:
  FUN_180352450();
code_r0x000181a7250b:
  FUN_180352450();
  FUN_180352440();
  pcVar7 = (code *)swi(3);
  (*pcVar7)();
  return;
code_r0x000181a71ff0:
  if (lVar3 != 0) {
    if (iVar1 != *(int *)(lVar3 + 0x1c)) {
code_r0x000181a7215a:
      if ((*(byte *)(*(longlong *)(lRam000000018582c578 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam000000018582c578 + 0x20));
      }
      if (lVar3 == 0) goto code_r0x000181a72506;
      if (iVar1 != *(int *)(lVar3 + 0x1c)) goto code_r0x000181a724ff;
      FUN_181a72690(param_1,param_2,param_3,0);
      FUN_181a72910(param_1,param_2,param_4,0);
      if (param_5 != 0) {
        if ((*(longlong *)(param_1 + 0x68) == 0) || (*(longlong *)(param_1 + 0x70) == 0))
        goto code_r0x000181a724a9;
        lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x58);
        uVar4 = *(undefined8 *)(*(longlong *)(param_1 + 0x70) + 0x98);
        lVar11 = *(longlong *)(param_1 + 0x70);
        if (cRam0000000185a3e8f5 == '\0') {
          FUN_18035da50(0x1858316b0);
          LOCK();
          UNLOCK();
          cRam0000000185a3e8f5 = '\x01';
        }
        if (*(longlong *)(lVar11 + 0xe8) == 0) goto code_r0x000181a724a9;
        uVar10 = FUN_1800084f0(0xc,uRam00000001858316b0);
        if (lVar3 == 0) goto code_r0x000181a724a9;
        uVar5 = *(undefined8 *)(param_2 + 0x38);
        if (cRam0000000185a3e871 == '\0') {
          FUN_18035da50(0x185779ab8);
          LOCK();
          UNLOCK();
          FUN_18035da50(0x185806db8);
          LOCK();
          UNLOCK();
          cRam0000000185a3e871 = '\x01';
        }
        lVar11 = lRam0000000185779ab8;
        if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (lVar11 != 0) {
          if (*(int *)(lRam0000000185a33ed0 + 0xe4) == 0) {
            FUN_180359940();
          }
          lVar11 = FUN_1803ab790(lVar11,1);
          uVar12 = FUN_1803a7d00(lVar11 + 0x20);
        }
        cVar8 = FUN_181ba8230(lVar3,uVar12,0);
        if (cVar8 != '\0') {
          if ((*(longlong *)(lVar3 + 0x10) == 0) ||
             (lVar11 = *(longlong *)(*(longlong *)(lVar3 + 0x10) + 0x250), lVar11 == 0))
          goto code_r0x000181a724a9;
          lVar11 = *(longlong *)(lVar11 + 0x30);
          lVar6 = *(longlong *)(lVar3 + 0x28);
          uVar2 = *(undefined4 *)(lVar3 + 0x30);
          lVar13 = FUN_1803a51b0(uRam0000000185806db8);
          *(undefined8 *)(lVar13 + 0x28) = uVar4;
          *(undefined4 *)(lVar13 + 0x30) = uVar10;
          *(undefined8 *)(lVar13 + 0x38) = uVar5;
          *(longlong *)(lVar13 + 0x10) = param_5;
          *(longlong *)(lVar13 + 0x18) = lVar11 - lVar6;
          *(undefined4 *)(lVar13 + 0x20) = uVar2;
          *(undefined1 *)(lVar13 + 0x24) = 0;
          FUN_181ba80a0(lVar3,lVar13,0);
        }
        *(int *)(lVar3 + 0x30) = *(int *)(lVar3 + 0x30) + 1;
      }
      if ((*(longlong *)(param_2 + 0x38) == 0) ||
         (lVar3 = *(longlong *)(*(longlong *)(param_2 + 0x38) + 0x38), lVar3 == 0))
      goto code_r0x000181a724a9;
      lVar3 = *(longlong *)(lVar3 + 0x60);
      if ((lVar3 != 0) && (*(int *)(lVar3 + 0x10) != 0)) {
        if ((*(longlong *)(param_1 + 0x68) == 0) ||
           (*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x128) == 0)) goto code_r0x000181a724a9;
        FUN_180036090(3);
      }
      lVar3 = *(longlong *)(param_1 + 0x18);
      if (lVar3 != 0) {
        (**(code **)(lVar3 + 0x18))
                  (*(undefined8 *)(lVar3 + 0x40),param_2,*(undefined8 *)(lVar3 + 0x28));
      }
      if ((*(longlong *)(param_1 + 0x68) == 0) ||
         (lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x68) + 0x130), lVar3 == 0))
      goto code_r0x000181a724a9;
      if (*(char *)(lVar3 + 0x20) != '\0') {
        lVar3 = *(longlong *)(lVar3 + 0x18);
        if (lVar3 == 0) goto code_r0x000181a724a9;
        lVar11 = *(longlong *)
                  (*(longlong *)(*(longlong *)(lRam0000000185810270 + 0x20) + 0xc0) + 0x80);
        *(int *)(lVar3 + 0x1c) = *(int *)(lVar3 + 0x1c) + 1;
        lVar6 = *(longlong *)(lVar3 + 0x10);
        if (lVar6 == 0) goto code_r0x000181a724a9;
        uVar9 = *(uint *)(lVar3 + 0x18);
        if (uVar9 < *(uint *)(lVar6 + 0x18)) {
          *(uint *)(lVar3 + 0x18) = uVar9 + 1;
          if (uVar9 < *(uint *)(lVar6 + 0x18)) {
            *(longlong *)(lVar6 + 0x20 + (longlong)(int)uVar9 * 8) = param_2;
            return;
          }
          FUN_180352460();
          pcVar7 = (code *)swi(3);
          (*pcVar7)();
          return;
        }
        FUN_1836f50c0(lVar3,param_2,
                      *(undefined8 *)(*(longlong *)(*(longlong *)(lVar11 + 0x20) + 0xc0) + 0x70));
      }
      return;
    }
    if (*(uint *)(lVar3 + 0x18) <= uStack_48) goto code_r0x000181a7215a;
    lVar11 = *(longlong *)(lVar3 + 0x10);
    if (lVar11 == 0) goto code_r0x000181a724fa;
    if (*(uint *)(lVar11 + 0x18) <= uStack_48) goto code_r0x000181a724e6;
    uVar4 = *(undefined8 *)(lVar11 + 0x20 + (longlong)(int)uStack_48 * 8);
    uStack_48 = uStack_48 + 1;
    lVar11 = *(longlong *)(param_1 + 0x60);
    if (lVar11 == 0) goto code_r0x000181a724f5;
    uVar9 = FUN_18332e340(lVar11,uVar4,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam00000001857f5b78 + 0x20) + 0xc0) + 0x108
                           ));
    if ((int)uVar9 < 0) {
      iVar14 = 0;
    }
    else {
      lVar11 = *(longlong *)(lVar11 + 0x18);
      if (lVar11 == 0) goto code_r0x000181a724f0;
      if (*(uint *)(lVar11 + 0x18) <= uVar9) goto code_r0x000181a724eb;
      iVar14 = *(int *)(lVar11 + ((longlong)(int)uVar9 + 2) * 0x18);
    }
    if (param_6 != '\0') {
      lVar11 = *(longlong *)(param_1 + 0x48);
      if (lVar11 != 0) {
        (**(code **)(lVar11 + 0x18))(*(undefined8 *)(lVar11 + 0x40),uVar4,param_7);
      }
      if (param_7 != 0) {
        FUN_181bcd460(param_7,0x51,*(undefined8 *)(param_1 + 0x70),uVar4,0);
      }
    }
    lVar11 = *(longlong *)(param_1 + 0x38);
    if (lVar11 != 0) {
      (**(code **)(lVar11 + 0x18))
                (*(undefined8 *)(lVar11 + 0x40),uVar4,*(undefined8 *)(lVar11 + 0x28));
    }
    cVar8 = '\0';
    if (iVar14 == 0) {
      cVar8 = param_6;
    }
    if (cVar8 != '\0') {
      lVar11 = *(longlong *)(param_1 + 0x40);
      if (lVar11 != 0) {
        (**(code **)(lVar11 + 0x18))(*(undefined8 *)(lVar11 + 0x40),uVar4,param_7);
      }
      if (param_7 != 0) {
        FUN_181bcd460(param_7,0x50,*(undefined8 *)(param_1 + 0x70),uVar4,0);
      }
    }
    if ((iVar14 == 0) && (lVar11 = *(longlong *)(param_1 + 0x28), lVar11 != 0)) {
      (**(code **)(lVar11 + 0x18))
                (*(undefined8 *)(lVar11 + 0x40),uVar4,*(undefined8 *)(lVar11 + 0x28));
    }
    goto code_r0x000181a71ff0;
  }
  goto code_r0x000181a7250b;
}

