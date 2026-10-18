// store_purchase_multi VA 0x181ca2940 RVA 0x1ca2940

void FUN_181ca2940(longlong *param_1)

{
  code *pcVar1;
  
  if (cRam0000000185a3ee94 == '\0') {
    FUN_18035da50(0x1857e4a40);
    LOCK();
    UNLOCK();
    cRam0000000185a3ee94 = '\x01';
  }
  if (param_1[0xb] != 0) {
    FUN_1811cbc00(param_1[0xb],uRam00000001857e4a40,0,0);
    (**(code **)(*param_1 + 0x478))
              (param_1,param_1[0x85],(int)param_1[0x86],0,*(undefined8 *)(*param_1 + 0x480));
    return;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

