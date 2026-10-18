// store_purchase_single VA 0x181ca28b0 RVA 0x1ca28b0

void FUN_181ca28b0(longlong *param_1)

{
  code *pcVar1;
  
  if (cRam0000000185a3ee93 == '\0') {
    FUN_18035da50(0x1857e4ab8);
    LOCK();
    UNLOCK();
    cRam0000000185a3ee93 = '\x01';
  }
  if ((int)param_1[0x87] != 1) {
    if (param_1[0xb] == 0) {
      FUN_180352450();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
    FUN_1811cbc00(param_1[0xb],uRam00000001857e4ab8,0,0);
  }
  (**(code **)(*param_1 + 0x478))(param_1,param_1[0x85],1,0,*(undefined8 *)(*param_1 + 0x480));
  return;
}

