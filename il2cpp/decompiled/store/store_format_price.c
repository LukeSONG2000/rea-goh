// store_format_price VA 0x181c92ea0 RVA 0x1c92ea0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_181c92ea0(longlong param_1,longlong param_2)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  uint uVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  ulonglong uVar9;
  ulonglong uStack_28;
  ulonglong uStack_20;
  ulonglong uStack_18;
  ulonglong uStack_10;
  
  if (cRam0000000185a3ee3f == '\0') {
    FUN_18035da50(0x185830428);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    cRam0000000185a3ee3f = '\x01';
  }
  if (param_2 != 0) {
    if (*(int *)(param_2 + 0x10) == 4) {
      if (*(longlong *)(param_1 + 0x28) != 0) {
        lVar8 = FUN_18114c870(*(longlong *)(param_1 + 0x28),*(undefined8 *)(param_2 + 0x18));
        if (((lVar8 != 0) && (lVar8 = *(longlong *)(lVar8 + 0x20), lVar8 != 0)) &&
           (*(int *)(lVar8 + 0x10) != 0)) {
          return lVar8;
        }
        lVar8 = *(longlong *)(param_2 + 0x28);
        if ((lVar8 == 0) || (*(int *)(lVar8 + 0x10) == 0)) {
          if (*(longlong *)(param_1 + 0x10) != 0) {
            if (*(char *)(*(longlong *)(param_1 + 0x10) + 0x20) == '\0') {
              lVar8 = *(longlong *)(param_1 + 0x20);
              if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
                FUN_180359940(lRam00000001857ff558);
              }
              if ((lVar8 != 0) &&
                 (lVar6 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x60a8),
                 lVar6 != 0)) {
                uVar7 = FUN_180ca5830(lVar6,0);
                lVar8 = FUN_181164190(lVar8,uVar7,0,0,0);
                return lVar8;
              }
            }
            else if (*(longlong *)(param_1 + 0x18) != 0) {
              uVar7 = *(undefined8 *)(*(longlong *)(param_1 + 0x18) + 0x1488);
              uVar5 = *(undefined8 *)(param_1 + 0x40);
              uVar1 = *(uint *)(param_2 + 0x14);
              if (*(int *)(lRam0000000185830428 + 0xe4) == 0) {
                FUN_180359940();
              }
              uStack_18 = _UNK_18529f6b0;
              uStack_10 = _UNK_18529f6b8;
              uStack_28 = (ulonglong)(uint)((int)uVar1 >> 0x1f) & 0xffffffff80000000;
              uVar4 = -uVar1;
              if ((int)-uVar1 < 0) {
                uVar4 = uVar1;
              }
              uStack_20 = (ulonglong)uVar4;
              if (cRam0000000185a497f2 == '\0') {
                FUN_18035da50(0x1857afb88);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x185830428);
                LOCK();
                UNLOCK();
                cRam0000000185a497f2 = '\x01';
              }
              if (*(int *)(lRam0000000185830428 + 0xe4) == 0) {
                FUN_180359940();
              }
              if (*(int *)(lRam00000001857afb88 + 0xe4) == 0) {
                FUN_180359940();
              }
              FUN_183b40680(&uStack_28,&uStack_18,0);
              uStack_18 = uStack_28;
              uStack_10 = uStack_20;
              uVar9 = FUN_1803a4e80(lRam0000000185830428,&uStack_18);
              if (cRam0000000185a494f3 == '\0') {
                FUN_18035da50();
                LOCK();
                UNLOCK();
                cRam0000000185a494f3 = '\x01';
              }
              if (*(int *)(lRam00000001857d4e88 + 0xe4) == 0) {
                FUN_180359940();
              }
              uStack_20 = 0;
              uStack_18 = 0;
              uStack_10 = **(undefined8 **)(lRam00000001857d4e88 + 0xb8);
              uStack_28 = uVar9;
              lVar8 = FUN_1838daf80(uVar5,uVar7,&uStack_28,0);
              return lVar8;
            }
          }
        }
        else if (*(longlong *)(param_1 + 0x20) != 0) {
          lVar8 = FUN_181164190(*(longlong *)(param_1 + 0x20),lVar8,0,0,0);
          return lVar8;
        }
      }
    }
    else if (*(longlong *)(param_1 + 0x30) != 0) {
      lVar8 = *(longlong *)(*(longlong *)(param_1 + 0x30) + 0x10);
      if (cRam0000000185a3aeef == '\0') {
        FUN_18035da50(0x18582c470,lVar8,0);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857ff558);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1858301b8);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185792f28);
        LOCK();
        UNLOCK();
        cRam0000000185a3aeef = '\x01';
      }
      if (*(int *)(lRam000000018582c470 + 0xe4) == 0) {
        FUN_180359940();
      }
      if (cRam0000000185a4901c == '\0') {
        FUN_18035da50(0x18582c470);
        LOCK();
        UNLOCK();
        cRam0000000185a4901c = '\x01';
      }
      if (*(int *)(lRam000000018582c470 + 0xe4) == 0) {
        FUN_180359940();
      }
      uVar7 = **(undefined8 **)(lRam000000018582c470 + 0xb8);
      LOCK();
      UNLOCK();
      uVar5 = FUN_1803a4e80(uRam0000000185a33e58,&stack0x00000008);
      lVar6 = FUN_1838dacb0(uVar7,uRam0000000185792f28,uVar5,0);
      if ((lVar6 != 0) && (*(int *)(lVar6 + 0x10) != 0)) {
        if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
          FUN_180359940(lRam00000001857ff558);
        }
        if ((lVar8 == 0) ||
           (lVar2 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x53a8), lVar2 == 0))
        {
          FUN_180352450();
          pcVar3 = (code *)swi(3);
          lVar8 = (*pcVar3)();
          return lVar8;
        }
        uVar7 = FUN_180ca5830(lVar2,0);
        uStack_18 = 0;
        uVar7 = FUN_181164190(lVar8,uVar7,0,0);
        lVar6 = FUN_1838dc320(lVar6,uRam00000001858301b8,uVar7,0);
      }
      return lVar6;
    }
  }
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  lVar8 = (*pcVar3)();
  return lVar8;
}

