// ==== process_damage_modification @ 0x181365fc0 ====


void FUN_181365fc0(undefined8 param_1,longlong param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  
  if (param_2 != 0) {
    FUN_181bcec50(param_2,param_3,param_4,0x1c,0xff,0);
    return;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

