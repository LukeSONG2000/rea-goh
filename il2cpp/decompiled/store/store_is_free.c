// store_is_free VA 0x181ca3920 RVA 0x1ca3920

ulonglong FUN_181ca3920(longlong param_1)

{
  code *pcVar1;
  ulonglong in_RAX;
  ulonglong uVar2;
  
  if (*(char *)(param_1 + 0x448) == '\0') {
    return in_RAX & 0xffffffffffffff00;
  }
  if (*(longlong *)(param_1 + 0x428) != 0) {
    return (ulonglong)*(byte *)(*(longlong *)(param_1 + 0x428) + 0x110);
  }
  FUN_180352450();
  pcVar1 = (code *)swi(3);
  uVar2 = (*pcVar1)();
  return uVar2;
}

