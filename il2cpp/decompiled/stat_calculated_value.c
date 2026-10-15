// ==== stat_calculated_value @ 0x180fde5c0 ====


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong FUN_180fde5c0(undefined8 param_1,char param_2,longlong *param_3,undefined1 param_4,
                       float param_5,undefined4 param_6)

{
  undefined1 uVar1;
  code *pcVar2;
  longlong lVar3;
  undefined8 *puVar4;
  ushort uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  undefined4 extraout_XMM0_Dc;
  undefined4 extraout_XMM0_Dd;
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  ulonglong in_stack_fffffffffffffec8;
  longlong lStack_118;
  uint uStack_110;
  int iStack_10c;
  ulonglong uStack_108;
  undefined8 uStack_100;
  longlong *plStack_f8;
  undefined8 uStack_f0;
  float fStack_e8;
  undefined8 uStack_e0;
  longlong *plStack_d8;
  longlong lStack_d0;
  longlong *plStack_c8;
  
  if (cRam0000000185a39eee == '\0') {
    FUN_18035da50(0x1857a8f50);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f30);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f40);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f18);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8f10);
    LOCK();
    UNLOCK();
    cRam0000000185a39eee = '\x01';
  }
  lStack_d0 = 0;
  lStack_118 = 0;
  uStack_110 = 0;
  iStack_10c = 0;
  uStack_108 = 0;
  if (param_3 == (longlong *)0x0) {
code_r0x000180fdf0af:
    FUN_180352450();
code_r0x000180fdf0b5:
    FUN_180352460();
code_r0x000180fdf0bb:
    FUN_180352450();
code_r0x000180fdf0c0:
    FUN_183aed800(0);
code_r0x000180fdf0c7:
    FUN_180352450();
  }
  else {
    fVar6 = (float)FUN_1800156b0(4,lRam00000001857a8f48,param_3,param_2);
    if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
      FUN_180359940(lRam00000001857a8f18);
    }
    if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
       (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
       lVar3 == 0)) goto code_r0x000180fdf0af;
    fVar6 = fVar6 + param_5;
    plStack_c8 = param_3;
    lStack_d0 = FUN_180fe09f0(lVar3,0);
    uStack_e0 = 0;
    plStack_d8 = &lStack_d0;
    lVar3 = FUN_180fe0480(param_1,param_2,0);
    if (lVar3 == 0) goto code_r0x000180fdf0de;
    iStack_10c = *(int *)(lVar3 + 0x1c);
    uStack_f0 = 0;
    uStack_110 = 0;
    uStack_108 = 0;
    uStack_100 = 0;
    plStack_f8 = &lStack_118;
    lStack_118 = lVar3;
    while (lStack_118 != 0) {
      if ((iStack_10c != *(int *)(lStack_118 + 0x1c)) ||
         (*(uint *)(lStack_118 + 0x18) <= uStack_110)) {
        if ((*(byte *)(*(longlong *)(lRam00000001857a8f30 + 0x20) + 0x135) & 1) == 0) {
          FUN_18034f1d0(*(longlong *)(lRam00000001857a8f30 + 0x20));
        }
        if (lStack_118 == 0) goto code_r0x000180fdf0c7;
        if (iStack_10c != *(int *)(lStack_118 + 0x1c)) goto code_r0x000180fdf0c0;
        uStack_110 = *(int *)(lStack_118 + 0x18) + 1;
        uStack_108 = uStack_108 & 0xffffffffffffff00;
        if (lStack_d0 != 0) {
          FUN_180005650(0,uRam000000018580d620,lStack_d0);
        }
        if (param_2 != '=') {
          fVar7 = (float)FUN_180fdff40(param_1,param_2,param_3,0);
          fVar8 = (float)FUN_180fde5c0(param_1,0x3d,param_3,param_4,
                                       in_stack_fffffffffffffec8 & 0xffffffff00000000,param_6,0);
          fVar6 = fVar6 + fVar8 * fVar7;
        }
        if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857a8f18);
        }
        if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
           (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10)
           , lVar3 == 0)) goto code_r0x000180fdf0af;
        fVar7 = 0.0;
        fStack_e8 = 0.0;
        lStack_d0 = FUN_180fe09f0(lVar3,0);
        uStack_e0 = 0;
        plStack_d8 = &lStack_d0;
        lVar3 = FUN_180fe0280(param_1,param_2,0);
        if (lVar3 == 0) goto code_r0x000180fdf10c;
        iStack_10c = *(int *)(lVar3 + 0x1c);
        uStack_f0 = 0;
        uStack_110 = 0;
        uStack_108 = 0;
        uStack_100 = 0;
        plStack_f8 = &lStack_118;
        lStack_118 = lVar3;
        goto code_r0x000180fdeac0;
      }
      lVar3 = *(longlong *)(lStack_118 + 0x10);
      if (lVar3 == 0) goto code_r0x000180fdf0bb;
      if (*(uint *)(lVar3 + 0x18) <= uStack_110) goto code_r0x000180fdf0b5;
      uVar1 = *(undefined1 *)((longlong)(int)uStack_110 + 0x20 + lVar3);
      uStack_108 = CONCAT71(uStack_108._1_7_,uVar1);
      uStack_110 = uStack_110 + 1;
      lVar3 = *param_3;
      if (*(ushort *)(lVar3 + 0x12e) != 0) {
        uVar10 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar10 * 0x10) == lRam00000001857a8f48) {
            puVar4 = (undefined8 *)
                     ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar10 * 0x10) + 4) *
                      0x10 + 0x138 + lVar3);
            goto code_r0x000180fde875;
          }
          uVar5 = (short)uVar10 + 1;
          uVar10 = (ulonglong)uVar5;
        } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
      }
      puVar4 = (undefined8 *)FUN_18034f630(param_3,lRam00000001857a8f48,4);
code_r0x000180fde875:
      fVar7 = (float)(*(code *)*puVar4)(param_3,uVar1,puVar4[1]);
      fVar6 = fVar6 + fVar7;
    }
  }
  FUN_180352450();
  FUN_180352440();
  FUN_180352440();
code_r0x000180fdf0de:
  FUN_180352450();
code_r0x000180fdf0e4:
  FUN_180352460();
code_r0x000180fdf0e9:
  FUN_180352450();
code_r0x000180fdf0ee:
  FUN_183aed800(0);
code_r0x000180fdf0f5:
  FUN_180352450();
code_r0x000180fdf0fa:
  FUN_180352450();
  FUN_180352440();
  FUN_180352440();
code_r0x000180fdf10c:
  FUN_180352450();
code_r0x000180fdf112:
  FUN_180352460();
code_r0x000180fdf117:
  FUN_180352450();
code_r0x000180fdf11c:
  FUN_183aed800(0);
code_r0x000180fdf123:
  FUN_180352450();
code_r0x000180fdf128:
  FUN_180352450();
  FUN_180352440();
  FUN_180352440();
code_r0x000180fdf13a:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar10 = (*pcVar2)();
  return uVar10;
code_r0x000180fdeac0:
  if (lStack_118 == 0) goto code_r0x000180fdf0fa;
  if ((iStack_10c != *(int *)(lStack_118 + 0x1c)) || (*(uint *)(lStack_118 + 0x18) <= uStack_110)) {
    if ((*(byte *)(*(longlong *)(lRam00000001857a8f30 + 0x20) + 0x135) & 1) == 0) {
      FUN_18034f1d0(*(longlong *)(lRam00000001857a8f30 + 0x20));
    }
    if (lStack_118 == 0) goto code_r0x000180fdf0f5;
    if (iStack_10c != *(int *)(lStack_118 + 0x1c)) goto code_r0x000180fdf0ee;
    uStack_110 = *(int *)(lStack_118 + 0x18) + 1;
    uStack_108 = uStack_108 & 0xffffffffffffff00;
    if (lStack_d0 != 0) {
      FUN_180005650(0,uRam000000018580d620,lStack_d0);
    }
    fVar8 = _UNK_18529ef3c;
    fVar7 = fVar7 + _UNK_18529ef3c;
    uVar9 = FUN_180015180();
    auVar11._8_4_ = extraout_XMM0_Dc;
    auVar11._0_8_ = uVar9;
    auVar11._12_4_ = extraout_XMM0_Dd;
    if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
      FUN_180359940(lRam00000001857a8f18);
    }
    if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
       (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
       lVar3 == 0)) goto code_r0x000180fdf0af;
    auVar12._4_12_ = auVar11._4_12_;
    auVar12._0_4_ = (float)uVar9 + fVar7 * fVar6;
    lStack_d0 = FUN_180fe09f0(lVar3,0);
    uStack_e0 = 0;
    plStack_d8 = &lStack_d0;
    lVar3 = FUN_180fe0480(param_1,param_2,0);
    if (lVar3 == 0) goto code_r0x000180fdf13a;
    iStack_10c = *(int *)(lVar3 + 0x1c);
    uStack_f0 = 0;
    uStack_110 = 0;
    uStack_108 = 0;
    uStack_100 = 0;
    plStack_f8 = &lStack_118;
    lStack_118 = lVar3;
    goto code_r0x000180fdee20;
  }
  lVar3 = *(longlong *)(lStack_118 + 0x10);
  if (lVar3 == 0) goto code_r0x000180fdf0e9;
  if (*(uint *)(lVar3 + 0x18) <= uStack_110) goto code_r0x000180fdf0e4;
  uVar1 = *(undefined1 *)((longlong)(int)uStack_110 + 0x20 + lVar3);
  uStack_108 = CONCAT71(uStack_108._1_7_,uVar1);
  uStack_110 = uStack_110 + 1;
  lVar3 = *plStack_c8;
  if (*(ushort *)(lVar3 + 0x12e) != 0) {
    uVar10 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar10 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar10 * 0x10) + 4) * 0x10 +
                  0x138 + lVar3);
        goto code_r0x000180fdeb69;
      }
      uVar5 = (short)uVar10 + 1;
      uVar10 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  puVar4 = (undefined8 *)FUN_18034f630(param_3,lRam00000001857a8f48,4);
code_r0x000180fdeb69:
  fVar8 = (float)(*(code *)*puVar4)(param_3,uVar1,puVar4[1]);
  fVar7 = fVar7 + fVar8;
  lVar3 = *plStack_c8;
  if (*(ushort *)(lVar3 + 0x12e) != 0) {
    uVar10 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar10 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar10 * 0x10) + 5) * 0x10 +
                  0x138 + lVar3);
        fStack_e8 = fVar7;
        goto code_r0x000180fdebd5;
      }
      uVar5 = (short)uVar10 + 1;
      uVar10 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  fStack_e8 = fVar7;
  puVar4 = (undefined8 *)FUN_18034f630(param_3,lRam00000001857a8f48,5);
code_r0x000180fdebd5:
  fVar8 = (float)(*(code *)*puVar4)(param_3,uVar1,param_4,param_6,puVar4[1]);
  fVar7 = fVar7 + fVar8;
  fStack_e8 = fVar7;
  goto code_r0x000180fdeac0;
code_r0x000180fdee20:
  uVar10 = auVar12._0_8_;
  if (lStack_118 == 0) goto code_r0x000180fdf128;
  if ((iStack_10c != *(int *)(lStack_118 + 0x1c)) || (*(uint *)(lStack_118 + 0x18) <= uStack_110)) {
    if ((*(byte *)(*(longlong *)(lRam00000001857a8f30 + 0x20) + 0x135) & 1) == 0) {
      FUN_18034f1d0(*(longlong *)(lRam00000001857a8f30 + 0x20));
    }
    if (lStack_118 == 0) goto code_r0x000180fdf123;
    if (iStack_10c == *(int *)(lStack_118 + 0x1c)) {
      _uStack_110 = CONCAT44(iStack_10c,*(int *)(lStack_118 + 0x18) + 1);
      uStack_108 = uStack_108 & 0xffffffffffffff00;
      if (lStack_d0 != 0) {
        FUN_180005650(0,uRam000000018580d620,lStack_d0);
      }
      if (((byte)(param_2 - 6U) < 2) || (param_2 == '\x01')) {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        if ((auVar12._0_4_ < fVar8) || (0x7f800000 < (uint)ABS(fVar8))) {
          uVar10 = (ulonglong)(uint)fVar8;
        }
      }
      if (param_2 == '\x19') {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        if ((float)uVar10 < 0.0) {
          uVar10 = 0;
        }
      }
      switch(param_2) {
      case '\x10':
      case '\x12':
      case '\x13':
      case '\x18':
      case '\x1a':
      case '\x1b':
      case '\x1c':
      case '\x1d':
      case '\x1e':
      case '\x1f':
      case ' ':
      case '!':
      case '\"':
      case '#':
      case '$':
      case '%':
      case '&':
      case '\'':
      case '(':
      case ')':
      case '*':
      case '+':
      case ',':
      case '-':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case ':':
      case ';':
      case '<':
      case '=':
      case '>':
        break;
      default:
        uVar10 = (ulonglong)(uint)(float)(int)(float)uVar10;
      }
      return uVar10;
    }
    goto code_r0x000180fdf11c;
  }
  lVar3 = *(longlong *)(lStack_118 + 0x10);
  if (lVar3 == 0) goto code_r0x000180fdf117;
  if (*(uint *)(lVar3 + 0x18) <= uStack_110) goto code_r0x000180fdf112;
  uVar1 = *(undefined1 *)((longlong)(int)uStack_110 + 0x20 + lVar3);
  uStack_108 = CONCAT71(uStack_108._1_7_,uVar1);
  uStack_110 = uStack_110 + 1;
  lVar3 = *plStack_c8;
  if (*(ushort *)(lVar3 + 0x12e) != 0) {
    uVar10 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar10 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar10 * 0x10) + 5) * 0x10 +
                  0x138 + lVar3);
        goto code_r0x000180fdeec5;
      }
      uVar5 = (short)uVar10 + 1;
      uVar10 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  puVar4 = (undefined8 *)FUN_18034f630(param_3,lRam00000001857a8f48,5);
code_r0x000180fdeec5:
  fVar6 = (float)(*(code *)*puVar4)(param_3,uVar1,param_4,param_6,puVar4[1]);
  auVar12._0_4_ = auVar12._0_4_ + fVar6;
  goto code_r0x000180fdee20;
}

