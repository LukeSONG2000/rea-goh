// unit_dispel VA 0x181bc8c20 RVA 0x1bc8c20

ulonglong FUN_181bc8c20(longlong param_1,longlong param_2,undefined8 param_3,undefined8 param_4,
                       int param_5,char param_6,undefined8 param_7)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  char cVar5;
  longlong lVar6;
  ulonglong uVar7;
  longlong lStack_d8;
  longlong lStack_d0;
  uint uStack_c8;
  int iStack_c4;
  longlong lStack_c0;
  uint uStack_b8;
  longlong alStack_b0 [2];
  longlong *plStack_a0;
  longlong lStack_98;
  undefined4 uStack_90;
  int iStack_8c;
  undefined8 uStack_80;
  longlong *plStack_78;
  undefined8 uStack_70;
  longlong *plStack_68;
  undefined *puStack_40;
  
  if (param_6 != '\0') {
    if (*(longlong *)(param_1 + 0x188) == 0) goto code_r0x000181bc8c94;
    puStack_40 = &UNK_181bc8c59;
    FUN_181cfa310(*(longlong *)(param_1 + 0x188),param_3,0);
  }
  lVar3 = *(longlong *)(param_1 + 0x60);
  if (lVar3 == 0) {
code_r0x000181bc8c94:
    puStack_40 = &UNK_181bc8c99;
    FUN_180352450();
    pcVar4 = (code *)swi(3);
    uVar7 = (*pcVar4)();
    return uVar7;
  }
  if (cRam0000000185a3e092 == '\0') {
    FUN_18035da50(0x18580d110);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d130);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d128);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185820d00);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857855b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185810510);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d120);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1858104a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580cc70);
    LOCK();
    UNLOCK();
    cRam0000000185a3e092 = '\x01';
  }
  lStack_d8 = 0;
  alStack_b0[0] = 0;
  lStack_d0 = 0;
  uStack_c8 = 0;
  iStack_c4 = 0;
  lStack_c0 = 0;
  if (param_5 < 1) {
    return 0;
  }
  if (*(longlong *)(lVar3 + 0x58) == 0) {
code_r0x000181a6d90b:
    FUN_180352450();
code_r0x000181a6d911:
    FUN_180352450();
code_r0x000181a6d917:
    FUN_180352450();
code_r0x000181a6d91c:
    FUN_180352450();
code_r0x000181a6d921:
    FUN_180352450();
code_r0x000181a6d926:
    FUN_180352450();
code_r0x000181a6d92b:
    FUN_180352450();
    FUN_180352440();
code_r0x000181a6d936:
    FUN_180352450();
code_r0x000181a6d93b:
    FUN_180352450();
code_r0x000181a6d940:
    FUN_180352450();
code_r0x000181a6d945:
    FUN_180352450();
    FUN_180352440();
code_r0x000181a6d951:
    FUN_180352460();
code_r0x000181a6d956:
    FUN_180352450();
code_r0x000181a6d95c:
    FUN_180352450();
code_r0x000181a6d962:
    FUN_183aed800(0);
code_r0x000181a6d969:
    FUN_180352450();
code_r0x000181a6d96e:
    FUN_180352450();
    FUN_180352440();
  }
  else {
    lVar6 = FUN_1819269a0(*(longlong *)(lVar3 + 0x58),0);
    if ((*(longlong *)(lVar3 + 0x68) == 0) ||
       (lVar2 = *(longlong *)(*(longlong *)(lVar3 + 0x68) + 0x310), lVar2 == 0))
    goto code_r0x000181a6d90b;
    uStack_b8 = 0;
    lStack_d8 = FUN_18366e0e0(lVar2,uRam00000001857855b0);
    uStack_70 = 0;
    plStack_68 = &lStack_d8;
    if (lVar6 == 0) goto code_r0x000181a6d97e;
    alStack_b0[0] = FUN_1800084f0(0,uRam00000001858104b8,lVar6);
    uStack_80 = 0;
    plStack_78 = alStack_b0;
    do {
      if (alStack_b0[0] == 0) goto code_r0x000181a6d945;
      cVar5 = FUN_1800084f0(0,uRam0000000185820d00);
      if (cVar5 == '\0') break;
      if (alStack_b0[0] == 0) goto code_r0x000181a6d940;
      lVar6 = FUN_180036260();
      if (lVar6 == 0) goto code_r0x000181a6d93b;
      uStack_90 = 0;
      iStack_8c = *(int *)(lVar6 + 0x1c);
      uStack_c8 = 0;
      lStack_c0 = 0;
      alStack_b0[1] = 0;
      plStack_a0 = &lStack_d0;
      lStack_d0 = lVar6;
      lStack_98 = lVar6;
      iStack_c4 = iStack_8c;
      do {
        do {
          cVar5 = FUN_1833f1b60(&lStack_d0,lRam000000018580d130);
          lVar6 = lStack_c0;
          if (cVar5 == '\0') goto code_r0x000181a6d6cf;
          if (lStack_c0 == 0) goto code_r0x000181a6d92b;
          if (*(longlong *)(lStack_c0 + 0x38) == 0) goto code_r0x000181a6d926;
          lVar2 = *(longlong *)(*(longlong *)(lStack_c0 + 0x38) + 0x38);
          if (lVar2 == 0) goto code_r0x000181a6d921;
          if (param_2 == 0) goto code_r0x000181a6d91c;
          cVar5 = FUN_182d56640(param_2,*(undefined8 *)(lVar2 + 0xb8),uRam000000018580cc70);
        } while (cVar5 == '\0');
        if (lStack_d8 == 0) goto code_r0x000181a6d917;
        FUN_180035e40(lStack_d8,lVar6);
        if (lStack_d8 == 0) goto code_r0x000181a6d911;
      } while (*(int *)(lStack_d8 + 0x18) != param_5);
code_r0x000181a6d6cf:
      if (lStack_d8 == 0) goto code_r0x000181a6d936;
    } while (*(int *)(lStack_d8 + 0x18) != param_5);
    if (alStack_b0[0] != 0) {
      FUN_180005650(0,uRam000000018580d620,alStack_b0[0]);
    }
    if (lStack_d8 != 0) {
      if (*(int *)(lStack_d8 + 0x18) < 1) {
code_r0x000181a6d87e:
        if (lStack_d8 != 0) {
          uVar1 = *(uint *)(lStack_d8 + 0x18);
          if (*plStack_68 != 0) {
            uStack_b8 = uVar1;
            FUN_180005650(0,uRam000000018580d620,*plStack_68);
          }
          return (ulonglong)uVar1;
        }
        goto code_r0x000181a6d95c;
      }
      lStack_98 = lStack_d8;
      uStack_90 = 0;
      iStack_8c = *(int *)(lStack_d8 + 0x1c);
      lStack_d0 = lStack_d8;
      uStack_c8 = 0;
      lStack_c0 = 0;
      alStack_b0[1] = 0;
      plStack_a0 = &lStack_d0;
      iStack_c4 = iStack_8c;
      while (lStack_d0 != 0) {
        if ((iStack_c4 != *(int *)(lStack_d0 + 0x1c)) || (*(uint *)(lStack_d0 + 0x18) <= uStack_c8))
        {
          if ((*(byte *)(*(longlong *)(lRam000000018580d130 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam000000018580d130 + 0x20));
          }
          if (lStack_d0 == 0) goto code_r0x000181a6d969;
          if (iStack_c4 != *(int *)(lStack_d0 + 0x1c)) goto code_r0x000181a6d962;
          uStack_c8 = *(int *)(lStack_d0 + 0x18) + 1;
          lStack_c0 = 0;
          goto code_r0x000181a6d87e;
        }
        lVar6 = *(longlong *)(lStack_d0 + 0x10);
        if (lVar6 == 0) goto code_r0x000181a6d956;
        if (*(uint *)(lVar6 + 0x18) <= uStack_c8) goto code_r0x000181a6d951;
        lStack_c0 = *(longlong *)(lVar6 + 0x20 + (longlong)(int)uStack_c8 * 8);
        uStack_c8 = uStack_c8 + 1;
        FUN_181a6d990(lVar3,lStack_c0,param_4,1,param_7,0);
      }
      goto code_r0x000181a6d96e;
    }
  }
  FUN_180352450();
code_r0x000181a6d97e:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  uVar7 = (*pcVar4)();
  return uVar7;
}

