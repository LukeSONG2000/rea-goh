// subobject_action VA 0x1811b8560 RVA 0x11b8560

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1811b8560(undefined8 param_1,int *param_2)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  code *pcVar6;
  undefined8 uStack_98;
  int iStack_90;
  undefined8 uStack_88;
  float fStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  int iStack_68;
  undefined8 uStack_58;
  undefined8 uStack_50;
  
  if (cRam0000000185a3aac2 == '\0') {
    FUN_18035da50(0x1858368d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d4e68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c7d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18582c770);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857744c0);
    LOCK();
    UNLOCK();
    cRam0000000185a3aac2 = '\x01';
  }
  lVar1 = *(longlong *)(param_2 + 2);
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056e == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056e = '\x01';
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
  if (lVar1 == 0) {
DAT_1811b8c26:
    uStack_78 = uRam00000001857744f0;
    iStack_68 = *param_2;
    uStack_70 = 0xffffffffffffffff;
    uVar4 = FUN_183b04fe0(&uStack_78,0);
    FUN_182b771b0(param_1,uRam00000001857744a8,uVar4,0xffffffff,uRam000000018582c770);
    return;
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (*(longlong *)(lVar1 + 0x10) == 0) goto DAT_1811b8c26;
  if (*param_2 != 0) {
    if (*param_2 == 2) {
      if (lVar1 != 0) {
        lVar1 = FUN_182b23520(lVar1,uRam00000001858368d0);
        if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (cRam0000000185a5056e == '\0') {
          FUN_18035da50(0x185814ea8);
          LOCK();
          UNLOCK();
          cRam0000000185a5056e = '\x01';
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
        if (lVar1 != 0) {
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (*(longlong *)(lVar1 + 0x10) != 0) {
            FUN_18442fb30(lVar1,*(undefined8 *)(param_2 + 0xc),0,0);
            return;
          }
        }
        lVar1 = FUN_1844a1720(param_1,0);
        if (lVar1 != 0) {
          uVar2 = FUN_1844aedc0(lVar1,0);
          uVar4 = uRam00000001857744c0;
code_r0x0001811b8858:
          FUN_182b771b0(param_1,uVar4,uVar2,0xffffffff,uRam000000018582c770);
          return;
        }
      }
    }
    else if (*param_2 == 1) {
      if (lVar1 != 0) {
        lVar1 = FUN_182b23520(lVar1,uRam00000001857d4e68);
        if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
          FUN_180359940();
        }
        if (cRam0000000185a5056e == '\0') {
          FUN_18035da50(0x185814ea8);
          LOCK();
          UNLOCK();
          cRam0000000185a5056e = '\x01';
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
        if (lVar1 != 0) {
          if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
            FUN_180359940();
          }
          if (*(longlong *)(lVar1 + 0x10) != 0) {
            FUN_184439710(lVar1,*(undefined8 *)(param_2 + 0xc),0);
            return;
          }
        }
        lVar1 = FUN_1844a1720(param_1,0);
        if (lVar1 != 0) {
          uVar2 = FUN_1844aedc0(lVar1,0);
          uVar4 = uRam00000001857744a0;
          goto code_r0x0001811b8858;
        }
      }
    }
    else if (*param_2 == 3) {
      if (lVar1 != 0) {
        FUN_1844a56e0(lVar1,1,0);
        return;
      }
    }
    else {
      if (*param_2 != 4) {
        if (*param_2 != 5) {
          return;
        }
        FUN_1811acd90(lVar1,param_2[0xe],1,0,0,param_2[0xf],0);
        return;
      }
      if (lVar1 != 0) {
        FUN_1844a56e0(lVar1,0,0);
        return;
      }
    }
    goto code_r0x0001811b8cdd;
  }
  lVar5 = *(longlong *)(param_2 + 4);
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (cRam0000000185a5056e == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a5056e = '\x01';
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
  if (lVar5 == 0) {
code_r0x0001811b8a57:
    lVar5 = FUN_1844a1720(param_1,0);
    if (lVar5 == 0) {
code_r0x0001811b8cdd:
      FUN_180352450();
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    uVar4 = FUN_1844aedc0(lVar5,0);
    FUN_182b6d2e0(param_1,uRam00000001857744b8,uVar4,0xffffffff,uRam000000018582c7d0);
    FUN_180cbae40(lVar1,0);
    if (lVar1 == 0) goto code_r0x0001811b8cd7;
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar5 + 0x10) == 0) goto code_r0x0001811b8a57;
    if (((lVar1 == 0) || (lVar3 = FUN_1844a54d0(lVar1,0), lVar5 == 0)) ||
       (uVar4 = FUN_1844a54d0(lVar5,0), lVar3 == 0)) goto code_r0x0001811b8cdd;
    FUN_1844b9350(lVar3,uVar4,0);
    FUN_180cbae40(lVar1,0);
  }
  lVar5 = FUN_1844a54d0(lVar1,0);
  iStack_90 = param_2[8];
  if (lVar5 != 0) {
    uStack_98 = *(undefined8 *)(param_2 + 6);
    if (cRam0000000185a50933 == '\0') {
      FUN_18035da50(0x1857b8340);
      LOCK();
      UNLOCK();
      cRam0000000185a50933 = '\x01';
    }
    lVar3 = *(longlong *)(lVar5 + 0x10);
    if (lVar3 == 0) {
      FUN_1844c5660(lVar5,0);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    pcVar6 = pcRam0000000185a50998;
    if ((pcRam0000000185a50998 == (code *)0x0) &&
       (pcVar6 = (code *)FUN_1803679a0(&UNK_184b5e270), pcVar6 == (code *)0x0)) {
      uVar4 = func_0x000180367810(&UNK_184b5e270);
      FUN_1803660c0(uVar4,0);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    pcRam0000000185a50998 = pcVar6;
    (*pcRam0000000185a50998)(lVar3,&uStack_98);
    lVar1 = FUN_1844a54d0(lVar1,0);
    uStack_88 = CONCAT44((float)((ulonglong)*(undefined8 *)(param_2 + 9) >> 0x20) * _UNK_18529f7e4,
                         (float)*(undefined8 *)(param_2 + 9) * _UNK_18529f7e0);
    fStack_80 = (float)param_2[0xb] * _UNK_18529ec98;
    uStack_58 = 0;
    uStack_50 = 0;
    pcVar6 = pcRam0000000185a500b8;
    if ((pcRam0000000185a500b8 == (code *)0x0) &&
       (pcVar6 = (code *)FUN_1803679a0(&UNK_184b597a0), pcVar6 == (code *)0x0)) {
      uVar4 = func_0x000180367810(&UNK_184b597a0);
      FUN_1803660c0(uVar4,0);
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    pcRam0000000185a500b8 = pcVar6;
    (*pcRam0000000185a500b8)(&uStack_88,&uStack_58);
    if (lVar1 == 0) {
      FUN_180352450();
      pcVar6 = (code *)swi(3);
      (*pcVar6)();
      return;
    }
    uStack_78 = uStack_58;
    uStack_70 = uStack_50;
    if (cRam0000000185a5093a == '\0') {
      FUN_18035da50(0x1857b8340);
      LOCK();
      UNLOCK();
      cRam0000000185a5093a = '\x01';
    }
    lVar5 = *(longlong *)(lVar1 + 0x10);
    if (lVar5 != 0) {
      pcVar6 = pcRam0000000185a509d0;
      if ((pcRam0000000185a509d0 == (code *)0x0) &&
         (pcVar6 = (code *)FUN_1803679a0(&UNK_184b5d640), pcVar6 == (code *)0x0)) {
        uVar4 = func_0x000180367810(&UNK_184b5d640);
        FUN_1803660c0(uVar4,0);
        pcVar6 = (code *)swi(3);
        (*pcVar6)();
        return;
      }
      pcRam0000000185a509d0 = pcVar6;
      (*pcRam0000000185a509d0)(lVar5,&uStack_78);
      return;
    }
    FUN_1844c5660(lVar1,0);
    pcVar6 = (code *)swi(3);
    (*pcVar6)();
    return;
  }
code_r0x0001811b8cd7:
  FUN_180352450();
  pcVar6 = (code *)swi(3);
  (*pcVar6)();
  return;
}

