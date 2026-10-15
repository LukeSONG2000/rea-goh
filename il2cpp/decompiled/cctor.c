// ==== GameConstants..cctor @ 0x1811f55b0 ====

void GameConstants_GameConstants_cctor(void)

{
  longlong lVar1;
  longlong lVar2;
  
  if (DAT_185a3ac34 == '\0') {
    FUN_18035da50(&DAT_185776310);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185776320);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185779f98);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857792d8);
    LOCK();
    UNLOCK();
    DAT_185a3ac34 = '\x01';
  }
  **(undefined4 **)(DAT_185779f98 + 0xb8) = 0x3f7fffef;
  lVar1 = FUN_1803a51b0(DAT_185776320);
  lVar2 = **(longlong **)(*(longlong *)(DAT_185776310 + 0x20) + 0xc0);
  FUN_180186ad0(lVar1,0x96,*(undefined8 *)(*(longlong *)(*(longlong *)(lVar2 + 0x20) + 0xc0) + 0x10)
               );
  lVar2 = FUN_182a47cd0(*(undefined8 *)(*(longlong *)(*(longlong *)(lVar2 + 0x20) + 0xc0) + 0x18));
  if (lVar2 != 0) {
    *(undefined8 *)(lVar1 + 0x30) = 0;
  }
  *(longlong *)(*(longlong *)(DAT_185779f98 + 0xb8) + 8) = lVar1;
  *(undefined8 *)(*(longlong *)(DAT_185779f98 + 0xb8) + 0x10) = DAT_1857792d8;
  return;
}

