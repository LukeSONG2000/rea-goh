// sweep_initialize VA 0x18164dde0 RVA 0x164dde0

void FUN_18164dde0(longlong param_1,longlong param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  code *pcVar4;
  char cVar5;
  int iVar6;
  uint uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  ulonglong uVar11;
  byte bVar12;
  uint uVar13;
  longlong lStack_80;
  uint uStack_78;
  int iStack_74;
  longlong lStack_70;
  int iStack_68;
  uint uStack_64;
  undefined8 uStack_60;
  longlong *plStack_58;
  undefined8 uStack_50;
  ulonglong uVar14;
  
  if (cRam0000000185a3c633 == '\0') {
    FUN_18035da50(0x1857ff470);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185801c68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34c0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3490);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd7d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a34a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857c6d48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185827178);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd7d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fd7e0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c633 = '\x01';
  }
  lStack_80 = 0;
  uStack_78 = 0;
  iStack_74 = 0;
  lStack_70 = 0;
  iStack_68 = 0;
  lVar10 = *(longlong *)(param_1 + 0x58);
  uVar8 = FUN_1803a51b0(uRam00000001857ff470);
  FUN_180c84ee0(uVar8,param_1,uRam00000001857fd7d8,0);
  if (lVar10 == 0) {
code_r0x00018164e66d:
    FUN_180352450();
code_r0x00018164e673:
    FUN_180352460();
code_r0x00018164e679:
    FUN_180352450();
code_r0x00018164e67e:
    FUN_180352450();
code_r0x00018164e683:
    FUN_180352450();
code_r0x00018164e688:
    FUN_180352450();
  }
  else {
    FUN_181af8180(lVar10,uVar8,0);
    lVar10 = *(longlong *)(param_1 + 0x58);
    uVar8 = FUN_1803a51b0(uRam00000001857ff470);
    FUN_180c84ee0(uVar8,param_1,uRam00000001857fd7d8,0);
    if ((lVar10 == 0) || (FUN_181af80e0(lVar10,uVar8,0), param_2 == 0)) goto code_r0x00018164e66d;
    if (*(int *)(param_2 + 0x18) == 0) {
      return;
    }
    uStack_50 = 0;
    iStack_74 = *(int *)(param_2 + 0x1c);
    uStack_78 = 0;
    lStack_70 = 0;
    uStack_60 = 0;
    plStack_58 = &lStack_80;
    do {
      do {
        lStack_80 = param_2;
        if (param_2 == 0) goto code_r0x00018164e6b2;
        if ((iStack_74 != *(int *)(param_2 + 0x1c)) || (*(uint *)(param_2 + 0x18) <= uStack_78)) {
          if ((*(byte *)(*(longlong *)(lRam00000001857a34c0 + 0x20) + 0x135) & 1) == 0) {
            FUN_18034f1d0(*(longlong *)(lRam00000001857a34c0 + 0x20));
          }
          if (lStack_80 == 0) goto code_r0x00018164e6ad;
          if (iStack_74 != *(int *)(lStack_80 + 0x1c)) goto code_r0x00018164e6a6;
          uStack_78 = *(int *)(lStack_80 + 0x18) + 1;
          lStack_70 = 0;
          lVar10 = FUN_1803a51b0(uRam00000001857c6d48);
          FUN_1836f4d00(lVar10,param_2,uRam00000001857fd7d0);
          *(longlong *)(param_1 + 0xf0) = lVar10;
          *(undefined8 *)(param_1 + 0xd0) = param_3;
          *(undefined8 *)(param_1 + 0xd8) = param_4;
          *(undefined8 *)(param_1 + 0xf8) = 0;
          if (lVar10 == 0) goto code_r0x00018164e66d;
          bVar12 = 1;
          uStack_64 = 0;
          uStack_50 = 0;
          iStack_74 = *(int *)(lVar10 + 0x1c);
          uStack_78 = 0;
          lStack_70 = 0;
          uStack_60 = 0;
          plStack_58 = &lStack_80;
          uVar14 = 0;
          lStack_80 = lVar10;
          goto code_r0x00018164e2a0;
        }
        lVar10 = *(longlong *)(param_2 + 0x10);
        if (lVar10 == 0) goto code_r0x00018164e6a1;
        if (*(uint *)(lVar10 + 0x18) <= uStack_78) goto code_r0x00018164e673;
        lStack_70 = *(longlong *)(lVar10 + 0x20 + (longlong)(int)uStack_78 * 8);
        uStack_78 = uStack_78 + 1;
      } while (*(char *)(param_1 + 0x110) != '\x01');
      if (lStack_70 == 0) goto code_r0x00018164e69c;
      if (*(longlong *)(lStack_70 + 0x10) == 0) goto code_r0x00018164e697;
      lVar10 = *(longlong *)(*(longlong *)(lStack_70 + 0x10) + 0xe8);
      if (lVar10 == 0) goto code_r0x00018164e692;
    } while (*(char *)(lVar10 + 0x40) != '\0');
    lVar10 = *(longlong *)(param_1 + 0x38);
    lVar2 = *(longlong *)(param_1 + 0x48);
    if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
      FUN_180359940(lRam00000001857ff558);
    }
    if (lVar2 != 0) {
      lVar3 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x20b0);
      if (lVar3 != 0) {
        uVar8 = FUN_180ca5830(lVar3,0);
        uVar8 = FUN_181164190(lVar2,uVar8,0,0,0);
        lVar2 = *(longlong *)(param_1 + 0x48);
        if (lVar2 != 0) {
          lVar3 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x2a98);
          if (lVar3 != 0) {
            uVar9 = FUN_180ca5830(lVar3,0);
            uVar9 = FUN_181164190(lVar2,uVar9,0,0,0);
            if (lVar10 != 0) {
              FUN_18126b6f0(lVar10,uVar8,uVar9,0,0,0,0,1,2,0,0,0);
              return;
            }
            goto code_r0x00018164e679;
          }
          goto code_r0x00018164e67e;
        }
        goto code_r0x00018164e683;
      }
      goto code_r0x00018164e688;
    }
  }
  FUN_180352450();
code_r0x00018164e692:
  FUN_180352450();
code_r0x00018164e697:
  FUN_180352450();
code_r0x00018164e69c:
  FUN_180352450();
code_r0x00018164e6a1:
  FUN_180352450();
code_r0x00018164e6a6:
  FUN_183aed800(0);
code_r0x00018164e6ad:
  FUN_180352450();
code_r0x00018164e6b2:
  FUN_180352450();
  FUN_180352440();
code_r0x00018164e6be:
  FUN_180352460();
code_r0x00018164e6c3:
  FUN_180352450();
code_r0x00018164e6c8:
  FUN_180352450();
code_r0x00018164e6cd:
  FUN_180352450();
code_r0x00018164e6d2:
  FUN_183aed800(0);
code_r0x00018164e6d9:
  FUN_180352450();
code_r0x00018164e6de:
  FUN_180352450();
  FUN_180352440();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
code_r0x00018164e2a0:
  uVar13 = (uint)uVar14;
  if (lStack_80 != 0) {
    if ((iStack_74 != *(int *)(lStack_80 + 0x1c)) || (*(uint *)(lStack_80 + 0x18) <= uStack_78)) {
      if ((*(byte *)(*(longlong *)(lRam00000001857a34c0 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam00000001857a34c0 + 0x20));
      }
      if (lStack_80 == 0) goto code_r0x00018164e6d9;
      if (iStack_74 == *(int *)(lStack_80 + 0x1c)) {
        uStack_78 = *(int *)(lStack_80 + 0x18) + 1;
        lStack_70 = 0;
        goto code_r0x00018164e49a;
      }
      goto code_r0x00018164e6d2;
    }
    lVar10 = *(longlong *)(lStack_80 + 0x10);
    if (lVar10 == 0) goto code_r0x00018164e6cd;
    if (*(uint *)(lVar10 + 0x18) <= uStack_78) goto code_r0x00018164e6be;
    lVar10 = *(longlong *)(lVar10 + 0x20 + (longlong)(int)uStack_78 * 8);
    uStack_78 = uStack_78 + 1;
    lStack_70 = lVar10;
    if (lVar10 == 0) goto code_r0x00018164e6c8;
    uVar11 = 0;
    if (*(longlong *)(lVar10 + 0x10) != 0) {
      uVar11 = FUN_1813b2000(*(undefined8 *)(lVar10 + 0x10),0,0);
    }
    cVar5 = FUN_18164e6f0(param_1,lVar10,uVar11,&iStack_68,0);
    if (cVar5 == '\0') {
      bVar12 = 0;
      goto code_r0x00018164e49a;
    }
    uVar13 = uVar13 + iStack_68;
    uVar14 = (ulonglong)uVar13;
    uStack_64 = uVar13;
    if (((*(char *)(param_1 + 0x110) == '\x01') || (*(char *)(param_1 + 0x110) == '\b')) &&
       (uVar11 != 0)) {
      if (*(longlong *)(param_1 + 0xf8) == 0) {
        if (*(int *)(lRam0000000185801c68 + 0xe4) == 0) {
          FUN_180359940();
        }
        uVar8 = FUN_181aefde0(uVar11,0);
        *(undefined8 *)(param_1 + 0xf8) = uVar8;
      }
      else {
        if ((*(char *)(*(longlong *)(param_1 + 0xf8) + 0x12) != *(char *)(uVar11 + 0x12)) ||
           (cVar5 = FUN_1838d7290(*(undefined8 *)(*(longlong *)(param_1 + 0xf8) + 0x28),
                                  *(undefined8 *)(uVar11 + 0x28)), cVar5 != '\0')) {
          FUN_180c9a510(param_1,uRam00000001857fd7e0,0xffffffff,0);
          bVar12 = 0;
code_r0x00018164e49a:
          if (!(bool)(bVar12 & 0 < (int)uVar13)) {
            return;
          }
          if ((*(longlong *)(param_1 + 0x30) == 0) ||
             (lVar10 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0xd8), lVar10 == 0))
          goto code_r0x00018164e66d;
          iVar6 = FUN_180fffb20(lVar10,10,0);
          uVar7 = uVar13;
          if (iVar6 < (int)uVar13) {
            if (*(longlong *)(param_1 + 0xf0) == 0) goto code_r0x00018164e66d;
            iVar1 = *(int *)(*(longlong *)(param_1 + 0xf0) + 0x18);
            if (*(int *)(lRam0000000185827178 + 0xe4) == 0) {
              FUN_180359940();
            }
            uVar7 = iVar6 / iVar1;
            if ((int)uVar13 <= (int)uVar7) {
              uVar7 = uVar13;
            }
            *(undefined1 *)(param_1 + 0x125) = 1;
          }
          if ((int)uVar7 < 1) {
            FUN_18164ec40(param_1,*(undefined8 *)(param_1 + 0xf8),0);
            return;
          }
          if ((uVar7 != 1) || (*(longlong *)(param_1 + 0xf8) != 0)) {
            if (*(char *)(param_1 + 0x124) != '\0') {
              *(uint *)(param_1 + 0x120) = uVar7;
              *(undefined1 *)(param_1 + 0x124) = 0;
            }
            FUN_18164f080(param_1,uVar7,*(undefined8 *)(param_1 + 0xf8),0);
            return;
          }
          if (cRam0000000185a3c63a == '\0') {
            FUN_18035da50(0x185814ea8);
            LOCK();
            UNLOCK();
            cRam0000000185a3c63a = '\x01';
          }
          lVar10 = *(longlong *)(param_1 + 0xe0);
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (cRam0000000185a5056f == '\0') {
            FUN_18035da50(0x185814ea8);
            LOCK();
            UNLOCK();
            cRam0000000185a5056f = '\x01';
          }
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (cRam0000000185a5054a == '\0') {
            FUN_18035da50(0x185814ea8);
            LOCK();
            UNLOCK();
            cRam0000000185a5054a = '\x01';
          }
          if (lVar10 != 0) {
            if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
              FUN_180359940();
            }
            if (*(longlong *)(lVar10 + 0x10) != 0) {
              if (*(longlong *)(param_1 + 0xe0) == 0) goto code_r0x00018164e66d;
              FUN_1812ba220(*(longlong *)(param_1 + 0xe0),0,0,0);
            }
          }
          FUN_18164f580(param_1,1,0);
          return;
        }
        lVar10 = *(longlong *)(param_1 + 0xf8);
        if (lVar10 == 0) goto code_r0x00018164e6c3;
        iVar6 = *(int *)(lVar10 + 0x20);
        iVar1 = *(int *)(uVar11 + 0x20);
        cVar5 = FUN_181af0020(lVar10,0);
        if (cVar5 != '\0') {
          *(int *)(lVar10 + 0x20) = iVar1 + iVar6;
        }
      }
    }
    goto code_r0x00018164e2a0;
  }
  goto code_r0x00018164e6de;
}

