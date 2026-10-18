// store_details_back VA 0x181c4b660 RVA 0x1c4b660

void FUN_181c4b660(longlong *param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  code *pcVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  longlong *plVar9;
  undefined8 uVar10;
  undefined *puVar11;
  undefined8 unaff_RDI;
  
  if (cRam0000000185a3ec82 == '\0') {
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ec82 = '\x01';
  }
  lVar6 = param_1[0x3e];
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
  if (lVar6 == 0) {
    return;
  }
  if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
    FUN_180359940();
  }
  if (*(longlong *)(lVar6 + 0x10) == 0) {
    return;
  }
  if (param_1[0x40] == 0) goto code_r0x000181c4b885;
  if (*(char *)(param_1[0x40] + 0x14) != '\0') {
    (**(code **)(*param_1 + 0x1b8))(param_1,*(undefined8 *)(*param_1 + 0x1c0));
    return;
  }
  lVar6 = param_1[0x3e];
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
  if (lVar6 == 0) {
code_r0x000181c4b821:
    uVar10 = **(undefined8 **)(lRam0000000185a33e80 + 0xb8);
  }
  else {
    if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
      FUN_180359940();
    }
    if (*(longlong *)(lVar6 + 0x10) == 0) goto code_r0x000181c4b821;
    if (((longlong *)param_1[0x3e] == (longlong *)0x0) ||
       (plVar9 = (longlong *)FUN_1803a7d00(*(longlong *)param_1[0x3e] + 0x20),
       plVar9 == (longlong *)0x0)) goto code_r0x000181c4b885;
    uVar10 = (**(code **)(*plVar9 + 0x1b8))(plVar9,*(undefined8 *)(*plVar9 + 0x1c0));
  }
  if (param_1[0x3e] != 0) {
    FUN_181c517e0(param_1,7,uVar10,*(undefined4 *)(param_1[0x3e] + 0x44c),0);
    if (param_1[0x3e] != 0) {
      uVar3 = *(undefined4 *)(param_1[0x3e] + 0x44c);
      if (cRam0000000185a3ec7a == '\0') {
        FUN_18035da50(0x1857d9c68,uVar3,0);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185814ea8);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857d9c70);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857d9c50);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857d9c58);
        LOCK();
        UNLOCK();
        cRam0000000185a3ec7a = '\x01';
      }
      lVar6 = FUN_1803a51b0(uRam00000001857d9c50);
      if (lVar6 != 0) {
        *(longlong **)(lVar6 + 0x10) = param_1;
        *(undefined4 *)(lVar6 + 0x18) = uVar3;
        *(undefined1 *)((longlong)param_1 + 0x209) = 0;
        if (param_1[0xb] != 0) {
          FUN_180fcd590(param_1[0xb],0);
          lVar7 = param_1[0x3d];
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
          if (lVar7 != 0) {
            if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
              FUN_180359940();
            }
            if (*(longlong *)(lVar7 + 0x10) != 0) {
              iVar2 = *(int *)(lVar6 + 0x18);
              if (cRam0000000185a3ec7b == '\0') {
                FUN_18035da50(0x18580d858,iVar2,0);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x1857fe510);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x1857bfac8);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x185814ea8);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x1857d9ca0);
                LOCK();
                UNLOCK();
                FUN_18035da50(0x1857d9ca8);
                LOCK();
                UNLOCK();
                cRam0000000185a3ec7b = '\x01';
              }
              lVar6 = FUN_1803a51b0(uRam00000001857d9ca8);
              if (lVar6 != 0) {
                *(longlong **)(lVar6 + 0x10) = param_1;
                *(int *)(lVar6 + 0x18) = iVar2;
                if (param_1[0x40] != 0) {
                  if ((*(int *)(param_1[0x40] + 0x1c) == 2) && (iVar2 != 2)) {
                    if (param_1[0x2e] == 0) goto code_r0x000181c4a77f;
                    FUN_18000a130(0xf,uRam00000001857fe510,param_1[0x2e],0xb,unaff_RDI);
                  }
                  if (param_1[0x40] != 0) {
                    *(undefined4 *)(param_1[0x40] + 0x1c) = *(undefined4 *)(lVar6 + 0x18);
                    if (param_1[0x40] != 0) {
                      *(undefined4 *)(param_1[0x40] + 0x20) = *(undefined4 *)(lVar6 + 0x18);
                      *(undefined1 *)(lVar6 + 0x1c) = 0;
                      lVar7 = param_1[0x3d];
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
                      if (lVar7 != 0) {
                        if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                          FUN_180359940();
                        }
                        if (*(longlong *)(lVar7 + 0x10) != 0) {
                          lVar7 = FUN_181c51cc0(param_1,*(undefined4 *)(lVar6 + 0x18),0);
                          *(longlong *)(lVar6 + 0x20) = lVar7;
                          *(undefined4 *)(lVar6 + 0x30) = 0;
                          switch(*(undefined4 *)(lVar6 + 0x18)) {
                          case 0:
                            *(undefined4 *)(lVar6 + 0x30) = 0x1d;
                            if (param_1[0x2e] == 0) goto code_r0x000181c4a77f;
                            FUN_18000a130(0xf,uRam00000001857fe510,param_1[0x2e],9);
                            break;
                          case 2:
                            if (lVar7 == 0) goto code_r0x000181c4a77f;
                            if (*(int *)(lVar7 + 0x18) == 0) {
                              *(undefined1 *)(lVar6 + 0x1c) = 1;
                            }
                            else {
                              *(undefined4 *)(lVar6 + 0x30) = 0x1e;
                              if (param_1[0x2e] == 0) goto code_r0x000181c4a77f;
                              FUN_18000a130(0xf,uRam00000001857fe510,param_1[0x2e],10);
                            }
                            break;
                          case 3:
                            *(undefined4 *)(lVar6 + 0x30) = 0x1f;
                            break;
                          case 6:
                            *(undefined4 *)(lVar6 + 0x30) = 0x2d;
                            break;
                          case 8:
                            *(undefined4 *)(lVar6 + 0x30) = 0x2e;
                            break;
                          case 9:
                            *(undefined4 *)(lVar6 + 0x30) = 0x2f;
                            break;
                          case 10:
                            *(undefined4 *)(lVar6 + 0x30) = 0x30;
                          }
                          uVar10 = FUN_181c4aed0(param_1,*(undefined8 *)(lVar6 + 0x20),0);
                          *(undefined8 *)(lVar6 + 0x28) = uVar10;
                          lVar7 = param_1[0x3d];
                          lVar8 = FUN_1803a51b0(uRam000000018580d858);
                          lVar5 = lRam00000001857d9ca0;
                          puVar11 = *(undefined **)(lRam00000001857d9ca0 + 8);
                          *(undefined **)(lVar8 + 0x10) = puVar11;
                          *(longlong *)(lVar8 + 0x28) = lVar5;
                          *(longlong *)(lVar8 + 0x20) = lVar6;
                          cVar1 = *(char *)(lVar5 + 0x52);
                          *(longlong *)(lVar8 + 0x40) = lVar8;
                          if (((*(byte *)(lVar5 + 0x4c) & 0x10) == 0) || (cVar1 != '\0')) {
                            *(longlong *)(lVar8 + 0x40) = lVar6;
                          }
                          else {
                            puVar11 = &UNK_180006440;
                          }
                          *(undefined **)(lVar8 + 0x18) = puVar11;
                          *(code **)(lVar8 + 0x38) = FUN_1800063e0;
                          if (lVar7 == 0) goto code_r0x000181c4a77f;
                          FUN_1812ba220(lVar7,CONCAT71((int7)((ulonglong)puVar11 >> 8),1),lVar8,0);
                          lVar6 = param_1[0x3e];
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
                          if (lVar6 != 0) {
                            if (*(int *)(lRam0000000185814ea8 + 0xe4) == 0) {
                              FUN_180359940();
                            }
                            if (*(longlong *)(lVar6 + 0x10) != 0) {
                              if (param_1[0x28] == 0) goto code_r0x000181c4a77f;
                              FUN_181256d00(param_1[0x28],0x24,0);
                              FUN_181c4a7c0(param_1,0);
                            }
                          }
                        }
                      }
                      return;
                    }
                  }
                }
              }
code_r0x000181c4a77f:
              FUN_180352450();
              pcVar4 = (code *)swi(3);
              (*pcVar4)();
              return;
            }
          }
          lVar7 = param_1[10];
          uVar10 = FUN_1803a51b0(uRam00000001857d9c68);
          FUN_180c84ee0(uVar10,lVar6,uRam00000001857d9c70,0);
          if (lVar7 != 0) {
            FUN_182c81d20(lVar7,uVar10,0,0,0,uRam00000001857d9c58);
            return;
          }
        }
      }
      FUN_180352450();
      pcVar4 = (code *)swi(3);
      (*pcVar4)();
      return;
    }
  }
code_r0x000181c4b885:
  FUN_180352450();
  pcVar4 = (code *)swi(3);
  (*pcVar4)();
  return;
}

