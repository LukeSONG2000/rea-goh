// stat_scratch_start VA 0x180fe09f0 RVA 0xfe09f0

longlong FUN_180fe09f0(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  code *pcVar3;
  longlong lVar4;
  int iVar5;
  undefined8 uVar6;
  longlong lVar7;
  
  if (cRam0000000185a39ef8 == '\0') {
    FUN_18035da50(0x1857a8e70);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8e80);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8e78);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x1857a8e60);
    LOCK();
    UNLOCK();
    cRam0000000185a39ef8 = '\x01';
  }
  lVar7 = *(longlong *)(param_1 + 0x10);
  iVar5 = *(int *)(param_1 + 0x18) + 1;
  *(int *)(param_1 + 0x18) = iVar5;
  if (lVar7 != 0) {
    if (*(int *)(lVar7 + 0x18) <= iVar5) {
      uVar6 = FUN_1803a51b0(uRam00000001857a8e60);
      FUN_182d09b30(uVar6,uRam00000001857a8e80);
      lVar4 = lRam00000001857a8e70;
      *(int *)(lVar7 + 0x1c) = *(int *)(lVar7 + 0x1c) + 1;
      lVar2 = *(longlong *)(lVar7 + 0x10);
      if (lVar2 == 0) goto code_r0x000180fe0afe;
      uVar1 = *(uint *)(lVar7 + 0x18);
      if (*(uint *)(lVar2 + 0x18) <= uVar1) {
        FUN_1836f50c0(lVar7,uVar6,
                      *(undefined8 *)(*(longlong *)(*(longlong *)(lVar4 + 0x20) + 0xc0) + 0x70));
        return param_1;
      }
      *(uint *)(lVar7 + 0x18) = uVar1 + 1;
      if (*(uint *)(lVar2 + 0x18) <= uVar1) {
        FUN_180352460();
        pcVar3 = (code *)swi(3);
        lVar7 = (*pcVar3)();
        return lVar7;
      }
      *(undefined8 *)(lVar2 + 0x20 + (longlong)(int)uVar1 * 8) = uVar6;
    }
    return param_1;
  }
code_r0x000180fe0afe:
  FUN_180352450();
  pcVar3 = (code *)swi(3);
  lVar7 = (*pcVar3)();
  return lVar7;
}

