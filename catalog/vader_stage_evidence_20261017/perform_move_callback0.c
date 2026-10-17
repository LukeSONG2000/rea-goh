// perform_move_callback0 VA 0x181cf7910 RVA 0x1cf7910

void FUN_181cf7910(longlong param_1)

{
  ulonglong *puVar1;
  undefined8 *puVar2;
  undefined1 auVar3 [16];
  uint uVar4;
  longlong lVar5;
  longlong lVar6;
  code *pcVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  char cVar10;
  undefined4 uVar11;
  int iVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  longlong lVar15;
  undefined8 uVar16;
  int iVar17;
  undefined8 unaff_RBX;
  longlong lVar18;
  undefined4 uVar19;
  undefined8 in_stack_ffffffffffffffe8;
  
  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffffe8 >> 0x20);
  if (cRam0000000185a3f063 == '\0') {
    FUN_18035da50(0x1858316b0);
    LOCK();
    UNLOCK();
    cRam0000000185a3f063 = '\x01';
  }
  if (*(longlong *)(param_1 + 0x10) != 0) {
    *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0x250) = 2;
    if (*(int *)(param_1 + 0x18) == 1) {
      if ((*(longlong *)(param_1 + 0x10) == 0) ||
         (*(longlong *)(*(longlong *)(param_1 + 0x10) + 0xe8) == 0)) goto code_r0x000181cf798e;
      FUN_180005650(4,uRam00000001858316b0);
    }
    lVar6 = *(longlong *)(param_1 + 0x10);
    if (lVar6 != 0) {
      if (cRam0000000185a3e924 == '\0') {
        FUN_18035da50(0x1857a3910);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1857a3928);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185831458);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185831450);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x1858314c0);
        LOCK();
        UNLOCK();
        FUN_18035da50(0x185831420);
        LOCK();
        UNLOCK();
        cRam0000000185a3e924 = '\x01';
      }
      if (*(longlong *)(lVar6 + 0x88) == 0) {
        return;
      }
      lVar18 = *(longlong *)(*(longlong *)(lVar6 + 0x88) + 0x1e8);
      if (lVar18 != 0) {
        if ((*(char *)(lVar18 + 0x265) == '\0') || (*(char *)(lVar6 + 0x210) == '\0')) {
          lVar18 = *(longlong *)(lVar6 + 0x140);
          if (lVar18 != 0) {
            if (*(int *)(lVar18 + 0x20) < 1) {
              cVar10 = FUN_181bc2ac0(lVar6,0);
              if (cVar10 == '\0') {
                if ((*(longlong *)(lVar6 + 0x88) == 0) ||
                   (lVar6 = *(longlong *)(*(longlong *)(lVar6 + 0x88) + 0x70), lVar6 == 0))
                goto code_r0x000181bc4857;
                FUN_181938fe0(lVar6,0);
              }
              else {
                FUN_181bc2f40(lVar6,0);
              }
              return;
            }
            if (lVar18 != 0) {
              if (*(int *)(lVar18 + 0x20) == 0) {
                uVar16 = func_0x000180006ea0(*(undefined8 *)
                                              (*(longlong *)(lRam0000000185831450 + 0x20) + 0xc0),
                                             0xb);
                FUN_18011f880(lVar18,uVar16);
                pcVar7 = (code *)swi(3);
                (*pcVar7)();
                return;
              }
              lVar5 = *(longlong *)(lVar18 + 0x10);
              if (lVar5 != 0) {
                uVar4 = *(uint *)(lVar18 + 0x18);
                if (*(uint *)(lVar5 + 0x18) <= uVar4) {
                  FUN_180352460();
                  pcVar7 = (code *)swi(3);
                  (*pcVar7)();
                  return;
                }
                lVar15 = (longlong)(int)uVar4 * 0x20;
                puVar1 = (ulonglong *)(lVar15 + 0x20 + lVar5);
                uVar8 = *puVar1;
                uVar9 = puVar1[1];
                auVar3 = *(undefined1 (*) [16])(lVar15 + 0x30 + lVar5);
                lVar15 = (longlong)(int)uVar4 * 0x20;
                puVar2 = (undefined8 *)(lVar15 + 0x20 + lVar5);
                *puVar2 = 0;
                puVar2[1] = 0;
                puVar2 = (undefined8 *)(lVar15 + 0x30 + lVar5);
                *puVar2 = 0;
                puVar2[1] = 0;
                if (*(longlong *)(lVar18 + 0x10) != 0) {
                  iVar17 = *(int *)(lVar18 + 0x18) + 1;
                  iVar12 = 0;
                  if (iVar17 != *(int *)(*(longlong *)(lVar18 + 0x10) + 0x18)) {
                    iVar12 = iVar17;
                  }
                  *(int *)(lVar18 + 0x18) = iVar12;
                  *(int *)(lVar18 + 0x20) = *(int *)(lVar18 + 0x20) + -1;
                  *(int *)(lVar18 + 0x24) = *(int *)(lVar18 + 0x24) + 1;
                  FUN_181bc3f20(lVar6,uVar8 & 0xffffffff,uVar9,auVar3._0_8_,auVar3._8_4_,auVar3[0xc]
                                ,0);
                  return;
                }
              }
            }
          }
        }
        else {
          uVar11 = FUN_181bb9960(lVar6,0);
          if ((((*(longlong *)(lVar6 + 0x98) != 0) &&
               (lVar18 = *(longlong *)(*(longlong *)(lVar6 + 0x98) + 0x40), lVar18 != 0)) &&
              (*(longlong *)(lVar6 + 0x88) != 0)) &&
             (*(longlong *)(*(longlong *)(lVar6 + 0x88) + 0x10) != 0)) {
            uVar16 = *(undefined8 *)(lVar18 + 0x58);
            uVar13 = FUN_180014310();
            if ((*(longlong *)(lVar6 + 0x88) != 0) &&
               (*(longlong *)(*(longlong *)(lVar6 + 0x88) + 0xa8) != 0)) {
              uVar14 = FUN_1800084f0(6,uRam00000001857a3910);
              FUN_182b70cf0(lVar6,uRam0000000185831420,uVar11,uVar16,uVar13,uVar14,
                            CONCAT44(uVar19,0xffffffff),uRam0000000185831458,unaff_RBX);
              return;
            }
          }
        }
      }
code_r0x000181bc4857:
      FUN_180352450();
      pcVar7 = (code *)swi(3);
      (*pcVar7)();
      return;
    }
  }
code_r0x000181cf798e:
  FUN_180352450();
  pcVar7 = (code *)swi(3);
  (*pcVar7)();
  return;
}

