// subobject_awake VA 0x1811b8250 RVA 0x11b8250

void FUN_1811b8250(longlong param_1)

{
  undefined8 uVar1;
  code *pcVar2;
  longlong lVar3;
  longlong lVar4;
  uint uVar5;
  
  if (cRam0000000185a3aac0 == '\0') {
    FUN_18035da50(0x185774550);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774568);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774518);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774520);
    LOCK();
    UNLOCK();
    FUN_18035da50(0x185774528);
    LOCK();
    UNLOCK();
    cRam0000000185a3aac0 = '\x01';
  }
  lVar3 = FUN_1803a51b0(uRam0000000185774518);
  lVar4 = FUN_182a47cd0();
  if (lVar4 != 0) {
    *(undefined8 *)(lVar3 + 0x30) = 0;
  }
  lVar4 = *(longlong *)(param_1 + 0x20);
  *(longlong *)(param_1 + 0x28) = lVar3;
  uVar5 = 0;
  while (lVar4 != 0) {
    if (*(int *)(lVar4 + 0x18) <= (int)uVar5) {
      return;
    }
    lVar3 = *(longlong *)(param_1 + 0x20);
    if (lVar3 == 0) break;
    if (*(uint *)(lVar3 + 0x18) <= uVar5) {
      FUN_183aeda10(0);
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
    lVar3 = *(longlong *)(lVar3 + 0x10);
    if (lVar3 == 0) break;
    if (*(uint *)(lVar3 + 0x18) <= uVar5) {
      FUN_180352460();
      pcVar2 = (code *)swi(3);
      (*pcVar2)();
      return;
    }
    if (*(longlong *)(param_1 + 0x28) == 0) break;
    uVar1 = *(undefined8 *)
             (*(longlong *)(*(longlong *)(lRam0000000185774568 + 0x20) + 0xc0) + 0x110);
    FUN_18334c740(*(longlong *)(param_1 + 0x28),
                  *(undefined8 *)(lVar3 + 0x20 + (longlong)(int)uVar5 * 0x10),
                  *(undefined8 *)(lVar3 + 0x28 + (longlong)(int)uVar5 * 0x10),
                  CONCAT71((int7)((ulonglong)uVar1 >> 8),1),uVar1);
    uVar5 = uVar5 + 1;
    lVar4 = *(longlong *)(param_1 + 0x20);
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  (*pcVar2)();
  return;
}

