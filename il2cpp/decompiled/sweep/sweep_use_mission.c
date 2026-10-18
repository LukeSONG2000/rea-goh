// sweep_use_mission VA 0x18164dbe0 RVA 0x164dbe0

void FUN_18164dbe0(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                  undefined1 param_5)

{
  code *pcVar1;
  longlong lVar2;
  
  if (cRam0000000185a3c631 == '\0') {
    FUN_18035da50(0x1857c6d38);
    LOCK();
    UNLOCK();
    cRam0000000185a3c631 = '\x01';
  }
  lVar2 = FUN_1803a51b0(uRam00000001857c6d38);
  if (lVar2 != 0) {
    *(undefined8 *)(lVar2 + 0x10) = param_2;
    *(undefined1 *)(param_1 + 0x110) = param_5;
    FUN_18164dc70(param_1,lVar2,param_3,param_4,0,1,0);
    return;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

