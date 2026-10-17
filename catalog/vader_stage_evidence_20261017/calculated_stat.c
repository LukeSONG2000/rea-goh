// calculated_stat VA 0x180fde5c0 RVA 0xfde5c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

float FUN_180fde5c0(void)

{
  undefined1 uVar1;
  code *pcVar2;
  longlong lVar3;
  undefined8 *puVar4;
  ushort uVar5;
  char in_DL;
  longlong *in_R8;
  undefined1 in_R9B;
  float fVar7;
  undefined4 extraout_XMM0_Da;
  float extraout_XMM0_Da_00;
  float fVar8;
  float fVar9;
  undefined4 extraout_XMM0_Da_01;
  float extraout_XMM0_Da_02;
  float extraout_XMM0_Da_03;
  float fVar10;
  undefined4 extraout_XMM0_Da_04;
  float extraout_XMM0_Da_05;
  float in_stack_00000028;
  undefined4 in_stack_00000030;
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
  ulonglong uVar6;
  
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
  if (in_R8 == (longlong *)0x0) {
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
    fVar7 = (float)FUN_1800156b0(4,lRam00000001857a8f48);
    if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
      FUN_180359940(lRam00000001857a8f18);
    }
    if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
       (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
       lVar3 == 0)) goto code_r0x000180fdf0af;
    fVar7 = fVar7 + in_stack_00000028;
    plStack_c8 = in_R8;
    lStack_d0 = FUN_180fe09f0(lVar3,0);
    uStack_e0 = 0;
    plStack_d8 = &lStack_d0;
    lVar3 = FUN_180fe0480(extraout_XMM0_Da,in_DL,0);
    if (lVar3 == 0) goto code_r0x000180fdf0de;
    iStack_10c = *(int *)(lVar3 + 0x1c);
    uStack_f0 = 0;
    uStack_100._0_4_ = (float)lVar3;
    fVar8 = (float)uStack_100;
    uStack_110 = 0;
    uStack_108 = 0;
    uStack_100 = 0;
    plStack_f8 = &lStack_118;
    lStack_118 = lVar3;
    while (lStack_118 != 0) {
      if ((iStack_10c != *(int *)(lStack_118 + 0x1c)) ||
         (*(uint *)(lStack_118 + 0x18) <= uStack_110)) {
        if ((*(byte *)(*(longlong *)(lRam00000001857a8f30 + 0x20) + 0x135) & 1) == 0) {
          fVar8 = (float)FUN_18034f1d0(*(longlong *)(lRam00000001857a8f30 + 0x20));
        }
        if (lStack_118 == 0) goto code_r0x000180fdf0c7;
        if (iStack_10c != *(int *)(lStack_118 + 0x1c)) goto code_r0x000180fdf0c0;
        uStack_110 = *(int *)(lStack_118 + 0x18) + 1;
        uStack_108 = uStack_108 & 0xffffffffffffff00;
        if (lStack_d0 != 0) {
          fVar8 = (float)FUN_180005650(0,uRam000000018580d620,lStack_d0);
        }
        if (in_DL != '=') {
          fVar8 = (float)FUN_180fdff40(fVar8,in_DL);
          fVar9 = FUN_180fde5c0();
          fVar7 = fVar7 + fVar9 * fVar8;
        }
        if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857a8f18);
        }
        if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
           (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10)
           , lVar3 == 0)) goto code_r0x000180fdf0af;
        fVar8 = 0.0;
        fStack_e8 = 0.0;
        lStack_d0 = FUN_180fe09f0(lVar3,0);
        uStack_e0 = 0;
        plStack_d8 = &lStack_d0;
        lVar3 = FUN_180fe0280(extraout_XMM0_Da_01,in_DL,0);
        if (lVar3 == 0) goto code_r0x000180fdf10c;
        iStack_10c = *(int *)(lVar3 + 0x1c);
        uStack_f0 = 0;
        uStack_100._0_4_ = (float)lVar3;
        fVar9 = (float)uStack_100;
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
      lVar3 = *in_R8;
      if (*(ushort *)(lVar3 + 0x12e) != 0) {
        uVar6 = 0;
        do {
          if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar6 * 0x10) == lRam00000001857a8f48) {
            puVar4 = (undefined8 *)
                     ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar6 * 0x10) + 4) *
                      0x10 + 0x138 + lVar3);
            goto code_r0x000180fde875;
          }
          uVar5 = (short)uVar6 + 1;
          uVar6 = (ulonglong)uVar5;
        } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
      }
      puVar4 = (undefined8 *)FUN_18034f630(fVar8,lRam00000001857a8f48,4);
      fVar8 = extraout_XMM0_Da_00;
code_r0x000180fde875:
      fVar8 = (float)(*(code *)*puVar4)(fVar8,uVar1,puVar4[1]);
      fVar7 = fVar7 + fVar8;
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
  fVar7 = (float)(*pcVar2)();
  return fVar7;
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
    fVar9 = _UNK_18529ef3c;
    fVar8 = fVar8 + _UNK_18529ef3c;
    fVar10 = (float)FUN_180015180();
    if (*(int *)(lRam00000001857a8f18 + 0xe4) == 0) {
      FUN_180359940(lRam00000001857a8f18);
    }
    if ((**(longlong **)(lRam00000001857a8f18 + 0xb8) == 0) ||
       (lVar3 = FUN_183006ef0(**(longlong **)(lRam00000001857a8f18 + 0xb8),uRam00000001857a8f10),
       lVar3 == 0)) goto code_r0x000180fdf0af;
    fVar10 = fVar10 + fVar8 * fVar7;
    lStack_d0 = FUN_180fe09f0(lVar3,0);
    uStack_e0 = 0;
    plStack_d8 = &lStack_d0;
    lVar3 = FUN_180fe0480(extraout_XMM0_Da_04,in_DL,0);
    if (lVar3 == 0) goto code_r0x000180fdf13a;
    iStack_10c = *(int *)(lVar3 + 0x1c);
    uStack_f0 = 0;
    uStack_100._0_4_ = (float)lVar3;
    fVar7 = (float)uStack_100;
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
    uVar6 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar6 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar6 * 0x10) + 4) * 0x10 +
                  0x138 + lVar3);
        goto code_r0x000180fdeb69;
      }
      uVar5 = (short)uVar6 + 1;
      uVar6 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  puVar4 = (undefined8 *)FUN_18034f630(fVar9,lRam00000001857a8f48,4);
  fVar9 = extraout_XMM0_Da_02;
code_r0x000180fdeb69:
  fVar9 = (float)(*(code *)*puVar4)(fVar9,uVar1,puVar4[1]);
  fVar8 = fVar8 + fVar9;
  lVar3 = *plStack_c8;
  if (*(ushort *)(lVar3 + 0x12e) != 0) {
    uVar6 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar6 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar6 * 0x10) + 5) * 0x10 +
                  0x138 + lVar3);
        fStack_e8 = fVar8;
        goto code_r0x000180fdebd5;
      }
      uVar5 = (short)uVar6 + 1;
      uVar6 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  fStack_e8 = fVar8;
  puVar4 = (undefined8 *)FUN_18034f630(fVar9,lRam00000001857a8f48,5);
  fVar9 = extraout_XMM0_Da_03;
code_r0x000180fdebd5:
  fVar9 = (float)(*(code *)*puVar4)(fVar9,uVar1,in_R9B,in_stack_00000030,puVar4[1]);
  fVar8 = fVar8 + fVar9;
  fStack_e8 = fVar8;
  goto code_r0x000180fdeac0;
code_r0x000180fdee20:
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
      if (((byte)(in_DL - 6U) < 2) || (in_DL == '\x01')) {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        if ((fVar10 < fVar9) || (0x7f800000 < (uint)ABS(fVar9))) {
          fVar10 = fVar9;
        }
      }
      if (in_DL == '\x19') {
        if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (fVar10 < 0.0) {
          fVar10 = 0.0;
        }
      }
      switch(in_DL) {
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
        fVar10 = (float)(int)fVar10;
      }
      return fVar10;
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
    uVar6 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar3 + 0xb0) + uVar6 * 0x10) == lRam00000001857a8f48) {
        puVar4 = (undefined8 *)
                 ((longlong)(*(int *)(*(longlong *)(lVar3 + 0xb0) + 8 + uVar6 * 0x10) + 5) * 0x10 +
                  0x138 + lVar3);
        goto code_r0x000180fdeec5;
      }
      uVar5 = (short)uVar6 + 1;
      uVar6 = (ulonglong)uVar5;
    } while (uVar5 < *(ushort *)(lVar3 + 0x12e));
  }
  puVar4 = (undefined8 *)FUN_18034f630(fVar7,lRam00000001857a8f48,5);
  fVar7 = extraout_XMM0_Da_05;
code_r0x000180fdeec5:
  fVar7 = (float)(*(code *)*puVar4)(fVar7,uVar1,in_R9B,in_stack_00000030,puVar4[1]);
  fVar10 = fVar10 + fVar7;
  goto code_r0x000180fdee20;
}

