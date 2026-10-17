// persistent_is_immune VA 0x181a6e500 RVA 0x1a6e500

ulonglong FUN_181a6e500(longlong param_1,undefined8 param_2)

{
  longlong lVar1;
  code *pcVar2;
  char cVar3;
  ulonglong uVar4;
  undefined4 auStackX_8 [2];
  
  lVar1 = *(longlong *)(param_1 + 0x58);
  if (lVar1 != 0) {
    if (cRam0000000185a3d882 == '\0') {
      FUN_18035da50(0x1857856a0);
      LOCK();
      UNLOCK();
      cRam0000000185a3d882 = '\x01';
    }
    lVar1 = *(longlong *)(lVar1 + 0x18);
    if (lVar1 != 0) {
      auStackX_8[0] = 0;
      cVar3 = FUN_182d56dd0(lVar1,param_2,auStackX_8,
                            *(undefined8 *)
                             (*(longlong *)
                               (*(longlong *)
                                 (*(longlong *)
                                   (*(longlong *)(*(longlong *)(lRam00000001857856a0 + 0x20) + 0xc0)
                                   + 0xb0) + 0x20) + 0xc0) + 0xa0));
      if (cVar3 == '\0') {
        auStackX_8[0] = 0xffffffff;
      }
      return (ulonglong)(byte)((byte)((uint)auStackX_8[0] >> 0x1f) ^ 1);
    }
  }
  FUN_180352450();
  pcVar2 = (code *)swi(3);
  uVar4 = (*pcVar2)();
  return uVar4;
}

