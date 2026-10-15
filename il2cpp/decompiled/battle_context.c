// ==== BattleContext..ctor @ 0x1817dcf70 ====


void BattleContext_BattleContext_ctor(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  
  if (DAT_185a3d044 == '\0') {
    FUN_18035da50(&DAT_1857c3450);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3440);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3470);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3460);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3498);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3488);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3400);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185806218);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3418);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3408);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1858061f8);
    LOCK();
    UNLOCK();
    DAT_185a3d044 = '\x01';
  }
  lVar2 = FUN_1803a51b0(DAT_1857c3408);
  lVar1 = DAT_1857c33f0;
  *(undefined4 *)(lVar2 + 0x10) = 0x20;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,400);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x308) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c3418);
  lVar1 = DAT_1857c3400;
  *(undefined4 *)(lVar2 + 0x10) = 200;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,0x200);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x310) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1858061f8);
  lVar1 = DAT_185806218;
  *(undefined4 *)(lVar2 + 0x10) = 0x100;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,8);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x318) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c33d8);
  lVar1 = DAT_1857c3440;
  *(undefined4 *)(lVar2 + 0x10) = 0x10;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,0x140);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 800) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c33e8);
  lVar1 = DAT_1857c3470;
  *(undefined4 *)(lVar2 + 0x10) = 0x10;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,200);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x328) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c33c8);
  lVar1 = DAT_1857c3450;
  *(undefined4 *)(lVar2 + 0x10) = 0x10;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,200);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x330) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c3488);
  lVar1 = DAT_1857c3498;
  *(undefined4 *)(lVar2 + 0x10) = 0x10;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,2);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x338) = lVar2;
  lVar2 = FUN_1803a51b0(DAT_1857c33d0);
  lVar1 = DAT_1857c3460;
  *(undefined4 *)(lVar2 + 0x10) = 200;
  lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 8);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  lVar4 = FUN_1803a51b0(lVar3);
  lVar3 = *(longlong *)
           (*(longlong *)
             (*(longlong *)
               (*(longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x20) + 0xc0) + 0x10) + 0x20) +
             0xc0) + 0x18);
  if ((*(byte *)(lVar3 + 0x135) & 1) == 0) {
    lVar3 = FUN_18034f1d0(lVar3);
  }
  uVar5 = FUN_1803a3dc0(lVar3,6);
  *(undefined8 *)(lVar4 + 0x10) = uVar5;
  *(longlong *)(lVar2 + 0x18) = lVar4;
  *(longlong *)(param_1 + 0x340) = lVar2;
  return;
}



// ==== BattleContext.Factory.Get @ 0x1817dd8e0 ====


longlong Factory_Get_341(longlong param_1,longlong param_2,ulonglong param_3)

{
  undefined8 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char cVar5;
  longlong lVar6;
  undefined8 uVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong lVar10;
  undefined8 *puVar11;
  longlong lVar12;
  ushort uVar13;
  longlong *plVar14;
  byte bVar15;
  byte bVar16;
  
  if (DAT_185a3d045 == '\0') {
    FUN_18035da50(&DAT_1857c3390);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3388);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33a8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c33b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3328);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3320);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3340);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3330);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3360);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3350);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3370);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3368);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32d0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32c8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32e0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32d8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_185769108);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32e8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3310);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3300);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3270);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3268);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3288);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3278);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32a0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3290);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32b8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c32b0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3220);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3210);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3238);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3230);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3250);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3248);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3260);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3258);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c31f8);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c31f0);
    LOCK();
    UNLOCK();
    FUN_18035da50(&DAT_1857c3208);
    LOCK();
    UNLOCK();
    DAT_185a3d045 = '\x01';
  }
  if (*(longlong *)(param_1 + 0xe0) == 0) {
LAB_1817dec86:
                    /* WARNING: Subroutine does not return */
    FUN_180352450();
  }
  cVar5 = PlayerDomainController_IsPlayerInOnboarding(*(longlong *)(param_1 + 0xe0),0);
  uVar9 = *(undefined8 *)(param_1 + 0x28);
  uVar1 = *(undefined8 *)(param_1 + 0x30);
  lVar6 = FUN_1803a51b0(DAT_1857c33a0);
  BattleContext_BattleContext_ctor(lVar6,0);
  bVar15 = (byte)param_3 & 1;
  if ((param_3 & 1) == 0) {
    uVar7 = *(undefined8 *)(param_1 + 0x18);
  }
  else {
    uVar7 = FUN_1803a51b0();
  }
  if (lVar6 == 0) goto LAB_1817dec86;
  *(undefined8 *)(lVar6 + 0xe8) = uVar7;
  *(undefined8 *)(lVar6 + 0x1a0) = *(undefined8 *)(param_1 + 0x20);
  *(undefined8 *)(lVar6 + 0x30) = uVar9;
  *(undefined8 *)(lVar6 + 0x38) = uVar1;
  bVar16 = ((byte)(param_3 >> 1) | (byte)param_3) & 1;
  if (bVar16 == 0) {
    lVar8 = *(longlong *)(param_1 + 0x38);
  }
  else {
    lVar8 = FUN_1803a51b0(DAT_1857c3330);
    if (DAT_185a3f13f == '\0') {
      FUN_18035da50(&DAT_1857fdaf0);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857a3360);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857f4ea0);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857f5158);
      LOCK();
      UNLOCK();
      DAT_185a3f13f = '\x01';
    }
    uVar9 = FUN_1803a51b0(DAT_1857f5158);
    FUN_182d09b30(uVar9,DAT_1857f4ea0);
    *(undefined8 *)(lVar8 + 0x78) = uVar9;
    uVar9 = FUN_1803a51b0(DAT_1857a3360);
    BattleLootDO_BattleLootDO_ctor(uVar9,0);
    *(undefined8 *)(lVar8 + 0xa0) = uVar9;
    lVar10 = FUN_1803a51b0(DAT_1857fdaf0);
    BattleLevelUpLootDO_BattleLevelUpLootDO_ctor(lVar10,0);
    if (lVar10 == 0) goto LAB_1817dec86;
    *(undefined1 *)(lVar10 + 0x14) = 0;
    *(longlong *)(lVar8 + 0xa8) = lVar10;
  }
  *(longlong *)(lVar6 + 0x1a8) = lVar8;
  *(undefined8 *)(lVar6 + 0x1b8) = *(undefined8 *)(param_1 + 0x40);
  *(undefined8 *)(lVar6 + 0x1d0) = *(undefined8 *)(param_1 + 0x48);
  *(undefined8 *)(lVar6 + 0x1d8) = *(undefined8 *)(param_1 + 0x50);
  *(undefined8 *)(lVar6 + 0x1e0) = *(undefined8 *)(param_1 + 0x58);
  plVar14 = *(longlong **)(param_1 + 0x60);
  if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
  lVar8 = *plVar14;
  if (*(ushort *)(lVar8 + 0x12e) != 0) {
    uVar13 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar13 * 0x10) == DAT_1857c3278) {
        puVar11 = (undefined8 *)
                  ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar13 * 0x10) *
                   0x10 + 0x138 + lVar8);
        goto LAB_1817ddde6;
      }
      uVar13 = uVar13 + 1;
    } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
  }
  puVar11 = (undefined8 *)FUN_18034f630(plVar14,DAT_1857c3278,0);
LAB_1817ddde6:
  uVar9 = (*(code *)*puVar11)(plVar14,0,0,bVar15,puVar11[1]);
  *(undefined8 *)(lVar6 + 0x18) = uVar9;
  plVar14 = *(longlong **)(param_1 + 0x68);
  if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
  lVar8 = *plVar14;
  if (*(ushort *)(lVar8 + 0x12e) != 0) {
    uVar13 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar13 * 0x10) == DAT_1857c32b0) {
        puVar11 = (undefined8 *)
                  ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar13 * 0x10) *
                   0x10 + 0x138 + lVar8);
        goto LAB_1817dde52;
      }
      uVar13 = uVar13 + 1;
    } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
  }
  puVar11 = (undefined8 *)FUN_18034f630(plVar14,DAT_1857c32b0,0);
LAB_1817dde52:
  uVar9 = (*(code *)*puVar11)(plVar14,0,0,bVar15,puVar11[1]);
  *(undefined8 *)(lVar6 + 0x20) = uVar9;
  *(undefined8 *)(lVar6 + 0x28) = *(undefined8 *)(param_1 + 0x70);
  *(undefined8 *)(lVar6 + 0x230) = *(undefined8 *)(param_1 + 0x78);
  if (bVar16 == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x80);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0x48) = uVar9;
  if ((param_3 & 1) == 0) {
    *(undefined8 *)(lVar6 + 0x50) = *(undefined8 *)(param_1 + 0x88);
    lVar8 = *(longlong *)(param_1 + 0x90);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c33a8);
    *(undefined8 *)(lVar6 + 0x50) = uVar9;
    lVar8 = FUN_1803a51b0(DAT_1857c32d0);
    if (DAT_185a3e882 == '\0') {
      FUN_18035da50(&DAT_185784ad8);
      LOCK();
      UNLOCK();
      DAT_185a3e882 = '\x01';
    }
    lVar10 = FUN_1803a51b0(DAT_185784ad8);
    if (DAT_185a39d40 == '\0') {
      FUN_18035da50(&DAT_185806e08);
      LOCK();
      UNLOCK();
      DAT_185a39d40 = '\x01';
    }
    uVar9 = FUN_1803a51b0();
    *(undefined8 *)(lVar10 + 0x10) = uVar9;
    *(longlong *)(lVar8 + 0x20) = lVar10;
  }
  *(longlong *)(lVar6 + 0x58) = lVar8;
  *(undefined8 *)(lVar6 + 0x60) = *(undefined8 *)(param_1 + 0x98);
  *(undefined8 *)(lVar6 + 0x68) = *(undefined8 *)(param_1 + 0xa0);
  if ((param_3 & 1) == 0) {
    *(undefined8 *)(lVar6 + 0x78) = *(undefined8 *)(param_1 + 0xa8);
    lVar8 = 0xb0;
  }
  else {
    uVar9 = FUN_1803a51b0();
    *(undefined8 *)(lVar6 + 0x78) = uVar9;
    lVar8 = 0xb8;
  }
  *(undefined8 *)(lVar6 + 0x80) = *(undefined8 *)(lVar8 + param_1);
  *(undefined8 *)(lVar6 + 0x88) = *(undefined8 *)(param_1 + 0xc0);
  *(undefined8 *)(lVar6 + 0x150) = *(undefined8 *)(param_1 + 0x2c8);
  *(undefined8 *)(lVar6 + 0x90) = *(undefined8 *)(param_1 + 200);
  *(undefined8 *)(lVar6 + 0x98) = *(undefined8 *)(param_1 + 0xd0);
  *(undefined8 *)(lVar6 + 0x1b0) = *(undefined8 *)(param_1 + 0xd8);
  *(undefined8 *)(lVar6 + 0x1c0) = *(undefined8 *)(param_1 + 0xe0);
  if (bVar16 == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0xe8);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0x200) = uVar9;
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0xf0);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0x248) = uVar9;
  *(undefined8 *)(lVar6 + 0x250) = *(undefined8 *)(param_1 + 0xf8);
  *(undefined8 *)(lVar6 + 600) = *(undefined8 *)(param_1 + 0x100);
  *(undefined8 *)(lVar6 + 0x260) = *(undefined8 *)(param_1 + 0x108);
  *(undefined8 *)(lVar6 + 0xa0) = *(undefined8 *)(param_1 + 0x110);
  *(undefined8 *)(lVar6 + 0x1c8) = *(undefined8 *)(param_1 + 0x118);
  *(undefined8 *)(lVar6 + 0x220) = *(undefined8 *)(param_1 + 0x120);
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x128);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0xb8) = uVar9;
  *(undefined8 *)(lVar6 + 0x228) = *(undefined8 *)(param_1 + 0x130);
  *(undefined8 *)(lVar6 + 0xc0) = *(undefined8 *)(param_1 + 0x138);
  *(undefined8 *)(lVar6 + 0xd0) = *(undefined8 *)(param_1 + 0x140);
  *(undefined8 *)(lVar6 + 0x238) = *(undefined8 *)(param_1 + 0x148);
  if (bVar16 == 0) {
    *(undefined8 *)(lVar6 + 0xd8) = *(undefined8 *)(param_1 + 0x150);
    uVar9 = *(undefined8 *)(param_1 + 0x158);
  }
  else {
    lVar8 = FUN_1803a51b0(DAT_1857c32f8);
    if (DAT_185a3b972 == '\0') {
      FUN_18035da50(&DAT_18582aff0);
      LOCK();
      UNLOCK();
      DAT_185a3b972 = '\x01';
    }
    uVar9 = FUN_1803a51b0(DAT_18582aff0);
    *(undefined8 *)(lVar8 + 0x10) = uVar9;
    *(longlong *)(lVar6 + 0xd8) = lVar8;
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0xe0) = uVar9;
  *(undefined8 *)(lVar6 + 0xf0) = *(undefined8 *)(param_1 + 0x160);
  *(undefined8 *)(lVar6 + 200) = *(undefined8 *)(param_1 + 0x168);
  if (bVar16 == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x170);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0xf8) = uVar9;
  *(undefined8 *)(lVar6 + 0x100) = *(undefined8 *)(param_1 + 0x178);
  *(undefined8 *)(lVar6 + 0x108) = *(undefined8 *)(param_1 + 0x180);
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x188);
  }
  else {
    uVar9 = FUN_1803a51b0();
  }
  *(undefined8 *)(lVar6 + 0x110) = uVar9;
  plVar14 = *(longlong **)(param_1 + 400);
  if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
  lVar8 = *plVar14;
  if (*(ushort *)(lVar8 + 0x12e) != 0) {
    uVar13 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar13 * 0x10) == DAT_1857c32e8) {
        puVar11 = (undefined8 *)
                  ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar13 * 0x10) *
                   0x10 + 0x138 + lVar8);
        goto LAB_1817de256;
      }
      uVar13 = uVar13 + 1;
    } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
  }
  puVar11 = (undefined8 *)FUN_18034f630(plVar14,DAT_1857c32e8,0);
LAB_1817de256:
  uVar9 = (*(code *)*puVar11)(plVar14,0,0,bVar15,puVar11[1]);
  *(undefined8 *)(lVar6 + 0x118) = uVar9;
  plVar14 = *(longlong **)(param_1 + 0x198);
  if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
  lVar8 = *plVar14;
  if (*(ushort *)(lVar8 + 0x12e) != 0) {
    uVar13 = 0;
    do {
      if (*(longlong *)(*(longlong *)(lVar8 + 0xb0) + (ulonglong)uVar13 * 0x10) == DAT_1857c3270) {
        puVar11 = (undefined8 *)
                  ((longlong)*(int *)(*(longlong *)(lVar8 + 0xb0) + 8 + (ulonglong)uVar13 * 0x10) *
                   0x10 + 0x138 + lVar8);
        goto LAB_1817de2d2;
      }
      uVar13 = uVar13 + 1;
    } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
  }
  puVar11 = (undefined8 *)FUN_18034f630(plVar14,DAT_1857c3270,0);
LAB_1817de2d2:
  uVar9 = (*(code *)*puVar11)(plVar14,0,0,bVar15,puVar11[1]);
  *(undefined8 *)(lVar6 + 0x130) = uVar9;
  *(undefined8 *)(lVar6 + 0x138) = *(undefined8 *)(param_1 + 0x1a0);
  if (bVar16 == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x1a8);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c32c8);
  }
  *(undefined8 *)(lVar6 + 0x2b8) = uVar9;
  *(undefined8 *)(lVar6 + 0x1e8) = *(undefined8 *)(param_1 + 0x268);
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x278);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c3260);
  }
  *(undefined8 *)(lVar6 + 0x1f0) = uVar9;
  *(undefined8 *)(lVar6 + 0x268) = *(undefined8 *)(param_1 + 0x1b0);
  *(undefined8 *)(lVar6 + 0x70) = *(undefined8 *)(param_1 + 0x10);
  *(undefined8 *)(lVar6 + 0x270) = *(undefined8 *)(param_1 + 0x1b8);
  if (bVar16 == 0) {
    lVar8 = *(longlong *)(param_1 + 0x1c0);
  }
  else {
    lVar8 = FUN_1803a51b0(DAT_1857c31f0);
    if (DAT_185a3ad93 == '\0') {
      FUN_18035da50(&DAT_18578fcf8);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18578fd00);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18578fcd8);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_18578fce8);
      LOCK();
      UNLOCK();
      DAT_185a3ad93 = '\x01';
    }
    lVar10 = FUN_1803a51b0(DAT_18578fcd8);
    lVar12 = FUN_182a47bd0(*(undefined8 *)
                            (*(longlong *)
                              (*(longlong *)
                                (**(longlong **)(*(longlong *)(DAT_18578fd00 + 0x20) + 0xc0) + 0x20)
                              + 0xc0) + 0x18));
    if (lVar12 != 0) {
      *(undefined8 *)(lVar10 + 0x30) = 0;
    }
    *(longlong *)(lVar8 + 0x10) = lVar10;
    lVar10 = FUN_1803a51b0(DAT_18578fce8);
    lVar12 = FUN_182a47bd0(*(undefined8 *)
                            (*(longlong *)
                              (*(longlong *)
                                (**(longlong **)(*(longlong *)(DAT_18578fcf8 + 0x20) + 0xc0) + 0x20)
                              + 0xc0) + 0x18));
    if (lVar12 != 0) {
      *(undefined8 *)(lVar10 + 0x30) = 0;
    }
    *(longlong *)(lVar8 + 0x18) = lVar10;
  }
  *(longlong *)(lVar6 + 0x278) = lVar8;
  *(undefined8 *)(lVar6 + 0x280) = *(undefined8 *)(param_1 + 0x1c8);
  *(undefined8 *)(lVar6 + 0x290) = *(undefined8 *)(param_1 + 0x1e0);
  *(undefined8 *)(lVar6 + 0x298) = *(undefined8 *)(param_1 + 0x1e8);
  *(undefined8 *)(lVar6 + 0x2a0) = *(undefined8 *)(param_1 + 0x1f0);
  *(undefined8 *)(lVar6 + 0x2a8) = *(undefined8 *)(param_1 + 0x250);
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 600);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c3368);
  }
  *(undefined8 *)(lVar6 + 0x128) = uVar9;
  *(undefined8 *)(lVar6 + 0x2b0) = *(undefined8 *)(param_1 + 0x260);
  if (param_2 == 0) goto LAB_1817dec86;
  lVar8 = *(longlong *)(param_2 + 0x78);
  if (lVar8 == 0) {
    lVar8 = FUN_1803a51b0(DAT_1857c3350);
  }
  *(longlong *)(lVar6 + 0x120) = lVar8;
  *(undefined8 *)(lVar6 + 0x1f8) = *(undefined8 *)(param_1 + 0x280);
  *(undefined8 *)(lVar6 + 0x208) = *(undefined8 *)(param_1 + 0x288);
  *(undefined8 *)(lVar6 + 0x210) = *(undefined8 *)(param_1 + 0x290);
  if ((param_3 & 1) == 0) {
    lVar8 = *(longlong *)(param_1 + 0x270);
  }
  else {
    lVar8 = FUN_1803a51b0(DAT_1857c3208);
    if (DAT_185a3f0ec == '\0') {
      FUN_18035da50(&DAT_185830428);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857f5928);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857f5938);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857f58a8);
      LOCK();
      UNLOCK();
      DAT_185a3f0ec = '\x01';
    }
    uVar9 = FUN_1803a51b0(DAT_1857f5938);
    FUN_182d09b30(uVar9,DAT_1857f5928);
    *(undefined8 *)(lVar8 + 0x18) = uVar9;
    uVar9 = FUN_1803a51b0(DAT_1857f58a8);
    *(undefined8 *)(lVar8 + 0x20) = uVar9;
    if (*(int *)(DAT_185830428 + 0xe4) == 0) {
      FUN_180359940(DAT_185830428);
    }
    lVar10 = *(longlong *)(DAT_185830428 + 0xb8);
    uVar2 = *(undefined4 *)(lVar10 + 0x14);
    uVar3 = *(undefined4 *)(lVar10 + 0x18);
    uVar4 = *(undefined4 *)(lVar10 + 0x1c);
    *(undefined4 *)(lVar8 + 0x40) = *(undefined4 *)(lVar10 + 0x10);
    *(undefined4 *)(lVar8 + 0x44) = uVar2;
    *(undefined4 *)(lVar8 + 0x48) = uVar3;
    *(undefined4 *)(lVar8 + 0x4c) = uVar4;
  }
  *(longlong *)(lVar6 + 0x2c8) = lVar8;
  uVar9 = FUN_1803a51b0(DAT_1857c3360);
  *(undefined8 *)(lVar6 + 0x2c0) = uVar9;
  *(undefined8 *)(lVar6 + 0x2d0) = *(undefined8 *)(param_1 + 0x298);
  if ((param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x2a0);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c32d8);
  }
  *(undefined8 *)(lVar6 + 0x2d8) = uVar9;
  if (cVar5 == '\0' && (param_3 & 1) == 0) {
    uVar9 = *(undefined8 *)(param_1 + 0x2a8);
  }
  else {
    uVar9 = FUN_1803a51b0(DAT_1857c3250);
  }
  *(undefined8 *)(lVar6 + 0x2e0) = uVar9;
  *(undefined8 *)(lVar6 + 0x2e8) = *(undefined8 *)(param_1 + 0x240);
  *(undefined8 *)(lVar6 + 0x2f0) = *(undefined8 *)(param_1 + 0x248);
  *(undefined8 *)(lVar6 + 0x2f8) = *(undefined8 *)(param_1 + 0x2b8);
  *(undefined8 *)(lVar6 + 0x300) = *(undefined8 *)(param_1 + 0x2c0);
  uVar9 = DAT_1857c3238;
  lVar8 = *(longlong *)(lVar6 + 0x2c8);
  if (lVar8 == 0) {
    lVar10 = 0;
  }
  else {
    lVar10 = FUN_1803a50f0(lVar8,DAT_1857c3238);
    if (lVar10 == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_180352e70(lVar8,uVar9);
    }
  }
  *(longlong *)(lVar6 + 0x218) = lVar10;
  if ((param_3 & 1) == 0) {
    if (*(longlong *)(param_1 + 0x268) == 0) goto LAB_1817dec86;
    if (*(char *)(*(longlong *)(param_1 + 0x268) + 0xb70) == '\0') {
      lVar8 = *(longlong *)(param_1 + 0x1d0);
    }
    else {
      lVar8 = *(longlong *)(param_1 + 0x1d8);
    }
  }
  else {
    uVar9 = *(undefined8 *)(lVar6 + 0x2c8);
    lVar8 = FUN_1803a51b0(DAT_185769108);
    *(longlong *)(lVar8 + 0x28) = lVar6;
    *(undefined8 *)(lVar8 + 0x10) = uVar9;
  }
  *(longlong *)(lVar6 + 0x288) = lVar8;
  uVar9 = FUN_1803a51b0();
  *(undefined8 *)(lVar6 + 0xb0) = uVar9;
  switch(*(undefined1 *)(param_2 + 0x18)) {
  case 0:
    plVar14 = *(longlong **)(param_1 + 0x200);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3268;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3268) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3268;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 1:
    plVar14 = *(longlong **)(param_1 + 0x1f8);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3220;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3220) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3220;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 2:
    plVar14 = *(longlong **)(param_1 + 0x208);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3290;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3290) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3290;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 3:
    plVar14 = *(longlong **)(param_1 + 0x210);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c32b8;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c32b8) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c32b8;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 4:
  case 5:
    plVar14 = *(longlong **)(param_1 + 0x218);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3288;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3288) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3288;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 6:
    plVar14 = *(longlong **)(param_1 + 0x220);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3300;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3300) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3300;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 7:
    plVar14 = *(longlong **)(param_1 + 0x228);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3210;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3210) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c3210;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 8:
    plVar14 = *(longlong **)(param_1 + 0x230);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c32a0;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c32a0) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
        lVar10 = DAT_1857c32a0;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  case 9:
    plVar14 = *(longlong **)(param_1 + 0x238);
    if (plVar14 == (longlong *)0x0) goto LAB_1817dec86;
    lVar8 = *plVar14;
    lVar10 = DAT_1857c3310;
    if (*(ushort *)(lVar8 + 0x12e) != 0) {
      lVar12 = *(longlong *)(lVar8 + 0xb0);
      uVar13 = 0;
      do {
        if (*(longlong *)(lVar12 + (ulonglong)uVar13 * 0x10) == DAT_1857c3310) goto LAB_1817dec69;
        uVar13 = uVar13 + 1;
      } while (uVar13 < *(ushort *)(lVar8 + 0x12e));
    }
    break;
  default:
    goto switchD_1817de7b6_default;
  }
  puVar11 = (undefined8 *)FUN_18034f630(plVar14,lVar10,0);
LAB_1817de816:
  uVar9 = (*(code *)*puVar11)(plVar14,0,0,0,puVar11[1]);
  *(undefined8 *)(lVar6 + 0xa8) = uVar9;
switchD_1817de7b6_default:
  if (bVar16 != 0) {
    uVar9 = *(undefined8 *)(lVar6 + 0xa8);
    uVar1 = *(undefined8 *)(param_1 + 0x2b0);
    lVar8 = FUN_1803a51b0(DAT_1857c3320);
    if (DAT_185a3d7cc == '\0') {
      FUN_18035da50(&DAT_185785d50);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857b14d8);
      LOCK();
      UNLOCK();
      FUN_18035da50(&DAT_1857b1810);
      LOCK();
      UNLOCK();
      DAT_185a3d7cc = '\x01';
    }
    uVar7 = FUN_1803a51b0(DAT_1857b1810);
    FUN_182d09b30(uVar7,DAT_1857b14d8);
    *(undefined8 *)(lVar8 + 0x28) = uVar7;
    uVar7 = FUN_1803a51b0(DAT_1857b1810);
    FUN_182d09b30(uVar7,DAT_1857b14d8);
    *(undefined8 *)(lVar8 + 0x30) = uVar7;
    lVar10 = FUN_1803a51b0(DAT_185785d50);
    if (DAT_185a3b52b == '\0') {
      FUN_18035da50(&DAT_185815958);
      LOCK();
      UNLOCK();
      DAT_185a3b52b = '\x01';
    }
    uVar7 = FUN_1803a51b0(DAT_185815958);
    *(undefined8 *)(lVar10 + 0x18) = uVar7;
    *(undefined4 *)(lVar8 + 0x50) = 0;
    *(undefined8 *)(lVar8 + 0x58) = 0;
    *(longlong *)(lVar8 + 0x38) = lVar10;
    *(undefined8 *)(lVar8 + 0x48) = uVar1;
    *(byte *)(lVar8 + 0x18) = bVar15;
    *(byte *)(lVar8 + 0x19) = (byte)((uint)param_3 >> 2) & 1;
    *(undefined8 *)(lVar8 + 0x20) = uVar9;
    *(longlong *)(lVar6 + 0xa8) = lVar8;
  }
  lVar8 = FUN_1803a51b0(DAT_1857c3340);
  *(longlong *)(lVar8 + 0x10) = lVar6;
  *(longlong *)(lVar6 + 0x148) = lVar8;
  return lVar6;
LAB_1817dec69:
  puVar11 = (undefined8 *)
            ((longlong)*(int *)(lVar12 + 8 + (ulonglong)uVar13 * 0x10) * 0x10 + 0x138 + lVar8);
  goto LAB_1817de816;
}

