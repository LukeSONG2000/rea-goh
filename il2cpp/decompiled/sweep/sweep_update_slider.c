// sweep_update_slider VA 0x1816530c0 RVA 0x16530c0

void FUN_1816530c0(longlong param_1)

{
  code *pcVar1;
  
  *(undefined1 *)(param_1 + 500) = 1;
  if (*(longlong *)(param_1 + 0x128) != 0) {
    FUN_180e8c060();
    *(undefined1 *)(param_1 + 500) = 0;
    return;
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}

