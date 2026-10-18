// store_cached_product VA 0x18114c870 RVA 0x114c870

undefined8 FUN_18114c870(longlong param_1,longlong param_2)

{
  longlong lVar1;
  code *pcVar2;
  uint uVar3;
  undefined8 uVar4;
  
  if (cRam0000000185a3a766 == '\0') {
    FUN_18035da50(0x185765b28);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185765b30);
    LOCK();
    UNLOCK();
    cRam0000000185a3a766 = '\x01';
  }
  if ((((param_2 != 0) && (*(int *)(param_2 + 0x10) != 0)) && (*(longlong *)(param_1 + 0x210) != 0))
     && (lVar1 = *(longlong *)(param_1 + 0x210), *(int *)(lVar1 + 0x20) != *(int *)(lVar1 + 0x28)))
  {
    uVar3 = FUN_18332e340(lVar1,param_2,
                          *(undefined8 *)
                           (*(longlong *)(*(longlong *)(lRam0000000185765b28 + 0x20) + 0xc0) + 0x108
                           ));
    if (-1 < (int)uVar3) {
      lVar1 = *(longlong *)(lVar1 + 0x18);
      if (lVar1 == 0) {
        FUN_180352450();
        pcVar2 = (code *)swi(3);
        uVar4 = (*pcVar2)();
        return uVar4;
      }
      if (uVar3 < *(uint *)(lVar1 + 0x18)) {
        return *(undefined8 *)(lVar1 + ((longlong)(int)uVar3 + 2) * 0x18);
      }
      FUN_180352460();
      pcVar2 = (code *)swi(3);
      uVar4 = (*pcVar2)();
      return uVar4;
    }
  }
  return 0;
}

