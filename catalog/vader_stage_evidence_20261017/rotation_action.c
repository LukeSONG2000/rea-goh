// rotation_action VA 0x18131c520 RVA 0x131c520

void FUN_18131c520(longlong param_1)

{
  longlong *plVar1;
  code *pcVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  undefined8 *puVar6;
  ushort uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined1 auStack_28 [32];
  
  if (cRam0000000185a3b141 == '\0') {
    FUN_18035da50(0x1857a0da0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a9018);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a97f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185814ea8);
    LOCK();
    UNLOCK();
    cRam0000000185a3b141 = '\x01';
  }
  if ((*(longlong *)(param_1 + 0x30) != 0) && (lVar3 = *(longlong *)(param_1 + 0x50), lVar3 != 0)) {
    if (*(int *)(lVar3 + 0x10) == 0) {
      lVar3 = FUN_183cbddf0(*(longlong *)(param_1 + 0x30),0);
    }
    else {
      if (*(longlong *)(lVar3 + 0x18) == 0) goto code_r0x00018131c817;
      lVar3 = FUN_183c9dd40(*(longlong *)(lVar3 + 0x18),0);
    }
    if (lVar3 != 0) {
      lVar3 = FUN_182b23520(lVar3,uRam00000001857a9018);
      if (((lVar3 != 0) && (*(longlong *)(param_1 + 0x30) != 0)) &&
         (lVar4 = *(longlong *)(param_1 + 0x58), lVar4 != 0)) {
        lVar3 = *(longlong *)(lVar3 + 0x28);
        if (*(int *)(lVar4 + 0x10) == 0) {
          lVar4 = FUN_183cbddf0(*(longlong *)(param_1 + 0x30),0);
        }
        else {
          if (*(longlong *)(lVar4 + 0x18) == 0) goto code_r0x00018131c817;
          lVar4 = FUN_183c9dd40(*(longlong *)(lVar4 + 0x18),0);
        }
        lVar5 = lRam00000001857a0da0;
        if (lVar4 != 0) {
          if (*(longlong *)(lRam00000001857a0da0 + 0x38) == 0) {
            FUN_18034f250(lRam00000001857a0da0);
          }
          lVar4 = FUN_182b23910(lVar4,0,**(undefined8 **)(lVar5 + 0x38));
          if (lVar3 != 0) {
            lVar5 = FUN_1800084f0(0,uRam00000001857a97e0,lVar3);
            if ((lVar5 != 0) && (plVar1 = *(longlong **)(lVar5 + 0x1d8), plVar1 != (longlong *)0x0))
            {
              lVar5 = *plVar1;
              if (*(ushort *)(lVar5 + 0x12e) != 0) {
                uVar7 = 0;
                do {
                  if (*(longlong *)(*(longlong *)(lVar5 + 0xb0) + (ulonglong)uVar7 * 0x10) ==
                      lRam00000001857a97f8) {
                    puVar6 = (undefined8 *)
                             ((longlong)
                              (*(int *)(*(longlong *)(lVar5 + 0xb0) + 8 + (ulonglong)uVar7 * 0x10) +
                              8) * 0x10 + 0x138 + lVar5);
                    goto code_r0x00018131c709;
                  }
                  uVar7 = uVar7 + 1;
                } while (uVar7 < *(ushort *)(lVar5 + 0x12e));
              }
              puVar6 = (undefined8 *)FUN_18034f630(plVar1,lRam00000001857a97f8,8);
code_r0x00018131c709:
              puVar6 = (undefined8 *)(*(code *)*puVar6)(auStack_28,plVar1,puVar6[1]);
              lVar5 = *(longlong *)(param_1 + 0x60);
              uVar9 = (undefined4)*puVar6;
              uVar10 = (undefined4)((ulonglong)*puVar6 >> 0x20);
              uVar8 = *(undefined4 *)(puVar6 + 1);
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
                  puVar6 = (undefined8 *)FUN_180fd4f40(auStack_28,lVar4,lVar3,0);
                  uVar9 = (undefined4)*puVar6;
                  uVar10 = (undefined4)((ulonglong)*puVar6 >> 0x20);
                  uVar8 = *(undefined4 *)(puVar6 + 1);
                }
              }
              if (lVar5 != 0) {
                *(ulonglong *)(lVar5 + 0x38) = CONCAT44(uVar10,uVar9);
                *(undefined4 *)(lVar5 + 0x40) = uVar8;
                return;
              }
            }
          }
        }
      }
    }
  }
code_r0x00018131c817:
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

