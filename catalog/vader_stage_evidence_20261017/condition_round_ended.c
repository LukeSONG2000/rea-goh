// condition_round_ended VA 0x1816b9f40 RVA 0x16b9f40

/* WARNING: Removing unreachable block (ram,0x0001816ba2eb) */
/* WARNING: Removing unreachable block (ram,0x0001816ba2f8) */
/* WARNING: Removing unreachable block (ram,0x0001816ba330) */
/* WARNING: Removing unreachable block (ram,0x0001816ba33e) */
/* WARNING: Removing unreachable block (ram,0x0001816ba34b) */
/* WARNING: Removing unreachable block (ram,0x0001816ba3dd) */
/* WARNING: Removing unreachable block (ram,0x0001816ba3f2) */
/* WARNING: Removing unreachable block (ram,0x0001816ba403) */
/* WARNING: Removing unreachable block (ram,0x0001816ba40c) */
/* WARNING: Removing unreachable block (ram,0x0001816ba415) */
/* WARNING: Removing unreachable block (ram,0x0001816ba359) */
/* WARNING: Removing unreachable block (ram,0x0001816ba366) */
/* WARNING: Removing unreachable block (ram,0x0001816ba36f) */
/* WARNING: Removing unreachable block (ram,0x0001816ba38c) */
/* WARNING: Removing unreachable block (ram,0x0001816ba3b9) */
/* WARNING: Removing unreachable block (ram,0x0001816ba3c2) */

void FUN_1816b9f40(longlong param_1)

{
  longlong lVar1;
  code *pcVar2;
  int iVar3;
  longlong lVar4;
  longlong lStackX_20;
  longlong lStack_88;
  uint uStack_80;
  int iStack_7c;
  longlong lStack_78;
  undefined8 uStack_70;
  longlong *plStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  longlong *plStack_50;
  longlong lStack_48;
  undefined *puStack_30;
  
  puStack_30 = &UNK_1816b9f53;
  FUN_1816b9fe0(param_1,1,0);
  if (cRam0000000185a3c8f2 == '\0') {
    FUN_18035da50(0x1857a4060,0,0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3e08);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3dc0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3dd0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3dc8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x18580d620);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3de0);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3dd8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3db8);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a3db0);
    LOCK();
    UNLOCK();
    cRam0000000185a3c8f2 = '\x01';
  }
  lStackX_20 = 0;
  lStack_88 = 0;
  uStack_80 = 0;
  iStack_7c = 0;
  lStack_78 = 0;
  if (*(int *)(lRam00000001857a4060 + 0xe4) == 0) {
    FUN_180359940();
  }
  lStackX_20 = FUN_1816b6540(0,0);
  uStack_58 = 0;
  plStack_50 = &lStackX_20;
  lVar4 = FUN_1803a51b0(uRam00000001857a3db0);
  FUN_182d09b30(lVar4,uRam00000001857a3db8);
  lVar1 = *(longlong *)(param_1 + 0x18);
  if (lVar1 != 0) {
    uStack_60 = 0;
    iStack_7c = *(int *)(lVar1 + 0x1c);
    uStack_80 = 0;
    lStack_78 = 0;
    uStack_70 = 0;
    plStack_68 = &lStack_88;
    lStack_88 = lVar1;
    lStack_48 = lVar4;
code_r0x0001816ba180:
    if (lStack_88 != 0) {
      if ((iStack_7c == *(int *)(lStack_88 + 0x1c)) && (uStack_80 < *(uint *)(lStack_88 + 0x18))) {
        lVar1 = *(longlong *)(lStack_88 + 0x10);
        if (lVar1 != 0) {
          if (*(uint *)(lVar1 + 0x18) <= uStack_80) {
            FUN_180352460();
            goto code_r0x0001816ba4ba;
          }
          lVar1 = *(longlong *)(lVar1 + 0x20 + (longlong)(int)uStack_80 * 8);
          uStack_80 = uStack_80 + 1;
          lStack_78 = lVar1;
          if (*(longlong *)(param_1 + 0x48) != 0) goto code_r0x0001816ba1dc;
          goto code_r0x0001816ba4bf;
        }
        goto code_r0x0001816ba4c4;
      }
      if ((*(byte *)(*(longlong *)(lRam00000001857a3dd0 + 0x20) + 0x135) & 1) == 0) {
        FUN_18034f1d0(*(longlong *)(lRam00000001857a3dd0 + 0x20));
      }
      if (lStack_88 == 0) goto code_r0x0001816ba4d0;
      if (iStack_7c != *(int *)(lStack_88 + 0x1c)) goto code_r0x0001816ba4c9;
      uStack_80 = *(int *)(lStack_88 + 0x18) + 1;
      lStack_78 = 0;
      FUN_1816bac90(param_1,lVar4,0,0);
      if (lVar4 != 0) {
        *(int *)(lVar4 + 0x1c) = *(int *)(lVar4 + 0x1c) + 1;
        iVar3 = *(int *)(lVar4 + 0x18);
        *(undefined4 *)(lVar4 + 0x18) = 0;
        if (0 < iVar3) {
          FUN_183af4330(*(undefined8 *)(lVar4 + 0x10),0,iVar3,0);
        }
        if (lStackX_20 != 0) {
          FUN_180005650(0,uRam000000018580d620,lStackX_20);
        }
        return;
      }
      goto code_r0x0001816ba50c;
    }
code_r0x0001816ba4d5:
    FUN_180352450();
    FUN_180352440();
    FUN_180352460();
    FUN_180352450();
    FUN_180352450();
    FUN_180352450();
    FUN_183aed800(0);
    FUN_180352450();
    FUN_180352450();
    FUN_180352440();
code_r0x0001816ba50c:
    FUN_180352450();
    FUN_180352450();
  }
  FUN_180352450();
  FUN_180352440();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
code_r0x0001816ba1dc:
  iVar3 = FUN_1832c5170(*(longlong *)(param_1 + 0x48),0,
                        *(undefined8 *)
                         (*(longlong *)(*(longlong *)(lRam00000001857a3e08 + 0x20) + 0xc0) + 0x108))
  ;
  if (-1 < iVar3) {
    if (lVar1 == 0) {
code_r0x0001816ba4ba:
      FUN_180352450();
code_r0x0001816ba4bf:
      FUN_180352450();
code_r0x0001816ba4c4:
      FUN_180352450();
code_r0x0001816ba4c9:
      FUN_183aed800(0);
code_r0x0001816ba4d0:
      FUN_180352450();
      goto code_r0x0001816ba4d5;
    }
    FUN_1817dc3d0(lVar1,lStackX_20,lVar4);
  }
  goto code_r0x0001816ba180;
}

