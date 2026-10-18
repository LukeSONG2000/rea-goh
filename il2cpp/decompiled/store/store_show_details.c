// store_show_details VA 0x181c4bab0 RVA 0x1c4bab0

void FUN_181c4bab0(longlong param_1,undefined8 param_2)

{
  longlong *plVar1;
  code *pcVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong lVar10;
  longlong lVar11;
  undefined *puVar12;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined4 uStack_38;
  
  if (cRam0000000185a3ec84 == '\0') {
    FUN_18035da50(0x1857b84a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185829e38);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857fe698);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d858);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b80);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b68);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b78);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b58);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b0d60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857ff558);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e8b80);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b60);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857e8b90);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b38);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857feca0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9b48);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9a70);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857d9a88);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857b84a8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec84 = '\x01';
  }
  lVar4 = FUN_1803a51b0(uRam00000001857d9a88);
  if (lVar4 == 0) goto code_r0x000181c4c67e;
  *(longlong *)(lVar4 + 0x10) = param_1;
  *(undefined8 *)(lVar4 + 0x18) = param_2;
  *(undefined1 *)(param_1 + 0x209) = 1;
  if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
  *(undefined8 *)(*(longlong *)(param_1 + 0x200) + 0x38) = *(undefined8 *)(lVar4 + 0x18);
  if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
  *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x1c) = 1;
  if (*(longlong *)(lVar4 + 0x18) == 0) goto code_r0x000181c4c67e;
  lVar11 = *(longlong *)(*(longlong *)(lVar4 + 0x18) + 0x108);
  if ((lVar11 != 0) && (*(int *)(lVar11 + 0x10) == 5)) {
    lVar11 = *(longlong *)(param_1 + 0x140);
    if (lVar11 == 0) goto code_r0x000181c4c67e;
    if (cRam0000000185a3ac3a == '\0') {
      FUN_18035da50(0x185788a00);
      LOCK();
      UNLOCK();
      cRam0000000185a3ac3a = '\x01';
    }
    uStack_48 = 0x22;
    uStack_40 = 0;
    FUN_18001f4d0(lVar11,&uStack_48);
  }
  lVar11 = *(longlong *)(param_1 + 0x140);
  if (lVar11 == 0) goto code_r0x000181c4c67e;
  if (cRam0000000185a3ac3a == '\0') {
    FUN_18035da50(0x185788a00);
    LOCK();
    UNLOCK();
    cRam0000000185a3ac3a = '\x01';
  }
  uStack_48 = 0x21;
  uStack_40 = 0;
  FUN_18001f4d0(lVar11,&uStack_48);
  uVar5 = FUN_181c485b0(param_1,0);
  if (*(longlong *)(lVar4 + 0x18) == 0) goto code_r0x000181c4c67e;
  uVar9 = *(undefined8 *)(*(longlong *)(lVar4 + 0x18) + 0x10);
  if (cRam0000000185a3ec9c == '\0') {
    FUN_18035da50(0x1857bcfe8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857bcff8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857bcff0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857bcfc8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec9c = '\x01';
  }
  lVar11 = *(longlong *)(param_1 + 0x148);
  uStack_48 = uRam00000001857bcff8;
  uStack_40 = 0xffffffffffffffff;
  uStack_38 = 6;
  uVar6 = FUN_183b04fe0(&uStack_48,0);
  uStack_48 = uRam00000001857bcff0;
  uStack_40 = 0xffffffffffffffff;
  uStack_38 = 1;
  uVar7 = FUN_183b04fe0(&uStack_48,0);
  lVar8 = FUN_1803a51b0(uRam00000001857bcfc8);
  *(undefined8 *)(lVar8 + 0x28) = uVar9;
  *(undefined8 *)(lVar8 + 0x10) = uVar6;
  *(undefined8 *)(lVar8 + 0x18) = uVar5;
  *(undefined8 *)(lVar8 + 0x20) = uVar7;
  if (lVar11 == 0) goto code_r0x000181c4c67e;
  FUN_182af6880(lVar11,lVar8,uRam00000001857bcfe8);
  *(undefined4 *)(param_1 + 0x100) = 1;
  *(undefined4 *)(param_1 + 0x104) = 6;
  lVar11 = *(longlong *)(lVar4 + 0x18);
  if (lVar11 == 0) goto code_r0x000181c4c67e;
  if (*(int *)(lVar11 + 0x18) != 3) {
    if (*(int *)(lVar11 + 0x18) == 7) {
      lVar11 = *(longlong *)(param_1 + 0x58);
      lVar8 = FUN_1803a51b0(uRam00000001857d9b38);
      *(undefined8 *)(lVar8 + 0x24) = 0xffffffffffffffff;
      *(undefined4 *)(lVar8 + 0x2c) = 0xffffffff;
      if (*(longlong *)(lVar4 + 0x18) == 0) goto code_r0x000181c4c67e;
      *(undefined8 *)(lVar8 + 0x10) = *(undefined8 *)(*(longlong *)(lVar4 + 0x18) + 0x10);
      if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
      *(undefined4 *)(lVar8 + 0x20) = *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x2c);
      if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
      *(undefined4 *)(lVar8 + 0x18) = *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x20);
      *(undefined1 *)(lVar8 + 0x1c) = 1;
      if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
      *(undefined8 *)(lVar8 + 0x50) = *(undefined8 *)(*(longlong *)(param_1 + 0x200) + 0x50);
      uVar5 = uRam00000001857d9b60;
    }
    else {
      if (*(int *)(lVar11 + 0x18) != 6) {
        if (*(longlong *)(param_1 + 0x90) != 0) {
          cVar3 = FUN_181c70840(*(longlong *)(param_1 + 0x90),lVar11,0x15,0);
          if (cVar3 != '\0') {
            lVar4 = *(longlong *)(param_1 + 0x1e8);
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
            if (lVar4 != 0) {
              if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                FUN_180359940();
              }
              if (*(longlong *)(lVar4 + 0x10) != 0) {
                lVar4 = *(longlong *)(param_1 + 0x1e8);
                lVar8 = FUN_1803a51b0(uRam000000018580d858);
                lVar11 = lRam00000001857d9b58;
                puVar12 = *(undefined **)(lRam00000001857d9b58 + 8);
                *(undefined **)(lVar8 + 0x10) = puVar12;
                *(longlong *)(lVar8 + 0x28) = lVar11;
                *(longlong *)(lVar8 + 0x20) = param_1;
                cVar3 = *(char *)(lVar11 + 0x52);
                *(longlong *)(lVar8 + 0x40) = lVar8;
                if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar3 != '\0')) {
                  *(longlong *)(lVar8 + 0x40) = param_1;
                }
                else {
                  puVar12 = &UNK_180006440;
                }
                *(undefined **)(lVar8 + 0x18) = puVar12;
                *(code **)(lVar8 + 0x38) = FUN_1800063e0;
                if (lVar4 != 0) {
                  FUN_1812ba220(lVar4,0,lVar8,0);
                  return;
                }
                goto code_r0x000181c4c67e;
              }
            }
            FUN_181c4da40(param_1,0);
            return;
          }
          if (*(longlong *)(lVar4 + 0x18) != 0) {
            cVar3 = FUN_181c7a050(*(longlong *)(lVar4 + 0x18),7,0);
            if (cVar3 == '\0') {
              if (*(longlong *)(lVar4 + 0x18) != 0) {
                if ((*(int *)(*(longlong *)(lVar4 + 0x18) + 0x18) == 1) &&
                   (cVar3 = FUN_181c4ddf0(param_1,0), cVar3 != '\0')) {
                  if (*(longlong *)(param_1 + 0x200) != 0) {
                    if (*(char *)(*(longlong *)(param_1 + 0x200) + 0x48) != '\0') {
                      if (*(longlong *)(param_1 + 0x58) == 0) goto code_r0x000181c4c67e;
                      FUN_180fccd70(*(longlong *)(param_1 + 0x58),0,0);
                    }
                    FUN_181c4ee90(param_1,*(undefined8 *)(lVar4 + 0x18),0);
                    return;
                  }
                }
                else {
                  lVar11 = *(longlong *)(param_1 + 0x1f0);
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
                  if (lVar11 != 0) {
                    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                      FUN_180359940();
                    }
                    if (*(longlong *)(lVar11 + 0x10) != 0) {
                      FUN_181c4cb70(param_1,*(undefined8 *)(lVar4 + 0x18),0,0);
                      return;
                    }
                  }
                  lVar11 = *(longlong *)(param_1 + 0x50);
                  uVar5 = FUN_1803a51b0(uRam00000001857b84a0);
                  FUN_180c84ee0(uVar5,lVar4,uRam00000001857d9a70,0);
                  if (lVar11 != 0) {
                    FUN_182c81d20(lVar11,uVar5,0,0,0,uRam00000001857b84a8);
                    return;
                  }
                }
              }
            }
            else if (*(longlong *)(param_1 + 0x138) != 0) {
              cVar3 = FUN_1819e5a10(*(longlong *)(param_1 + 0x138),0);
              if (cVar3 == '\0') {
                lVar11 = *(longlong *)(param_1 + 0xa0);
                lVar8 = *(longlong *)(param_1 + 0xa8);
                if (*(int *)(lRam00000001857ff558 + 0xe4) == 0) {
                  FUN_180359940(lRam00000001857ff558);
                }
                if ((lVar8 != 0) &&
                   (lVar10 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x1f30),
                   lVar10 != 0)) {
                  uVar5 = FUN_180ca5830(lVar10,0);
                  uVar5 = FUN_181164190(lVar8,uVar5,0,0,0);
                  lVar8 = *(longlong *)(param_1 + 0xa8);
                  if ((lVar8 != 0) &&
                     (lVar10 = *(longlong *)(*(longlong *)(lRam00000001857ff558 + 0xb8) + 0x1f28),
                     lVar10 != 0)) {
                    uVar9 = FUN_180ca5830(lVar10,0);
                    uVar9 = FUN_181164190(lVar8,uVar9,0,0,0);
                    lVar10 = FUN_1803a51b0(uRam000000018580d858);
                    lVar8 = lRam00000001857d9b48;
                    puVar12 = *(undefined **)(lRam00000001857d9b48 + 8);
                    *(undefined **)(lVar10 + 0x10) = puVar12;
                    *(longlong *)(lVar10 + 0x28) = lVar8;
                    *(longlong *)(lVar10 + 0x20) = lVar4;
                    cVar3 = *(char *)(lVar8 + 0x52);
                    *(longlong *)(lVar10 + 0x40) = lVar10;
                    if (((*(byte *)(lVar8 + 0x4c) & 0x10) == 0) || (cVar3 != '\0')) {
                      *(longlong *)(lVar10 + 0x40) = lVar4;
                    }
                    else {
                      puVar12 = &UNK_180006440;
                    }
                    *(undefined **)(lVar10 + 0x18) = puVar12;
                    *(code **)(lVar10 + 0x38) = FUN_1800063e0;
                    if (lVar11 != 0) {
                      FUN_18126b6f0(lVar11,uVar5,uVar9,lVar10,0,0,0,0,2,0,0,0);
                      return;
                    }
                  }
                }
              }
              else if (*(longlong *)(param_1 + 0x58) != 0) {
                FUN_180fccd70(*(longlong *)(param_1 + 0x58),0,0);
                plVar1 = *(longlong **)(param_1 + 0x1d0);
                if (plVar1 != (longlong *)0x0) {
                  (**(code **)(*plVar1 + 0x188))
                            (plVar1,*(undefined8 *)(param_1 + 0x1e8),*(undefined8 *)(*plVar1 + 400))
                  ;
                  if (*(longlong *)(param_1 + 0x1d0) != 0) {
                    *(undefined4 *)(*(longlong *)(param_1 + 0x1d0) + 0x9c) = 6;
                    lVar4 = *(longlong *)(param_1 + 0x1d0);
                    uVar5 = FUN_1803a51b0(uRam0000000185829e38);
                    FUN_180c7c3a0(uVar5,param_1,uRam00000001857d9b68,0);
                    if (lVar4 != 0) {
                      *(undefined8 *)(lVar4 + 0xa8) = uVar5;
                      lVar4 = *(longlong *)(param_1 + 0x1d0);
                      lVar8 = FUN_1803a51b0(uRam00000001857fe698);
                      lVar11 = lRam00000001857d9b78;
                      puVar12 = *(undefined **)(lRam00000001857d9b78 + 8);
                      *(undefined **)(lVar8 + 0x10) = puVar12;
                      *(longlong *)(lVar8 + 0x28) = lVar11;
                      *(longlong *)(lVar8 + 0x20) = param_1;
                      cVar3 = *(char *)(lVar11 + 0x52);
                      *(longlong *)(lVar8 + 0x40) = lVar8;
                      if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar3 != '\x02')) {
                        *(longlong *)(lVar8 + 0x40) = param_1;
                      }
                      else {
                        puVar12 = &UNK_1800083d0;
                      }
                      *(undefined **)(lVar8 + 0x18) = puVar12;
                      *(code **)(lVar8 + 0x38) = FUN_1800293a0;
                      if (lVar4 != 0) {
                        *(longlong *)(lVar4 + 0xb0) = lVar8;
                        lVar4 = *(longlong *)(param_1 + 0x1d0);
                        lVar8 = FUN_1803a51b0(uRam000000018580d858);
                        lVar11 = lRam00000001857d9b80;
                        puVar12 = *(undefined **)(lRam00000001857d9b80 + 8);
                        *(undefined **)(lVar8 + 0x10) = puVar12;
                        *(longlong *)(lVar8 + 0x28) = lVar11;
                        *(longlong *)(lVar8 + 0x20) = param_1;
                        cVar3 = *(char *)(lVar11 + 0x52);
                        *(longlong *)(lVar8 + 0x40) = lVar8;
                        if (((*(byte *)(lVar11 + 0x4c) & 0x10) == 0) || (cVar3 != '\0')) {
                          *(longlong *)(lVar8 + 0x40) = param_1;
                        }
                        else {
                          puVar12 = &UNK_180006440;
                        }
                        *(undefined **)(lVar8 + 0x18) = puVar12;
                        *(code **)(lVar8 + 0x38) = FUN_1800063e0;
                        if (lVar4 != 0) {
                          *(longlong *)(lVar4 + 0xc0) = lVar8;
                          return;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        goto code_r0x000181c4c67e;
      }
      lVar11 = *(longlong *)(param_1 + 0x58);
      lVar8 = FUN_1803a51b0(uRam00000001857e8b90);
      if ((*(longlong *)(lVar4 + 0x18) == 0) || (lVar8 == 0)) goto code_r0x000181c4c67e;
      *(undefined8 *)(lVar8 + 0x18) = *(undefined8 *)(*(longlong *)(lVar4 + 0x18) + 0x10);
      if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
      *(undefined4 *)(lVar8 + 0x20) = *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x2c);
      if (*(longlong *)(param_1 + 0x200) == 0) goto code_r0x000181c4c67e;
      *(undefined4 *)(lVar8 + 0x24) = *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x20);
      *(undefined4 *)(lVar8 + 0x28) = 6;
      uVar5 = uRam00000001857e8b80;
    }
    if (lVar11 != 0) {
      FUN_182b8cfb0(lVar11,lVar8,1,uVar5);
      return;
    }
    goto code_r0x000181c4c67e;
  }
  if (*(longlong *)(param_1 + 0x58) == 0) goto code_r0x000181c4c67e;
  lVar11 = FUN_180fcd800(*(longlong *)(param_1 + 0x58),1,param_1,0);
  if (*(longlong *)(param_1 + 0x228) == 0) goto code_r0x000181c4c67e;
  lVar8 = FUN_182b30790(*(longlong *)(param_1 + 0x228),uRam00000001857b0d60);
  uVar5 = *(undefined8 *)(lVar4 + 0x18);
  lVar10 = FUN_1803a51b0(uRam000000018580d858);
  lVar4 = lRam00000001857feca0;
  puVar12 = *(undefined **)(lRam00000001857feca0 + 8);
  *(undefined **)(lVar10 + 0x10) = puVar12;
  *(longlong *)(lVar10 + 0x28) = lVar4;
  *(longlong *)(lVar10 + 0x20) = lVar11;
  cVar3 = *(char *)(lVar4 + 0x52);
  *(longlong *)(lVar10 + 0x40) = lVar10;
  if ((*(byte *)(lVar4 + 0x4c) & 0x10) == 0) {
    if (lVar11 == 0) {
      uVar5 = func_0x000180352470();
      FUN_180352410(uVar5,0);
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
code_r0x000181c4c648:
    *(longlong *)(lVar10 + 0x40) = lVar11;
  }
  else {
    if (cVar3 != '\0') goto code_r0x000181c4c648;
    puVar12 = &UNK_180006440;
  }
  *(undefined **)(lVar10 + 0x18) = puVar12;
  *(code **)(lVar10 + 0x38) = FUN_1800063e0;
  if (lVar8 != 0) {
    FUN_181950e50(lVar8,uVar5,lVar10,4,0);
    return;
  }
code_r0x000181c4c67e:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

