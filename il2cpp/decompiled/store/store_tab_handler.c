// store_tab_handler VA 0x181c4b210 RVA 0x1c4b210

void FUN_181c4b210(longlong param_1,ulonglong param_2)

{
  code *pcVar1;
  undefined8 uVar2;
  
  if (*(char *)(param_1 + 0x208) != '\0') {
    if (*(longlong *)(param_1 + 0x200) == 0) {
      FUN_180352450();
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
    *(undefined4 *)(*(longlong *)(param_1 + 0x200) + 0x2c) = 0;
    FUN_181c4a1d0(param_1,param_2,0);
    uVar2 = FUN_181c485b0(param_1,0);
    FUN_181c517e0(param_1,4,uVar2,param_2 & 0xffffffff,0);
  }
  return;
}

